/*
    This is the command parser for the terminal.
    It will parse the command string and execute
    the corresponding function.

    Files on disk need to end with extension .sh
    to be accepted. This parser accepts IF statements
    similar to C language.

    Below is an example of possible syntax:


--------------------------------------------------------------------

// Example script with tags, goto statements, and if statements

#tag start // define a tag named "start"

// set up LED pin as output
pinMode(13, OUTPUT);

// loop through the sequence of blinking the LED
int delay_time = 500;
for (int i = 0; i < 10; i++) {
    digitalWrite(13, HIGH);
    delay(delay_time);
    digitalWrite(13, LOW);
    delay(delay_time);
}

// check if user input is "yes"
if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input == "yes") {
        goto confirmation;
    }
}

#tag confirmation // define a tag named "confirmation"

// print confirmation message
Serial.println("Thank you for your confirmation!");

// wait for user input
while (!Serial.available()) {
    delay(100);
}

// check if user input is "done"
String input = Serial.readStringUntil('\n');
if (input == "done") {
    goto end;
} else {
    goto confirmation;
}

#tag end // define a tag named "end"

// print final message
Serial.println("Script execution complete.");

--------------------------------------------------------------------

*/

#include <Arduino.h> // required for Arduino functions and types
#include <string.h> // required for string functions
#include "SdFat.h"
#include <map> // include the map header file

#include "commands_disk.hpp"

bool evalCondition(String condition);

void func_parse_script(char *args, Stream *response) {
    if (args == NULL || args[0] == '\0') {
        response->println("No file specified");
        return;
    }

    String path = getPath(args);
    // open file in read mode
    File32 file = sd.open(path, FILE_READ);
    if (!file) {
        response->print("Failed to open file: " + path);
        return;
    }

    // define a map to store tag names and line numbers
    std::map<String, int> tags;
    // keep track of the current line number
    int line_number = 0;

    while (file.available()) {
        String line = file.readStringUntil('\n');
        // remove leading/trailing whitespaces
        line.trim();

        // remove comments (starting with '#' or '//')
        int comment_pos = line.indexOf('#');
        if (comment_pos != -1) {
            line = line.substring(0, comment_pos);
        }
        comment_pos = line.indexOf("//");
        if (comment_pos != -1) {
            line = line.substring(0, comment_pos);
        }

        // skip empty lines
        if (line.length() == 0) {
            continue;
        }

        // handle tags (lines starting with '@')
        if (line.charAt(0) == '@') {
            String tag_name = line.substring(1);
            tags[tag_name] = line_number;
            continue;
        }

        // split the line into command and arguments
        int space_pos = line.indexOf(' ');
        String command, arguments;
        if (space_pos == -1) {
            command = line;
        } else {
            command = line.substring(0, space_pos);
            arguments = line.substring(space_pos + 1);
        }

        // execute the command
        if (command == "delay") {
            // usage: delay <milliseconds>
            long ms = arguments.toInt();
            delay(ms);
        } else if (command == "print") {
            // usage: print <message>
            response->println(arguments);
        } else if (command == "pinMode") {
            // usage: pinMode <pin> <mode>
            int pin = arguments.substring(0, arguments.indexOf(' ')).toInt();
            int mode = arguments.substring(arguments.indexOf(' ') + 1).toInt();
            pinMode(pin, mode);
        } else if (command == "if") {
            // usage: if <condition> <tag>
            int space_pos = arguments.indexOf(' ');
            String condition = arguments.substring(0, space_pos);
            String tag_name = arguments.substring(space_pos + 1);
            bool condition_result = evalCondition(condition);
            if (condition_result) {
                // jump to the specified tag
                line_number = tags[tag_name];
                file.seek(line_number);
                continue;
            }
        } else if (command == "goto") {
            // usage: goto <tag>
            String tag_name = arguments;
            if (tags.count(tag_name)) {
                // jump to the specified tag
                line_number = tags[tag_name];
                file.seek(line_number);
                continue;
            } else {
                response->println("Unknown tag: " + tag_name);
            }
        } else {
            response->println("Unknown command: " + command);
        }

        // increment the line number
        line_number++;
    }
    file.close();
}

bool evalCondition(String condition) {
    // supported operators: ==, !=, <, >, <=, >=
    int op_pos = condition.indexOf("==");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 2);
        return lhs == rhs;
    }
    op_pos = condition.indexOf("!=");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 2);
        return lhs != rhs;
    }
    op_pos = condition.indexOf("<=");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 2);
        return lhs.toInt() <= rhs.toInt();
    }
    op_pos = condition.indexOf(">=");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 2);
        return lhs.toInt() >= rhs.toInt();
    }
    op_pos = condition.indexOf("<");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 1);
        return lhs.toInt() < rhs.toInt();
    }
    op_pos = condition.indexOf(">");
    if (op_pos != -1) {
        String lhs = condition.substring(0, op_pos);
        String rhs = condition.substring(op_pos + 1);
        return lhs.toInt() > rhs.toInt();
    }
    return false; // invalid condition
}