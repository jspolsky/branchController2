#pragma once

// Code for persisting configuration data to the Teensy EEPROM
//
// When we store things in EEPROM we store two bytes "bC" in the first
// two bytes. If we ever read anything else there, we assume that
// EEPROM is not initialized.
//
// Then we store a persistence_t structure, byte-for-byte. The 
// first uint16_t of the structure contains the size of the structure
// as stored, in bytes.
//

#include <Arduino.h>
#include <BranchController.h>
#include <FastLED.h>


namespace Persist {

    struct persistence_t {

        uint16_t    cb;             // Must always be sizeof(persistence_t) - for versioning
        uint8_t     ip3;            // third octet of IP address
        uint8_t     ip4;            // fourth octet of IP address

        CRGB        rgbSolidColor;  // current color to display 
        uint8_t     pattern;        // whether we are in solid color mode (0) or test pattern (1) -- maps to enum Pattern in LED.cpp
        uint8_t     brightness;     // global brightness for fastled

    };

    extern persistence_t data;
    
    void setup();
    void write();

}