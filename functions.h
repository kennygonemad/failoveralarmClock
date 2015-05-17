#include <Arduino.h>
#include <Wire.h>

#define ClockI2CAddress 0x68

#define RIGHT 1
#define UP 2
#define DOWN 3
#define LEFT 4
#define SELECT 5
#define NOBUTTON 6



byte decToBcd(byte val)
{
	return((val / 10 * 16) + (val % 10));
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
	return((val / 16 * 10) + (val % 16));
}
void setTime(byte seconds, byte minutes, byte hours, byte dow, byte dom, byte month, byte year){

	Wire.beginTransmission(ClockI2CAddress);
	Wire.write(0);
	Wire.write(decToBcd(seconds));
	Wire.write(decToBcd(minutes));
	Wire.write(decToBcd(hours));
	Wire.write(decToBcd(dow));
	Wire.write(decToBcd(dom));
	Wire.write(decToBcd(month));
	Wire.write(decToBcd(year));
	Wire.endTransmission();

}
void readTime(byte *seconds, byte *minutes, byte *hours, byte *dow, byte *dom, byte *month, byte *year){

	Wire.beginTransmission(ClockI2CAddress);
	Wire.write(0); // set DS3231 register pointer to 00h
	Wire.endTransmission();
	Wire.requestFrom(ClockI2CAddress, 7);
	// request seven bytes of data from DS3231 starting from register 00h
	*seconds = bcdToDec(Wire.read() & 0x7f);
	*minutes = bcdToDec(Wire.read());
	*hours = bcdToDec(Wire.read() & 0x3f);
	*dow = bcdToDec(Wire.read());
	*dom = bcdToDec(Wire.read());
	*month = bcdToDec(Wire.read());
	*year = bcdToDec(Wire.read());

}
int readButtons(int reading = 0){
	int ret = 50;
	int pinRead = analogRead(reading);
	if (pinRead == 0)
		ret = 1;
	if (pinRead > 200 && pinRead < 210)
		ret = 2;
	if (pinRead > 400 && pinRead < 410)
		ret = 3;
	if (pinRead > 600 && pinRead < 650)
		ret = 4;
	if (pinRead > 800 && pinRead < 850)
		ret = 5;
	if (pinRead > 1000 && pinRead < 1030)
		ret = 6;

	return ret;

}


String printableTime(int secs, int mins, int h)
{
	
	String printable;
	if (h < 10)
		printable += "0" + (String)h;
	else
		printable += h;
	printable += ":";
	if (mins < 10)
		printable += "0" + (String)mins;
	else
		printable += mins;
	printable += ":";
	if (secs < 10)
		printable += "0" + (String)secs;
	else
		printable += secs;
	return printable;
}
String printableDate(int dom, int month, int year)
{
	String printable;
	if (dom < 10)
		printable += "0" + (String)dom;
	else
		printable += dom;
	printable += "/";
	if (month < 10)
		printable += "0" + (String)month;
	else
		printable += month;
	printable += "/";
	if (year < 10)
		printable += "0" + (String)year;
	else
		printable += year;
	return printable;
}
