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
#include <full/full_log.h>

#include <common/util/XMLParser/XmlParser.h>
#include <common/util/parser.h>

#include <map>
#include <cstddef>
#include <string>
//#include <ostream>
#include <fstream>
#include <iomanip>

#include <cstdarg>

#include <common/util/va_list_helpers.h>

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
#include <boost/log/support/date_time.hpp>

namespace Log { namespace full_log {

//----------------------------------

namespace logging = boost::log;
namespace sinks = boost::log::sinks;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace attrs = boost::log::attributes;
namespace keywords = boost::log::keywords;

//----------------------------------
#if 0
std::ostream& operator<< (std::ostream& strm, level::ELogLevel level)
{
	static const char* strings[] =
	{
		"  trace",
		"   info",
		"  debug",
		"warning",
		"  error",
		"  fatal"
	};

	if (static_cast< std::size_t >(level) < sizeof(strings) / sizeof(*strings))
		strm << strings[level];
	else
		strm << static_cast< int >(level);

	return strm;
}

void default_formatter(logging::record_view const& rec, logging::formatting_ostream& strm)
{
	//strm << logging::extract< unsigned int >("LineID", rec) << ": ";
	//strm << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S");
	//strm << expr::attr<logging::attributes::current_thread_id::value_type>("ThreadID");
	strm << "[" << logging::extract <level::ELogLevel>("Severity", rec) << "]: ";
	strm << rec[expr::smessage];
}
#endif
//----------------------------------

BOOST_LOG_ATTRIBUTE_KEYWORD(line_id, "LineID", unsigned int)
BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", level::ELogLevel)
BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
//BOOST_LOG_ATTRIBUTE_KEYWORD(thread_id, "ThreadID", )

#if 0

static std::string default_string_format("%TimeStamp% (Thread id: %ThreadID%) [%Severity%]: %Message%");

static logging::formatter default_stream_format = expr::stream << std::setw(6) << std::setfill('0') << line_id << std::setfill(' ')
<< ": <" << severity << ">\t"
<< expr::if_(expr::has_attr(tag_attr))
[
	expr::stream << "[" << tag_attr << "] "
]
<< expr::smessage;

typedef void(*formatter)(logging::record_view const&, logging::formatting_ostream&);
static formatter log_format = default_formatter;

#endif

static logging::formatter default_stream_format = expr::stream
<< expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S")
<< " (Thread id: " << expr::attr<logging::attributes::current_thread_id::value_type>("ThreadID") << ")"
<< expr::if_(expr::attr<level::ELogLevel>("Severity"))[expr::stream << " [" << severity << "]"]
<< expr::if_(expr::attr<std::string>("Channel")) [expr::stream << " (" << channel << ")"]
<< ": " << expr::smessage;

static bool rotation = false;
static unsigned long rotation_size = 10 * 1024 * 1024;
static unsigned long rotation_time = 0;

static std::string default_sink_path("full_log");

//----------------------------------

typedef src::severity_channel_logger<level::ELogLevel, std::string> tLogger;
typedef expr::channel_severity_filter_actor<std::string, level::ELogLevel> tLevel_filter;
typedef sinks::synchronous_sink<sinks::text_ostream_backend> tText_sink;
typedef sinks::synchronous_sink<sinks::text_file_backend> tFile_sink;

static tLogger lg;
static tLevel_filter filter_array = expr::channel_severity_filter(channel, severity);

#if 0
struct slogger
{
	level::ELogLevel filter_level;
	std::string sink_path;
	src::severity_logger<level::ELogLevel> slg;
};
#endif

boost::shared_ptr<std::ostream> console_stream(&std::clog, boost::null_deleter());

typedef std::map<const std::string, boost::shared_ptr<tFile_sink>> tSinks_map;
static tSinks_map sinks_map;

//----------------------------------

static common::util::parser::full_log_config_info getInfo(const char* conf)
{
	using namespace common::util;
	parser::full_log_config_info info;
	if (parser::isTXT(conf)){
		if (!parser::set_up_full_log(conf, &info)){
			fprintf(stderr, "Error while loading the file %s\n", conf);
		}
	}
	else {
		if (!XMLParser::set_up_full_log(conf, &info)){
			fprintf(stderr, "Error while loading the file %s\n", conf);
		}
	}
	return info;
}

static void set_up_format(const common::util::parser::full_log_config_info& info)
{
	using namespace common::util::parser;
	logging::formatter fmt = expr::stream;
	switch (info.fmt.timestamp)
	{
	case MILLISECONDS:
		break;
	case TIME:
		expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%H:%M:%S");
		break;
	case LONG_TIME:
		fmt << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S");
		break;
	case SECONDS:
	default:
		//fmt << expr::format_date_time<boost::posix_time::ptime>("TimeStamp", "%S");
		break;
	};

	if (info.fmt.ID){

	}
}

static level::ELogLevel string_to_level(const std::string & str)
{
	if (str == "TRACE") return (level::TRACE);
	else if (str == "DEBUG") return (level::DEBUG);
	else if (str == "INFO") return (level::INFO);
	else if (str == "WARNING") return (level::WARNING);
	else if (str == "ERROR") return (level::ERROR);
	else if (str == "FATAL") return (level::FATAL);
	return (static_cast<level::ELogLevel> (5));
	//TODO catch exception
	//return (static_cast<level::ELogLevel> (std::stoi(str)));
}

static void addChannel(const std::string& name, const common::util::parser::slogger_info& info)
{
	filter_array[name] = string_to_level(info.filter_level);	
}

static void setDefaultSinks(const std::string& filename, const common::util::parser::rotation_info& rot)
{
	if (filename != ""){
		default_sink_path = filename;
	}

	if (rot.rotation){

		boost::shared_ptr<sinks::text_file_backend> backend =
			boost::make_shared<sinks::text_file_backend>(
			keywords::file_name = default_sink_path + "%5N.log",
			keywords::rotation_size = rot.rotation_size,
			keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)
			);
		boost::shared_ptr<tFile_sink> sink(new tFile_sink(backend));

		boost::shared_ptr<tText_sink> sink2 = boost::make_shared<tText_sink>();

		sink2->locked_backend()->add_stream(console_stream);
		sink2->set_formatter(default_stream_format);
		sink2->set_filter(filter_array);
		
		sink->set_formatter(default_stream_format);
		sink->set_filter(filter_array);
		//sink->set_filter(severity >= string_to_level(info.filter_level));
		logging::core::get()->add_sink(sink);
		logging::core::get()->add_sink(sink2);
	}
	else {
		boost::shared_ptr<tText_sink> sink = boost::make_shared<tText_sink>();

		sink->locked_backend()->add_stream(console_stream);
		//FIXME seems to fail on linux
		//sink->locked_backend()->add_stream(boost::make_shared<std::ofstream>(default_sink_path+".log"));
		sink->set_formatter(default_stream_format);
		//sink->set_filter(severity >= string_to_level(info.filter_level));
		logging::core::get()->add_sink(sink);
	}
}

static void addSink(const std::string& name, const common::util::parser::slogger_info& info)
{
	//TODO
	//info.sink_path = (info.sink_path != "") ? info.sink_path : default_sink_path;

	boost::shared_ptr<sinks::text_file_backend> backend =
		boost::make_shared<sinks::text_file_backend>(
		keywords::file_name = info.sink_path + "%5N.log",
		keywords::rotation_size = info.rotation.rotation_size,
		keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0)
		);
	boost::shared_ptr<tFile_sink> sink(new tFile_sink(backend));

