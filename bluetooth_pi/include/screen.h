#ifndef SCREEN_H_
#define SCREEN_H_

#include <platformTypes.h>

namespace screen{

    void config(void);
    void test(void);

    struct Coord{
        sint32 w;
        sint32 h;
    };

    extern Coord coord;

}; //namespace screen

#endif //SCREEN_H_