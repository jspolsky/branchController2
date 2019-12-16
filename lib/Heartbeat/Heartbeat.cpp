
#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>

#define HEARTBEAT_PERIOD_MS 1700

namespace Heartbeat {

    void setup(void)
    {
        // initialize LED digital pin as an output.
        pinMode(pinHeartbeat, OUTPUT);
    }

    void loop(void)
    {
        uint32_t tm = millis() % HEARTBEAT_PERIOD_MS;
        double rad = tm * PI * 2 / HEARTBEAT_PERIOD_MS;
        double brightness = (sin(rad) + 1.0) * 128.0;
        if (cos(rad) > 0) brightness = 256.0 - brightness;

        // turn the LED on (HIGH is the voltage level)
        analogWrite(pinHeartbeat, int(brightness));
    }

}
