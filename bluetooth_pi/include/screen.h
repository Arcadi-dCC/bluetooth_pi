#ifndef SCREEN_H
#define SCREEN_H

#include <platformTypes.h>

namespace screen{

    //Totes les pantalles que poden ser imprimides
    enum Pantalla{
        MENU1_BLUETOOTH,
        GRAFIC_ESPECTRAL,
        CANALS_ACTIUS,
        INHIBIR_MAN,

        MENU2_CLOCKS,
        MOD_CLOCKS,

        TOTAL_PANTALLES
    };

    extern Pantalla pantalla;

    extern uint8 canals_actius[5];
    extern uint8 intensitats[5];

    void config(void);

    void clearMainScreen(void);

    void print_MENU1_BLUETOOTH(void);
    void print_GRAFIC_ESPECTRAL(uint8 canal, uint8 intensitat);
    void print_CANALS_ACTIUS(uint8 canal, uint8 intensitat);
    void printNoActius(void);
    void print_MENU2_CLOCKS(void);
    void print_MOD_CLOCKS(void);

    void updateTime(void);
    void updateTopBarAlarm(void);
    void updateTopBarJam(void);
    void sleep(void);

}; //namespace screen

#endif //SCREEN_H
