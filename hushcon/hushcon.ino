/** \file
 * Scroll twitter messages for "#hushcon"
 */
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#include "ESP8266.h"

#define Button1 4

LiquidCrystal lcd(10,9,8,A1,A2,2,3);

#if 1
// The device needs jumpers soldered from the AVR hardware serial port
// to the serial port on the esp8266.
ESP8266 wifi(Serial, 115200);
#else
// As built there is a software serial port on the AVR, but it has lots
// of serial bit errors.
SoftwareSerial wifi_serial(6, 5);
ESP8266 wifi(wifi_serial, 115200);
#endif


// WIFI_ESSID and WIFI_PASS defined in config.h
#include "config.h"

#define HOST "216.184.2.32"

void setup()
{
	Serial.begin(115200);
	Serial.println("pager");

	lcd.begin(8,1);
	lcd.clear();
	lcd.print("#HCD2016");

#if 0
	// echo data from hardware serial to the software serial port
	// to test out the esp8266 config.
	while(1)
	{
		int c = Serial.read();
		if (c != -1)
		{
			wifi_serial.print((char) c);
			Serial.print((char) c);
		}

		c = wifi_serial.read();
		if (c != -1)
			Serial.print((char) c);
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
		int start = 0;

/*
		// find the start of the data
        	for(uint32_t i = 0; i < len - 4; i++)
		{
			if (buf[i+0] == '\r'
			&&  buf[i+1] == '\n'
			&&  buf[i+2] == '\r'
			&&  buf[i+3] == '\n'
			)
				start = i + 4;
		}
*/

		lcd.clear();
		lcd.print("rc:");
		int len = 3;
		for(uint32_t i = start ; i < len ; i++)
		{
			if(len % 8 == 0)
				lcd.clear();
			char c = buf[i];
			if (c == '\r' || c == '\n')
				continue;

			lcd.print(c);
			len++;
			delay(100);
		}
			
		delay(125);
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
