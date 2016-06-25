#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#define Button1 4

LiquidCrystal lcd(10,9,8,A1,A2,2,3);

void setup()
{
  lcd.clear();
  lcd.print("HCD2016");
  pinMode(Button1,INPUT);
}

void loop()
{
  if(digitalRead(Button1) == HIGH)
  {
    lcd.clear();
    lcd.print("Button 1");
  }
}
