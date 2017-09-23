#ifndef __TIME_FUNCS__
#define __TIME_FUNCS__
#pragma once


class TimeFuncs
{
public:
	static int Make_time_t(int daynum, int timenum);
	static int GetTodayFirstSecond();
	static int GetTodayDayNum();
	static void DayInit();
	static int GetLastWorkingDayNum();// Get the last working day, skip sunday,saturday
	static void SetLastWorkingDayNum(int lastdaynum);// manually set the last working day to skip holiday.
	//format time to yyyy/mm/dd HH:MM:SS
	static char * FormatYMDHMS(time_t ntime, char *buff);
	
};


#endif
