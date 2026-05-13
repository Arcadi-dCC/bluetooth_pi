#include <Arduino.h>
#include <screen.h>
#include <menu.h>
#include <clocks.h>
#include <buttons.h>
#include <bluetooth.h>

void setup() {
  Serial.begin(115200); // Initialize serial communication
  while(!Serial); //Esperem a que serial es configuri
  Serial.println("Serial COM Enabled");

  menu::init();

  if(clocks::init())
  {
    Serial.println("Failed to initialize clock timer!");
      for(;;);
  }
  
  screen::config();

  if(buttons::init())
  {
    Serial.println("Failed to initialize buttons timer!");
    for(;;);
  }

  if(bluetooth::init())
  {
      Serial.println("Failed to initialize bluetooth!");
      for(;;);
  }
}

void loop() {
  menu::stateMgr();
  clocks::timeMgr();
  bluetooth::jamMgr();
}
