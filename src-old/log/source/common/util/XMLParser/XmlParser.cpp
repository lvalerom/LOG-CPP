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
#include <common/util/XMLParser/XmlParser.h>

#include <fstream>
#include <vector>

#include <common/util/XMLParser/RapidXml/rapidxml.hpp>

#include <common/util/parser.h>

namespace common { namespace util { namespace XMLParser {

bool set_up_full_log(const char* file, parser::full_log_config_info* info)
{
	using std::string;
	using std::ifstream;
	using namespace parser;

	info->id_conf = false;
	info->multiple_sinks = false;

	if(file){
		ifstream in(file);
		if (!in) return (false);
		string xml("");
		while (in){
			string line;
			getline(in, line);
			xml += line;
		}

		using namespace rapidxml;

		xml_document<> doc;
		std::vector<char> text(xml.begin(), xml.end());
		text.push_back('\0');

		try{
			doc.parse<0>(&text[0]);
		}
		catch (rapidxml::parse_error e){
			return (false);
		}

		xml_node<>* root = doc.first_node("log");
		xml_node<>* format = root->first_node("format");
		xml_node<>* conf   = root->first_node("configuration");
		xml_node<>* def    = root->first_node("default");

		if (format){
			xml_node<>* fmt_node = format->first_node("timestamp");
			if (fmt_node){
				string value = fmt_node->first_attribute("value")->value();
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
			fmt_node = format->first_node("level");
			if (fmt_node){
				string value = fmt_node->first_attribute("value")->value();
				if (value == "true"){
					info->fmt.level = true;
				}
				else {
					info->fmt.level = false;
				}
			}
			fmt_node = format->first_node("threadId");
			if (fmt_node){
				string value = fmt_node->first_attribute("value")->value();
				if (value == "true"){
					info->fmt.threadId = true;
				}
				else {
					info->fmt.threadId = false;
				}
			}
			fmt_node = format->first_node("ID");
			if (fmt_node){
				string value = fmt_node->first_attribute("value")->value();
				if (value == "true"){
					info->fmt.ID = true;
				}
				else {
					info->fmt.ID = false;
				}
			}
		}

		if (conf){
			xml_node<>* conf_node = conf->first_node("id");
			if (conf_node){
				string value = conf_node->first_attribute("value")->value();
				if (value == "true"){
					info->id_conf = true;
				}
				else {
					info->id_conf = false;
				}
			}
			conf_node = conf->first_node("multiple_sinks");
			if (conf_node){
				string value = conf_node->first_attribute("value")->value();
				if (value == "true"){
					info->multiple_sinks = true;
				}
				else {
					info->multiple_sinks = false;
				}
			}
		}

		if (def){
			xml_node<>* def_node = def->first_node("sink");
			if (def_node){
				string value = def_node->first_attribute("value")->value();
				info->default_info.sink_path = value;
			}
			def_node = def->first_node("level");
			if (def_node){
				string value = def_node->first_attribute("value")->value();
				info->default_info.filter_level = value;
			}
			def_node = def->first_node("rotation");
			if (def_node){
				string value = def_node->first_attribute("value")->value();
				if (value == "true"){
					info->default_info.rotation.rotation = true;
					info->default_info.rotation.rotation_size = 10 * 1024 * 1024;
					info->default_info.rotation.rotation_time = 0;
				}
				else{
					info->default_info.rotation.rotation = false;
				}
			}
			def_node = def->first_node("rotation_size");
			if (def_node){
				string value = def_node->first_attribute("value")->value();
				info->default_info.rotation.rotation_size = std::strtoul(value.c_str(), 0, 0);
			}
			def_node = def->first_node("rotation_time");
			if (def_node){
				string value = def_node->first_attribute("value")->value();
				info->default_info.rotation.rotation_time = std::strtoul(value.c_str(), 0, 0);
			}
		}

		if (info->id_conf){
			xml_node<>* idConf_node = root->first_node("id_configuration");
			xml_node<>* channel_node = idConf_node->first_node("channel");
			while (channel_node){

				string name = channel_node->first_attribute("name")->value();

				xml_node<>* channelComp_node = channel_node->first_node("level");
				if (channelComp_node){
					string value = channelComp_node->first_attribute("value")->value();
				}
				channelComp_node = channel_node->first_node("sink");
				channelComp_node = channel_node->first_node("rotation");
				channelComp_node = channel_node->first_node("rotation_size");
				channelComp_node = channel_node->first_node("rotation_time");
				xml_node<>* child = channel_node->first_node("child");
				while (child){
					child = child->next_sibling();
				}
				channel_node = channel_node->next_sibling();
			}
		}

		return (true);
	}

	return (false);
}
}}}
