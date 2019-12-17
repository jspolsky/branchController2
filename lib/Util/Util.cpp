
#include <Arduino.h>
#include <BranchController.h>
#include <Util.h>

// better debugging. Inspired from https://gist.github.com/asheeshr/9004783 with some modifications

namespace Util {

    void setup(void) {
        #ifdef DEBUG
            uint32_t tmEnd = millis() + 1000;
            Serial.begin(250000);
            while (!Serial && millis() < tmEnd);
        #endif
    }
}

void dprintf(char const* pszFmt UNUSED_IN_RELEASE, ... ) {

    #ifdef DEBUG
        char const* pszTmp;

        va_list argv;
        va_start(argv, pszFmt);

        pszTmp = pszFmt;
        while (*pszTmp)
        {
            if (*pszTmp == '%')
            {
                pszTmp++;
                switch (*pszTmp)
                {
                    case 'd':   
                        Serial.print(va_arg(argv, int));    
                        break;

                    case 'x':   
                        Serial.print(va_arg(argv, int), HEX);    
                        break;

                    case 'l':   
                        Serial.print(va_arg(argv, long));
                        break;

                    case 'u':
                        Serial.print(va_arg(argv, unsigned long));
                        break;

                    case 'f':
                        Serial.print(va_arg(argv, double));
                        break;

                    case 'F':
                        Serial.print(va_arg(argv, double), 8);
                        break;

                    case 'c':
                        Serial.print((char) va_arg(argv, int));
                        break;

                    case 's':
                        Serial.print(va_arg(argv, char*));
                        break;

                    case '%':
                        Serial.print('%');
                        break;

                    default:
                        break;
                }
            }
            else if (*pszTmp == '\n')
            {
                Serial.println();
            }
            else
            {
                Serial.print(*pszTmp);
            }

            pszTmp++;
        }
    #endif
}
