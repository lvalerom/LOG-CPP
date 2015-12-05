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
#ifndef __FULL_LOG_H
#define __FULL_LOG_H

#undef log_trace
#undef log_debug
#undef log_info
#undef log_warning
#undef log_error
#undef log_fatal

#undef blog_trace
#undef blog_debug
#undef blog_info
#undef blog_warning
#undef blog_error
#undef blog_fatal

#ifdef NLOG

#define log_trace ((void)0)
#define log_debug ((void)0)
#define log_info ((void)0)
#define log_warning ((void)0)
#define log_error ((void)0)
#define log_fatal ((void)0)

#define blog_trace ((void)0)
#define blog_debug ((void)0)
#define blog_info ((void)0)
#define blog_warning ((void)0)
#define blog_error ((void)0)
#define blog_fatal ((void)0)

#else
/*
#define log_trace Log::full_log::trace
#define log_debug Log::full_log::debug
#define log_info Log::full_log::info
#define log_warning Log::full_log::warning
#define log_error Log::full_log::error
#define log_fatal Log::full_log::fatal
*/
#define log_trace Log::full_log::trace_
#define log_debug Log::full_log::debug_
#define log_info Log::full_log::info_
#define log_warning Log::full_log::warning_
#define log_error Log::full_log::error_
#define log_fatal Log::full_log::fatal_

//#include <boost/log/sources/severity_channel_logger.hpp>
//#include <string>

//#define LOG(severity, channel) BOOST_LOG_CHANNEL_SEV(Log::full_log::lg, severity, channel)

#include <ostream>

namespace Log
{
	namespace full_log
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
			inline std::ostream& operator << (std::ostream& strm, ELogLevel level)
			{
				static const char* strings[] =
				{
					"  TRACE",
					"   INFO",
					"  DEBUG",
					"WARNING",
					"  ERROR",
					"  FATAL"
				};

				if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
					strm << strings[level];
				else
					strm << static_cast<int>(level);

				return strm;
			}
		}

		//typedef boost::log::sources::severity_channel_logger<level::ELogLevel, std::string> tLogger;
		//tLogger lg;

		void set_up(level::ELogLevel lvl = level::FATAL ,const char* file = 0, const char* conf = 0);

		void trace(const char* format, ...);
		void debug(const char* format, ...);
		void info(const char* format, ...);
		void warning(const char* format, ...);
		void error(const char* format, ...);
		void fatal(const char* format, ...);

		void trace_(const char* format, const char* id, ...);
		void debug_(const char* format, const char* id, ...);
		void info_(const char* format, const char* id, ...);
		void warning_(const char* format, const char* id, ...);
		void error_(const char* format, const char* id, ...);
		void fatal_(const char* format, const char* id, ...);
	}
}

#endif // NLOG

#endif