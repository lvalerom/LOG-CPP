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
#include <iostream>

#include "logConfig.h"

#include "log/log.h"

int main(int argc, char* argv[])
{
    log_setup(0,"cids.conf","clog.conf","qwerty.log",true);
    log_trace("canal") << "traza";
    log_error("asdf") << "asdf traza";
    log_debug("net") << "net traza";
    return 0;
}
