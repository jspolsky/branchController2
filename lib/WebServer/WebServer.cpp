#include <Arduino.h>
#include <WebServer.h>
#include <Ethernet.h>
#include <Persist.h>
#include <LED.h>
#include <Util.h>

char *strsep(char **stringp, const char *delim);

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

        // Here's the plan: read one line at a time - up to BUFFER_SIZE bytes, then discard
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
                    output_html();

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

        if (!strcmp("/", szGet))
            return;

        char *pszTok = NULL;
        while (NULL != (pszTok = strsep(&szGet,"/?&")))
        {
            if (*pszTok)
            {
                if (!strcmp(pszTok, "r"))
                {
                    LED::setSolidColor(CRGB::Red);
                    return;
                }
                else if (!strcmp(pszTok, "g"))
                {
                    LED::setSolidColor(CRGB::Green);
                    return;
                }
                else if (!strcmp(pszTok, "b"))
                {
                    LED::setSolidColor(CRGB::Blue);
                    return;
                }
                else if (!strcmp(pszTok, "w"))
                {
                    LED::setSolidColor(CRGB::White);
                    return;
                }
                else if (!strncmp(pszTok, "w=", 2))
                    Persist::data.max_power = atoi(pszTok + 2);
                else if (!strncmp(pszTok, "o=", 2))
                    Persist::data.first_color = pszTok[2];
                else if (!strncmp(pszTok, "c=", 2))
                    Persist::data.color_correction = strtol(pszTok + 2, NULL, 16);
                else if (!strncmp(pszTok, "t=", 2))
                    Persist::data.color_temperature = strtol(pszTok + 2, NULL, 16);
                else if (!strncmp(pszTok, "b=", 2)) {
                    Persist::data.brightness = min(255, atoi(pszTok + 2));
                    LEDS.setBrightness(Persist::data.brightness);
                    Persist::data.gamma_correction = false; // because g= will be missing if gamma correction is unchecked
                    Persist::data.static_ip = false;        // because s= will be missing if static ip is unchecked
                }
                else if (!strncmp(pszTok, "g=", 2))
                    Persist::data.gamma_correction = (pszTok[2] == '1');
                else if (!strncmp(pszTok, "s=", 2))
                    Persist::data.static_ip = (pszTok[2] == '1');

                else if (!strncmp(pszTok, "i0=", 3))
                    Persist::data.ip_addr[0] = (uint8_t) atoi(pszTok + 3);
                else if (!strncmp(pszTok, "i1=", 3))
                    Persist::data.ip_addr[1] = (uint8_t) atoi(pszTok + 3);
                else if (!strncmp(pszTok, "i2=", 3))
                    Persist::data.ip_addr[2] = (uint8_t) atoi(pszTok + 3);
                else if (!strncmp(pszTok, "i3=", 3))
                    Persist::data.ip_addr[3] = (uint8_t) atoi(pszTok + 3);



                else
                    dbgprintf("unidentified token: {%s}\n", pszTok);
            }
        }

        Persist::write();
        LED::load_persistant_data();

    }

    void output_html()
    {
        client.println(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<!DOCTYPE HTML>"
            "<html>"
                "<h1>Branch Controller</h1>" 
                "<p>built " __DATE__ " " __TIME__ "</p>"
                "<hr>"
                "<form action=/>"
                    "Maximum Power: "
                    "<input name=w value='"

                        ); client.println(Persist::data.max_power); client.println(
                    
                    "'> milliwatts"
                    "<br>"
                    "RGB/GRB Order: "
                    "<select name=o>"
                        "<option value=r"
                        
                            ); if (Persist::data.first_color == 'r') client.println(" selected"); client.println(

                        ">RGB</option>"
                        "<option value=g"
                        
                            ); if (Persist::data.first_color == 'g') client.println(" selected"); client.println(

                        ">GRB</option>"
                    "</select>"
                    "<br>"
                    "Color Correction: "
                    "<input name=c value="
                    
                            ); client.println(Persist::data.color_correction, 16); client.println(

                    "> (hex RGB)"
                    "<br>"
                    "Color Temperature: "
                    "<input name=t value="
                    
                            ); client.println(Persist::data.color_temperature, 16); client.println(
                    
                    "> (hex RGB)"
                    "<br>"
                    "Brightness: "
                    "<input name=b value="
                    
                            ); client.println(Persist::data.brightness); client.println(
                    
                    "> (0-255)"
                    "<br>"
                    "<input type=checkbox id=g name=g value=1 checked><label for=g>Gamma Correction (Not implemented)</label>"
                    "<br>"
                    "<input type=checkbox id=s name=s value=1 "
                    
                            ); if (Persist::data.static_ip) client.println("checked"); client.println(
                    
                    "><label for=s>Static IP Address: </label> "
                    "<input name=i0 size=3 value="

                            ); client.println(Persist::data.ip_addr[0]); client.println(

                    ">.<input name=i1 size=3 value="

                            ); client.println(Persist::data.ip_addr[1]); client.println(

                    ">.<input name=i2 size=3 value="

                            ); client.println(Persist::data.ip_addr[2]); client.println(

                    ">.<input name=i3 size=3 value="

                            ); client.println(Persist::data.ip_addr[3]); client.println(

                    "><br / >" 

                    "<input type=submit>"
                    "<br>"
                    "Test colors: "
                    "<a href=/r>Red</a> "
                    "<a href=/g>Green</a> "
                    "<a href=/b>Blue</a> "
                    "<a href=/w>White</a> "
                "</form>"
            "</html>");
    }
}

