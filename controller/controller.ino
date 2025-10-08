//==============================================================================
//                              INCLUDES 
//==============================================================================
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

//==============================================================================
//                               CONFIG 
//==============================================================================
#define PIN_ENA_IN 7
#define PIN_ENA 8 
#define PIN_DIR 10
#define PIN_CLK 9
#define PIN_DIR_POTENTIOMETER A0
#define PERIOD_MAX 50
#define PERIOD_MIN 2
#define DEADZONE 40
#define POTENTIOMETER_MAX 1024
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MIDDLE 512
#define IMPULSE_DELAY_MICRO 250

#define LCD_ADDRESS 0x27
#define COLUMN_COUNT 20
#define ROW_COUNT 2
#define LCD_REFRESH_PERIOD 250

typedef unsigned long time_ms;

//==============================================================================
//                            STEPPER CONTROL 
//==============================================================================

time_ms last_impulse{0};
time_ms period{PERIOD_MAX};
bool enabled{false};
bool reverse{false};

void init_stepper(){
    pinMode(PIN_ENA, OUTPUT);
    digitalWrite(PIN_ENA, LOW);
    pinMode(PIN_DIR, OUTPUT);
    digitalWrite(PIN_DIR, LOW);
    pinMode(PIN_CLK, OUTPUT);
    digitalWrite(PIN_CLK, LOW);
    pinMode(PIN_DIR_POTENTIOMETER, INPUT);
    pinMode(PIN_ENA_IN, INPUT);
}

void enable_stepper(){
    digitalWrite(PIN_ENA, HIGH);
    enabled = true;
}

void disable_stepper(){
    digitalWrite(PIN_ENA, LOW);
    enabled = false;
}

void update_stepper(){
   int potentiometer = analogRead(PIN_DIR_POTENTIOMETER) - POTENTIOMETER_MIDDLE;
   if(potentiometer < 0){
       potentiometer *= -1;
       reverse = true;
       digitalWrite(PIN_DIR, HIGH);
   }else{
       reverse = false;
       digitalWrite(PIN_DIR, LOW);
   }
   if(potentiometer < DEADZONE){
       potentiometer = 0;
   }
   if(potentiometer == 0){
       disable_stepper();
       return;
   }
   period = map(potentiometer,
                POTENTIOMETER_MIN, POTENTIOMETER_MIDDLE,
                PERIOD_MAX, PERIOD_MIN); 
}

void spin_stepper(){
    if(digitalRead(PIN_ENA_IN)==LOW){
        digitalWrite(PIN_ENA, LOW);
        return;
    }else{
      digitalWrite(PIN_ENA, HIGH);
    }
    if(millis() - last_impulse < period){
        return;
    }
    digitalWrite(PIN_CLK, HIGH);
   // delayMicroseconds(IMPULSE_DELAY_MICRO);
    digitalWrite(PIN_CLK, LOW);
    last_impulse = millis();
    
}


//==============================================================================
//                              MAIN 
//==============================================================================

void setup(){
    init_stepper();
}

void loop(){
    update_stepper();
    spin_stepper();
}
