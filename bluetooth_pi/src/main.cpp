#include <Arduino.h>
#include <bluetooth.h>
#include <buttons.h>

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");

  /*if(bluetooth::init())
  {
      Serial.println("Failed to initialize radio!");
      for(;;);
  }*/

  if(buttons::init())
  {
    Serial.println("Failed to initialize timer from buttons!");
    for(;;);
  }
}

void loop() {

    switch(buttons::input)
    {
        case buttons::DRE_CURT:{
            buttons::input = buttons::RES;
            Serial.println("Botó dret, curt.");
            break;
        }
        case buttons::DRE_LLARG:{
            buttons::input = buttons::RES;
            Serial.println("Botó dret, llarg.");
            break;
        }
        case buttons::DRE_SOLTAR:{
            buttons::input = buttons::RES;
            Serial.println("Botó dret, soltat.");
            break;
        }
        case buttons::ESQ_CURT:{
            buttons::input = buttons::RES;
            Serial.println("Botó esquerre, curt.");
            break;
        }
        case buttons::ESQ_LLARG:{
            buttons::input = buttons::RES;
            Serial.println("Botó esquerre, llarg.");
            break;
        }
        case buttons::ESQ_SOLTAR:{
            buttons::input = buttons::RES;
            Serial.println("Botó esquerre, soltat.");
            break;
        }
        default:{
            /*Do nothing*/
        }
    }
}