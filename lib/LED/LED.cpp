#include <LED.h>
#include <Util.h>

namespace LED {

    CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

    void setup() {

        LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
        LEDS.setBrightness(BRIGHTNESS);

    }

    void loop() {

        static uint8_t hue = 0;

        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
            // if (j % 20 == 0) leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB::White;
            }
        }

        // Set the first n leds on each strip to show which strip it is
        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j <= i; j++) {
            leds[(i*NUM_LEDS_PER_STRIP) + j] = CRGB(0x65,0x43,0x21);
            }
        }

        hue++;

        LEDS.show();
        LEDS.delay(1);

    }

}