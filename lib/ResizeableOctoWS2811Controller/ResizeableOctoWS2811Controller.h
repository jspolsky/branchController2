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

        //
        // The only difference between RGB and GRB color mode
        // is whether we call loadAndScale1 first or loadAndScale0 first.
        //
        // For performance reasons, do not "optimize" this code by moving the if
        // statement into the loop!
        //
        if (config & WS2811_GRB)
        {
            while (pixels.has(1))
            {
                Lines b;

                for (int i = 0; i < 8; i++)
                {
                    b.bytes[i] = pixels.loadAndScale1(i);
                }
                transpose8x1_MSB(b.bytes, pData);
                pData += 8;
                for (int i = 0; i < 8; i++)
                {
                    b.bytes[i] = pixels.loadAndScale0(i);
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
        }
        else
        {
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
        }

        pocto->show();

    }

    // change the actual size of strips, at runtime!
    void ChangeSize(uint16_t nPixels)
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

    // change the RGB/GRB order of the strips, at runtime!
    void ChangeColorOrder(char first_color) {

        config = WS2811_800kHz;
        if (first_color == 'r')
            config |= WS2811_RGB;
        else if (first_color == 'g')
            config |= WS2811_GRB;

    }
};
