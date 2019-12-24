#include <OpenPixelControl.h>
#include <BranchController.h>
#include <Util.h>
#include <Display.h>
#include <LED.h>

// implements http://openpixelcontrol.org/

namespace OpenPixelControl {

    enum Status { ready,            // Listening, ready for a client to connect
                  connected         // A client has connected
                };

    Status status;
    EthernetClient client;

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

        status = ready;

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

    void loop( EthernetServer &server ) {

        if (status == ready)
        {

            // listen for connections
            client = server.available();
            if (client) {

                dbgprintf("Client connected\n");
                Display::status(3, "OpenPixel Connected");
                LED::openPixelClientConnection(true);
                status = connected;
                ixHighestChannelSeen = 0;
                ixHeader = 0;
                ixRGB = 0;
                memset( (void*) rgHeader, 0, sizeof(rgHeader));
            }
        }

        if (status == connected)
        {
            if (!client.connected())
            {
                // client has disconnected!
                client.stop();
                dbgprintf("client disconnected\n");
                Display::status(3, "");
                LED::openPixelClientConnection(false);
                status = ready;
                return;
            }

            // client is still connected -- read bytes!
            read_available(client);

        }

    }


    void read_available( EthernetClient& client ) {

        // how many bytes are even available to read?
        size_t cbAvail = client.available();
        if (cbAvail == 0) 
            return;

        if (ixHeader < 4)
        {
            // still need to read the header

            // read the MIN of 
            //      -- the number of bytes in the header remaining, i.e. (4 - ixHeader)
            //      -- the number of bytes we actually have (cbAvail)

            ixHeader += client.read(rgHeader + ixHeader, min(4 - ixHeader, cbAvail));

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
                // for now just warn.
                // TODO: actually eat the bad message
                //
                if (command != 0 || cbMessage == 0 || channel < 1 || channel > 8 || cbMessage > (3 * NUM_LEDS_PER_STRIP))
                {
                    dbgprintf("Suspicious Message - Channel %d - Command %d - Message Size: %d\n", channel, command, cbMessage);
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

        cbAvail = client.available();
        if (cbAvail == 0) 
            return;
        
        // Read the MIN of
        //
        //      -- the number of bytes available
        //      -- the number of bytes that remain to be read (cbMessage - ixRGB)

        if (channel < 1 || channel > 8)
        {
            dbgprintf("I don't get it channel is out of range this is unacceptable\n");
        }

        uint8_t *pstrip = (uint8_t*) LED::getRGBAddress(channel-1);
        size_t cbToRead = min(cbAvail, cbMessage - ixRGB);
        
        // bounds checkings!
        if (ixRGB + cbToRead > (3*NUM_LEDS_PER_STRIP))
        {
            dbgprintf("Impossible situation - too many bytes\n");
            return;
        }

        uint32_t cbRead = client.read(pstrip + ixRGB, cbToRead);
        ixRGB += cbRead;

        if (ixRGB >= cbMessage)
        {
            // done!
            LEDS.show();
            bNeedToShow = false;
            ixHeader = 0;
            ixRGB = 0;
        }



        // TODO
        // HERE IS CODE TO CONSUME THE REST OF THE MESSAGE IF IT'S NOT COOL

        /* size_t cbToRead = min(cbAvail, cbMessage);
        while (cbToRead)
        {
            (void) client.read();
            cbToRead--;
            cbMessage--;
        } */
    }
}