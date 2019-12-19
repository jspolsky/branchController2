#include <Remote.h>
#include <Util.h>
#include <Display.h>
#include <IRremote.h>

namespace Remote {

    IRrecv irrecv(pinIRReceiver);
    decode_results results;
    unsigned long lastResult;       // only valid if loop() returns true
    
    void setup() {
        irrecv.enableIRIn(); // Start the receiver
        // irrecv.blink13(true);                    /* conflicts with eth */
        // Serial.println("Enabled IRin");
    }

    bool loop() {

        if (irrecv.decode(&results)) {
            lastResult = results.value;
            irrecv.resume(); // Receive the next value
            return true;
        }
        else
        {
            return false;
        }

    }

}
