#include "functions.h"
#include <LiquidCrystal.h>
#include <Wire.h>

byte seconds, minutes, hours, dow, dom, month, year, currentDay;
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
	currentDay = dom;
//	wireCheck = Wire.available();
}

void loop()
{
	printTime();
	//if (currentDay != dom)
		printDate();
	delay(1000);
	//currentDay = dom;
	//setTime(0, 0, 23, 5, 14, 5, 15);
}

int changeTime(int secs, int mins, int h){
	int buffer = h;

}
int changeDate(){

}
int changeAlarm(){

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
	panel.print(printableDate(dom, month, hours));

}
void inline checkTime()
{
	readTime(&seconds, &minutes, &hours, &dow, &dom, &month, &year);
}