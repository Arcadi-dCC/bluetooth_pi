#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <platformTypes.h>

namespace bluetooth{

    uint8 init(void);
    
    void jamMgr(void);

    enum Action{

        START_JAM = 0,
        JAMMING,
        STOP_JAM,
        OFF
    };

    extern Action action;

}; //namespace bluetooth

#endif // BLUETOOTH_H
