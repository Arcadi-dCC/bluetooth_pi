#include <Arduino.h>
#include <clocks.h>
#include <buttons.h>
#include <bluetooth.h>

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");

  if(clocks::init())
  {
    Serial.println("Failed to initialize clock timer!");
      for(;;);
  }

  if(buttons::init())
  {
    Serial.println("Failed to initialize timer from buttons!");
    for(;;);
  }

  if(bluetooth::init())
  {
      Serial.println("Failed to initialize bluetooth!");
      for(;;);
  }
}

void loop() {
    clocks::timeMgr();
    bluetooth::inhibir = true;
    bluetooth::inhibEsp();
}