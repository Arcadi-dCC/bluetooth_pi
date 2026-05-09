#ifndef SCREEN_H_
#define SCREEN_H_

#include <platformTypes.h>

namespace screen{

    void config(void);

    void print_MENU1_BLUETOOTH(void);
    void print_GRAFIC_ESPECTRAL(void);
    void print_CANALS_ACTIUS(void);
    void print_INHIBIR_MAN(void);
    void print_MENU2_CLOCKS(void);
    void print_MOD_RELLOTGE(void);
    void print_MOD_ALARMA(void);

    void updateTime(void);

}; //namespace screen

#endif //SCREEN_H_