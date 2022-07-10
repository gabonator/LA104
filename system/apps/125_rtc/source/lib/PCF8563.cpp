/*
* PCF8563.cpp - Library to set & get time from RTC PCF8563
* Created by Manjunath CV. March 23, 2019, 03:02 AM
* Released into the public domain.
*/

#include <time.h>
#include <Arduino.h>
#include <Wire.h>
#include <RTC.h>

bool PCF8563::begin()
{
    Wire.begin(); // join i2c bus
    Wire.beginTransmission (PCF8563_ADDR);
    return (Wire.endTransmission() == 0 ?  true : false);
}


bool PCF8563::isRunning(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();

   flag = bitRead(data,5);

    return (!flag);
}

void PCF8563::startClock(void) //Not Updated
{
    uint8_t data;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();

    bitClear(data, 5);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x00);  // Seconds Register
    Wire.write(data);
    Wire.endTransmission();
}

void PCF8563::stopClock(void) //Not Updated
{
	uint8_t data;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x00);
	Wire.endTransmission();

	Wire.requestFrom(PCF8563_ADDR, 1);
	data = Wire.read();

	bitSet(data, 5);

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x00);  // Seconds Register
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
get & set Second
-----------------------------------------------------------*/
uint8_t PCF8563::getSeconds()
{
    uint8_t second;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    second = Wire.read();
    return (bcd2bin(second));

}

void PCF8563::setSeconds(uint8_t second)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Second Register
    Wire.write(bin2bcd(second));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMinutes
-----------------------------------------------------------*/
uint8_t PCF8563::getMinutes()
{
    uint8_t minute;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x03);  // Minute Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    minute = Wire.read();
    return (bcd2bin(minute));
}

void PCF8563::setMinutes(uint8_t minute)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x03);  // Minute Register
    Wire.write(bin2bcd(minute));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getHours
-----------------------------------------------------------*/
uint8_t PCF8563::getHours()
{
    uint8_t hour;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x04);  // Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    hour = Wire.read();
        return (bcd2bin(hour));
}

void  PCF8563::setHours(uint8_t hour)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x04);  // Hour Register
     Wire.write(bin2bcd(hour));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getWeek
-----------------------------------------------------------*/
uint8_t PCF8563::getWeek()
{
	uint8_t week;
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x06);  // Week Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 1);
	week = Wire.read();
	return week+1;
}

void PCF8563::setWeek(uint8_t week)
{
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x06);  // Week Register
	Wire.write(week-1);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getDay
-----------------------------------------------------------*/
uint8_t PCF8563::getDay()
{
    uint8_t day;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x05);  // Day Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 1);
    day = Wire.read();
    return (bcd2bin(day));
}

void PCF8563::setDay(uint8_t day)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x05);  // Day Register
    Wire.write(bin2bcd(day));
    Wire.endTransmission();
}

/*-----------------------------------------------------------
getMonth()
-----------------------------------------------------------*/
uint8_t PCF8563::getMonth()
{
	uint8_t month;
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  // Month Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 1);
	month = Wire.read();
	bitClear(month,7);		//Clear Century;
	return (bcd2bin(month));
}
/*-----------------------------------------------------------
setMonth() 
-----------------------------------------------------------*/

void PCF8563::setMonth(uint8_t month)
{
	uint8_t data, century_bit;
	if (month >= 1 && month <= 12)
	{
		
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x07);  // Month Register
		Wire.endTransmission();
		Wire.requestFrom(PCF8563_ADDR, 1);
		data =  Wire.read();
		
		//Read Century bit and return it safe
		century_bit = bitRead(data, 7);
		month = bin2bcd(month);
		bitWrite(month,7,century_bit);
		
		Wire.beginTransmission(PCF8563_ADDR);
		Wire.write(0x07);  // Month Register
		Wire.write(month);
		Wire.endTransmission();
	}
}

/*-----------------------------------------------------------
getYear (Completed)
	* Always return 4 Digit year 
	* Takes Care of Century.
-----------------------------------------------------------*/
uint16_t PCF8563::getYear()
{
	uint8_t century_bit,data;
	uint16_t century,year;

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  // Read Month register for Century
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR,1);
	data =  Wire.read();
	century_bit = bitRead(data, 7);
	if(century_bit == 0)
	{
		century = 1900;
	}
	else
	{
		century = 2000;
	}
	
	//Read Year Register and add Century
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x08);  // Year Register
	Wire.endTransmission();
	Wire.requestFrom(PCF8563_ADDR, 1);
	year = Wire.read();
	year = bcd2bin(year) + century;
	return (year);
}

/*-----------------------------------------------------------
setYear (Completed)
	* Takes Care of Century.
	* Accepts both 2 and 4 Digit Years.
-----------------------------------------------------------*/
void PCF8563::setYear(uint16_t year)
{
	uint8_t century,data;
	
	// If year is 2 digits.
	if(year < 100)
		year = year + 2000;

	century = year / 100;	// Find Century 
	year = year % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8563_ADDR, 1);
	data = Wire.read();
	
	// Set century bit to 1 for year > 2000;
	if(century == 20)
	{
		bitSet(data,7);
	}
	else
	{
		bitClear(data,7);
	}
	
	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  // Month Register
	Wire.write(data);
	Wire.endTransmission();

	// Write 2 Digit year to Year Register(0x08)
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x08);  // Year Register to write year
	Wire.write(bin2bcd(year));
	Wire.endTransmission();
}

