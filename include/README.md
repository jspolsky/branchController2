Hey! I hate C++ objects. So I don't really use them that much. We use namespaces instead.

Each module consists of a .h file and a .cpp file.

include/ModuleName.h
---

```
// Description of the module is 
// in the .h file

#pragma once
namespace ModuleName {
    ... various declarations ...
}

```

ModuleName.cpp
---

```
#include "include/ModuleName.h"
namespace ModuleName {
    ... various definitions ...
}
```
