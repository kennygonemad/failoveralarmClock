#include "functions.h"
#include <LiquidCrystal.h>
#include <Wire.h>
#include <EEPROM.h>
#define alarmAddress 0
#define speakerPin 0
#define LDRPin 1
#define LDRThreshold 10
#define alarmOffset 20
byte seconds, minutes, hours, dow, dom, month, year, currentDay, alarmSec, alarmMin, alarmHour;
LiquidCrystal panel(8, 9, 4, 5, 6, 7);

void setup()
{
	//setTime(0, 0, 23, 4, 14, 5, 15);
	panel.begin(16, 2);
	//printTime();
	//printDate();
	Serial.begin(9600);
	Wire.begin();
	checkTime();
	currentDay = 0;
	alarmHour = EEPROM.read(alarmAddress);
	alarmMin = EEPROM.read(alarmAddress+1);
	alarmSec = EEPROM.read(alarmAddress+2);
	//Serial.print(printableDate())
//	wireCheck = Wire.available();
}

void loop()
{
	printTime();
	//if (currentDay != dom)
	printDate();
	if (readButtons() == SELECT)
		if (changeTime())
			if (changeDate())
				changeAlarm();
	
	playAlarm(checkAlarm(LDRPin), speakerPin);

	delay(1000);
	currentDay = dom;
	//setTime(0, 0, 23, 5, 14, 5, 15);

}


void playAlarm(int alarmType, int bellPin)
{
	int period;
	if (alarmType == 1)
		period = 1000;
	else if (alarmType == 2)
		period = 100;
	else return;

	while (readButtons() != SELECT)
	{
		digitalWrite(bellPin, HIGH);
		delay(period);
		digitalWrite(bellPin, LOW);
		delay(period);
	}

	return;
}


int checkAlarm(int LDR)
{
	checkTime();
	int LDRValue = analogRead(LDR);
	if (alarmHour == hours)
	{
		if (alarmMin == minutes)
		{
			if (LDRValue < LDRThreshold)
				return 1;
		}
		else if ((alarmMin + alarmOffset) == minutes)
			if (LDRValue > LDRThreshold)
				return 2;
	}
}



