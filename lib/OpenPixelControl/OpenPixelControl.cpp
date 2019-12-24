#include <OpenPixelControl.h>
#include <BranchController.h>
#include <Util.h>

namespace OpenPixelControl {

    enum Status { ready,            // Listening, ready for a client to connect
                  connected         // A client has connected
                };

    Status status;

    EthernetClient client;

    void setup() {

        status = ready;

    }

    void loop( EthernetServer &server ) {

        if (status == ready)
        {

            // listen for connections
            client = server.available();
            if (client) {

                dbgprintf("Client connected\n");
                status = connected;

            }
        }

        if (status == connected)
        {
            if (!client.connected())
            {
                // client has disconnected!
                client.stop();
                dbgprintf("client disconnected\n");
                status = ready;
                return;
            }

            // client is still connected -- read bytes!
            read_available(client);

        }

    }

    void read_available( EthernetClient& client ) {

        while (client.available()) 
        {
            char c = client.read();
            dbgprintf("%x\n", c);
        }
    }
}