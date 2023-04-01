#include <Arduino.h>
#include <SD.h>

#define MAX_BUFFER_SIZE 1024

const char CMD_HELP[] = "help";
const char CMD_OPEN[] = "open";
const char CMD_SAVE[] = "save";
const char CMD_SAVEAS[] = "saveas";
const char CMD_INSERT[] = "insert";
const char CMD_DELETE[] = "delete";
const char CMD_MOVE[] = "move";
const char CMD_QUIT[] = "quit";

const char MSG_CURSOR[] = "Cursor position: %d\n";
const char MSG_HELP[] = "Available commands:\n"
                        "help                : Display this message\n"
                        "open <filename>     : Open a file for editing\n"
                        "save                : Save changes to the current file\n"
                        "saveas <filename>   : Save changes to a new file\n"
                        "insert <text>       : Insert text at the current cursor position\n"
                        "delete <count>      : Delete the next <count> characters\n"
                        "move <count>        : Move the cursor <count> positions to the right\n"
                        "quit                : Quit the editor\n";
const char MSG_OPEN[] = "Opening file: %s\n";
const char MSG_OPEN_ERR[] = "Error opening file: %s\n";
const char MSG_SAVE[] = "Saving changes to file...\n";
const char MSG_SAVE_ERR[] = "Error saving changes to file: %s\n";
const char MSG_SAVE_AS[] = "Saving changes to file: %s...\n";
const char MSG_SAVE_AS_ERR[] = "Error saving changes to file: %s\n";
const char MSG_INSERT[] = "Enter text to insert: ";
const char MSG_DELETE[] = "Enter count of characters to delete: ";
const char MSG_MOVE[] = "Enter count of positions to move the cursor: ";
const char MSG_QUIT[] = "Are you sure you want to quit? (y/n)\n";

char buffer[MAX_BUFFER_SIZE];
int cursor_pos = 0;