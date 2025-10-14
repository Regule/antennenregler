//==============================================================================
//                               CONFIG 
//==============================================================================

// Motor control
#define PIN_ENA 8 
#define PIN_DIR 10
#define PIN_CLK 9

// Front panel potentiometer
#define PIN_DIR_POTENTIOMETER A0
#define PERIOD_MAX 50
#define PERIOD_MIN 2
#define DEADZONE 40
#define POTENTIOMETER_MAX 1024
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MIDDLE 512
#define IMPULSE_DELAY_MICRO 250

// Front panel buttons
#define PIN_BUTTON_MINUS 6
#define PIN_BUTTON_X 4
#define PIN_BUTTON_PLUS 5
#define PIN_BUTTON_C 3
#define PIN_CONTROLLER_ENABLE 7 
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
}

void update_stepper(){
   int potentiometer = analogRead(PIN_DIR_POTENTIOMETER) - POTENTIOMETER_MIDDLE;
   if(potentiometer < 0){
       potentiometer *= -1;
       reverse = true;
   }else{
       reverse = false;
   }
   if(potentiometer < DEADZONE){
       potentiometer = 0;
   }
   if(!enabled || potentiometer == 0){
      digitalWrite(PIN_ENA, LOW);
      return;
   }
   digitalWrite(PIN_ENA, HIGH);
   digitalWrite(PIN_DIR, reverse?HIGH:LOW);

   period = map(potentiometer,
                POTENTIOMETER_MIN, POTENTIOMETER_MIDDLE,
                PERIOD_MAX, PERIOD_MIN); 
}

void spin_stepper(){
    if(!enabled){
        return;
    if(millis() - last_impulse < period){
        return;
    }
    digitalWrite(PIN_CLK, HIGH);
    delayMicroseconds(IMPULSE_DELAY_MICRO);
    digitalWrite(PIN_CLK, LOW);
    last_impulse = millis();  
  }
}

//==============================================================================
//                        BUTTONS HANDLING 
//==============================================================================
int button_c_state = HIGH;

void handle_buttons(){
    if(button_c_state == HIGH && digitalRead(PIN_BUTTON_C) == LOW){
        enabled = !enabled;
        button_c_state = LOW;
    }else if(button_c_state == LOW && digitalRead(PIN_BUTTON_C) == HIGH){
        button_c_state = HIGH;
    }
    if(enabled){
        digitalWrite(PIN_CONTROLLER_ENABLE,HIGH);
    }else{
        digitalWrite(PIN_CONTROLLER_ENABLE,LOW);
    }
}

//==============================================================================
//                              MAIN 
//==============================================================================

void setup(){
    init_stepper();

    // Setting up buttons
    pinMode(PIN_BUTTON_MINUS, INPUT); 
    pinMode(PIN_BUTTON_X, INPUT); 
    pinMode(PIN_BUTTON_PLUS , INPUT); 
    pinMode(PIN_BUTTON_C , INPUT); 

    // Setting up connections to controller
    pinMode(PIN_CONTROLLER_ENABLE, OUTPUT);
    digitalWrite(PIN_CONTROLLER_ENABLE, LOW);
}

void loop(){
    update_stepper();
    spin_stepper();
    handle_buttons();
}
