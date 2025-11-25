#include <AccelStepper.h>
 

#define PIN_DIR_POTENTIOMETER A0
#define SPEED_MAX 1000
#define SPEED_MIN 0
#define DEADZONE 40
#define POTENTIOMETER_MAX 1024
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MIDDLE 512
#define PIN_BUTTON_C 3

AccelStepper stepper(AccelStepper::FULL4WIRE, 7, 6, 5, 4);
 

void update_stepper(){
   int potentiometer = analogRead(PIN_DIR_POTENTIOMETER) - POTENTIOMETER_MIDDLE;
   bool reverse;
   if(potentiometer < 0){
       potentiometer *= -1;
       reverse = true;
   }else{
       reverse = false;
   }
   if(potentiometer < DEADZONE){
       potentiometer = 0;
   }

   double speed = map(potentiometer,
                POTENTIOMETER_MIN, POTENTIOMETER_MIDDLE,
                SPEED_MIN, SPEED_MAX); 
   if(reverse){
      speed = -1*speed;
   }
   stepper.setSpeed(speed);
}

int button_c_state = HIGH;
bool enabled = false;

void handle_buttons(){
    if(button_c_state == HIGH && digitalRead(PIN_BUTTON_C) == LOW){
        enabled = !enabled;
        button_c_state = LOW;
        if(enabled){
         stepper.enableOutputs();
        }else{
         stepper.disableOutputs();
        }
    }else if(button_c_state == LOW && digitalRead(PIN_BUTTON_C) == HIGH){
        button_c_state = HIGH;
    }
}

void setup()
{  
   pinMode(PIN_DIR_POTENTIOMETER, INPUT);
   stepper.setMaxSpeed(SPEED_MAX);
   stepper.setSpeed(0);
   stepper.disableOutputs();        
}
 
void loop()
{  
   update_stepper();
   stepper.runSpeed();
}