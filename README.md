branchController
================

Teensy 3.2 branch controller for up to 8 separate LED strips, based on instructions received over TCP-IP.

This is code that runs on the [Teensy 3.2 development board](https://www.pjrc.com/store/teensy32.html) to allow it to control up to 8 separate LED strips.

It is designed for WS2812b-style LED strips, with a single wire protocol. These are what Adafruit calls "NeoPixels". Personally I like to use the WS2815 variant which runs on 12 volts and has a backup wire allowing any single pixel to fail. It does *not* handle APA102c-type strips, which use a two wire protocol consisting of both the signal and a clock.

Features:

* Supports up to 4416 pixels (552 per strip) at a frame rate of 60 Hz, the theoretical limit of the protocol

* Ethernet control. Connects to an ethernet network to receive pixel data. This allows you to use many branch controllers controlled from a single PC

* 128x32 OLED display for diagnostics

* IR sensor allows you to use an IR remote control for basic controls

About the project
-----------------

This code was built using PlatformIO, an open source platform for embedded development which is a million times better than using the Arduino IDE. You will still need Teensyduino to flash the Teensy. For more about the research behind this project, follow my blog, [BlinkyLights](https://blinkylights.blog/).