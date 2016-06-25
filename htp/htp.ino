/** \file
 * HACK THE PLANET.
 *
 * Demo to scroll "Hack the planet" on the Hushcon pager.
 */

#define BUTTON1	4
#define BUTTON2 7
#define BUTTON3 A3

#define LCD_EN 8
#define LCD_DB0 A1
#define LCD_DB1 A2
#define LCD_DB2 2
#define LCD_DB3 3
#define LCD_RS 10
#define LCD_RW 9

#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_RW, LCD_EN, LCD_DB0, LCD_DB1, LCD_DB2, LCD_DB3);

const char message[] = "Hack the planet! Grand Central. Hack the";
int start = 0;

void setup()
{
	lcd.begin(8,1);
}



void loop()
{
	lcd.begin(8,1);
	lcd.print(message);
	for(int i = 0 ; i < sizeof(message) - 9 ; i++)
	{
		delay(250);
		lcd.scrollDisplayLeft();
	}
}
