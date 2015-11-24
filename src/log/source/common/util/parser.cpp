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
#include <common/util/parser.h>

#include <stdlib.h>
#include <fstream>
#include <string>

namespace common { namespace util { namespace parser {

bool isTXT(const char* file)
{
	std::ifstream in(file);
	std::string line;
	std::getline(in, line);
	bool ret = line == "LOG CONFIGURATION TXT FILE";
	in.close();
	return (ret);
}

bool set_up_full_log(const char* file, full_log_config_info* info)
{

	using std::string;
	using std::ifstream;
	
	info->id_conf = false;
	info->multiple_sinks = false;

	if (file){
		ifstream in(file);
		if (!in) return (false);

		string line; getline(in, line);

		while (in && !in.eof()){
			char start = in.peek();
			if (start == '#' || start == '\n'){
				string comment; getline(in, comment);
			}
			else if (start == '$'){
				string key, value;
				getline(in, key, '=');
				if (key != ""){
					getline(in,value);
					if (value != ""){
						if (key == "$timestamp"){
							if (value == "seconds"){
								info->fmt.timestamp = SECONDS;
							}
							else if (value == "milliseconds"){
								info->fmt.timestamp = MILLISECONDS;
							}
							else if (value == "time"){
								info->fmt.timestamp = TIME;
							}
							else if (value == "long_time"){
								info->fmt.timestamp = LONG_TIME;
							}
							else{
								fprintf(stderr, "Error invalid timestamp format\n");
							}
						}
						else if (key == "$level"){
							if (value == "true"){
								info->fmt.level = true;
							}
							else {
								info->fmt.level = false;
							}
						}
						else if (key == "$threadId"){
							if (value == "true"){
								info->fmt.threadId = true;
							}
							else {
								info->fmt.threadId = false;
							}
						}
						else if (key == "$ID"){
							if (value == "true"){
								info->fmt.ID = true;
							}
							else {
								info->fmt.ID = false;
							}
						}
						else{
							fprintf(stderr, "Error invalid format\n");
						}
					}
					else{
						fprintf(stderr, "Error invalid format\n");
					}
				}
				else {
					fprintf(stderr, "Error invalid format\n");
				}
			}
			else if (start == '&'){
				string key, value;
				getline(in, key, '=');
				if (key != ""){
					getline(in, value);
					if (value != ""){
						if (key == "&id"){
							if (value == "true"){
								info->id_conf = true;
							}
							else {
								info->id_conf = false;
							}
						}
						else if (key == "&multiple_sinks"){
							if (value == "true"){
								info->multiple_sinks = true;
							}
							else {
								info->multiple_sinks = false;
							}
						}
						else{
							fprintf(stderr, "Error invalid main configuration format\n");
						}
					}
					else{
						fprintf(stderr, "Error invalid main configuration format\n");
					}
				} else{
					fprintf(stderr, "Error invalid main configuration format\n");
				}
			}
			else if (start == '%'){
				string key, value;
				getline(in, key, '=');
				if (key != ""){
					getline(in, value);
					if (value != ""){
						if (key == "%default_sink"){
							info->default_info.sink_path = value;
						}
						else if (key == "%default_level"){
							info->default_info.filter_level = value;
						}
						else if (key == "%default_rotation"){
							if (value == "true"){
								info->default_info.rotation.rotation = true;
								info->default_info.rotation.rotation_size = 10 * 1024 * 1024;
								info->default_info.rotation.rotation_time = 0;
							}
							else{
								info->default_info.rotation.rotation = false;
							}
						}
						else if (key == "%default_rotation_size"){
							info->default_info.rotation.rotation_size = std::strtoul(value.c_str(),0,0);
						}
						else if (key == "%default_rotation_time"){
							info->default_info.rotation.rotation_time = std::strtoul(value.c_str(), 0, 0);
						}
						else{
							fprintf(stderr, "Error invalid default configuration format\n");
						}
					}
					else{
						fprintf(stderr, "Error invalid default configuration format\n");
					}
				}
				else{
					fprintf(stderr, "Error invalid default configuration format\n");
				}
			}
			else if (info->id_conf){
				if (info->multiple_sinks){
					string id, level, sink;
					getline(in, id, '=');
					getline(in, level, '=');
					getline(in, sink);
					if (id != "" && level != ""){
						idInfo_map::iterator it(info->id_info.find(id));
						if (it != info->id_info.end()){
							it->second.filter_level = level;
							it->second.sink_path = sink;
							it->second.rotation.rotation =  info->default_info.rotation.rotation;
							it->second.rotation.rotation_size = info->default_info.rotation.rotation_size;
							it->second.rotation.rotation_time = info->default_info.rotation.rotation_time;
						}
						else{
							slogger_info loggerInfo;
							loggerInfo.filter_level = level;
							loggerInfo.sink_path = sink;
							loggerInfo.rotation.rotation = info->default_info.rotation.rotation;
							loggerInfo.rotation.rotation_size = info->default_info.rotation.rotation_size;
							loggerInfo.rotation.rotation_time = info->default_info.rotation.rotation_time;
							info->id_info.insert(std::make_pair(id, loggerInfo));
						}
					}
					else{
						fprintf(stderr, "Error invalid id configuration format\n");
					}
				}
				else{
					string id, level;
					getline(in, id, '=');
					getline(in, level);
					if (id != "" && level != ""){
						idInfo_map::iterator it(info->id_info.find(id));
						if (it != info->id_info.end()){
							it->second.filter_level = level;
							it->second.sink_path = "";
							it->second.rotation.rotation = info->default_info.rotation.rotation;
							it->second.rotation.rotation_size = info->default_info.rotation.rotation_size;
							it->second.rotation.rotation_time = info->default_info.rotation.rotation_time;
						}
						else{
							slogger_info loggerInfo;
							loggerInfo.filter_level = level;
							loggerInfo.sink_path = "";
							loggerInfo.rotation.rotation = info->default_info.rotation.rotation;
							loggerInfo.rotation.rotation_size = info->default_info.rotation.rotation_size;
							loggerInfo.rotation.rotation_time = info->default_info.rotation.rotation_time;
							info->id_info.insert(std::make_pair(id, loggerInfo));
						}
					}
					else{
						fprintf(stderr, "Error invalid id configuration format\n");
					}
				}
			}
			else{
				string line; getline(in, line);
			}
		}

		return (true);
	}

	return (false);
}

}}}