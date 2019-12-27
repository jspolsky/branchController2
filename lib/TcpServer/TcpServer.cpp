#include <TcpServer.h>
#include <Util.h>
#include <SPI.h>
#include <BranchController.h>
#include <Ethernet.h>
#include <Display.h>
#include <Persist.h>
#include <MacAddress.h>
#include <OpenPixelControl.h>
#include <WebServer.h>


namespace TcpServer {

    enum Status { uninitialized, noHardware, noCable, noDHCP, ready };
    Status status = uninitialized;

    const char* rgchRetry = "Press AUTO to connect";

    void setup() {

        MacAddress::read();
        Ethernet.init(pinEthernetCS);

        // Wait up to 5 seconds for linkStatus to return LinkON , because
        // sometimes when the hardware is first
        // powering up it still returns LinkOFF even if there is a cable.

        Display::status(0, "Obtaining IP Address");
        uint32_t mTimeout = millis() + 5000;
        while (millis() < mTimeout && Ethernet.linkStatus() == LinkOFF)
        {
            delay(10);
        }

        if (Ethernet.linkStatus() == LinkOFF)
        {
            dbgprintf("Ethernet link status OFF\n");
            status = noCable;
            Display::status(0, "Cable Disconnected");
            Display::status(3, rgchRetry);
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
                Display::status(0, "No ethernet port");
                return;
            }

            dbgprintf("No DHCP Server\n");
            status = noDHCP;
            Display::status(0, "No DHCP Server");
            Display::status(3, rgchRetry);
            return;
        }

        char rgch[22];
        sprintf(rgch, "IP %d.%d.%d.%d", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3]);
        Display::status(0, rgch);

        dbgprintf("DHCP IP address %d.%d.%d.%d\n", Ethernet.localIP()[0], Ethernet.localIP()[1], Ethernet.localIP()[2], Ethernet.localIP()[3] );
        dbgprintf("DHCP Net Mash   %d.%d.%d.%d\n", Ethernet.subnetMask()[0], Ethernet.subnetMask()[1], Ethernet.subnetMask()[2], Ethernet.subnetMask()[3] );
        dbgprintf("DHCP Gateway    %d.%d.%d.%d\n", Ethernet.gatewayIP()[0], Ethernet.gatewayIP()[1], Ethernet.gatewayIP()[2], Ethernet.gatewayIP()[3] );

        OpenPixelControl::setup();
        WebServer::setup();
        status = ready;

    }


    void loop() {

        if (status != ready)
            return;

        OpenPixelControl::loop();
        WebServer::loop();

        Ethernet.maintain();

    }

    bool initialized() {
        return (status == ready);
    }

}
