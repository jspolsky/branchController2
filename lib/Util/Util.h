#pragma once

#ifdef RELEASE
    #define UNUSED_IN_RELEASE __attribute__((unused))
#else 
    #define UNUSED_IN_RELEASE 
#endif

namespace Util 
{
    void setup(void);
}

void dbgprintf(char const *str, ...);
