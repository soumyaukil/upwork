#include "stdafx.h"
#include "TimeFuncs.h"

static time_t Today_FirstSeconds;//the today's first seconds.
static int    Today_DayNum;  //today's day number, as 20050122;
static tm Today_Tm;
static int LastWorkingDayNum;// skip sunday, saturday.

void TimeFuncs::DayInit()
{
	struct tm when;
	time_t now;
	time( &now );
	when = *localtime( &now );

	Today_FirstSeconds=(now -((when.tm_hour*60+when.tm_min)*60+when.tm_sec));
	
	Today_Tm=when;
	Today_DayNum=(Today_Tm.tm_year+1900)*10000 +(Today_Tm.tm_mon+1)*100 +Today_Tm.tm_mday;
	
	//Find the last working day num;
	time_t lastday;
	switch(when.tm_wday) {
	case 0:
		lastday=Today_FirstSeconds-(3600*24)*2;
		break;
	case 1:
		lastday=Today_FirstSeconds-(3600*24)*3;
		break;
	default:
		lastday=Today_FirstSeconds-(3600*24);
	}
	struct tm tm2;
	tm2=*(localtime(&lastday));
	LastWorkingDayNum=(tm2.tm_year+1900)*10000 +(tm2.tm_mon+1)*100 +tm2.tm_mday;
}



int TimeFuncs::GetTodayFirstSecond()
{
	return Today_FirstSeconds;
}

int TimeFuncs::GetTodayDayNum()
{
	return Today_DayNum;
}


int TimeFuncs::GetLastWorkingDayNum()
{
	return LastWorkingDayNum;
}

static void SetLastWorkingDayNum(int lastdaynum)
{
	LastWorkingDayNum=lastdaynum; 
}

int TimeFuncs::Make_time_t(int daynum, int timenum)
{
	if ((daynum==Today_DayNum)||(daynum==0)) {
		return timenum+Today_FirstSeconds;
	}else if (daynum==(Today_DayNum-1)){//yesterday;
		return timenum+Today_FirstSeconds-24*3600;
	}else {
		struct tm tm1;
		memset(&tm1,0,sizeof(tm1));
		tm1.tm_year=daynum/10000-1900; 
		tm1.tm_mon=((daynum/100)%100)-1;
		tm1.tm_min=daynum%100;
		tm1.tm_isdst=-1; 
		int ret=mktime(&tm1);
		return ret+timenum;
	}

}

char * TimeFuncs::FormatYMDHMS(time_t ntime, char *buff)
{
	if (ntime<=0) {
		time(&ntime);
	}

	struct tm* ptmTemp = localtime(&ntime);
	if (ptmTemp == NULL ||
		!_tcsftime(buff, 20, "%m/%d/%y %H:%M:%S", ptmTemp))
		buff[0] = '\0';
	return buff;
}
