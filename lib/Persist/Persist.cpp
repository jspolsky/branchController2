#include <Persist.h>
#include <Util.h>
#include <EEPROM.h>

namespace Persist {

    persistence_t data;

    void setup() {

        // set some default values

        memset(&data, 0, sizeof(data));
        data.cb = sizeof(data);
        data.rgbSolidColor = CRGB::Black;
        data.pattern = (uint8_t) 1; // LED::patternTest
        data.brightness = 32;
        
        dbgprintf("Initializing Persisted Data\n");

        byte bSig[2];
        bSig[0] = EEPROM.read(0);
        bSig[1] = EEPROM.read(1);

        dbgprintf("Signature read - %d %d\n", bSig[0], bSig[1] );
        if (bSig[0] == 'b' && bSig[1] == 'C')
        {
            // read EEPROM now - start with cb!
            uint16_t cbOnDisk = 0;
            uint8_t* pb = (uint8_t*) &cbOnDisk;
            pb[0] = EEPROM.read(2);
            pb[1] = EEPROM.read(3);

            dbgprintf("Structure on disk is %d bytes\n", cbOnDisk);

            // Don't read more than sizeof(data) or cbOnDisk
            uint8_t* pbData = (uint8_t*) &data;
            uint16_t cbToRead = min(cbOnDisk, sizeof(data)) - 2;    // -2 because we're not reading cb again
            for (uint16_t i = 0; i < cbToRead; i++)
            {
                pbData[i + 2] = EEPROM.read( i + 4 );               // skip over signature and cb
            }

        }
        else
        {
            dbgprintf("Signature not found - not reading from EEPROM\n");
        }

        dbgprintf("cb: %d color: %x,%x,%x pattern: %d brightness: %d\n",
            data.cb,
            data.rgbSolidColor.r,
            data.rgbSolidColor.g,
            data.rgbSolidColor.b,
            data.pattern,
            data.brightness        
        );

    }

    void write() {

        dbgprintf("Persist::Write with %d bytes\n", data.cb);

        for (uint16_t i = 0; i < data.cb; i++)
        {
            uint8_t* pb = (uint8_t*) &data;
            EEPROM.write(i + 2, pb[i]);
        }
        // if we haven't crashed yet, write the signature marks
        EEPROM.write(0, 'b');
        EEPROM.write(1, 'C');

        dbgprintf("Persist::Write done\n");
    }



}