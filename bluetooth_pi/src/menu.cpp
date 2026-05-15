#include <menu.h>
#include <menuFun.h>
#include <buttons.h>
#include <screen.h>
#include <Arduino.h>

namespace menu{

    const State fsm_paths[TOTAL_ESTATS][buttons::TOTAL_INPUTS] =
    {
        /*                             { ESQ_CURT , ESQ_LLARG , ESQ_SOLTAR , DRE_CURT , DRE_LLARG , TIMER } */
        /* MENU1_GRAFIC_ESPECTRAL   */ { MENU1_CANALS_ACTIUS , GRAFIC_ESPECTRAL , NA , MENU2_CANVI_HORA , APAGAR , NA},
        /* MENU1_CANALS_ACTIUS      */ { MENU1_INHIBIR_ESPECTRE , CANALS_ACTIUS , NA , MENU2_CANVI_HORA , APAGAR , NA },
        /* MENU1_INHIBIR_ESPECTRE   */ { MENU1_GRAFIC_ESPECTRAL , IN , NA , MENU2_CANVI_HORA , APAGAR , NA },
        
        /* GRAFIC_ESPECTRAL         */ { NA , NA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* CANALS_ACTIUS            */ { NA , NA , NA , MENU1_CANALS_ACTIUS , APAGAR , NA },

        /* MENU2_CANVI_HORA         */ { MENU2_CANVI_ALARMA , HORA_ESPERA_HORA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* MENU2_CANVI_ALARMA       */ { MENU2_ACTIVAR_ALARMA , ALARMA_ESPERA_HORA , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },
        /* MENU2_ACTIVAR_ALARMA     */ { MENU2_CANVI_HORA , IN , NA , MENU1_GRAFIC_ESPECTRAL , APAGAR , NA },

        /* HORA_ESPERA_HORA         */ { IN , HORA_INCR_HORA , NA , MENU2_CANVI_HORA , APAGAR , HORA_ESPERA_MINUT },
        /* HORA_INCR_HORA           */ { NA , NA , HORA_ESPERA_HORA , HORA_ESPERA_HORA , APAGAR , IN },
        /* HORA_ESPERA_MINUT        */ { IN , HORA_INCR_MINUT , NA , MENU2_CANVI_HORA , APAGAR , MENU2_CANVI_HORA },
        /* HORA_INCR_MINUT          */ { NA , NA , HORA_ESPERA_MINUT , HORA_ESPERA_MINUT , APAGAR , IN },

        /* ALARMA_ESPERA_HORA       */ { IN , ALARMA_INCR_HORA , NA , MENU2_CANVI_ALARMA , APAGAR , ALARMA_ESPERA_MINUT },
        /* ALARMA_INCR_HORA         */ { NA , NA , ALARMA_ESPERA_HORA , ALARMA_ESPERA_HORA , APAGAR , IN },
        /* ALARMA_ESPERA_MINUT      */ { IN , ALARMA_INCR_MINUT , NA , MENU2_CANVI_ALARMA , APAGAR , MENU2_CANVI_ALARMA },
        /* ALARMA_INCR_MINUT        */ { NA , NA , ALARMA_ESPERA_MINUT , ALARMA_ESPERA_MINUT , APAGAR , IN },
        
        /* APAGAR                   */ { NA , NA , NA , NA , NA , NA }
    };

    //State Actuator. Calls the actions linked to each state.
    void (*stateAct[TOTAL_ESTATS])(void) =
    {
        /* MENU1_GRAFIC_ESPECTRAL */    screen::print_MENU1_BLUETOOTH,
        /* MENU1_CANALS_ACTIUS */       screen::print_MENU1_BLUETOOTH,
        /* MENU1_INHIBIR_ESPECTRE */    toggleInhibir,

        /* GRAFIC_ESPECTRAL */          beginAnalisiEsp,
        /* CANALS_ACTIUS */             beginAnalisiEsp,

        /* MENU2_CANVI_HORA */          screen::print_MENU2_CLOCKS,
        /* MENU2_CANVI_ALARMA */        screen::print_MENU2_CLOCKS,
        /* MENU2_ACTIVAR_ALARMA */      toggleAlarma,

        /* HORA_ESPERA_HORA */          alarmIncSingle,
        /* HORA_INCR_HORA */            alarmIncMult,
        /* HORA_ESPERA_MINUT */         alarmIncSingle,
        /* HORA_INCR_MINUT */           alarmIncMult,

        /* ALARMA_ESPERA_HORA */        alarmIncSingle,
        /* ALARMA_INCR_HORA */          alarmIncMult,
        /* ALARMA_ESPERA_MINUT */       alarmIncSingle,
        /* ALARMA_INCR_MINUT */         alarmIncMult,

        /* APAGAR */                    fun
    };

    //Initializes menu FSM
    void init(void){
        state = MENU1_GRAFIC_ESPECTRAL;
    }
    
    //State Manager. Manages the switching between states in the menu FSM.
    void stateMgr(void){

        if(buttons::input < buttons::TOTAL_INPUTS) //hi ha un nou input
        {
            if(fsm_paths[state][buttons::input] < TOTAL_ESTATS)
            {
                state = fsm_paths[state][buttons::input];
                stateAct[state]();
            }
            else if(fsm_paths[state][buttons::input] == IN)
            {
                stateAct[state]();        
            }
            else //fsm_paths[state][input] == NA
            {
                /*Do nothing. L'input no està associat a cap acció en l'estat actual.*/
            }
            buttons::input = buttons::RES; //assegura que input sigui igual a RES en acabar
        }
    }

};