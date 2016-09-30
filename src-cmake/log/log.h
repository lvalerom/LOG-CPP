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
#ifndef __LOG_LOG_H_
#define __LOG_LOG_H_

#undef log_setup
#undef log_trace

#define BOOST_LOG_DYN_LINK 1
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

#define LOG_DEBUG
#ifdef LOG_DEBUG
#include <stdio.h>
#define debugger_log printf
#else
#define debugger_log(str, ...) ((void)0)
#endif

enum ES
{
    TRACE = 0,
    DEBUG = 1,
    INFO = 2,
    WARNING = 3,
    ERROR = 4,
    FATAL = 5

};

inline std::ostream& operator << (std::ostream& strm, ES lvl)
{
    static const char* strings[] =
    {
        " TRACE "," DEBUG ","  INFO ","WARNING"," ERROR "," FATAL "
    };

    if(static_cast<std::size_t>(lvl) < sizeof(strings) / sizeof(*strings)){
        strm << strings[lvl];
    } else {
        strm << static_cast<int>(lvl);
    }

    return strm;
}

inline ES& operator >> (ES& lvl, const std::string& str)
{
    static const char* strings[] =
    {
        "TRACE","DEBUG","INFO","WARNING","ERROR","FATAL"
    };

    int i = -1;
    bool found = false;
    while(!found && i < 5){
        ++i;
        if(std::strcmp(str.c_str(),strings[i]) == 0){
            found = true;
        }
    }

    lvl = static_cast<ES>(i);
    return lvl;
}

//Macros

typedef boost::log::sources::severity_channel_logger_mt <
    ES,
    std::string > thread_safe_logger;

BOOST_LOG_GLOBAL_LOGGER(logger, thread_safe_logger)
#define LOG(Channel, ES) BOOST_LOG_CHANNEL_SEV(logger::get(), Channel, ES)

#define log_setup slog::setup
#define log_trace(Channel) LOG(Channel, ES::TRACE)
#define log_debug(Channel) LOG(Channel, ES::DEBUG)
#define log_info(Channel) LOG(Channel, ES::INFO)
#define log_warning(Channel) LOG(Channel, ES::WARNING)
#define log_error(Channel) LOG(Channel, ES::ERROR)
#define log_fatal(Channel) LOG(Channel, ES::FATAL)

namespace slog
{

void setup(unsigned int lvl = 5, const char* ids = nullptr, const char* file = nullptr, const char* conf = nullptr, bool console_mode = false);

}

#endif
