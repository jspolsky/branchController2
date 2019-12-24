#pragma once
#include <Arduino.h>
#include <Ethernet.h>

namespace OpenPixelControl {

    void setup();
    void loop( EthernetServer& server );
    void read_available( EthernetClient& client );

}