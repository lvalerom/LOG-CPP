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
#include <common/util/va_list_helpers.h>

#include <stdio.h>
#include <string.h>
#include <string>
#include <sstream>

namespace common { namespace util { namespace va_list_helper {

const char const fmt_specifier = '%';

char* to_cstring_sprintf(const char* fmt, va_list args)
{
	if (!fmt) return ("");
	char* buffer = new char[256];
	if(vsprintf(buffer, fmt, args)) return (buffer);
	return ("");
}

std::string to_string_sprintf(const char* fmt, va_list args)
{
	if (!fmt) return ("");
	char* buffer = new char[256];
	if (vsprintf(buffer, fmt, args)) return (buffer);
	return ("");
}

char* to_cstring(const char* fmt, va_list args)
{
	if (!fmt) return ("");
#if 0
	size_t len = strlen(fmt);
	char* ret = new char[len];

	
	unsigned int i = 0;
	while ( *fmt != '\0' ){

		if (*fmt == fmt_specifier){
			//TODO 
			*fmt++;
			if (*fmt == '\0') break;


		} else {
			ret[i] = *fmt;
		}

		++i;
		*fmt++;
	}
	//strcpy to a new resized array
	ret[i] = '\0';
#endif

	std::stringstream ss("");

	while ( *fmt != '\0' ){
		if (*fmt == fmt_specifier){
			*fmt++;
			if (*fmt == '\0') break;

			switch (*fmt)
			{
			case 'd':
			case 'i':
				ss << (va_arg(args, int));
				break;
			case 'u':
				//ss << (va_arg(args, int));
				break;
			case 'o':
				//ss << (va_arg(args, int));
				break;
			case 'x':
				//ss << (va_arg(args, int));
				break;
			case 'X':
				//ss << (va_arg(args, int));
				break;
			case 'f':
				ss << (float)(va_arg(args, double));
				break;
			case 'F':
				ss << (float)(va_arg(args, double));
				break;
			case 'e':
				//ss << (va_arg(args, int));
				break;
			case 'E':
				//ss << (va_arg(args, int));
				break;
			case 'g':
				//ss << (va_arg(args, int));
				break;
			case 'G':
				//ss << (va_arg(args, int));
				break;
			case 'a':
				//ss << (va_arg(args, int));
				break;
			case 'A':
				//ss << (va_arg(args, int));
				break;
			case 'c':
				ss << (char)(va_arg(args, int));
				break;
			case 's':
			{
				std::string str (va_arg(args, char *));
				ss << str;
				break;
			}
			case 'p':
				//ss << (va_arg(args, int));
				break;
			case 'n':
				//ss << (va_arg(args, int));
				break;
			case '%':
				ss << '%';
				break;
			default: break;
			}
		} else {
			//ret.append(1,*fmt);
			ss << *fmt;
		}
		*fmt++;
	}

	size_t len = ss.str().length();
	char* ret = new char[len];

	strcpy(ret, ss.str().c_str());

	return (ret);
}

}}}