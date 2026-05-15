#ifndef SCREEN_H_
#define SCREEN_H_

#include <platformTypes.h>

namespace screen{

    //Totes les pantalles que poden ser imprimides
    enum Pantalla{
        MENU1_BLUETOOTH,
        GRAFIC_ESPECTRAL,
        CANALS_ACTIUS,
        INHIBIR_MAN,

        MENU2_CLOCKS,
        MOD_RELLOTGE,
        MOD_ALARMA,

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
    void print_INHIBIR_MAN(void);
    void print_MENU2_CLOCKS(void);
    void print_MOD_RELLOTGE(void);
    void print_MOD_ALARMA(void);

    void updateTime(void);
    void updateTopBarAlarm(void);
    void updateTopBarJam(void);

}; //namespace screen

#endif //SCREEN_H_