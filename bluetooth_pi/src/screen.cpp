#include <screen.h>
#include <menuFun.h>
#include <clocks.h>
#include <bluetooth.h>
#include <buttons.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#define FONT 4
#define FONT_SIZE 1

#define TOPBAR_FONT 1
#define TOPBAR_FONT_SIZE 2
#define TOPBAR_THRESHOLD 16

//Posicions Y per cada línia a mostrar a la pantalla
#define LIN_SUP 40 //altura de la linia superior a la pantalla
#define LIN_SEP 56 //separacio entre linies. Dona per a 5 línies (0, 1, 2, 3 i 4)
#define MARGE 25 //Marge del text a escriure (deixa espai al cursor)
#define SUBMARGE 7 //Per fer subllistes, i donar marge extra als rectangles del rellotge, de menu2

namespace screen{

    TFT_eSPI screen = TFT_eSPI();

    Pantalla pantalla = TOTAL_PANTALLES; //indica pantalla mostrada actualment

    //Conigura i inicialitza la pantalla TFT
    void config(void){

        screen.init();
        screen.setRotation(2); //Mode vertical. Per girar 180º, passar 0 per parametre.
        screen.fillScreen(TFT_BLACK); //Pantalla en negre
        screen.setTextColor(TFT_WHITE); //Lletres en blanc
        screen.setTextFont(FONT);
        screen.setTextSize(FONT_SIZE);
        pantalla = TOTAL_PANTALLES; //Per defecte en cap pantalla, per assegurar que dibuixarà un menu
        print_MENU1_BLUETOOTH();
        updateTime();
        updateTopBarAlarm();
        updateTopBarJam();
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

    //Esborra el contingut de tota la pantalla, incloent el cursor, excepte la barra superior
    void clearMainScreen(void){
        screen::screen.fillRect(0, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK);
    }

    //Imprimeix per pantalla el menu 1 (bluetooth).
    void print_MENU1_BLUETOOTH(void)
    {
        if(bluetooth::action == bluetooth::READING)
        {
            bluetooth::action == bluetooth::STOP_JAM; //Apaga el mòdul bluetooth si s'entra aquí després d'analitzar l'espectre
        }
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

    //Actualitza per pantalla el gràfic espectral amb la informació del canal i la seva intensitat passats per paràmetre
    void print_GRAFIC_ESPECTRAL(uint8 canal, uint16 intensitat)
    {
        screen.fillRect(canal*3, linia(4)-BT_NUM_READINGS*2, 3, TFT_HEIGHT, TFT_BLACK); //neteja la columna del canal actual
        uint16 i = 2*intensitat+1; //per fer el gràfic més visible
        screen.fillRect(canal*3, linia(4)-i, 3, i, TFT_WHITE);

        //Dibuixem un petit rectangle a la posició del següent canal per indicar per on va l'escanneig.
        i = ((canal + 1) % BT_TOTAL_CHANNELS) * 3;
        screen.fillRect(i, linia(4)+10, 3, 6, TFT_WHITE);
    }

    //Imprimeix per pantalla una llista amb els canals més actius i el seu RSSI.
    void print_CANALS_ACTIUS(/*uint8 canal, uint16 intensitat*/void)
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
            updateTime(); //escriu l'hora tan dins del requadre com a la topbar

            screen.drawString("Modificar", MARGE+2*SUBMARGE, linia(1));

            screen.drawRect(MARGE, linia(2)-SUBMARGE, 175, 37, TFT_WHITE); //Rectangle decoratiu  al voltant de l'hora
            screen.setCursor(MARGE+SUBMARGE, linia(2));
            screen.printf("Alarma - %d:%02d", clocks::alarm.hh, clocks::alarm.mm);

            screen.drawString("Modificar", MARGE+2*SUBMARGE, linia(3));
            screen.drawString("Des/activar", MARGE+2*SUBMARGE, linia(4));

        }
        cursor();
    }

