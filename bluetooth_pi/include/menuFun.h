#ifndef MENUFUN_H
#define MENUFUN_H

namespace menu{

    enum State{
        MENU1_GRAFIC_ESPECTRAL,
        MENU1_CANALS_ACTIUS,
        MENU1_INHIBIR_ESPECTRE,

        GRAFIC_ESPECTRAL,
        CANALS_ACTIUS,

        MENU2_CANVI_HORA,
        MENU2_CANVI_ALARMA,
        MENU2_ACTIVAR_ALARMA,

        HORA_ESPERA_HORA,
        HORA_INCR_HORA,
        HORA_ESPERA_MINUT,
        HORA_INCR_MINUT,

        ALARMA_ESPERA_HORA,
        ALARMA_INCR_HORA,
        ALARMA_ESPERA_MINUT,
        ALARMA_INCR_MINUT,

        APAGAR,

        TOTAL_ESTATS,
        NA, //No aplica
        IN  //Input intraestat
    };

    extern State state;

    void beginAnalisiEsp(void);
    void toggleInhibir(void);
    void toggleAlarma(void);
    void alarmIncSingle(void);
    void alarmIncMult(void);

} //namespace menu

#endif // MENUFUN_H
