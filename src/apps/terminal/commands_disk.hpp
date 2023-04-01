#ifndef COMMANDS_DISK_HPP
#define COMMANDS_DISK_HPP

#include <Shellminator.hpp>
#include "Shellminator-IO.hpp"

#include "SdFat.h"
#include "sdios.h"

#include "terminal.hpp"

extern String currentPath;
extern SdFat sd;

extern void card_initialize();
extern void func_formatCard(char *args, Stream *response );
extern String getPath(char *args);
extern void func_touch(char *args, Stream *response);
extern void func_mkdir(char *args, Stream *response );
extern void func_rm(char *args, Stream *response );
extern void func_ls(char *args, Stream *response );
extern String getParentFolder(String currentPathEdited);
extern void func_cd(char *args, Stream *response);

#endif