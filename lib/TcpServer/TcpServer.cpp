#include <TcpServer.h>
#include <Util.h>
#include <SPI.h>
#include <BranchController.h>
#include <Ethernet.h>
#include <Display.h>
#include <Persist.h>
#include <MacAddress.h>

namespace TcpServer {

    enum Status { uninitialized, noHardware, noCable, ready, connected };
    Status status = uninitialized;

    // Initialize the Ethernet server library
    // with the IP address and port you want to use
    // (port 80 is default for HTTP):
    EthernetServer server(80);

    void setup() {

        MacAddress::read();

        Ethernet.init(pinEthernetCS); 
        
        IPAddress ip(192,168,1,203);
        
        Ethernet.begin(MacAddress::mac, ip);

        dbgprintf("Ethernet setup IP address %d.%d.%d.%d\n", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3] );

        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware) 
        {
            status = noHardware;
            return;
        }
    }


    void loop() {

        if (status == noHardware) 
            return;

        if (Ethernet.linkStatus() == LinkOFF)
        {
            status = noCable;
            return;
        }
        else
        {
            server.begin();
            status = ready;
        }


        // listen for incoming clients
        EthernetClient client = server.available();
        if (client) {
        dbgprintf("new client\n");
        // an http request ends with a blank line
        boolean currentLineIsBlank = true;
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
                client.println("Refresh: 5");  // refresh the page automatically every 5 sec
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
        dbgprintf("client disconnected\n");
        }
    }

    const char* getstatus()
    {
        switch(status)
        {
            case uninitialized:     return "Unitialized";
            case noHardware:        return "No Hardware";
            case noCable:           return "No Cable";
            case ready:             return "Ready";
            case connected:         return "Connected";
            default:                return "?";
        }
    }
}
