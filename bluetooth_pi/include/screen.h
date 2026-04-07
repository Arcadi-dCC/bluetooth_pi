#ifndef SCREEN_H_
#define SCREEN_H_

#include <platformTypes.h>

namespace screen{

    void config(void);
    void test(void);

    struct Coord{
        uint16 w;
        uint16 h;
    };

    extern Coord coord;

}; //namespace screen

#endif //SCREEN_H_