#include <menu.h>

namespace menu{

    enum Input{
        ESQ_CURT,
        ESQ_LLARG,
        ESQ_SOLTAR,
        DRE_CURT,
        DRE_LLARG,
        TIMER,

        TOTAL_INPUTS,
        RES
    };

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

    volatile Input input = RES;
    State state = MENU1_GRAFIC_ESPECTRAL;

    const State fsm_paths[TOTAL_ESTATS][TOTAL_INPUTS] =
    {
        /*                             { ESQ_CURT , ESQ_LLARG , ESQ_SOLTAR , DRE_CURT , DRE_LLARG , TIMER } */
        /* MENU1_GRAFIC_ESPECTRAL   */ { MENU1_CANALS_ACTIUS , GRAFIC_ESPECTRAL , NA , MENU2_CANVI_HORA , APAGAR , NA},
        /* MENU1_CANALS_ACTIUS      */ { MENU1_INHIBIR_MANUAL , CANAL_ACTIU_1 , NA , MENU2_CANVI_HORA , APAGAR , NA },
        /* MENU1_INHIBIR_MANUAL     */ { MENU1_INHIBIR_ESPECTRE , INHIBIR_MANUAL , NA , MENU2_CANVI_HORA , APAGAR , NA },
        /* MENU1_INHIBIR_ESPECTRE   */ { MENU1_GRAFIC_ESPECTRAL , INHIBIR_ESPECTRE , NA , MENU2_CANVI_HORA , APAGAR , NA },
        
        /* GRAFIC_ESPECTRAL         */ { NA , NA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* CANAL_ACTIU_1            */ { CANAL_ACTIU_2 , INHIBIR_MANUAL , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },
        /* CANAL_ACTIU_2            */ { CANAL_ACTIU_3 , INHIBIR_MANUAL , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },
        /* CANAL_ACTIU_3            */ { CANAL_ACTIU_4 , INHIBIR_MANUAL , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },
        /* CANAL_ACTIU_4            */ { CANAL_ACTIU_5 , INHIBIR_MANUAL , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },
        /* CANAL_ACTIU_5            */ { CANAL_ACTIU_1 , INHIBIR_MANUAL , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },
        /* INHIBIR_MANUAL           */ { IN , IN , IN , MENU1_INHIBIR_MANUAL , APAGAR , NA },
        /* INHIBIR_ESPECTRE         */ { NA , NA , NA , MENU1_INHIBIR_ESPECTRE , APAGAR , NA },

        /* MENU2_CANVI_HORA         */ { MENU2_CANVI_ALARMA , HORA_ESPERA_HORA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* MENU2_CANVI_ALARMA       */ { MENU2_ACTIVAR_ALARMA , ALARMA_ESPERA_HORA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* MENU2_ACTIVAR_ALARMA     */ { MENU2_CANVI_HORA , IN , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },

        /* HORA_ESPERA_HORA         */ { IN , HORA_INCR_HORA , NA , MENU2_CANVI_HORA , APAGAR , HORA_ESPERA_MINUT },
        /* HORA_INCR_HORA           */ { NA , NA , HORA_ESPERA_HORA , MENU2_CANVI_HORA , APAGAR , NA },
        /* HORA_ESPERA_MINUT        */ { IN , HORA_INCR_MINUT , NA , MENU2_CANVI_HORA , APAGAR , MENU2_CANVI_HORA },
        /* HORA_INCR_MINUT          */ { NA , NA , HORA_ESPERA_MINUT , MENU2_CANVI_HORA , APAGAR , NA },

        /* ALARMA_ESPERA_HORA       */ { IN , ALARMA_INCR_HORA , NA , MENU2_CANVI_ALARMA , APAGAR , ALARMA_ESPERA_MINUT },
        /* ALARMA_INCR_HORA         */ { NA , NA , ALARMA_ESPERA_HORA , MENU2_CANVI_ALARMA , APAGAR , NA },
        /* ALARMA_ESPERA_MINUT      */ { IN , ALARMA_INCR_MINUT , NA , MENU2_CANVI_ALARMA , APAGAR , ALARMA_ESPERA_CANAL },
        /* ALARMA_INCR_MINUT        */ { NA , NA , ALARMA_ESPERA_MINUT , MENU2_CANVI_ALARMA , APAGAR , NA },
        /* ALARMA_ESPERA_CANAL      */ { IN , ALARMA_INCR_CANAL , NA , MENU2_CANVI_ALARMA , APAGAR , MENU2_CANVI_ALARMA },
        /* ALARMA_INCR_CANAL        */ { NA , NA , ALARMA_ESPERA_CANAL , MENU2_CANVI_ALARMA , APAGAR , NA },
        
        /* APAGAR                   */ { NA , NA , NA , NA , NA , NA }
    };

    //State Actuator. Calls the actions linked to each state.
    void (*stateAct[TOTAL_ESTATS])(void) =
    {
        /* MENU1_GRAFIC_ESPECTRAL */    fun,
        /* MENU1_CANALS_ACTIUS */       fun,
        /* MENU1_INHIBIR_MANUAL */      fun,
        /* MENU1_INHIBIR_ESPECTRE */    fun,

        /* GRAFIC_ESPECTRAL */          fun,
        /* CANAL_ACTIU_1 */             fun,
        /* CANAL_ACTIU_2 */             fun,
        /* CANAL_ACTIU_3 */             fun,
        /* CANAL_ACTIU_4 */             fun,
        /* CANAL_ACTIU_5 */             fun,
        /* INHIBIR_MANUAL */            fun,
        /* INHIBIR_ESPECTRE */          fun,

        /* MENU2_CANVI_HORA */          fun,
        /* MENU2_CANVI_ALARMA */        fun,
        /* MENU2_ACTIVAR_ALARMA */      fun,

        /* HORA_ESPERA_HORA */          fun,
        /* HORA_INCR_HORA */            fun,
        /* HORA_ESPERA_MINUT */         fun,
        /* HORA_INCR_MINUT */           fun,

        /* ALARMA_ESPERA_HORA */        fun,
        /* ALARMA_INCR_HORA */          fun,
        /* ALARMA_ESPERA_MINUT */       fun,
        /* ALARMA_INCR_MINUT */         fun,
        /* ALARMA_ESPERA_CANAL */       fun,
        /* ALARMA_INCR_CANAL */         fun,

        /* APAGAR */                    fun
    };

    //Initializes menu FSM
    void init(void){
        state = MENU1_GRAFIC_ESPECTRAL;
        input = RES;

        //Start interrupt
    }
    
    //State Manager. Manages the switching between states in the menu FSM.
    void stateMgr(void){

        if(fsm_paths[state][input] < TOTAL_ESTATS)
        {
            state = fsm_paths[state][input];
            input = RES;
        }
        else if(fsm_paths[state][input] == NA)
        {
            input = RES;
        }
        else //fsm_paths[state][input] == IN
        {
            /*Do nothing*/
        }

        stateAct[state]();

        //END:restore interrupt
    }

    void fun(void){/*Dummy funciton*/};

};