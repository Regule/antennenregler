#include <AccelStepper.h>
 

#define PIN_DIR_POTENTIOMETER A0
#define SPEED_MAX 1000
#define SPEED_MIN 0
#define DEADZONE 40
#define POTENTIOMETER_MAX 1024
#define POTENTIOMETER_MIN 0
#define POTENTIOMETER_MIDDLE 512

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
      potentiometer = -1*potentiometer;
   }
   stepper.setSpeed(speed);
}

void setup()
{  
   pinMode(PIN_DIR_POTENTIOMETER, INPUT);
   stepper.setMaxSpeed(SPEED_MAX);
   stepper.setSpeed(0);        
}
 
void loop()
{  
   update_stepper();
   stepper.runSpeed();
}