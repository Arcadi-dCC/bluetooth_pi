#ifndef MENUFUN_H_
#define MENUFUN_H_

namespace menu{

    enum State{
        MENU1_GRAFIC_ESPECTRAL,
        MENU1_CANALS_ACTIUS,
        MENU1_INHIBIR_MANUAL,
        MENU1_INHIBIR_ESPECTRE,

        GRAFIC_ESPECTRAL,
        CANAL_ACTIU_1,
        CANAL_ACTIU_2,
        CANAL_ACTIU_3,
        CANAL_ACTIU_4,
        CANAL_ACTIU_5,
        INHIBIR_MANUAL,
        INHIBIR_ESPECTRE,

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
        ALARMA_ESPERA_CANAL,
        ALARMA_INCR_CANAL,

        APAGAR,

        TOTAL_ESTATS,
        NA, //No aplica
        IN  //Input intraestat
    };

    extern State state;

    void modAlarma(void);
    void fun(void);


} //namespace menu

#endif // MENUFUN_H_