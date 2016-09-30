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
#include <trivial/trivial_log.h>

#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/file.hpp>

#include <cstdarg>

#include <common/util/va_list_helpers.h>

namespace Log {	namespace trivial_log {

void set_up(level::ELogLevel lvl)
{
	using namespace boost::log;
	trivial::severity_level blvl = (trivial::severity_level) lvl;
	core::get()->set_filter(trivial::severity >= blvl);
}

void set_up(const char* file, level::ELogLevel lvl)
{
	using namespace boost::log;

	if(file && file != "") add_file_log(file);

	//if (file == "") add_file_log(file); tiemstamp

	trivial::severity_level blvl = (trivial::severity_level) lvl;
	core::get()->set_filter(trivial::severity >= blvl);
}

void set_up(const char* file, const char* conf, level::ELogLevel lvl)
{
	using namespace boost::log;

	if (file && file != "") add_file_log(file);

	trivial::severity_level blvl = (trivial::severity_level) lvl;
	core::get()->set_filter(trivial::severity >= blvl);
}

void trace(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	//char* cad = common::util::va_list_helper::to_cstring(format, args);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	//std::string cad = common::util::va_list_helper::to_string_sprintf(format, args);
	BOOST_LOG_TRIVIAL(trace) << cad;
	va_end(args);
}

void debug(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_TRIVIAL(debug) << cad;
	va_end(args);
}

void info(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_TRIVIAL(info) << cad;
	va_end(args);
}

void warning(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_TRIVIAL(warning) << cad;
	va_end(args);
}

void error(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_TRIVIAL(error) << cad;
	va_end(args);
}

void fatal(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_TRIVIAL(fatal) << cad;
	va_end(args);
}

}}
