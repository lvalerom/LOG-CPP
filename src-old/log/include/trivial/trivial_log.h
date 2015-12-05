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
#ifndef __TRIVIAL_LOG_H
#define __TRIVIAL_LOG_H

#undef log_trace
#undef log_debug
#undef log_info
#undef log_warning
#undef log_error
#undef log_fatal

#ifdef NLOG

#define log_trace ((void)0)
#define log_debug ((void)0)
#define log_info ((void)0)
#define log_warning ((void)0)
#define log_error ((void)0)
#define log_fatal ((void)0)

#else
#if 0 
#include <boost/log/trivial.hpp>

#define log_trace BOOST_LOG_TRIVIAL(trace)
#define log_debug BOOST_LOG_TRIVIAL(debug)
#define log_info BOOST_LOG_TRIVIAL(info)
#define log_warning BOOST_LOG_TRIVIAL(warning)
#define log_error BOOST_LOG_TRIVIAL(error)
#define log_fatal BOOST_LOG_TRIVIAL(fatal)
#endif

#define log_trace Log::trivial_log::trace
#define log_debug Log::trivial_log::debug
#define log_info Log::trivial_log::info
#define log_warning Log::trivial_log::warning
#define log_error Log::trivial_log::error
#define log_fatal Log::trivial_log::fatal

namespace Log
{
	namespace trivial_log
	{
		namespace level
		{
			enum ELogLevel
			{
				TRACE = 0,
				DEBUG = 1,
				INFO = 2,
				WARNING = 3,
				ERROR = 4,
				FATAL = 5
			};
		}

		void set_up(level::ELogLevel lvl = level::FATAL);
		void set_up(const char* file = 0, level::ELogLevel lvl = level::FATAL);
		void set_up(const char* file = 0, const char* conf = 0, level::ELogLevel lvl = level::FATAL);

		void trace(const char* format, ...);
		void debug(const char* format, ...);
		void info(const char* format, ...);
		void warning(const char* format, ...);
		void error(const char* format, ...);
		void fatal(const char* format, ...);

	}
}

#endif // NLOG

#endif