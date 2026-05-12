#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <platformTypes.h>

namespace bluetooth{

    uint8 init(void);
    
    void inhibEsp(void);

    extern bool inhibir;

}; //namespace bluetooth

#endif // BLUETOOTH_H
