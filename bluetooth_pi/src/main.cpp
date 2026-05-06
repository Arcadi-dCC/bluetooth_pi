#include <Arduino.h>
#include <bluetooth.h>
#include <buttons.h>

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");

  if(bluetooth::init())
  {
      Serial.println("Failed to initialize radio!");
      for(;;);
  }

  buttons::init();
  
}

void loop() {
    bluetooth::inhibir = true;
    bluetooth::inhibEsp();
}