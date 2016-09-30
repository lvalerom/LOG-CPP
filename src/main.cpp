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
#ifndef _WIN32
//#define BOOST_ALL_DYN_LINK
#define BOOST_LOG_DYN_LINK
#endif

#include <stdio.h>

//#define TRIVIAL_LOG
//#define NLOG
//#define FULL_LOG
#define SIMPLE_LOG
//#include <include/log.h>
#include "log/log.h"

#if 0
#include <cstddef>
#include <string>
#include <ostream>
#include <fstream>
#include <iomanip>

#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared_object.hpp>
#include <boost/phoenix/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/core.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/basic_logger.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

enum severity_level
{
	trace,
	info,
	debug,
	warning,
	error,
	fatal
};

std::ostream& operator<< (std::ostream& strm, severity_level level)
{
	static const char* strings[] =
	{
		"trace",
		"info",
		"debug",
		"warning",
		"error",
		"fatal"
	};

	if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[level];
	else
		strm << static_cast< int >(level);

	return strm;
}
BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
#endif

//#pragma comment (lib, "../log/Debug/log.lib")

int main(int argc, char* argv[])
{
	printf("Hola mundo\n");
	
#if 0
	//trivial
	//logging::add_file_log("sample.log");
	logging::add_file_log
		(
		keywords::file_name = "sample_%N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0,0,0),
			keywords::format = "[%TimeStamp%]: %Message%"
		);
	logging::core::get()->set_filter
		(
		logging::trivial::severity >= logging::trivial::error
		);
	//BOOST_LOG_TRIVIAL(fatal) << "traza";

	logging::add_common_attributes();
	src::severity_logger< logging::trivial::severity_level > lg;
	BOOST_LOG_SEV(lg, logging::trivial::fatal) << "fatal trace";
	//Log::log_set_up("sample.log",(unsigned int)0);
	//Log::log_set_up((unsigned int)6);
	//log_trace << "prueba trivial";
	//log_fatal << "prueba fatal";


	

	src::logger lg;

	src::severity_logger<severity_level> slg;

	

	/*logging::core::get()->set_filter
		(
		//logging::trivial::severity >= logging::trivial::fatal
		//severity_level >= trace
		);*/

	logging::formatter fmt = expr::stream
		<< std::setw(6) << std::setfill('0') << line_id << std::setfill(' ')
		<< ": <" << severity << ">\t"
		<< expr::if_(expr::has_attr(tag_attr))
		[
			expr::stream << "[" << tag_attr << "] "
		]
	<< expr::smessage;

	typedef sinks::synchronous_sink<sinks::text_ostream_backend> text_sink;

	boost::shared_ptr<text_sink> sink = boost::make_shared<text_sink>();

	boost::shared_ptr<std::ostream> stream(&std::clog, boost::null_deleter());

	sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>("full.log"));
	sink->locked_backend()->add_stream(stream);
		
	sink->set_formatter(fmt);
	sink->set_filter(severity >= warning);

	logging::core::get()->add_sink(sink);
	logging::add_common_attributes();

	BOOST_LOG_SEV(slg, warning) << "trace";

	BOOST_LOG(lg) << "logger";
#endif
	log_setup(0,0,"log.conf");

	log_trace("testing", "Default");
	log_trace("testing", "main");
	log_trace("testing", "main:Uno");
	log_trace("testing", "main:Dos");

	//LOG(trace, "Default") << "hola mundo";
	
	/*
	//log_trace("hola hola %i %f %s %c","id",123,123.09,"hola", 'A');
	log_trace("trace", "main");
	log_debug("debug %s _id","id","hola");
	log_info("info _main:Uno","main:Uno");
	log_warning("warning _main:Dos","main:Dos");
	log_error("eeeeeeeeeerror _id","id");
	log_fatal("fataaaaaaaal _id","id");
	
	log_trace("trace %s", "main");
	log_debug("debug %s %s _id", "id", "hola");
	log_info("info %s _main:Uno", "main:Uno");
	log_warning("warning %s _main:Dos", "main:Dos");
	log_error("eeeeeeeeeerror %s _id", "id");
	log_fatal("fataaaaaaaal %s _id", "id");
	*/
	return 0;
}
