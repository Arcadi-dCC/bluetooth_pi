#include <Arduino.h>
#include <bluetooth.h>
#include <clocks.h>

void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");

  if(bluetooth::init())
  {
      Serial.println("Failed to initialize bluetooth!");
      for(;;);
  }

  if(clocks::init())
  {
    Serial.println("Failed to initialize clock timer!");
      for(;;);
  }
}

void loop() {
    clocks::timeMgr();
    bluetooth::inhibir = true;
    bluetooth::inhibEsp();
}