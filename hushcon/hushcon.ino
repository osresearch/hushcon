/** \file
 * Scroll twitter messages for "#hushcon"
 */
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "ESP8266.h"

#define Button1 4

LiquidCrystal lcd(10,9,8,A1,A2,2,3);
//SoftwareSerial wifi_serial(6, 5);
//ESP8266 wifi(wifi_serial, 115200);
ESP8266 wifi(Serial, 115200);

#define WIFI_ESSID "trmm"
#define WIFI_PASS "0decafbad0"
#define HOST "216.184.2.32"

void setup()
{
	Serial.begin(115200);
	Serial.println("pager");

	lcd.begin(8,1);
	lcd.clear();
	lcd.print("#HCD2016");

	//wifi_serial.begin(115200);
	//wifi_serial.println("AT+UART=9600,8,1,0,0");
	//wifi_serial.begin(9600);

	//pinMode(5, INPUT_PULLUP);

	//wifi_serial.begin(115200);
	//wifi_serial.println("AT");

	//pinMode(6, OUTPUT);

/*
	while(1)
	{
		wifi_serial.println("AT");
		delay(100);
		//digitalWrite(6, 1);
		//delay(100);
		//digitalWrite(6, 0);
		//delay(100);
	}
*/


#if 0
	while(1)
	{
		char buf[128];

		if (Serial.available())
		{
			int len = Serial.readBytes(buf, sizeof(buf));
			Serial.write(buf, len);
			wifi_serial.write(buf, len);
		}
		if (wifi_serial.available())
		{
			int len = wifi_serial.readBytes(buf, sizeof(buf));
			Serial.write(buf, len);
		}
/*
		int c = Serial.read();
		if (c != -1)
		{
			wifi_serial.print((char) c);
			Serial.print((char) c);
		}

		c = wifi_serial.read();
		if (c != -1)
			Serial.print((char) c);
*/
	}
#endif
	
	pinMode(Button1, INPUT_PULLUP);

	if (!wifi.kick())
	{
		lcd.clear();
		lcd.print("WIFI ERR");
		Serial.println("WIFI ERR");
		while(1)
			;
	}

	lcd.clear();
	lcd.print(wifi.getVersion().c_str());
	Serial.println(wifi.getVersion().c_str());
	delay(500);
}

bool wifi_connected = false;

void loop()
{
#if 1
	if (!wifi_connected)
	{
		Serial.println("joining wifi");
		boolean status = wifi.joinAP(WIFI_ESSID, WIFI_PASS);
		if (!status)
		{
			lcd.clear();
			lcd.print("CON ERR ");
			Serial.println("con err");
			delay(1000);
			return;
		}

		lcd.clear();
		lcd.print(wifi.getLocalIP());
		Serial.println(wifi.getLocalIP());
		delay(1000);
		wifi_connected = true;
	}

	// try to retrieve the pager file
	delay(1000);

	if (!wifi.createTCP(HOST, 80))
	{
		lcd.clear();
		lcd.print("TCP ERR ");
		Serial.println("TCP ERR ");
		return;
	}

	const char url[] = "GET /~hudson/pager.txt HTTP/1.1\r\nHost: pages.swcp.com\r\n\r\n";
	wifi.send((const uint8_t*) url, sizeof(url)-1);

	uint8_t buf[512];
	
	uint32_t len = wifi.recv(buf, sizeof(buf), 10000);
	if (len > 0) {
		lcd.clear();
        	for(uint32_t i = 0; i < len; i++)
		{
			if (i % 8 == 0)
				lcd.clear();
            		lcd.print((char)buf[i]);
            		Serial.print((char)buf[i]);
			delay(125);
		}
	} else {
		lcd.clear();
		lcd.print("HTTP ERR");
		Serial.println("HTTP ERR");
		Serial.print("IP: ");
		Serial.println(wifi.getLocalIP());
	}
    
	wifi.releaseTCP();
    	delay(5000);
#endif
}
