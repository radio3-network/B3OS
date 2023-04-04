```
                           
 ──▒▒▒▒▒▒───▄████▄         
 ─▒─▄▒─▄▒──███▄█▀          
 ─▒▒▒▒▒▒▒─▐████──█─█       
 ─▒▒▒▒▒▒▒──█████▄          
 ─▒─▒─▒─▒───▀████▀         
 ▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄   
 █▄─▄─▀█▄▄▄░█─▄▄─█─▄▄▄▄█   
 ██─▄─▀██▄▄░█─██─█▄▄▄▄─█   
 █▄▄▄▄██▄▄▄▄█▄▄▄▄█▄▄▄▄▄█   
                           

```

B3 Operating System

A long awaited GUI and CLI operating system
for ESP32 devices.


This OS provides:

    + standard dialogs (e.g. select wifi network)
    + easy to use button/textarea, icons
    + keyboard on screen is automatically handled
    + linux-like cli available through WifI/telnet


Creating apps should be easy and don't require
compiling firmwares. In smartphones we just install
apps and the same should be available for ESP32.


![Animated demonstration](/docs/screenshots/video_2023-03-24_10-19-02.gif)



# Getting started

This project is based on ESP32 boards. At the moment we include support for the cheap Sunton boards (15 euros in 2023) that you can purchase from this location: https://www.aliexpress.com/item/1005004502250619.html because they include everything needed (display, touch screen, WiFi and an ESP32 with enough capacity).

If you get a Sunton ESP32 board, this project works straight away without changes. In case you need to add support for your device, look on this file to see if support is already added: https://github.com/radio3-network/B3OS/blob/main/src/hardware/choose.h and in case it is not listed, just open a ticket here so that we can help.

Code is developed using PlatformIO under Linux. If you are running/compiling under Windows, please modify the path here with the full path so that it can work: https://github.com/radio3-network/B3OS/blob/main/platformio.ini#L19


# Help needed

Where help is needed:

+ Documentation: Add your experience getting started
+ Add support for your boards (LiLy, eInk, others). Look here https://github.com/radio3-network/B3OS/blob/main/src/hardware/choose.h
+ Add more CLI apps, look here: https://github.com/radio3-network/B3OS/tree/main/src/apps/terminal
+ Add a text editor for CLI: https://github.com/radio3-network/B3OS/blob/main/src/apps/terminal/text_editor.cpp
+ Add dynamic loading of binaries, look here: https://github.com/radio3-network/B3OS/blob/main/src/apps/launcher/apps.cpp

Other things you can help?

Give ideas, write on the forum: https://github.com/orgs/radio3-network/discussions

Everyone is welcome! Either writing, trying out or sharing the word already helps so much.

Remember to star if you like this project. Extra points if you do a video review and let us know the link. :-)


# Legal

This project is fully open source and delivered under the Apache version 2.0 license.

Licenses from third-party libraries and resources adopted by this project are applicable and compatible with the Apache 2.0 license terms and conditions.

License: Apache-2.0
Copyright (c) B3OS contributors


# Thanks

This project is only possible due to the work and effort of many other projects and people.
On this section we try to list them.


## Espressif and Sunton
For the incredibly powerful and affordable ESP32 boards
+ https://www.aliexpress.com/item/1005004502250619.html
+ https://www.espressif.com/


## LVGL
For making modern GUI windows available for Arduino
+ https://lvgl.io/


## ESP32-SmartDisplay
For making it possible and easy to write code
for these Sunton boards.
+ https://github.com/rzeldent/esp32-smartdisplay


## Shellminator
Terminal library for accessing the device from
either the WiFi or a serial cable.
+ https://github.com/dani007200964/Shellminator


## Xila
For the good conversations and perspective to serve as host for this project one day.
+ https://xila.dev/