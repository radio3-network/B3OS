#ifndef CHOOSE_H
#define CHOOSE_H

/*

This is the place to choose the type of board
that you want to support. Look on the bottom
of this file and uncomment to choose the board
that you are using.

*/


//// Boards with CPU + Display + Touch ////


// Sunton boards
// purchase: https://www.aliexpress.com/item/1005004502250619.html

#include "boards/ESP32_2432S028R_BGR.h"
//#include "boards/ESP32_2432S028R_RGB.h"
//#include "boards/ESP32_3248S035C.h"
//#include "boards/ESP32_3248S035R.h"


// ESP8266 boards
//#include "boards/ESP2866_NodeMCU_V3.h"



// verify that a minimal hardware has been set
#if !defined(HAS_BOARD) && !defined(HAS_DISPLAY) && !defined(HAS_TOUCH)
#error To compile this code, please open hardware/choose.h and choose your hardware
#endif


/*

Board not listed here yet?

In case you don't see your board here, copy
one of the existing files and use as template.

You can also ask for help on our site.

In case you are combining different hardware
together (e.g. ESP32 + Display + touch screen)
and there is no board name, then please define
a name combining the different hardware together.

For example:

    ESP32_ILI9431_XPT2046_240x320_custom.h

    This says that you are using an ESP32
    together with a display of type ILI9431
    and a touch screen of type XPT2046 with
    a resolution of 240x320 pixels.

Please follow the same order on the description.
This helps to be consistent. Some displays use
RGB, others use BGR for colors. Put this also
on the file name when existing different versions.


       _
     _( )_
    (_(0)_)
      (_)       Can you help others getting their board working?
       | __
       |/_/       Leave a line with your contact below
       |          and commit an update to this file.
       |

        [2023] Max Brito https://github.com/maxbrito500


*/
#endif
