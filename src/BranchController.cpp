// BranchController.cpp
//
// Main entry point - provides Arduino's setup() and loop() functions which
// tie together all the main functionality of the different modules.

#include <Arduino.h>
#include <BranchController.h>
#include <Heartbeat.h>
#include <Util.h>
#include <Display.h>
#include <TcpServer.h>
#include <LED.h>
#include <Remote.h>
#include <Persist.h>

void RouteIRCode(unsigned int code);

void setup() {

    dbgprintf("Begin\n");

    Heartbeat::setup();
    Util::setup();
    Persist::setup();
    Display::setup();
    TcpServer::setup();
    LED::setup();
    Remote::setup();

    dbgprintf("BranchController Setup Complete\n");

}

void loop() {

    static unsigned int lastIrCode = 0;

    Heartbeat::loop();
    TcpServer::loop();
    LED::loop();
    if (Remote::loop())
    {
        lastIrCode = Remote::lastResult;
        RouteIRCode(lastIrCode);
    }

    // TODO Measure Frame Rate
}

void RouteIRCode(unsigned int code)
{
    // IR codes: https://gist.github.com/Derpidoo/e3042055e0f5c3708f9b98b75fe4d59e
    // or just try hitting a button to see what you get :) 
    Display::status(3, "");

    switch(code)
    {
        case 0xFF02FD:  // POWER
            if (LED::togglePower())
            {
                // switched on
                Display::on();
            }
            else
            {
                // switched off
                Display::off();
                Persist::write();
            }
            break;

        case 0xFFF00F:  // AUTO - reinitialize network plz
            if (!TcpServer::initialized())
            {
                Display::status(3, "");
                TcpServer::setup();
            }
            break;

        case 0xFF30CF:  // DIY1
            LED::testPattern();
            break;


        //
        // Here are all the colors:
        //

        case 0xFF1AE5: // RED
            LED::setSolidColor(CRGB::Red);
            break;

        case 0xFF2AD5:  // RED ROW 2
            LED::setSolidColor(CHSV(16, 255, 255));
            break;

        case 0xFF0AF5:  // RED ROW 3
            LED::setSolidColor(CHSV(32, 255, 255));
            break;

        case 0xFF38C7:  // RED ROW 4
            LED::setSolidColor(CHSV(48, 255, 255));
            break;
    
        case 0xFF18E7:  // RED ROW 5
            LED::setSolidColor(CHSV(64, 255, 255));
            break;

        case 0xFF9A65:  // GREEN
            LED::setSolidColor(CRGB::Green);
            break;

        case 0xFFAA55:  // GREEN ROW 2
            LED::setSolidColor(CHSV(108, 255, 255));
            break;

        case 0xFF8A75:  // GREEN ROW 3
            LED::setSolidColor(CHSV(121, 255, 255));
            break;

        case 0xFFB847:  // GREEN ROW 4
            LED::setSolidColor(CHSV(134, 255, 255));
            break;

        case 0xFF9867:  // GREEN ROW 5
            LED::setSolidColor(CHSV(147, 255, 255));
            break;

        case 0xFFA25D:  // BLUE
            LED::setSolidColor(CRGB::Blue);
            break;

        case 0xFF926D:  // BLUE ROW 2
            LED::setSolidColor(CHSV(179, 255, 255));
            break;

        case 0xFFB24D:  // BLUE ROW 3
            LED::setSolidColor(CHSV(198, 255, 255));
            break;

        case 0xFF7887:  // BLUE ROW 4
            LED::setSolidColor(CHSV(217,255,255));
            break;

        case 0xFF58A7:  // BLUE ROW 5
            LED::setSolidColor(CHSV(236, 255, 255));
            break;

        case 0xFF22DD:  // WHITE
            LED::setSolidColor(CRGB::White);
            break;

        case 0xFF12ED:  // WHITE ROW 2
            LED::setSolidColor(CRGB(192, 192, 192));
            break;

        case 0xFF32CD:  // WHITE ROW 3
            LED::setSolidColor(CRGB(128, 128, 128));
            break;

        case 0xFFF807:  // WHITE ROW 4
            LED::setSolidColor(CRGB(64, 64, 64));
            break;

        case 0xFFD827:  // WHITE ROW 5
            LED::setSolidColor(CRGB(32, 32, 32));
            break;

        default:
            char rgch[22];
            sprintf(rgch, "IR %X ??", code);
            Display::status(3, rgch);
        
    }
}
