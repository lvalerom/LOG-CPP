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
#ifndef __LOG_H
#define __LOG_H

//#define TRIVIAL_LOG
/*
#ifndef _WIN32
#define BOOST_ALL_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif
*/
#ifdef NLOG

#undef log_trace
#undef log_debug
#undef log_info
#undef log_warning
#undef log_error
#undef log_fatal

#undef log_setup

#define log_trace ((void)0)
#define log_debug ((void)0)
#define log_info ((void)0)
#define log_warning ((void)0)
#define log_error ((void)0)
#define log_fatal ((void)0)

#define log_setup ((void)0)

namespace Log
{
#if 0
	void log_set_up(unsigned int level = 5){}
	void log_set_up(unsigned int level = 5, const char* file = 0){}
#endif
	void log_set_up(unsigned int level = 5, const char* file = 0, const char* conf = 0){}
}

#else

#ifdef SIMPLE_LOG
#include "simple_log.h"

#undef log_setup

#define log_setup Log::log_set_up

namespace Log
{
#if 0
	void log_set_up(unsigned int level = 5){}
	void log_set_up(unsigned int level = 5, const char* file = 0){}
#endif
	void log_set_up(unsigned int level = 5, const char* file = 0, const char* conf = 0)
	{
		simple_log::set_up((simple_log::level::ELogLevel)level, file, conf);
	}
}

#elif defined TRIVIAL_LOG
#include "trivial_log.h"

#undef log_setup

#define log_setup Log::log_set_up

namespace Log
{
#if 0
	void log_set_up(unsigned int level = 5)
	{
		trivial_log::set_up((trivial_log::level::ELogLevel)level);
	}

	void log_set_up(unsigned int level = 5, const char* file = 0)
	{
		trivial_log::set_up(file,(trivial_log::level::ELogLevel)level);
	}
#endif
	void log_set_up(unsigned int level = 5, const char* file = 0, const char* conf = 0)
	{
		trivial_log::set_up(file,conf,(trivial_log::level::ELogLevel)level);
	}
}

#elif defined FULL_LOG
#include "full_log.h"

#undef log_setup

#define log_setup Log::log_set_up

namespace Log
{
#if 0
	void log_set_up(unsigned int level = 5){}
	void log_set_up(unsigned int level = 5, const char* file = 0){}
#endif
	void log_set_up(unsigned int level = 5, const char* file = 0, const char* conf = 0)
	{
		full_log::set_up((full_log::level::ELogLevel)level, file, conf);
	}
}

#elif !defined NLOG
#define NLOG
#undef log_trace
#undef log_debug
#undef log_info
#undef log_warning
#undef log_error
#undef log_fatal

#undef log_setup

#define log_trace ((void)0)
#define log_debug ((void)0)
#define log_info ((void)0)
#define log_warning ((void)0)
#define log_error ((void)0)
#define log_fatal ((void)0)

#define log_setup ((void)0)

namespace Log
{
#if 0
	void log_set_up(unsigned int level = 5){}
	void log_set_up(unsigned int level = 5, const char* file = 0){}
#endif
	void log_set_up(unsigned int level = 5, const char* file = 0, const char* conf = 0){}
}

#endif // FULL_LOG , TRIVIAL_LOG , SIMPLE_LOG

#endif // NLOG

#endif // __LOG_H
