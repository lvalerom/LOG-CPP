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
#ifndef __XMLPARSER_H
#define __XMLPARSER_H

namespace common
{
	namespace util
	{
		namespace parser
		{
			struct full_log_config_info;
		}
		namespace XMLParser
		{
			bool set_up_full_log(const char* file, parser::full_log_config_info* info);
		}
	}
}

#endif
