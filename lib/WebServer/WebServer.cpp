#include <Arduino.h>
#include <WebServer.h>
#include <Ethernet.h>
#include <Util.h>

#define BUFFER_SIZE 128

namespace WebServer {

    enum Status { ready,            // Listening, ready for a client to connect
                  connected         // A client has connected
                };

    Status status;
    EthernetClient client;
    EthernetServer server(80);

    char rgchRequest[BUFFER_SIZE + 1];
    uint16_t ixRequest = 0;

    void setup() 
    {

        server.begin();
        status = ready;

    }

    void loop() {

        if (status == ready)
        {

            client = server.available();
            if (client)
            {
                dbgprintf("Web client connected\n");
                ixRequest = 0;
                status = connected;
            }
        }

        if (status == connected)
        {
            if (!client.connected())
            {
                dbgprintf("Web client disconnected\n");
                status = ready;
                return;
            }

            // client is still connected -- read the bytes!
            read_available();
        }
    }

    void read_available() {

        // Here's the plan: read one line at a time - up to 128 bytes, then discard
        // When you see \n, process it
        //      if it starts with GET, record this as the "GET" command
        //      if it starts with \n again, that's the end of the HTTP header - process it
        //      otherwise discard and start over
        
        while (client.available())
        {
            char c = client.read();

            if (c == '\n')
            {
                rgchRequest[ixRequest] = '\0';
                if (!strncmp(rgchRequest, "GET ", 4))
                {
                    // GET command has a space in it followed by something like HTTP/1.1 which we can
                    // throw away
                    if (char *pchSpace = strchr(rgchRequest+4, ' '))
                        *pchSpace = '\0';
                    process_get(rgchRequest + 4);
                }
                if (ixRequest == 0)
                {
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");  // the connection will be closed after completion of the response
                    client.println();
                    client.println("<!DOCTYPE HTML>");
                    client.println("<html>");
                    // output the value of each analog input pin
                    for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
                        int sensorReading = analogRead(analogChannel);
                        client.print("analog input ");
                        client.print(analogChannel);
                        client.print(" is ");
                        client.print(sensorReading);
                        client.println("<br />");
                    }
                    client.println("</html>");
                    delay(1);
                    client.stop();
                }
                ixRequest = 0;
            }
            else
            {
                if (ixRequest == BUFFER_SIZE - 1)
                    rgchRequest[ixRequest] = '\0';
                else if (c != '\r')
                {
                    rgchRequest[ixRequest] = c;
                    ixRequest++;
                }
            }
        }
    }

    void process_get(char* szGet)
    {
        dbgprintf("GET: [%s]\n", szGet);
    }
}

