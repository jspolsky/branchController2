// BranchController.cpp
//
// Main entry point - provides Arduino's setup() and loop() functions which
// tie together all the main functionality of the different modules.

#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>
#include <Util.h>
#include <Display.h>
#include <TcpServer.h>
#include <LED.h>

void setup() {

    dbgprintf("Begin\n");

    Heartbeat::setup();
    Util::setup();
    Display::setup();
    TcpServer::setup();
    LED::setup();

    dbgprintf("BranchController Setup Complete\n");

}

void loop() {

    Heartbeat::loop();
    TcpServer::loop();
    LED::loop();

    Display::status(3, TcpServer::getstatus(), "192.168.1.177", 60);
}
