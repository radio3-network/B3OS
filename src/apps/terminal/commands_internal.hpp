#ifndef COMMANDS_INTERNAL_HPP
#define COMMANDS_INTERNAL_HPP

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "terminal.hpp"

// Function declarations
void func_whoami(char *args, Stream *response );
void func_wait(char *args, Stream *response );
void func_clearWiFi(char *args, Stream *response );
void func_reboot(char *args, Stream *response );
void func_echo(char *args, Stream *response );
void func_formatCard(char *args, Stream *response );
void func_version(char *args, Stream *response );
void func_logoWiFi(char *args, Stream *response );
//void func_exit(char *args, Stream *response );
void func_beepWiFi(char *args, Stream *response );

#endif