int changeTime(){

	checkTime();
	int secs = seconds;
	int mins = minutes;
	int h = hours;
	int buffer = hours;
	int selected = 1;
	int pressed = 0;
	int lastButton;
	int button;
	int rt = 0;
	int cursorLock = 5;
	int limit = 24;
	panel.setCursor(4, 0);
	panel.print("Set Time");
	panel.setCursor(4, 1);
	panel.print(printableTime(seconds, minutes, hours));
	panel.setCursor(5, 1);
	panel.blink();
	lastButton = readButtons();
	Serial.print("preloop\n");
	delay(2000);
	while (rt == 0)
	{
		Serial.print("loop\n");
		lastButton = button;
		button = readButtons();
		switch (button)
		{
		case SELECT:
			rt = 2;
			break;
		case LEFT:
			
			if (selected == 3)
				selected = 1;
			else
				selected++;
			switch (selected)
			{
			case 1:
				buffer = h;
				cursorLock = 5;
				panel.setCursor(5, 1);
				limit = 24;
				break;
			case 2:
				buffer = mins;
				cursorLock = 8;
				panel.setCursor(8, 1);
				limit = 60;
				break;
			case 3:
				buffer = secs;
				cursorLock = 11;
				panel.setCursor(11, 1);
				limit = 60;
			default:
				break;
			}
			delay(500);
			break;
		case UP:
			if (++buffer >= limit)
				buffer = 0;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			//panel.setCursor(cursorLock, 1);
			break;
		case DOWN:
			if (--buffer < 0) 
				buffer =  limit - 1;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			break;
			//panel.setCursor(cursorLock, 1);
		case RIGHT:
			rt = 1;
			break;
		default:
			break;
		}

		switch (selected)
		{
		case 1:
			h = buffer;
			panel.setCursor(5, 1);
			break;
		case 2:
			mins = buffer;
			panel.setCursor(8, 1);
			break;
		case 3:
			secs = buffer;
			panel.setCursor(11, 1);
			break;
		default:
			break;
		}
		if (lastButton != button)
			pressed = 0;
		panel.setCursor(4, 1);
		panel.print(printableTime(secs, mins, h));
		panel.setCursor(cursorLock, 1);
	}
	Serial.print("returns\n");
	if (rt == 1)
	{
		setTime(secs, mins, h, dow, dom, month, year);
		panel.setCursor(4, 1);
		panel.print(printableDate(dom, month, year));
		panel.noBlink();
		return 0;
	}
	else if (rt == 2)
		return 1;
	else
		return -1;
}
int changeDate(){
	checkTime();
	int d = dom;
	int m = month;
	int y = year;
	int buffer = d;
	int selected = 1;
	int pressed = 0;
	int lastButton;
	int button;
	int rt = 0;
	int cursorLock = 5;
	int limit = 31;
	panel.setCursor(4, 0);
	panel.print("Set Date");
	panel.setCursor(4, 1);
	panel.print(printableDate(d, m, y));
	panel.setCursor(5, 1);
	panel.blink();
	lastButton = readButtons();
	Serial.print("preloop\n");
	delay(2000);
	while (rt == 0)
	{
		Serial.print("loop\n");
		lastButton = button;
		button = readButtons();
		switch (button)
		{
		case SELECT:
			rt = 2;
			break;
		case LEFT:

			if (selected == 3)
				selected = 1;
			else
				selected++;
			switch (selected)
			{
			case 1:
				buffer = d;
				cursorLock = 5;
				panel.setCursor(5, 1);
				limit = 31;
				break;
			case 2:
				buffer = m;
				cursorLock = 8;
				panel.setCursor(8, 1);
				limit = 12;
				break;
			case 3:
				buffer = y;
				cursorLock = 11;
				panel.setCursor(11, 1);
				limit = 99;
			default:
				break;
			}
			delay(500);
			break;
		case UP:
			if (++buffer >= limit)
				buffer = 0;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			//panel.setCursor(cursorLock, 1);
			break;
		case DOWN:
			if (--buffer < 0)
				buffer = limit - 1;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			break;
			//panel.setCursor(cursorLock, 1);
		case RIGHT:
			rt = 1;
			break;
		default:
			break;
		}

		switch (selected)
		{
		case 1:
			d = buffer;
			panel.setCursor(5, 1);
			break;
		case 2:
			m = buffer;
			panel.setCursor(8, 1);
			break;
		case 3:
			y = buffer;
			panel.setCursor(11, 1);
			break;
		default:
			break;
		}
		if (lastButton != button)
			pressed = 0;
		panel.setCursor(4, 1);
		panel.print(printableDate(d, m, y));
		panel.setCursor(cursorLock, 1);
	}
	Serial.print("returns\n");
	if (rt == 1)
	{
		setTime(seconds, minutes, hours, 0, d, m, y);
		panel.setCursor(4, 1);
		panel.print(printableDate(dom, month, year));
		panel.noBlink();
		return 0;
	}
	else if (rt == 2)
	{
		return 1;
	}
	else
		return -1;
}
int changeAlarm(){
	//	checkTime();
	int secs = alarmSec;
	int mins = alarmMin;
	int h = alarmHour;
	int buffer = h;
	int selected = 1;
	int pressed = 0;
	int lastButton;
	int button;
	int rt = 0;
	int cursorLock = 5;
	int limit = 24;
	panel.setCursor(4, 0);
	panel.print("Set Alarm");
	panel.setCursor(4, 1);
	panel.print(printableTime(alarmSec, alarmMin, alarmHour));
	panel.setCursor(5, 1);
	panel.blink();
	lastButton = readButtons();
	Serial.print("preloop\n");
	delay(2000);
	while (rt == 0)
	{
		Serial.print("loop\n");
		lastButton = button;
		button = readButtons();
		switch (button)
		{
		case SELECT:
			rt = 2;
			break;
		case LEFT:

			if (selected == 3)
				selected = 1;
			else
				selected++;
			switch (selected)
			{
			case 1:
				buffer = h;
				cursorLock = 5;
				panel.setCursor(5, 1);
				limit = 24;
				break;
			case 2:
				buffer = mins;
				cursorLock = 8;
				panel.setCursor(8, 1);
				limit = 60;
				break;
			case 3:
				buffer = secs;
				cursorLock = 11;
				panel.setCursor(11, 1);
				limit = 60;
			default:
				break;
			}
			delay(500);
			break;
		case UP:
			if (++buffer >= limit)
				buffer = 0;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			//panel.setCursor(cursorLock, 1);
			break;
		case DOWN:
			if (--buffer < 0)
				buffer = limit - 1;
			if (pressed++ > 5)
				delay(100);
			else
				delay(500);
			break;
			//panel.setCursor(cursorLock, 1);
		case RIGHT:
			rt = 1;
			break;
		default:
			break;
		}

		switch (selected)
		{
		case 1:
			h = buffer;
			panel.setCursor(5, 1);
			break;
		case 2:
			mins = buffer;
			panel.setCursor(8, 1);
			break;
		case 3:
			secs = buffer;
			panel.setCursor(11, 1);
			break;
		default:
			break;
		}
		if (lastButton != button)
			pressed = 0;
		panel.setCursor(4, 1);
		panel.print(printableTime(secs, mins, h));
		panel.setCursor(cursorLock, 1);
	}
	Serial.print("returns\n");
	if (rt == 1)
	{
		//setTime(secs, mins, h, dow, dom, month, year);
		alarmSec = secs;
		alarmMin = mins;
		alarmHour = h;
		EEPROM.write(alarmAddress, alarmHour);
		EEPROM.write(alarmAddress + 1, alarmMin);
		EEPROM.write(alarmAddress + 2, alarmSec);
		panel.setCursor(4, 1);
		panel.print(printableDate(dom, month, year));
		panel.setCursor(12, 0);
		panel.print(" ");
		panel.noBlink();
		return 0;
	}
	else if (rt == 2)
	{
		panel.setCursor(12, 0);
		panel.print(" ");
		panel.noBlink();
		return 1;
	}
	else
		return -1;
}
void printTime(){
	checkTime();
	panel.setCursor(4, 0);
	panel.print(printableTime(seconds, minutes, hours));
	//panel.setCursor(4, 1);
}
void printDate(){
	checkTime();
	panel.setCursor(4, 1);
	panel.print(printableDate(dom, month, year));

}
void inline checkTime()
{
	readTime(&seconds, &minutes, &hours, &dow, &dom, &month, &year);
}