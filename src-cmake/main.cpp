#include <iostream>

#include "logConfig.h"

#include "log/log.h"

int main(int argc, char* argv[])
{
    log_setup(0,"log_ids.conf","qwerty.log","log.conf",true);
    log_trace("canal") << "traza";
    log_trace("asdf") << "asdf traza";
    log_error("net") << "net traza";
    return 0;
}
