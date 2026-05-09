#include <screen.h>

#include <SPI.h>
#include <TFT_eSPI.h>

#define SCR_WIDTH 320
#define SCR_HEIGHT 240
#define FONT_SIZE 4

namespace screen{

    TFT_eSPI screen = TFT_eSPI();
    Coord coord;

    //Configures and initiates the TFT LCD screen.
    void config(void){

        screen.init();
        screen.setRotation(2); //Landscape mode
        screen.fillScreen(TFT_BLACK); //All pixels turn cyan
        screen.setTextColor(TFT_WHITE); //Letters in black
    }

    void test(void){
        coord.w = 100;
        coord.h = 50;
        screen.drawCentreString("Hello, World!", coord.w, coord.h, FONT_SIZE);
    }

}; //namespace screen