/*-----------------------------------------------------------
setTime
-----------------------------------------------------------*/

void PCF8563::setTime(uint8_t hour, uint8_t minute, uint8_t second)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Year Register
    Wire.write(bin2bcd(second));	//0x02 Seconds
    Wire.write(bin2bcd(minute));	//0x03 Minutes
    Wire.write(bin2bcd(hour));		//0x04 Hours
    Wire.endTransmission();
}

/*-----------------------------------------------------------
setDate (Should be Optimised and add Century)
-----------------------------------------------------------*/
void PCF8563::setDate(uint8_t day, uint8_t month, uint16_t year)
{
	uint8_t century, data;
	
	// If year is 2 digits.
	if(year < 100)
		year = year + 2000;
	
	century = year  / 100;	// Find Century 
	year = year % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x05);
	Wire.write(bin2bcd(day));	//0x05 Day
	Wire.write(0);				//0x06 Weekday
	Wire.write(bin2bcd(month));	//0x07 Months
	Wire.write(bin2bcd(year));	//0x08 Years
	Wire.endTransmission();

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8563_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);

	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
setDateTime()
Taken from https://github.com/adafruit/RTClib/
-----------------------------------------------------------*/
void PCF8563::setDateTime(char* date, char* time)
{
	uint8_t day, month, hour, minute, second;
	uint16_t year;
	// sample input: date = "Dec 26 2009", time = "12:34:56"
	year = atoi(date + 9);
	setYear(year);
	// Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
	switch (date[0]) {
		case 'J': month = (date[1] == 'a') ? 1 : ((date[2] == 'n') ? 6 : 7); break;
		case 'F': month = 2; break;
		case 'A': month = date[2] == 'r' ? 4 : 8; break;
		case 'M': month = date[2] == 'r' ? 3 : 5; break;
		case 'S': month = 9; break;
		case 'O': month = 10; break;
		case 'N': month = 11; break;
		case 'D': month = 12; break;
	}
	setMonth(month);
	day = atoi(date + 4);
	setDay(day);
	hour = atoi(time);
	setHours(hour);
	minute = atoi(time + 3);
	setMinutes(minute);
	second = atoi(time + 6);
	setSeconds(second);
}
#if 0
/*-----------------------------------------------------------
setEpoch()
	* Weekday Might not work properly
-----------------------------------------------------------*/

void PCF8563::setEpoch(time_t epoch)
{
	uint8_t data, century;
	uint16_t year;
	struct tm epoch_tm, *ptr_epoch_tm;

	ptr_epoch_tm = gmtime(&epoch);
	epoch_tm = *ptr_epoch_tm;

	century = (epoch_tm.tm_year + 1870) / 100;	// Find Century 
	year = (epoch_tm.tm_year + 1870) % 100;		//Converting to 2 Digit

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x02);  // Seconds Register
	Wire.write(bin2bcd(epoch_tm.tm_sec));	//0x02
	Wire.write(bin2bcd(epoch_tm.tm_min));	//0x03
	Wire.write(bin2bcd(epoch_tm.tm_hour));	//0x04
	Wire.write(bin2bcd(epoch_tm.tm_mday));	//0x05
	Wire.write(bin2bcd(epoch_tm.tm_wday));	//0x06
	Wire.write(bin2bcd(epoch_tm.tm_mon+1));	//0x07
	Wire.write(bin2bcd(year));	//0x08
	Wire.endTransmission();

	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);		// Century and month Register
	Wire.endTransmission();

	Wire.requestFrom(PCF8563_ADDR, 1);
	data = Wire.read();

	// Set century bit to 1 for year > 2000;
	if(century == 20)
		bitSet(data,7);
	else
		bitClear(data,7);
	
	// Write Century bit to Month Register(0x07)
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x07);  //
	Wire.write(data);
	Wire.endTransmission();
}

/*-----------------------------------------------------------
getEpoch()
	* Weekday Might not work properly
-----------------------------------------------------------*/
time_t PCF8563::getEpoch()
{
	uint8_t century_bit;
	uint16_t century;
	time_t epoch;
	struct tm epoch_tm;
	
	Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x02);  // Seconds Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR,7);
	
	epoch_tm.tm_sec = bcd2bin(Wire.read());		//0x02 Seconds
	epoch_tm.tm_min = bcd2bin(Wire.read());		//0x03 Minutes
	epoch_tm.tm_hour = bcd2bin(Wire.read());	//0x04 Hours
	epoch_tm.tm_mday = bcd2bin(Wire.read());	//0x05 Day
	epoch_tm.tm_wday = bcd2bin(Wire.read());	//0x06 Weekday
	epoch_tm.tm_mon = Wire.read();				//0x07 Months
	epoch_tm.tm_year = bcd2bin(Wire.read());	//0x08 Years
	
	// Read Century Bit from Month Register
	century_bit = bitRead(epoch_tm.tm_mon, 7);
	bitClear(epoch_tm.tm_mon,7);
	epoch_tm.tm_mon = bcd2bin(epoch_tm.tm_mon)-1;
	
	if(century_bit == 0)
		century = 1900;
	else
		century = 2000;
	
	epoch_tm.tm_year = epoch_tm.tm_year + century - 1870;

    epoch = mktime(&epoch_tm);
    return (epoch);
}
#endif

