#ifndef SLEEEP_H
#define SLEEEP_H

#include <platformTypes.h>
#include <Arduino.h>

namespace sleeep
{
    void onWakeUp(void);
    void powerDown (bool timer, uint32 time);

    extern bool from_sleep;
    extern struct timeval instant;
    
}; // namespace sleep


#endif //SLEEP_H
