#include <menu.h>

namespace menu{

    enum Input{
        NOTHING,
        ESQ_CURT,
        ESQ_LLARG,
        ESQ_SOLTAR,
        DRE_CURT,
        DRE_LLARG,
        TIMER,
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
        ALARMA_ESPERA_HORA,
        ALARMA_INCR_MINUT,
        ALARMA_ESPERA_CANAL,
        ALARMA_INCR_CANAL,
        ALARMA_ESPERA_DURADA,
        ALARMA_INCR_DURADA,

    };

    volatile Input input;
    State state;

    //Initializes menu FSM
    void config(void){
        state = MENU1_GRAFIC_ESPECTRAL;
        
    }

    //Manages the switching between states in the menu FSM
    void stateMgr(void){

        switch(state){
            case MENU1_GRAFIC_ESPECTRAL:{
                switch(input){
                    case ESQ_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = GRAFIC_ESPECTRAL; break;}
                    default:{}
                }break;
            }
            case MENU1_CANALS_ACTIUS:{
                switch(input){
                    case ESQ_CURT: {state = MENU1_INHIBIR_MANUAL; break;}
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = CANAL_ACTIU_1; break;}
                    default:{}
                }break;
            }
            case MENU1_INHIBIR_MANUAL:{
                switch(input){
                    case ESQ_CURT: {state = MENU1_INHIBIR_ESPECTRE; break;}
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case MENU1_INHIBIR_ESPECTRE:{
                switch(input){
                    case ESQ_CURT: {state = MENU1_GRAFIC_ESPECTRAL; break;}
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = INHIBIR_ESPECTRE; break;}
                    default:{}
                }break;
            }
            case GRAFIC_ESPECTRAL:{
                switch(input){
                    case DRE_CURT: {state = MENU1_GRAFIC_ESPECTRAL; break;}
                    default:{}
                }break;
            }
            case CANAL_ACTIU_1:{
                switch(input){
                    case ESQ_CURT: {state = CANAL_ACTIU_2; break;}
                    case DRE_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case CANAL_ACTIU_2:{
                switch(input){
                    case ESQ_CURT: {state = CANAL_ACTIU_3; break;}
                    case DRE_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case CANAL_ACTIU_3:{
                switch(input){
                    case ESQ_CURT: {state = CANAL_ACTIU_4; break;}
                    case DRE_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case CANAL_ACTIU_4:{
                switch(input){
                    case ESQ_CURT: {state = CANAL_ACTIU_5; break;}
                    case DRE_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case CANAL_ACTIU_5:{
                switch(input){
                    case ESQ_CURT: {state = CANAL_ACTIU_1; break;}
                    case DRE_CURT: {state = MENU1_CANALS_ACTIUS; break;}
                    case ESQ_LLARG:{state = INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case INHIBIR_MANUAL:{
                switch(input){
                    case DRE_CURT: {state = MENU1_INHIBIR_MANUAL; break;}
                    default:{}
                }break;
            }
            case INHIBIR_ESPECTRE:{
                switch(input){
                    case DRE_CURT: {state = MENU1_INHIBIR_ESPECTRE; break;}
                    default:{}
                }break;
            }

            case MENU2_CANVI_HORA:{
                switch(input){
                    case ESQ_CURT: {state = MENU2_CANVI_ALARMA; break};
                    case DRE_CURT: {state = MENU1_GRAFIC_ESPECTRAL; break;}
                    case ESQ_LLARG:{state = HORA_ESPERA_HORA; break;}
                    default:{}
                }break;
            }
            case MENU2_CANVI_ALARMA:{
                switch(input){
                    case ESQ_CURT: {state = MENU2_ACTIVAR_ALARMA; break};
                    case DRE_CURT: {state = MENU1_GRAFIC_ESPECTRAL; break;}
                    case ESQ_LLARG:{state = ALARMA_ESPERA_HORA; break;}
                    default:{}
                }break;
            }
            case MENU2_ACTIVAR_ALARMA:{
                switch(input){
                    case ESQ_CURT: {state = MENU2_CANVI_HORA; break};
                    case DRE_CURT: {state = MENU1_GRAFIC_ESPECTRAL; break;}
                    default:{}
                }break;
            }
            case HORA_ESPERA_HORA:{
                switch(input){
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = HORA_INCR_HORA; break;}
                    case TIMER:    {state = HORA_ESPERA_MINUT; break;}
                    default:{}
                }break;
            }
            case HORA_INCR_HORA:{
                switch(input){
                    case ESQ_SOLTAR: {state = HORA_ESPERA_HORA; break};
                    default:{}
                }break;
            }
            case HORA_ESPERA_MINUT:{
                switch(input){
                    case DRE_CURT: {state = MENU2_CANVI_HORA; break;}
                    case ESQ_LLARG:{state = HORA_INCR_MINUT; break;}
                    case TIMER:    {state = MENU2_CANVI_HORA; break;}
                    default:{}
                }break;
            }
        }

        input = NOTHING;
        //END:restore interrupt
    }

};