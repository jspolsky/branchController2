#pragma once

// Code for driving 128x32 monochrome OLED display, connected with I2C
// Product page: https://www.adafruit.com/product/4440

#include <Arduino.h>

#define CB_DISPLAY_LINE 22

namespace Display {

    void setup();
    void status(int line, const char* msg);
    void write_lines();
    void off(); 
    void on();

    #ifdef ADAFRUIT_SAMPLE_CODE
        void adafruitSampleCode();  // Runs an infinite loop of adafruit sample code, consisting of these functions:
        void testdrawline();        // Draw many lines
        void testdrawrect();        // Draw rectangles (outlines)
        void testfillrect();        // Draw rectangles (filled)
        void testdrawcircle();      // Draw circles (outlines)
        void testfillcircle();      // Draw circles (filled)
        void testdrawroundrect();   // Draw rounded rectangles (outlines)
        void testfillroundrect();   // Draw rounded rectangles (filled)
        void testdrawtriangle();    // Draw triangles (outlines)
        void testfilltriangle();    // Draw triangles (filled)
        void testdrawchar();        // Draw characters of the default font
        void testdrawstyles();      // Draw 'stylized' characters
        void testscrolltext();      // Draw scrolling text
        void testdrawbitmap();      // Draw a small bitmap image
        void testanimate(const uint8_t *bitmap, uint8_t w, uint8_t h);
    #endif

}
