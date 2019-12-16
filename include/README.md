Hey! I hate C++ objects. So I don't really use them that much. We use namespaces instead.

Each "module" defines one namespace and consists of a .h file and a .cpp file. The average module implements both setup() and void() which do the usual Arduino things.

Global constants (like pin assignments) go in BranchController.h. 
Specific settings to this module go in ModuleName.h.

ModuleName.h
---

```
// Description of the module is 
// in the .h file

#pragma once

namespace ModuleName {
    void setup(void);
    void loop(void); 
}

```

ModuleName.cpp
---

```
#include <Arduino.h>
#include <BranchController.h>
#include <ModuleName.h>

namespace ModuleName {

    void setup(void)
    {

    }

    void loop(void)
    {

    }

}
```
