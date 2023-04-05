#include "apps/terminal/commands_disk.hpp"
#include "wrench.h"

/**
 * Run C-like scripts using Wrench from http://northarc.com/wrench/www/
 *
 * Arduino devices have limited memory for uploading programs.
 * This means that all programs need to be included inside the
 * firmware and uploaded manually by the end-user.
 *
 * This is nowhere practical for the purpose of B3OS where end-users
 * should be permitted to install/uninstall/upgrade individual apps
 * without needing to flash a completely new firmware.
 *
 * So far (in 2023) it is still not possible to run adhoc compiled
 * binaries. The best alternative to these compiled binaries are
 * script written isn C-like language that is familiar.
 *
 * C-like scripts that can effectively:
 *  + create UIs using LVGL
 *  + integrate with bluetooth, wifi, lora and other peripherals
 *  + escape the flash-memory restrictions
 *  + permit people to change/adapt the code
 *
 * The Wrench C-like interpreter matches all these requirements
 * and even more that enable B3OS to launch external apps.
 *
 *       A hearfelt thanks to Curt Hartung for his excellent work.
 *
 */

const char* wrenchCode =
    "print( \"Hello World!\\n\" );"
    "for( i=0; i<10; i++ )        "
    "{                            "
    "    print( i );              "
    "}                            "
    "print(\"\\n\");              ";


void print(WRState* w, const WRValue* argv, const int argn, WRValue& retVal, void* usr) {
    char buf[1024];
    for (int i = 0; i < argn; ++i) {
        printf("%s", argv[i].asString(buf, 1024));
    }
}

void func_parse_c_script(char* args, Stream* response) {
    
    WRState* w = wr_newState();              // create the state
    wr_registerFunction(w, "print", print);  // bind a function

    unsigned char* outBytes;  // compiled code is alloc'ed
    int outLen;

    //int err = wr_compile(wrenchCode, strlen(wrenchCode), &outBytes, &outLen);  // compile it
    /*
    if (err == 0) {
        wr_run(w, outBytes);  // load and run the code!
        delete[] outBytes;    // clean up
    }


    wr_destroyState(w);
    */
}