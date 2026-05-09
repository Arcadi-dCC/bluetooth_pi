#include <screen.h>
#include <menuFun.h>
#include <clocks.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#define FONT_SIZE 4

//Posicions Y per cada línia a mostrar a la pantalla
#define LIN_SUP 40 //altura de la linia superior a la pantalla
#define LIN_SEP 56 //separacio entre linies. Dona per a 5 línies (0, 1, 2, 3 i 4)
#define MARGE 25 //Marge del text a escriure (deixa espai al cursor)
#define SUBMARGE 7 //Per fer subllistes, i donar marge extra als rectangles del rellotge, de menu2

namespace screen{

    TFT_eSPI screen = TFT_eSPI();

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

    Pantalla pantalla = TOTAL_PANTALLES; //indica pantalla mostrada actualment

    //Conigura i inicialitza la pantalla TFT
    void config(void){

        screen.init();
        screen.setRotation(2); //Mode vertical. Per girar 180º, passar 0 per parametre.
        screen.fillScreen(TFT_BLACK); //Pantalla en negre
        screen.setTextColor(TFT_WHITE); //Lletres en blanc
        screen.setTextFont(FONT_SIZE);
        pantalla = TOTAL_PANTALLES; //Per defecte en cap pantalla, per assegurar que dibuixarà un menu
        print_MENU1_BLUETOOTH();
    }

    //Recull per paràmetre la línia en la que es vol escriure (0, 1, 2, 3, 4)
    //Retorna la posició y de la pantalla associada a la línia
    sint32 linia(uint8 num) { return (sint32)(LIN_SUP + num * LIN_SEP); }

    //Dibuixa un triangle indicador al limit esquerre de la pantalla, i a l'alçada y.
    void cursor(void)
    {
        screen.fillRect(0, linia(0), MARGE, TFT_HEIGHT, TFT_BLACK); //neteja la zona del cursor

        uint16 y;

        switch(menu::state)
        {
            //Casos on el cursor va a la línia 0
            case (menu::MENU1_GRAFIC_ESPECTRAL):
            case (menu::CANAL_ACTIU_1):
            case (menu::HORA_ESPERA_HORA):
            case (menu::HORA_INCR_HORA):
            case (menu::ALARMA_ESPERA_HORA):
            case (menu::ALARMA_INCR_HORA):
            {
                y = linia(0); break;
            }

            //Casos on el cursor va a la línia 1
            case (menu::MENU1_CANALS_ACTIUS):
            case (menu::CANAL_ACTIU_2):
            case (menu::MENU2_CANVI_HORA):
            case (menu::HORA_ESPERA_MINUT):
            case (menu::HORA_INCR_MINUT):
            case (menu::ALARMA_ESPERA_MINUT):
            case (menu::ALARMA_INCR_MINUT):
            {
                y = linia(1); break;
            }

            //Casos on el cursor va a la línia 2
            case (menu::MENU1_INHIBIR_MANUAL):
            case (menu::CANAL_ACTIU_3):
            case (menu::ALARMA_ESPERA_CANAL):
            case (menu::ALARMA_INCR_CANAL):
            {
                y = linia(2); break;
            }

            //Casos on el cursor va a la línia 3
            case (menu::MENU1_INHIBIR_ESPECTRE):
            case (menu::CANAL_ACTIU_4):
            case (menu::MENU2_CANVI_ALARMA):
            {
                y = linia(3); break;
            }

            //Casos on el cursor va a la línia 4
            case (menu::CANAL_ACTIU_5):
            case (menu::MENU2_ACTIVAR_ALARMA):
            {
                y = linia(4); break;
            }
            default:{
                /*Do nothing*/
            }
        }
        screen.fillTriangle(0,y,0,y+20,17,y+10, TFT_WHITE);
    }

