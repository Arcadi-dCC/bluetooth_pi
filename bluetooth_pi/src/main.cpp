#include <Arduino.h>
#include <bluetooth.h>
#include <buttons.h>

volatile bool short_press = false;
volatile bool long_press = false;
volatile bool release = false;
volatile unsigned char deciding_state = 0; //0-nothing going 1-debouncing 2-sorting 3-time threshold reached first

volatile hw_timer_t* timer0 = NULL;

//button ISR
void IRAM_ATTR buttonISR(void)
{
    switch(deciding_state)
    {
        case 0:{ //nothing going. Start debouncing
            if(digitalRead(6) == LOW)
            {
                deciding_state = 1;
                detachInterrupt(digitalPinToInterrupt(6));
                timerWrite((hw_timer_t*)timer0, 0);
                timerAlarmWrite((hw_timer_t*)timer0, 50e3, false);
                timerAlarmEnable((hw_timer_t*)timer0);
            }
            break;
        }
        case 2:{ //sorting. Button has been released before time threshold
            if(digitalRead(6) == HIGH)
            {
                deciding_state = 0;
                short_press = true;
            }
            break;
        }
        case 3:{ //time threshold has passed

            if(digitalRead(6) == HIGH)
            {
                deciding_state = 0;
                release = true;
            }
            break;
        }
        default:{
            /*Do nothing*/
        }
    }

    
}

//timer0 ISR
void IRAM_ATTR timerISR(void)
{
    switch(deciding_state){
        case 1:{ //debounce time finished. Check and begin sorting time.
            if(digitalRead(6) == LOW)
            {
                deciding_state = 2;
                timerWrite((hw_timer_t*)timer0, 0);
                timerAlarmWrite((hw_timer_t*)timer0, 2000e3, false);
                timerAlarmEnable((hw_timer_t*)timer0);
                attachInterrupt(digitalPinToInterrupt(6), buttonISR, CHANGE);
            }
            break;
        }
        case 2:{ //sorting. Time threshold has been reached without button being released
            if(digitalRead(6) == LOW)
            {
                deciding_state = 3;
                long_press = true;
            }
            break;
        }
        default:{
            /*Do nothing*/
        }
    }
}




void setup() {
  Serial.begin(115200); // Initialize serial communication
  delay(800);
  Serial.println("Serial COM Enabled");

  /*if(bluetooth::init())
  {
      Serial.println("Failed to initialize radio!");
      for(;;);
  }*/

//button interrupt setup
  pinMode(6, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(6), buttonISR, CHANGE);

  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt((hw_timer_t*)timer0, timerISR, true);
}

void loop() {
    if(short_press)
    {
        short_press = false;
        Serial.println("Button pressed shortly.");
    }

    if(long_press)
    {
        long_press = false;
        Serial.println("Button pressed loooong.");
    }

    if(release)
    {
        release = false;
        Serial.println("Button has been finally released");
    }
}