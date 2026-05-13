#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <platformTypes.h>

#define BT_TOTAL_CHANNELS 79 //número de canals bluetooth

namespace bluetooth{

    uint8 init(void);
    void jamMgr(void);
    uint8 activitatEspectre(uint8* buf, uint8 len);

    enum Action{

        START_JAM = 0,
        JAMMING,
        STOP_JAM,
        OFF
    };

    extern Action action;

}; //namespace bluetooth

#endif // BLUETOOTH_H