    //Actualitza els rellotges en pantalla, sempre a la barra superior i també al MENU 2 si s'està en aquesta pantalla.
    void updateTime(void)
    {
        //1r: Actualització a barra superior.
        screen.fillRect(0, 0, TFT_WIDTH/2, TOPBAR_THRESHOLD, TFT_BLACK);
        screen.setCursor(0, 0);
        screen.setTextSize(TOPBAR_FONT_SIZE); //Canvia temporalment a una lletra més petita
        screen.setTextFont(TOPBAR_FONT);
        screen.printf("%d:%02d", clocks::time.hh, clocks::time.mm);
        screen.setTextSize(FONT_SIZE); //Torna al tamany i lletra original.
        screen.setTextFont(FONT);

        //2n: Actualització a menu 2, si escau.
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
        pantalla = MOD_RELLOTGE;
        screen.fillRect(MARGE, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions
        screen.setCursor(MARGE, linia(0));
        screen.printf("Hora:  %d", clocks::time.hh);
        screen.setCursor(MARGE, linia(1));
        screen.printf("Minut: %02d", clocks::time.mm);
        cursor();
    }

    //Imprimeix per pantalla el menu per canviar l'alarma.
    void print_MOD_ALARMA(void)
    {
        pantalla = MOD_ALARMA;
        screen.fillRect(MARGE, linia(0)-SUBMARGE, TFT_WIDTH, TFT_HEIGHT, TFT_BLACK); //neteja la zona de les opcions
        screen.setCursor(MARGE, linia(0));
        screen.printf("Hora:  %d", clocks::alarm.hh);
        screen.setCursor(MARGE, linia(1));
        screen.printf("Minut: %02d", clocks::alarm.mm);
        screen.setCursor(MARGE, linia(2));
        screen.printf("Canal:  %d", clocks::alarm.ch);
        cursor();
    }

    //Mostra l'estat de l'alarma (on-verd / off-vermell) a la barra superior
    void updateTopBarAlarm(void)
    {
        screen.fillRect(175, 0, 35, TOPBAR_THRESHOLD, TFT_BLACK);
        
        //Selecciona el color del text segons l'estat de l'alarma
        if(clocks::alarm.on == true)
        {
            screen.setTextColor(TFT_GREEN);
        }
        else
        {
            screen.setTextColor(TFT_RED);
        }
        screen.setTextSize(TOPBAR_FONT_SIZE); //Canvia temporalment a una lletra més petita
        screen.setTextFont(TOPBAR_FONT);
        
        screen.drawString("AL", 175, 0);
        
        screen.setTextColor(TFT_WHITE); //Retorna al color original
        screen.setTextSize(FONT_SIZE); //Torna al tamany i lletra original.
        screen.setTextFont(FONT);
    }

    //Mostra si es permet l'ús de bluetooth (sí-verd / no-vermell) a la barra superior
    void updateTopBarJam(void)
    {
        screen.fillRect(215, 0, 30, TOPBAR_THRESHOLD, TFT_BLACK);
        
        //Selecciona el color del text segons l'estat de l'inhibidor
        switch (bluetooth::action){
                case bluetooth::START_JAM:
                case bluetooth::JAMMING:
                {
                    screen.setTextColor(TFT_RED);
                    break;
                }
                case bluetooth::STOP_JAM:
                case bluetooth::READING:
                case bluetooth::OFF:
                {
                    screen.setTextColor(TFT_GREEN);
                    break;
                }
                default: {/*Do nothing*/}
        }


        screen.setTextSize(TOPBAR_FONT_SIZE); //Canvia temporalment a una lletra més petita
        screen.setTextFont(TOPBAR_FONT);
        
        screen.drawString("BT", 215, 0);
        
        screen.setTextColor(TFT_WHITE); //Retorna al color original
        screen.setTextSize(FONT_SIZE); //Torna al tamany i lletra original.
        screen.setTextFont(FONT);
    }

}; //namespace screen
