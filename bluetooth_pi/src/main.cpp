#include <Arduino.h>
#include <screen.h>

void setup() {
    Serial.begin(115200);
    screen::config();
    screen::test();
}

void loop() {
  
}