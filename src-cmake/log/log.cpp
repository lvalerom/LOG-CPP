/*
Copyright 2015-2016 Luis Valero Martin

This file is part of BLA_LOG.
BLA_LOG is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
any later version.
BLA_LOG is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public License
along with BLA_LOG.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "log.h"

#include <fstream>
#include <string>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", ES)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)

BOOST_LOG_GLOBAL_LOGGER_INIT(logger, thread_safe_logger)
{
    thread_safe_logger logger;
    return logger;
}

void slog::setup(unsigned int lvl, const char* ids, const char* file, const char* conf, bool console_mode)
{
    debugger_log("Log setup\n");
    std::string timestamp {"%Y-%m-%d %H:%M:%S"};
    bool has_rotation = false;
    unsigned long long size_rotation = 0;
    unsigned int filename_digits = 0;
    unsigned int hour_rotation = 0;
    unsigned int min_rotation = 0;
    unsigned int sec_rotation = 0;

    if(conf){
        debugger_log("Config file: %s\n", conf);
        using std::string;
        using std::ifstream;

        ifstream in{conf};
        if(!in){
            debugger_log("Error while opening the config file...\n");
            return;
        }

        debugger_log("Config file opened...\n");

        while(in && !in.eof()){
            char start = in.peek();

            if(start == '#' || start == '\n'){
                string comment{""}; getline(in, comment);
                continue;
            }
            if(start == '$'){
                string strtimestamp{""}; getline(in, strtimestamp);
                debugger_log("Found timestamp config line: %s\n",strtimestamp.c_str());
                timestamp = strtimestamp.substr(1,strtimestamp.length()-1);
                debugger_log("TimeStamp format: %s\n",timestamp.c_str());
                continue;
            }

            if(start == '%'){
                string id {""}, rot_info{""};
                getline(in, id, '=');
                id = id.substr(1,id.length()-1);
                if (id != ""){
                    getline(in, rot_info);
                    if (rot_info != ""){
                        debugger_log("Found rotation info line: %s=%s\n",id.c_str(),rot_info.c_str());
                        if(id == "size"){
                            try {
                                size_rotation = std::stoull(rot_info);
                                debugger_log("Size rotation: %llu\n", size_rotation);
                            }catch(std::invalid_argument e){
                                debugger_log("Error invalid_argument exception...\n");
                                has_rotation = false;
                                continue;
                            }
                        }
                        if(id == "filename_digits"){
                            try {
                                filename_digits = std::stoull(rot_info);
                                debugger_log("Filename digits: %u\n", filename_digits);
                            }catch(std::invalid_argument e){
                                debugger_log("Error invalid_argument exception...\n");
                                has_rotation = false;
                                continue;
                            }
                        }
                        if(id == "time_hour"){
                            try {
                                hour_rotation = std::stoull(rot_info);
                                debugger_log("Hour rotation: %u\n", hour_rotation);
                            }catch(std::invalid_argument e){
                                debugger_log("Error invalid_argument exception...\n");
                                has_rotation = false;
                                continue;
                            }
                        }
                        if(id == "time_min"){
                            try {
                                min_rotation = std::stoull(rot_info);
                                debugger_log("Min rotation: %u\n", min_rotation);
                            }catch(std::invalid_argument e){
                                debugger_log("Error invalid_argument exception...\n");
                                has_rotation = false;
                                continue;
                            }
                        }
                        if(id == "time_sec"){
                            try {
                                sec_rotation = std::stoull(rot_info);
                                debugger_log("Sec rotation: %u\n", sec_rotation);
                            }catch(std::invalid_argument e){
                                debugger_log("Error invalid_argument exception...\n");
                                has_rotation = false;
                                continue;
                            }
                        }
                    }
                }
                has_rotation = true;
                continue;
            }
            string line {""}; getline(in, line);
        }
        debugger_log("Config file [DONE]\n");
    }

    boost::log::add_common_attributes();

    debugger_log("Setting formatter...\n");

    namespace expr = boost::log::expressions;
    boost::log::formatter fmt = expr::stream
            << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", timestamp)
            << " (Thread id: " << expr::attr<boost::log::attributes::current_thread_id::value_type>("ThreadID") << ")"
            << expr::if_(expr::attr<ES>("Severity"))[expr::stream << " [" << severity << "]"]
            << expr::if_(expr::attr<std::string>("Channel")) [expr::stream << " (" << channel << ")"]
            << ": " << expr::smessage;

    debugger_log("Formatter [DONE]\n");

    boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend;
    boost::shared_ptr<boost::log::sinks::text_file_backend> rot_backend;
    if(file){
        debugger_log("Setting stream backends...\n");
        if(!has_rotation){
            debugger_log("Backend without rotation...\n");
            backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
            backend->add_stream(boost::make_shared<std::ofstream>(file));
            debugger_log("Backend without rotation [DONE]\n");
        } else {
            std::stringstream ss{file};
            ss << file << "_%" << filename_digits << "N.log";
            debugger_log("Backend with rotation...\n");
            rot_backend = boost::make_shared<boost::log::sinks::text_file_backend>(
                        boost::log::keywords::file_name = ss.str(),
                        boost::log::keywords::rotation_size = size_rotation,
                        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(
                        hour_rotation, min_rotation, sec_rotation)
                    );
            debugger_log("Backend with rotation [DONE]\n");
        }
    }

    boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());

    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> sink_file_t;

    if(ids){
        debugger_log("Ids file: %s\n",ids);
        using std::string;
        using std::ifstream;

        ifstream in{ids};
        if(!in){
            debugger_log("Error while opening the ids file...\n");
            return;
        }

        debugger_log("Ids file opened...\n");

        while(in && !in.eof()){
            char start = in.peek();

            if(start == '#' || start == '\n'){
                string comment{""}; getline(in, comment);
                continue;
            }
            string id {""}, lv{""};
            getline(in, id, '=');
            if (id != ""){
                getline(in, lv);
                if (lv != ""){
                    debugger_log("Found id line: %s-%s\n",id.c_str(),lv.c_str());

                    ES llvl;
                    llvl >> lv;

                    debugger_log("Setting sinks...\n");
                    if(file && !has_rotation){
                        debugger_log("Sink without rotation...\n");
                        boost::shared_ptr<sink_t> sink(new sink_t(backend));
                        sink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        sink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(sink);
                        debugger_log("Sink without rotation [DONE]\n");
                    }

                    if(file && has_rotation){
                        debugger_log("Sink with rotation...\n");
                        boost::shared_ptr<sink_file_t> sink(new sink_file_t(rot_backend));
                        sink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        sink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(sink);
                        debugger_log("Sink with rotation [DONE]\n");
                    }

                    if(console_mode){
                        debugger_log("Console sink...\n");
                        boost::shared_ptr<sink_t> console_sink = boost::make_shared<sink_t>();
                        console_sink->locked_backend()->add_stream(stream);

                        console_sink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        console_sink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(console_sink);
                        debugger_log("Console sink [DONE]\n");
                    }
                    debugger_log("ID line: %s-%s [DONE]\n",id.c_str(),lv.c_str());
                }
            }
        }
        debugger_log("Ids file [DONE]\n");
    } else {
        debugger_log("Setting default sinks...\n");
        if(file && !has_rotation){
            debugger_log("Sink without rotation...\n");
            boost::shared_ptr<sink_t> sink(new sink_t(backend));
            sink->set_filter(severity >= static_cast<ES>(lvl));
            sink->set_formatter(fmt);

            boost::log::core::get()->add_sink(sink);
            debugger_log("Sink without rotation [DONE]\n");
        }
        if(file && has_rotation){
            debugger_log("Sink with rotation...\n");
            boost::shared_ptr<sink_file_t> sink(new sink_file_t(rot_backend));
            sink->set_filter(severity >= static_cast<ES>(lvl));
            sink->set_formatter(fmt);

            boost::log::core::get()->add_sink(sink);
            debugger_log("Sink with rotation [DONE]\n");
        }
        if(console_mode){
            debugger_log("Console sink...\n");
            boost::shared_ptr<sink_t> console_sink = boost::make_shared<sink_t>();
            console_sink->locked_backend()->add_stream(stream);

            console_sink->set_filter(severity >= static_cast<ES>(lvl));
            console_sink->set_formatter(fmt);

            boost::log::core::get()->add_sink(console_sink);
            debugger_log("Console sink [DONE]\n");
        }
        debugger_log("Default sinks [DONE]\n");
    }
    debugger_log("Log setup [DONE]\n");
}


