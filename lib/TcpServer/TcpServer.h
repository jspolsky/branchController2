#pragma once

// Code for listening, as a service, on a TCP port
// and responding appropriately.

#include <Arduino.h>

namespace TcpServer {

    void setup();
    void loop();
    const char* getstatus();

}
