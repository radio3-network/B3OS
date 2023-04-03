#ifndef COMMAND_PARSER_HPP
#define COMMAND_PARSER_HPP

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "SdFat.h"
#include "sdios.h"

#include "terminal.hpp"

extern void func_parse_script(char *args, Stream *response);
extern bool evalCondition(String condition);

#endif