/*-----------------------------------------------------------
Alarm Functions
-----------------------------------------------------------*/

/*-----------------------------------------------------------
void disableAlarm()
-----------------------------------------------------------*/

void PCF8563::disableAlarm()
{
    uint8_t minutes,hours,day,weekday;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Alarm Hour Register
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);

    minutes = Wire.read();
    hours = Wire.read();
    day = Wire.read();
    weekday = Wire.read();

    bitSet(minutes,7);
    bitSet(hours,7);
    bitSet(day,7);
    bitSet(weekday,7);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);  // Month Register
    Wire.write(minutes);
    Wire.write(hours);
    Wire.write(day);
    Wire.write(weekday);
    Wire.endTransmission();
}

void PCF8563::setAlarm(uint8_t hours, uint8_t minutes)
{
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x09);
	Wire.write(bin2bcd(minutes));
	Wire.write(bin2bcd(hours));
	Wire.write(0x80);
	Wire.write(0x80);
	Wire.endTransmission();
}

void PCF8563::setAlarm(uint8_t week,uint8_t day, uint8_t hours, uint8_t minutes)
{
	Wire.beginTransmission(PCF8563_ADDR);
	Wire.write(0x09);
	Wire.write(bin2bcd(minutes));
	Wire.write(bin2bcd(hours));
	Wire.write(bin2bcd(day));
	Wire.write(bin2bcd(week));
	Wire.endTransmission();
}


DateTime PCF8563::getAlarm()
{
	uint8_t hours, minutes, day, week;
	DateTime Alarm;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);
    Wire.endTransmission();
    Wire.requestFrom(PCF8563_ADDR, 4);
    
	minutes = Wire.read();
	bitClear(minutes,7);
	Alarm.minutes = bcd2bin(minutes);

	hours = Wire.read();
	bitClear(hours,7);
	Alarm.hours = bcd2bin(hours);
	
	day = Wire.read();
	bitClear(day,7);
	Alarm.day = bcd2bin(day);

	week = Wire.read();
	bitClear(week,7);
	Alarm.week = bcd2bin(week);

	return(Alarm);
}




/*-----------------------------------------------------------
void clearAlarm();
-----------------------------------------------------------*/

void PCF8563::clearAlarm(void)
{
    uint8_t data;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();
    bitClear(data,3);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.write(data);
    Wire.endTransmission();
}

/*-----------------------------------------------------------
bool isAlarmTriggered();
-----------------------------------------------------------*/

bool PCF8563::isAlarmTriggered(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x01);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data= Wire.read();
    flag = bitRead(data,3);

    return (flag);
}

bool PCF8563::isAlarmEnabled(void)
{
    uint8_t minutes,hours,day,weekday;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x09);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 4);
    minutes = Wire.read();
    hours = Wire.read();
    day = Wire.read();
    weekday = Wire.read();

    minutes = bitRead(minutes,7);
    hours = bitRead(hours,7);
    day = bitRead(day,7);
    weekday = bitRead(weekday,7);

    if(minutes && hours && day && weekday)
        return false;
    else
        return true;
}

/*-----------------------------------------------------------
Timer Functions

bool isTimerEnabled();
-----------------------------------------------------------*/

bool PCF8563::isTimerEnabled(void)
{
    uint8_t data;
    bool flag;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0E);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data= Wire.read();
    flag = bitRead(data,7);

    return (flag);
}

void PCF8563::enableTimer(void)
{
    uint8_t data;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0E);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();
    bitSet(data,7);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0E);
    Wire.write(data);
    Wire.endTransmission();
}

void PCF8563::disableTimer(void)
{
    uint8_t data;
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0E);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    data = Wire.read();
    bitClear(data,7);

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0E);
    Wire.write(data);
    Wire.endTransmission();
}

void PCF8563::setTimer(uint8_t t_seconds)
{
    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0F);
    Wire.write(t_seconds);
    Wire.endTransmission();
}

uint8_t PCF8563::getTimer(void)
{
    uint8_t t_seconds;

    Wire.beginTransmission(PCF8563_ADDR);
    Wire.write(0x0F);
    Wire.endTransmission();

    Wire.requestFrom(PCF8563_ADDR, 1);
    t_seconds = Wire.read();
    return (t_seconds);
}


/* Helpers */

uint8_t PCF8563::bcd2bin (uint8_t val)
{
    return val - 6 * (val >> 4);
}
uint8_t PCF8563::bin2bcd (uint8_t val)
{
    return val + 6 * (val / 10);
}
