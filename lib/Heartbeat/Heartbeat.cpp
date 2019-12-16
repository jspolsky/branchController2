
#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>

namespace Heartbeat {

    void setup(void)
    {
        // initialize LED digital pin as an output.
        pinMode(pinHeartbeat, OUTPUT);
    }

    void loop(void)
    {
        // turn the LED on (HIGH is the voltage level)
        digitalWrite(pinHeartbeat, HIGH);
        // wait for a second
        delay(1000);
        // turn the LED off by making the voltage LOW
        digitalWrite(pinHeartbeat, LOW);
        // wait for a second
        delay(1000);
    }

}
