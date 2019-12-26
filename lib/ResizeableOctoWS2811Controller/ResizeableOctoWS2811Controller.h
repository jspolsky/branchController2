#pragma once

#include <OctoWS2811.h>
#include <FastLED.h>
#include <Arduino.h>
#include <Util.h>

// This is a version of the parallel, OctoWS2811Controller from FastLED
// that I modified to be resizable. This means that we can allocate an array
// of the full 550 pixels per strip (4400 total), but, at runtime, if
// the OpenPixelController client sends us fewer pixels, we can show() them
// really fast without spending time sending data for extra pixels.
//
// I am particularly keen to implement this feature because it means that
// on shorter strips, we can still take advantage of FastLED's temporal dithering
// for better colors at lower brightnesses.

class CResizeableOctoWS2811Controller : public CPixelLEDController<GRB, 8, 0xFF>
{
    OctoWS2811 *pocto;
    uint8_t *drawbuffer, *framebuffer;
    int config = WS2811_RGB | WS2811_800kHz;

    void _init(int nLeds)
    {
        if (pocto == NULL)
        {

            drawbuffer = (uint8_t *)malloc(nLeds * 8 * 3);
            framebuffer = (uint8_t *)malloc(nLeds * 8 * 3);

            // byte ordering is handled in show by the pixel controller

            pocto = new OctoWS2811(nLeds, framebuffer, drawbuffer, config);
            pocto->begin();
        }
    }

public:
    CResizeableOctoWS2811Controller() { pocto = NULL; }
    virtual int size() { return CLEDController::size() * 8; }

    virtual void init()
    { /* do nothing yet */
    }

    typedef union {
        uint8_t bytes[8];
        uint32_t raw[2];
    } Lines;

    virtual void showPixels(PixelController<GRB, 8, 0xFF> &pixels)
    {
        _init(pixels.size());

        uint8_t *pData = drawbuffer;

        while (pixels.has(1))
        {
            Lines b;

            for (int i = 0; i < 8; i++)
            {
                b.bytes[i] = pixels.loadAndScale0(i);
            }
            transpose8x1_MSB(b.bytes, pData);
            pData += 8;
            for (int i = 0; i < 8; i++)
            {
                b.bytes[i] = pixels.loadAndScale1(i);
            }
            transpose8x1_MSB(b.bytes, pData);
            pData += 8;
            for (int i = 0; i < 8; i++)
            {
                b.bytes[i] = pixels.loadAndScale2(i);
            }
            transpose8x1_MSB(b.bytes, pData);
            pData += 8;
            pixels.stepDithering();
            pixels.advanceData();
        }

        pocto->show();

    }

    // TODO this is a hacky way to test resizing the FastLED controller temporarily
    // TODO arbitrary number of pixels
    // TODO hook it up to network code so we adjust based on what they send
    // TODO can we inherit from the library's version of this instead of copying so much
    // TODO the current framerate of 125 or so is niiiiice but seems too slow for just 15 
    //      pixels per strip. If we start with 15 pixels I get 900 fps! I think the problem
    //      is that my PixelController still thinks its large so showPixels, right above, is doing
    //      a lot of work. NOPE. THAT IS NOT THE PROBLEM. IT MIGHT BE MY OWN LOOP() CODE FOR
    //      TEST PATTERN

    void ChangeSize( uint16_t nPixels )
    {
        if (!pocto)
            return;

        while (pocto->busy())
            ;

        m_nLeds = nPixels;
        delete pocto;
        pocto = new OctoWS2811(nPixels, framebuffer, drawbuffer, config);
        pocto->begin();

        dbgprintf("Now supporting %d pixels per strip\n", nPixels);
    }
};
