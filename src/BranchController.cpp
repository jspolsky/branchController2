// BranchController.cpp
//
// Main entry point - provides Arduino's setup() and loop() functions which
// tie together all the main functionality of the different modules.

#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>
#include <Util.h>

void setup() {

    Heartbeat::setup();
    Util::setup();

    dprintf("BranchController Setup Complete\n");

}

void loop() {

    Heartbeat::loop();

}
