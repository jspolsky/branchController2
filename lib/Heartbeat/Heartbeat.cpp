
#include <Arduino.h>
#include <BranchController.h>
#include <Util.h>
#include <Heartbeat.h>

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
        if (cos(rad) > 0) brightness = 255.0 - brightness;
        // TODO: isn't this supposed to be 0..1023?

        // turn the LED on (HIGH is the voltage level)
        analogWrite(pinHeartbeat, int(brightness));
    }

}
