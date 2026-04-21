#include <Arduino.h>
#include <platformTypes.h>
#include <clocks.h>

void setup() {
    clocks::init();
}

void loop() {
    clocks::timeMgr();
}