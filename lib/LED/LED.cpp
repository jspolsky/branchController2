#include <LED.h>
#include <Util.h>
#include <Persist.h>
#include <Display.h>

namespace LED {

    enum Pattern pattern = patternTest;

    CRGB rgbarray[NUM_STRIPS * NUM_LEDS_PER_STRIP];

    bool fPowerOn = true;
    bool fOpenPixelClientConnected = false;
    CRGB rgbSolidColor;
    uint32_t tmFrameStart;
    unsigned int cFrames;

    void setup() {

        tmFrameStart = millis();
        cFrames = 0;

        FastLED.addLeds<OCTOWS2811>(rgbarray, NUM_LEDS_PER_STRIP);
        
        FastLED.setBrightness(Persist::data.brightness);

        pattern = (enum Pattern) Persist::data.pattern;
        rgbSolidColor = Persist::data.rgbSolidColor;

    }

    void loop() {

        if (!fPowerOn)
        {
            FastLED.showColor(CRGB::Black);;
            return;
        }

        if (fOpenPixelClientConnected)
        {
            // let the protocol drive the LEDs
            return;
        }
        
        if (pattern == patternSolid)
        {
            FastLED.showColor(rgbSolidColor);
            return;
        }

        // This is the test pattern:

        static uint8_t hue = 0;

        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j < NUM_LEDS_PER_STRIP; j++) {
            rgbarray[(i*NUM_LEDS_PER_STRIP) + j] = CHSV((32*i) + hue+j,192,255);
            }
        }

        // Set the first n leds on each strip to show which strip it is
        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j <= i; j++) {
            rgbarray[(i*NUM_LEDS_PER_STRIP) + j] = CRGB(0x65,0x43,0x21);
            }
        }

        hue++;

        FastLED.delay(1);
        CalculateFrameRate();

    }

    void CalculateFrameRate()
    {
        // frame rate calc
        cFrames++;
        if (millis() > (tmFrameStart + 1000))
        {
            char rgchBuf[CB_DISPLAY_LINE];
            sprintf(rgchBuf,"Frame rate: %u\n", cFrames);
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
        uint8_t b = FastLED.getBrightness();
        if (b < 255)
            b++;
        FastLED.setBrightness(b);
        Persist::data.brightness = b;
        return b;
    }

    uint8_t dimmer() {
        uint8_t b = FastLED.getBrightness();
        if (b > 1)
            b--;
        FastLED.setBrightness(b);
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

    void openPixelClientConnection(bool f) {

        fOpenPixelClientConnected = f;
    
    }

    CRGB* getRGBAddress( uint8_t iStrip ) {

        return rgbarray + (iStrip * NUM_LEDS_PER_STRIP);

    }


}
