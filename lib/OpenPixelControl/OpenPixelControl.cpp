#include <OpenPixelControl.h>
#include <BranchController.h>
#include <Util.h>
#include <Display.h>
#include <LED.h>

// implements http://openpixelcontrol.org/

namespace OpenPixelControl {

    EthernetUDP udp;
    
    //
    // OPC sends each channel separately. However FastLED.show() 
    // updates *every* LED strand. In an 8 strand environment that is 8 times
    // too slow.
    //
    // Therefore, we don't call .show() until the highest channel seen
    // so far has been received. The first frame will run slowly but after that
    // it will speed up and only call .show() after the final channel data 
    // has arrived.
    //
    uint8_t ixHighestChannelSeen = 0;           // 0: we don't know how many channels the client is sending
    bool bNeedToShow = false;                   // true if we are going to refresh the display

    void setup() {

        udp.begin(OPEN_PIXEL_PORT);

    }

    // ixHeader is the index we are at in the current OPC message header
    // for example, ixHeader == 0 means we have not seen any bytes yet
    // Since the message header is 4 bytes, ixHeader == 4 means we have read the header
    // and are now ready to read the body

    uint32_t ixHeader = 0;
    uint8_t rgHeader[4];

    // ixRGB is the index we are at in the CRGB array
    uint32_t ixRGB = 0;
    
    // when the message has been read, it will be parsed into these three variables:
    uint8_t channel = 0;
    uint8_t command = 0;
    uint16_t cbMessage = 0;

    // True if the channel, command, or cbMessage makes it impossible to
    // parse this message -- we're just going to swallow it.
    bool bThrowAwayMessage = false;

    void loop() {

        uint32_t cb = udp.parsePacket();

        if (cb)
        {
            LED::openPixelMessageReceived();

            ixHighestChannelSeen = 0;
            ixHeader = 0;
            ixRGB = 0;
            memset( (void*) rgHeader, 0, sizeof(rgHeader));

            read_available();

        }

    }


    void read_available() {

        // how many bytes are even available to read?
        size_t cbAvail = udp.available();
        if (cbAvail == 0) 
            return;

        if (ixHeader < 4)
        {
            // still need to read the header

            // read the MIN of 
            //      -- the number of bytes in the header remaining, i.e. (4 - ixHeader)
            //      -- the number of bytes we actually have (cbAvail)

            ixHeader += udp.read(rgHeader + ixHeader, min(4 - ixHeader, cbAvail));

            if (ixHeader < 4)
                // go home and wait for the rest of the header
                return; 

            if (ixHeader > 4)
                dbgprintf("Alert! Read past end of header -- this should never happen\n");

            if (ixHeader == 4) 
            {
                channel = rgHeader[0];
                command = rgHeader[1];
                cbMessage = rgHeader[2] << 8 | rgHeader[3];
                ixRGB = 0;  // ready to start reading RGB values

                //
                // Anything wrong with the message?
                //
                bThrowAwayMessage = false;

                char rgchError[CB_DISPLAY_LINE];

                if (command != 0)
                {
                    dbgprintf("OpenPixelControl - command %d not supported\n", command);
                    sprintf(rgchError, "OPC BAD CMD %d", command);
                    Display::status(3, rgchError);
                    bThrowAwayMessage = true;
                }
                else if (channel < 1 || channel > 8)
                {
                    dbgprintf("OpenPixelControl - channel %d not supported\n", channel);
                    sprintf(rgchError, "OPC BAD CHAN %d", channel);
                    Display::status(3, rgchError);
                    channel = 1;
                    bThrowAwayMessage = true;
                }
                else if (cbMessage > (3 * MAX_LEDS_PER_STRIP))
                {
                    dbgprintf("OpenPixelControl - too many pixels per strip (%d)\n", cbMessage / 3);
                    Display::status(3, "OPC TOO MANY PIXELS");
                    bThrowAwayMessage = true;
                }

                if (channel >= ixHighestChannelSeen)
                {
                    bNeedToShow = true;
                    ixHighestChannelSeen = channel;
                }

            }
        }

        // Now ixHeader is either < 4 -- still need to read more -- in which case, we can come back later,
        // or == 4, -- header is here -- in which case we are processing it.

        if (ixHeader < 4)
            return;        

        if (ixHeader != 4)
        {
            dbgprintf("Alert! impossible header value\n");
        }

       
        if (cbMessage == 0)
        {
            // we have finished eating the message
            ixHeader = 0;
            ixRGB = 0;
            memset( (void*) rgHeader, 0, sizeof(rgHeader));
            return;
        }

        cbAvail = udp.available();
        if (cbAvail == 0) 
            return;
        
        // Read the MIN of
        //
        //      -- the number of bytes available
        //      -- the number of bytes that remain to be read (cbMessage - ixRGB)

        uint8_t *pstrip = (uint8_t*) LED::getRGBAddress(channel-1, cbMessage / 3);
        size_t cbToRead = min(cbAvail, cbMessage - ixRGB);
        
        if (bThrowAwayMessage)
        {
            while (cbToRead)
            {
                (void) udp.read();
                cbToRead--;
                ixRGB++;
            }

            if (ixRGB >= cbMessage)
            {
                ixHeader = ixRGB = 0;
            }
            return;
        }

        uint32_t cbRead = udp.read(pstrip + ixRGB, cbToRead);
        ixRGB += cbRead;

        if (ixRGB >= cbMessage)
        {
            // done!
            if (bNeedToShow) {
                FastLED.show();
                LED::CalculateFrameRate();
                bNeedToShow = false;
            }
            ixHeader = ixRGB = 0;
        }
    }
}
