#include <LED.h>
#include <Util.h>
#include <Persist.h>
#include <Display.h>

namespace LED {

    enum Pattern pattern = patternTest;

    CRGB leds[NUM_STRIPS * NUM_LEDS_PER_STRIP];

    bool fPowerOn = true;
    CRGB rgbSolidColor;
    uint32_t tmFrameStart;
    uint32_t cFrames;

    void setup() {

        tmFrameStart = millis();
        cFrames = 0;

        LEDS.addLeds<OCTOWS2811>(leds, NUM_LEDS_PER_STRIP);
        LEDS.setBrightness(Persist::data.brightness);

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
        cFrames++;

        // frame rate calc
        if (millis() > (tmFrameStart + 1000))
        {
            char rgchBuf[CB_DISPLAY_LINE];
            sprintf(rgchBuf,"Frame rate: %d\n", cFrames);
            Display::status(2,rgchBuf);
            cFrames = 0;
            tmFrameStart = millis();
        }

    }

    void setSolidColor(CRGB rgb) {

        pattern = patternSolid;
        rgbSolidColor = rgb;

        Persist::data.pattern = (uint8_t) pattern;
        Persist::data.rgbSolidColor = rgb;

    }

    uint8_t brighter() {
        uint8_t b = LEDS.getBrightness();
        if (b < 255)
            b++;
        LEDS.setBrightness(b);
        Persist::data.brightness = b;
        return b;
    }

    uint8_t dimmer() {
        uint8_t b = LEDS.getBrightness();
        if (b > 1)
            b--;
        LEDS.setBrightness(b);
        Persist::data.brightness = b;
        return b;
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
