#ifndef SLEEEP_H
#define SLEEEP_H

#include <Arduino.h>

namespace sleeep
{
    void onWakeUp(void);
    void powerDown (void);

    extern bool from_sleep;
    extern struct timeval instant;
    
}; // namespace sleep


#endif //SLEEP_H
