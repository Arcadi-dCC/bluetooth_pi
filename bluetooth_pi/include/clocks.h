#ifndef clock_H
#define clock_H

#include <platformTypes.h>

namespace clocks{
    uint8 init(void);
    void timeMgr(void);

    //estructura per rellotge i alarma
    typedef struct Alarm{
    uint8 hh, mm, ss;
    bool on; //on-off per alarma
    }Alarm;

    extern Alarm time;
    extern Alarm alarm;
};

#endif //clock_H