    //Imprimeix per pantalla el menu 1 (bluetooth).
    void print_MENU1_BLUETOOTH(void)
    {
        if(pantalla != MENU1_BLUETOOTH)
        {
            pantalla = MENU1_BLUETOOTH;
            screen.fillRect(MARGE, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions
            screen.drawString("Grafic espectral", MARGE, linia(0));
            screen.drawString("Canals actius", MARGE, linia(1));
            screen.drawString("Inhibir canal (man)", MARGE, linia(2));
            screen.drawString("Inhibir espectre", MARGE, linia(3));
        }
        cursor();
    }

    //Imprimeix er pantalla el gràfic espectral.
    void print_GRAFIC_ESPECTRAL(void)
    {
        if(pantalla != GRAFIC_ESPECTRAL)
        {
            pantalla = GRAFIC_ESPECTRAL;
            screen.fillRect(0, linia(0), TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja el cursor i la zona de les opcions
        }
    }

    //Imprimeix per pantalla una llista amb els canals més actius i el seu RSSI.
    void print_CANALS_ACTIUS(void)
    {
        if(pantalla != CANALS_ACTIUS)
        {
            pantalla = CANALS_ACTIUS;
            screen.fillRect(MARGE, linia(0), TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions

            //Troba els canals actius i el seu RSSI
            uint8 canal[5];
            sint16 rssi[5];

            //trucar funció de bluetooth per plenar arrays

            for(uint8 i=0;i<5;i++)
            {
                screen.setCursor(MARGE, linia(i));
                screen.printf("Ch.%d", canal[i]);
                screen.setCursor(TFT_WIDTH/2, linia(i));
                screen.printf("RSSI: %d", rssi[i]);
            }
        }
        cursor();
    }

    //Imprimeix per pantalla el selector manual de canals a inhibir.
    void print_INHIBIR_MAN(void)
    {
        if(pantalla != INHIBIR_MAN)
        {
            pantalla = INHIBIR_MAN;
            screen.fillRect(0, linia(0), TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja el cursor i la zona de les opcions
        }
    }

    //Imprimeix per pantalla el menu 2 (rellotges).
    void print_MENU2_CLOCKS(void)
    {
        if(pantalla != MENU2_CLOCKS)
        {
            pantalla = MENU2_CLOCKS;
            screen.fillRect(MARGE, linia(0), TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions

            screen.drawRect(MARGE, linia(0)-SUBMARGE, 155, 37, TFT_WHITE); //Rectangle decoratiu  al voltant de l'hora
            screen.setCursor(MARGE+SUBMARGE, linia(0));
            screen.printf("Hora - %d:%02d", clocks::time.hh, clocks::time.mm);

            screen.drawString("Modificar", MARGE+2*SUBMARGE, linia(1));

            screen.drawRect(MARGE, linia(2)-SUBMARGE, 175, 37, TFT_WHITE); //Rectangle decoratiu  al voltant de l'hora
            screen.setCursor(MARGE+SUBMARGE, linia(2));
            screen.printf("Alarma - %d:%02d", clocks::alarm.hh, clocks::alarm.mm);

            screen.drawString("Modificar", MARGE+2*SUBMARGE, linia(3));
            screen.drawString("Des/activar", MARGE+2*SUBMARGE, linia(4));

        }
        cursor();
    }

    //Actualitza el rellotge en pantalla que apareix al MENU 2 (rellotge)
    void updateTime(void)
    {
        if(pantalla == MENU2_CLOCKS)
        {
            screen.fillRect(MARGE+1, linia(0)-SUBMARGE+1, 153, 35, TFT_BLACK);
            screen.setCursor(MARGE+SUBMARGE, linia(0));
            screen.printf("Hora - %d:%02d", clocks::time.hh, clocks::time.mm);
        }
    }

    //Imprimeix per pantalla el menu per canviar l'hora.
    void print_MOD_RELLOTGE(void)
    {
        if(pantalla != MOD_RELLOTGE)
        {
            pantalla = MOD_RELLOTGE;
            screen.fillRect(MARGE, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions
            screen.setCursor(MARGE, linia(0));
            screen.printf("Hora:  %d", clocks::time.hh);
            screen.setCursor(MARGE, linia(1));
            screen.printf("Minut: %02d", clocks::time.mm);
        }
        cursor();
    }

    //Imprimeix per pantalla el menu per canviar l'alarma.
    void print_MOD_ALARMA(void)
    {
        if(pantalla != MOD_ALARMA)
        {
            pantalla = MOD_ALARMA;
            screen.fillRect(MARGE, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions
            screen.setCursor(MARGE, linia(0));
            screen.printf("Hora:  %d", clocks::alarm.hh);
            screen.setCursor(MARGE, linia(1));
            screen.printf("Minut: %02d", clocks::alarm.mm);
            screen.setCursor(MARGE, linia(2));
            screen.printf("Canal:  %d", clocks::alarm.ch);
        }
        cursor();
    }

}; //namespace screen
