#pragma once

// Code for driving 8 LED strips in parallel using FastLED's
// OctoWS2811 support
//
// Best documentation: https://github.com/FastLED/FastLED/wiki/Parallel-Output

#include <Arduino.h>
#include <BranchController.h>
#define USE_OCTOWS2811
#include <OctoWS2811.h>
#include <FastLED.h>

namespace LED {

    void setup();
    void loop();

    void setSolidColor(CRGB rgb);

}