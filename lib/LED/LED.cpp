#include <LED.h>
#include <Util.h>
#include <Persist.h>
#include <Display.h>
#include <ResizeableOctoWS2811Controller.h>

namespace LED {

    const uint8_t NUM_STRIPS  = 8;            /* Don't try to change this; OctoWS2811 always assumes 8 */

    enum Pattern pattern = patternTest;

    CRGB rgbarray[NUM_STRIPS * MAX_LEDS_PER_STRIP]; 

    bool fPowerOn = true;
    uint32_t tmOpenPixelMessageReceived;
    bool fOpenPixelMode;            // Open Pixel message received recently (within 5 sec)
    CRGB rgbSolidColor;
    uint32_t tmFrameStart;
    unsigned int cFrames;
    CResizeableOctoWS2811Controller controller;
    uint16_t cLEDsPerStrip;

    void setup() {

        tmFrameStart = millis();
        tmOpenPixelMessageReceived = 0;
        fOpenPixelMode = false;
        cFrames = 0;

        FastLED.setBrightness(0);
        FastLED.addLeds(&controller, rgbarray, MAX_LEDS_PER_STRIP);
        cLEDsPerStrip = MAX_LEDS_PER_STRIP;

        load_persistant_data();
    }

    void load_persistant_data() {

        FastLED.setMaxPowerInMilliWatts(Persist::data.max_power);
        FastLED.setBrightness(Persist::data.brightness);
        FastLED.setCorrection(Persist::data.color_correction);
        FastLED.setTemperature(Persist::data.color_temperature);
        pattern = (enum Pattern) Persist::data.pattern;
        rgbSolidColor = Persist::data.rgbSolidColor;
        dbgprintf("Color order is %c\n", Persist::data.first_color);
        controller.ChangeColorOrder(Persist::data.first_color);

    }

    void loop() {

        if (!fPowerOn)
        {
            FastLED.showColor(CRGB::Black);;
            return;
        }

        if (fOpenPixelMode)
        {
            if (tmOpenPixelMessageReceived + 5000L > millis())
            {
                // let the protocol drive the LEDs
                return;
            }
            else
            {
                fOpenPixelMode = false;
                Display::status(3, "No OpenPixel Data");
            }
        }

        
        if (pattern == patternSolid)
        {
            FastLED.showColor(rgbSolidColor);
            return;
        }

        // This is the test pattern:

        static uint8_t hue = 0;

        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j < cLEDsPerStrip; j++) {
                rgbarray[(i*cLEDsPerStrip) + j] = CHSV((32*i) + hue+j,192,255);
            }
        }

        // Set the first n leds on each strip to show which strip it is
        for(int i = 0; i < NUM_STRIPS; i++) {
            for(int j = 0; j <= i; j++) {
                rgbarray[(i*cLEDsPerStrip) + j] = CRGB(0x65,0x43,0x21);
            }
        }

        hue++;

        // instead of calling show(), we call delay() which guarantees to call show()
        // but also gives FastLED a chance to do some temporal dithering.
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
        uint16_t b = FastLED.getBrightness();

        if (b <= 255)
        {
            if (b > 128) b += 32;
            else if (b > 64) b += 16;
            else if (b > 32) b += 8;
            else if (b > 16) b += 4;
            else if (b > 8) b += 2;
            else b++;
        } 
        if (b > 255) b = 255;

        FastLED.setBrightness(b);
        Persist::data.brightness = b;
        return b;
    }

    uint8_t dimmer() {
        uint8_t b = FastLED.getBrightness();
        if (b > 1)
        {
            if (b <= 8) b--;
            else if (b <= 16) b -= 2;
            else if (b <= 32) b -= 4;
            else if (b <= 64) b -= 8;
            else if (b <= 128) b -= 16;
            else b -= 32;
        }

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

    void openPixelMessageReceived() {

        tmOpenPixelMessageReceived = millis();
        if (!fOpenPixelMode)
        {
            fOpenPixelMode = true;
            Display::status(3, "OpenPixel Connection");
        }
    }

    CRGB* getRGBAddress(uint8_t iStrip, uint32_t nLEDs) {

        if (nLEDs != cLEDsPerStrip)
        {
            dbgprintf("OpenPixelControl has data for %d pixels but we only support %d\n", nLEDs, cLEDsPerStrip);
            controller.ChangeSize( nLEDs );
            cLEDsPerStrip = nLEDs;
        }
        return rgbarray + (iStrip * cLEDsPerStrip);

    }


}
