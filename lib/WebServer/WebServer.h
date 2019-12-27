#pragma once
#include <Arduino.h>
#include <Ethernet.h>

//
// implements a web server used for 
// advanced configuration options
//


namespace WebServer {

    void loop( EthernetServer& server );

}