	//boost::shared_ptr<tText_sink> sink2 = boost::make_shared<tText_sink>();

	//sink2->locked_backend()->add_stream(console_stream);
	//sink2->locked_backend()->add_stream(boost::make_shared<std::ofstream>(info.sink_path));
	sink->set_formatter(default_stream_format);
	sink->set_filter(severity >= string_to_level(info.filter_level));

	tSinks_map::iterator it(sinks_map.find(name));
	if (it != sinks_map.end()){
		it->second = sink;
	}
	else{
		sinks_map.insert(std::make_pair(name, sink));
	}

	logging::core::get()->add_sink(sink);
}

void set_up(level::ELogLevel lvl, const char* file, const char* conf)
{
	using namespace common::util;
	if (conf){
		parser::full_log_config_info info = getInfo(conf);
		
		//TODO set_up_format(info);
		addChannel("Default", info.default_info);
		
		logging::add_common_attributes();

		if (info.id_conf){
			if (info.multiple_sinks){
				//TODO
			}
			else{
				common::util::parser::idInfo_map::const_iterator it(info.id_info.begin());
				for (; it != info.id_info.end(); ++it){
					addChannel(it->first, it->second);
				}
			}
		}
		setDefaultSinks(info.default_info.sink_path, info.default_info.rotation);
	}

	if (file && file != ""){

	}
}

void trace_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::TRACE) << cad;
	va_end(args);
}

void debug_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::DEBUG) << cad;
	va_end(args);
}
void info_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::INFO) << cad;
	va_end(args);
}
void warning_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::WARNING) << cad;
	va_end(args);
}
void error_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::ERROR) << cad;
	va_end(args);
}
void fatal_(const char* format, const char* id, ...)
{
	va_list args;
	va_start(args, format);
	char* cad = common::util::va_list_helper::to_cstring_sprintf(format, args);
	BOOST_LOG_CHANNEL_SEV(lg, id, level::FATAL) << cad;
	va_end(args);
}

void trace(const char* format, ...)
{

}

void debug(const char* format, ...){}
void info(const char* format, ...){}
void warning(const char* format, ...){}
void error(const char* format, ...){}
void fatal(const char* format, ...){}

}}
