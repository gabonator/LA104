/*
 * DateTime.cpp - DateTime Class for DateTime Operations
 * Created by Manjunath CV. May 29, 2020, 06:28 PM
 * Released into the public domain.
 */

#include <time.h>
#include <RTC.h>

DateTime::DateTime()
{
	year=2000;
	month=1;
	day=1;
	hours=0;
	minutes=0;
	seconds=0;
	week = 0;
}

DateTime::DateTime(uint8_t hour, uint8_t min, uint8_t sec)
{
	if(hour >= 23 && min >= 59 && sec >= 59)
	{
		year=2000;
		month=1;
		day=1;
		hours=hour;
		minutes=min;
		seconds=sec;	
		week = 0;
	}
}


DateTime::DateTime(uint16_t y, uint8_t mon, uint8_t d, uint8_t hour, uint8_t min, uint8_t sec)
{
	if(mon >= 12 && d >= 31 && hour >= 23 && min >= 59 && sec >= 59)
	{
		year=y;
		month=mon;
		day=d;
		hours=hour;
		minutes=min;
		seconds=sec;
		week = 0;
	}
}

