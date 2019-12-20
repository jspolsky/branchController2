#include <MacAddress.h>
#include <Util.h>
#include <Display.h>

namespace MacAddress {
 
    uint8_t mac[6];

    // http://forum.pjrc.com/threads/91-teensy-3-MAC-address
    void readpart(uint8_t word, uint8_t *mac, uint8_t offset) {

        cli();
        
        FTFL_FCCOB0 = 0x41;             // Selects the READONCE command
        FTFL_FCCOB1 = word;             // read the given word of read once area

        // launch command and wait until complete
        FTFL_FSTAT = FTFL_FSTAT_CCIF;
        while(!(FTFL_FSTAT & FTFL_FSTAT_CCIF));

        *(mac+offset) =   FTFL_FCCOB5;       // collect only the top three bytes,
        *(mac+offset+1) = FTFL_FCCOB6;       // in the right orientation (big endian).
        *(mac+offset+2) = FTFL_FCCOB7;       // Skip FTFL_FCCOB4 as it's always 0.

        sei();
    }

    void read() {
        
        readpart(0xe,mac,0);
        readpart(0xf,mac,3);

        char rgch[22];
        sprintf(rgch, "MAC %02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        Display::status(1, rgch);

    }

}