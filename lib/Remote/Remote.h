#pragma once

// Code for responding to an IR remote control.
// Uses IRRemote library (2.3.3) which works well with Teensy 3.2.
// There are a lot of very cheap, 44-key IR remote controls (under $2 on aliexpress)
// which have color, brightness, speed, power buttons that are perfect for a 
// project where you can just use a single branch controller.

#include <Arduino.h>
#include <BranchController.h>

namespace Remote {

    void setup();
    bool loop();

    extern unsigned long lastResult;       // only valid if loop() returns true

}