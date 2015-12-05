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
#ifndef __SIMPLE_LOG_H
#define __SIMPLE_LOG_H

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

#define log_trace Log::simple_log::trace
#define log_debug Log::simple_log::debug
#define log_info Log::simple_log::info
#define log_warning Log::simple_log::warning
#define log_error Log::simple_log::error
#define log_fatal Log::simple_log::fatal

namespace Log
{
	namespace simple_log
	{
		namespace level
		{
			enum ELogLevel
			{
				TRACE = 0,
				DEBUG = 1,
				INFO = 2,
				WARNING = 3,
				_ERROR = 4,
				FATAL = 5
			};
		}

		void set_up(level::ELogLevel lvl = level::FATAL, const char* file = 0, const char* conf = 0);

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