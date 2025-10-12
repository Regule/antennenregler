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

    // Setting up connections to controller
    pinMode(PIN_CONTROLLER_ENABLE, INPUT); 
}

void loop()
{
    potentiometer_control = analogRead(PIN_POTENTIOMETER_CONTROL);
    potentiometer_encoder = analogRead(PIN_POTENTIOMETER_ENCODER);

    int control_value = potentiometer_control - 512;
     if(control_value < 0){
       control_value *= -1;
     }
     int period = map(control_value,
                0, 512,
                2, 50);

    enabled = digitalRead(PIN_CONTROLLER_ENABLE) == HIGH;

    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("P1=");
    lcd.print(potentiometer_control);
    lcd.print(" T=");
    lcd.print(period);
    lcd.setCursor(0,1);
    lcd.print("STATE=");
    lcd.print(enabled?"ENA ":"DIS ");
    
    delay(200);
     
}
     
