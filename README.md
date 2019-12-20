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

Basic Operation
---------------

By default branchController assumes you have 8 strips of 552 pixels each. You can operate a single branchController with a 44 button IR remote control like this one:

![44 button IR remote control](doc/44buttonIR.jpg)

The brightness and color buttons work out of the box so you can use your branch controller to create basic solid colors. 

Use the On/Off button on the remote to turn off the LEDs. When you press OFF, any changes you have made to the state of the branchController is saved to EEPROM so that it will come back in the same state when you power up. *If you don't press off, the state is never saved*.

The DIY1-6 buttons are reserved for internal test patterns and chases. You can edit the code for these to provide something that is a good backup if your PC or network fails.

Network Operation
-----------------

You can connect your branchController to a LAN using the built-in ethernet port and control it from any kind of computer that speaks TCP/IP.

When branchController starts up, it will look for a DHCP server and try to get an IP address and network configuration. If that works it will show you the IP address on the screen.

Every branch controller has a unique MAC address which will never change, so you can configure your DHCP server to always hand out the same IP address to the same branch controller, which makes it easier to sort out multiple branch controllers.

About the project
-----------------

This code was built using PlatformIO, an open source platform for embedded development which is a zillion times better than using the Arduino IDE. You will still need Teensyduino to flash the Teensy. For more about the research behind this project, follow my blog, [BlinkyLights](https://blinkylights.blog/).


Next Up
-------

- [ ] Display network status, MAC, TCP-IP on the screen
- [ ] Some way to restart (start) network if Ethernet is plugged in later

