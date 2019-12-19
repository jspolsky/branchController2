#include <LED.h>
#include <Util.h>
#include <Persist.h>

namespace LED {

    enum Pattern pattern = patternTest;

    CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

    bool fPowerOn = true;
    CRGB rgbSolidColor;

    void setup() {

        LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
        LEDS.setBrightness(BRIGHTNESS);

        pattern = (enum Pattern) Persist::data.pattern;
        rgbSolidColor = Persist::data.rgbSolidColor;

    }

    void loop() {

        if (!fPowerOn)
        {
            LEDS.showColor(CRGB::Black);;
            return;
        }
        
        if (pattern == patternSolid)
        {
            LEDS.showColor(rgbSolidColor);
            return;
        }

        // This is the test pattern:

        static uint8_t hue = 0;

        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            leds[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
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
    }

    void setSolidColor(CRGB rgb) {

        pattern = patternSolid;
        rgbSolidColor = rgb;

        Persist::data.pattern = (uint8_t) pattern;
        Persist::data.rgbSolidColor = rgb;

    }

    void testPattern() {

        pattern = patternTest;
        Persist::data.pattern = (uint8_t) pattern;

    }

    bool togglePower() {

        fPowerOn = !fPowerOn;
        return fPowerOn;

    }

}
