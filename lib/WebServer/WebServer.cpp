#include <Arduino.h>
#include <WebServer.h>
#include <Ethernet.h>
#include <Util.h>

namespace WebServer {

    void loop( EthernetServer& server ) {

        EthernetClient client = server.available();
        if (client) {
            dbgprintf("Web server connection\n");

            boolean currentLineIsBlank = true;

            // The current implementation of the WebServer is blocking. It waits for
            // the entire request and sends the entire response while the whole
            // system is essentially frozen. But does it matter?
            //
            
            while (client.connected()) {
                if (client.available()) {
                    char c = client.read();
                    Serial.write(c);
                    // if you've gotten to the end of the line (received a newline
                    // character) and the line is blank, the http request has ended,
                    // so you can send a reply
                    if (c == '\n' && currentLineIsBlank) {
                        // send a standard http response header
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
                        break;
                        }
                    if (c == '\n') {
                    // you're starting a new line
                        currentLineIsBlank = true;
                    } else if (c != '\r') {
                    // you've gotten a character on the current line
                        currentLineIsBlank = false;
                    }
                }
            }
            // give the web browser time to receive the data
            delay(1);
            // close the connection:
            client.stop();
            dbgprintf("Web server disconnected\n");

        }
    }



}

