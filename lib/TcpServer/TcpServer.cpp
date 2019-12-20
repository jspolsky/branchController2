#include <TcpServer.h>
#include <Util.h>
#include <SPI.h>
#include <BranchController.h>
#include <Ethernet.h>
#include <Display.h>
#include <Persist.h>
#include <MacAddress.h>

namespace TcpServer {

    enum Status { uninitialized, noHardware, noCable, noDHCP, ready, connected };
    Status status = uninitialized;

    // Initialize the Ethernet server library
    // with the IP address and port you want to use
    // (port 80 is default for HTTP):
    EthernetServer server(80);

    void setup() {

        MacAddress::read();
        Ethernet.init(pinEthernetCS);

        // Wait up to 5 seconds for linkStatus to return LinkON , because
        // sometimes when the hardware is first
        // powering up it still returns LinkOFF even if there is a cable.

        uint32_t mTimeout = millis() + 5000;
        while (millis() < mTimeout && Ethernet.linkStatus() == LinkOFF)
        {
            delay(10);
        }

        if (Ethernet.linkStatus() == LinkOFF)
        {
            dbgprintf("Ethernet link status OFF\n");
            status = noCable;
            return;
        }        

        dbgprintf("DHCP beginning\n");

        if (0 == Ethernet.begin(MacAddress::mac))
        {

            // Check for Ethernet hardware present
            if (Ethernet.hardwareStatus() == EthernetNoHardware) 
            {
                dbgprintf("No Ethernet Hardware\n");
                status = noHardware;
                return;
            }

            dbgprintf("No DHCP Server\n");
            status = noDHCP;
            return;
        }

        dbgprintf("DHCP IP address %d.%d.%d.%d\n", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3] );
        dbgprintf("DHCP Net Mash   %d.%d.%d.%d\n", Ethernet.subnetMask()[0], Ethernet.subnetMask()[1], Ethernet.subnetMask()[2], Ethernet.subnetMask()[3] );
        dbgprintf("DHCP Gateway    %d.%d.%d.%d\n", Ethernet.gatewayIP()[0], Ethernet.gatewayIP()[1], Ethernet.gatewayIP()[2], Ethernet.gatewayIP()[3] );

        server.begin();
        status = ready;

    }


    void loop() {

        if (status == noHardware || status == noDHCP || status == noCable)
            return;

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


        Ethernet.maintain();

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
