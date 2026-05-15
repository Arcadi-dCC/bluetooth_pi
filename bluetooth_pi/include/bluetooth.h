#ifndef BLUETOOTH_H
#define BLUETOOTH_H

#include <platformTypes.h>

#define BT_TOTAL_CHANNELS 79 //número de canals bluetooth

#define BT_NUM_READINGS 100 //repeticions a fer durant la detecció d'activitat

namespace bluetooth{

    uint8 init(void);
    void jamMgr(void);
    void readMgr(void);

    enum Action{

        START_JAM = 0,
        JAMMING,
        STOP_JAM,
        READING,
        OFF
    };

    extern Action action;

}; //namespace bluetooth

#endif // BLUETOOTH_H
