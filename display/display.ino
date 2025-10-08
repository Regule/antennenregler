//==============================================================================
//                              INCLUDES 
//==============================================================================
#include "LiquidCrystal_I2C.h"
#include "Wire.h"

//==============================================================================
//                               CONFIG 
//==============================================================================

// LCD
#define LCD_ADDRESS 0x27
#define COLUMN_COUNT 20
#define ROW_COUNT 2

// Buttons and potentiometers
#define PIN_POTENTIOMETER_CONTROL A0
#define PIN_POTENTIOMETER_ENCODER A1
#define PIN_BUTTON_MINUS 4
#define PIN_BUTTON_X 5
#define PIN_BUTTON_PLUS 6
#define PIN_BUTTON_C 7

// Connection to controller board
#define PIN_CONTROLLER_ENABLE 8 



//==============================================================================
//                              GLOBALS 
//==============================================================================
LiquidCrystal_I2C lcd(LCD_ADDRESS, COLUMN_COUNT, ROW_COUNT);

int button_c_state = HIGH;
bool enabled = false;
int potentiometer_control = 0;
int potentiometer_encoder = 0;




//==============================================================================
//                             SETUP-LOOP 
//==============================================================================
void setup()
{
    lcd.init();
    lcd.backlight();

    // Setting up potentiometers
    pinMode(PIN_POTENTIOMETER_CONTROL , INPUT); 
    pinMode(PIN_POTENTIOMETER_ENCODER, INPUT); 

    // Setting up buttons
    pinMode(PIN_BUTTON_MINUS, INPUT); 
    pinMode(PIN_BUTTON_X, INPUT); 
    pinMode(PIN_BUTTON_PLUS , INPUT); 
    pinMode(PIN_BUTTON_C , INPUT); 

    // Setting up connections to controller
    pinMode(PIN_CONTROLLER_ENABLE, OUTPUT); 
    digitalWrite(PIN_CONTROLLER_ENABLE, LOW);
}

void loop()
{
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
    potentiometer_control = analogRead(PIN_POTENTIOMETER_CONTROL);
    potentiometer_encoder = analogRead(PIN_POTENTIOMETER_ENCODER);

    int control_value = potentiometer_control - 512;
     if(control_value < 0){
       control_value *= -1;
     }
     int period = map(control_value,
                0, 512,
                2, 50);
    

    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("P1=");
    lcd.print(potentiometer_control);
    lcd.print(" T=");
    lcd.print(period);
    lcd.setCursor(0,1);
    lcd.print("STATE=");
    lcd.print(enabled?"ENA ":"DIS ");
    lcd.print(" C=");
    if(digitalRead(PIN_BUTTON_C)==HIGH){lcd.print("1");}else{lcd.print("0");}    
    
    delay(200);

     
}
     
