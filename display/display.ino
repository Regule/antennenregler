#include "LiquidCrystal_I2C.h"
#include "Wire.h"

#define LCD_ADDRESS 0x27
#define COLUMN_COUNT 20
#define ROW_COUNT 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, COLUMN_COUNT, ROW_COUNT);

int pot1 = 0;
int pot2 = 0;
int inPin4 = 4;
int inPin5 = 5;
int inPin6 = 6;
int inPin7 = 7;
int outPin8 = 8;

int inPin7State = HIGH;
bool enabled = false;

void setup()
{
    lcd.init();
    lcd.backlight();
    pinMode(pot1, INPUT); 
    pinMode(pot2, INPUT); 
    pinMode(inPin4, INPUT); 
    pinMode(inPin5, INPUT); 
    pinMode(inPin6, INPUT); 
    pinMode(inPin7, INPUT); 
    pinMode(outPin8, OUTPUT); 
    digitalWrite(outPin8, LOW);
}

void loop()
{
    if(inPin7State == HIGH && digitalRead(inPin7) == LOW){
        enabled = !enabled;
        inPin7State = LOW;
    }else if(inPin7State == LOW && digitalRead(inPin7) == HIGH){
        inPin7State = HIGH;
    }
    if(enabled){
        digitalWrite(outPin8,HIGH);
    }else{
        digitalWrite(outPin8,LOW);
    }
    pot1 = analogRead(A0);
    pot2 = analogRead(A1);

    int potentiometer = pot1 - 512;
     if(potentiometer < 0){
       potentiometer *= -1;
     }
     int period = map(potentiometer,
                0, 512,
                2, 50);
    

    lcd.clear();
    
    lcd.setCursor(0,0);
    lcd.print("P1=");
    lcd.print(pot1);
    lcd.print(" T=");
    lcd.print(period);
    lcd.setCursor(0,1);
    lcd.print("STATE=");
    lcd.print(enabled?"ENA ":"DIS ");
    lcd.print(" C=");
    if(digitalRead(inPin7)==HIGH){lcd.print("1");}else{lcd.print("0");}    
    
    delay(200);

     
}
     
