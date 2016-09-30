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
#ifndef __PARSER_H
#define __PARSER_H

#include <string>
#include <map>

namespace common
{
	namespace util
	{
		namespace parser
		{

			enum timestamp_fmt
			{
				SECONDS,
				MILLISECONDS,
				TIME,
				LONG_TIME
			};

			struct rotation_info
			{
				bool rotation;
				unsigned long rotation_size;
				unsigned long rotation_time;
			};

			struct slogger_info
			{
				std::string filter_level;
				std::string sink_path;
				rotation_info rotation;
			};

			struct sFormat{
				timestamp_fmt timestamp;
				bool level;
				bool threadId;
				bool ID;
			};

			typedef std::map<std::string, slogger_info> idInfo_map;

			struct full_log_config_info
			{
				slogger_info default_info;
				bool id_conf;
				bool multiple_sinks;
				sFormat fmt;
				idInfo_map id_info;
			};

			bool isTXT(const char* file);
			bool set_up_full_log(const char* file, full_log_config_info* info);
		}
	}
}

#endif
