/*
Copyright 2015 Luis Valero Martin

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


//CLEAN
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

void slog::setup(unsigned int lvl, const char* ids, const char* file, const char* conf, bool consoleMode)
{
    debugger_log("Log setup\n");
    std::string timestamp {"%Y-%m-%d %H:%M:%S"};
    bool hasRotation = false;
    unsigned int rotation = 0;

    if(conf){
        debugger_log("Config file: %s\n", conf);
        using std::string;
        using std::ifstream;
        using std::stringstream;

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
                string strrotation{""}; getline(in, strrotation);
                debugger_log("Found file rotation config line: %s\n",strrotation.c_str());
                try {
                    rotation = std::stoi(strrotation.substr(1,strrotation.length()-2));
                    debugger_log("Size rotation: %u\n", rotation);
                }catch(std::invalid_argument e){
                    debugger_log("Error invalid_argument exception...\n");
                    hasRotation = false;
                    continue;
                }
                hasRotation = true;
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
    boost::shared_ptr<boost::log::sinks::text_file_backend> rotBackend;
    if(file){
        debugger_log("Setting stream backends...\n");
        if(!hasRotation){
            debugger_log("Backend without rotation...\n");
            backend = boost::make_shared<boost::log::sinks::text_ostream_backend>();
            backend->add_stream(boost::make_shared<std::ofstream>(file));
            debugger_log("Backend without rotation [DONE]\n");
        } else {
            //FIXME
            debugger_log("Backend with rotation...\n");
            rotBackend = boost::make_shared<boost::log::sinks::text_file_backend>(
                        boost::log::keywords::file_name = "file_%5N.log",
                        boost::log::keywords::rotation_size = 5 * 1024 * 1024,
                        boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(12, 0, 0)
                    );
            debugger_log("Backend with rotation [DONE]\n");
        }
    }

    boost::shared_ptr< std::ostream > stream(&std::clog, boost::null_deleter());

    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_file_backend> sinkFile_t;
    typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> text_sink;

    if(ids){
        debugger_log("Ids file: %s\n",ids);
        using std::string;
        using std::ifstream;
        using std::stringstream;

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
                    if(file && !hasRotation){
                        debugger_log("Sink without rotation...\n");
                        boost::shared_ptr<sink_t> sink(new sink_t(backend));
                        sink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        sink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(sink);
                        debugger_log("Sink without rotation [DONE]\n");
                    }

                    if(file && hasRotation){
                        //FIXME
                        debugger_log("Sink with rotation...\n");
                        boost::shared_ptr<sinkFile_t> sink(new sinkFile_t(rotBackend));
                        sink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        sink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(sink);
                        debugger_log("Sink with rotation [DONE]\n");
                    }

                    if(consoleMode){
                        debugger_log("Console sink...\n");
                        boost::shared_ptr<text_sink> consoleSink = boost::make_shared<text_sink>();
                        consoleSink->locked_backend()->add_stream(stream);

                        consoleSink->set_filter(boost::log::expressions::attr< std::string >("Channel") == id && severity >= llvl);
                        consoleSink->set_formatter(fmt);

                        boost::log::core::get()->add_sink(consoleSink);
                        debugger_log("Console sink [DONE]\n");
                    }
                    debugger_log("ID line: %s-%s [DONE]\n",id.c_str(),lv.c_str());
                }
            }
        }
        debugger_log("Ids file [DONE]\n");
    } else {
        debugger_log("Setting default sinks...\n");
        if(file && !hasRotation){
            debugger_log("Sink without rotation...\n");
            boost::shared_ptr<sink_t> sink(new sink_t(backend));
            sink->set_filter(severity >= static_cast<ES>(lvl));
            sink->set_formatter(fmt);

            boost::log::core::get()->add_sink(sink);
            debugger_log("Sink without rotation [DONE]\n");
        }
        if(file && hasRotation){
            //FIXME
            debugger_log("Sink with rotation...\n");
            boost::shared_ptr<sinkFile_t> sink(new sinkFile_t(rotBackend));
            sink->set_filter(severity >= static_cast<ES>(lvl));
            sink->set_formatter(fmt);

            boost::log::core::get()->add_sink(sink);
            debugger_log("Sink with rotation [DONE]\n");
        }
        if(consoleMode){
            debugger_log("Console sink...\n");
            boost::shared_ptr<text_sink> consoleSink = boost::make_shared<text_sink>();
            consoleSink->locked_backend()->add_stream(stream);

            consoleSink->set_filter(severity >= static_cast<ES>(lvl));
            consoleSink->set_formatter(fmt);

            boost::log::core::get()->add_sink(consoleSink);
            debugger_log("Console sink [DONE]\n");
        }
        debugger_log("Default sinks [DONE]\n");
    }
    debugger_log("Log setup [DONE]\n");
}


