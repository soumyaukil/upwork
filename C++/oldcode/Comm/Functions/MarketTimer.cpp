// MarketTimer.cpp: implementation of the MarketTimer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "client.h"
#include "MarketTimer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MarketTimer::MarketTimer()
{
	mclockdiff=0;
	mdiff=0;

	time_t time1;
	tm tm1;
	time(&time1);
	tm1=*localtime(&time1);
	tm1.tm_hour=0;
	tm1.tm_sec=0;
	tm1.tm_min=0;
	mfirstSecond=mktime(&tm1);

}

MarketTimer::~MarketTimer()
{

}

//set the time zone difference;
BOOL MarketTimer::SetDiff(time_t markettime, int year, int mon, int day, int hour, int min, int sec)
{
	tm tm1;
	time_t localtm;
	tm1.tm_year=year-1900;
	tm1.tm_mon= mon-1;
	tm1.tm_mday =day;
	tm1.tm_hour=hour;
	tm1.tm_min= min;
	tm1.tm_sec=sec;
	tm1.tm_isdst=-1;
	localtm=mktime(&tm1);
	mdiff=markettime-localtm;

	time_t localclock;
	time(&localclock);

	mclockdiff=markettime-localclock;
	mfirstSecond=markettime - (((hour*60)+min)*60+sec);
	return TRUE;

}

time_t MarketTimer::GetMktTime()
{
	time_t ret;
	time(&ret);
	return ret+mclockdiff;
}

time_t MarketTimer::MkMktTime(int year, int mon, int day, int hour, int min, int sec)
{
	tm tm1;
	time_t localtm;
	time_t mkttm;
	tm1.tm_year=year-1900;
	tm1.tm_mon= mon-1;
	tm1.tm_mday =day;
	tm1.tm_hour=hour;
	tm1.tm_min= min;
	tm1.tm_sec=sec;
	tm1.tm_isdst=-1;
	localtm=mktime(&tm1);

	mkttm=localtm+mdiff;
	return mkttm;
}


struct tm * MarketTimer::MarketTime(const time_t *timer)
{
	time_t localtm;
	localtm=*timer-mdiff;
	return (localtime(&localtm));
}

BOOL MarketTimer::AdjTime(time_t markettime)
{
	time_t localclock;
	time(&localclock);
	mclockdiff=markettime-localclock;
	return TRUE;
}

CString MarketTimer::FormatMDYHMS(time_t mkttime)
{
	TCHAR szBuffer[100];
	if (mkttime==0){
		szBuffer[0]='\0';
	}else {
		struct tm* ptmTemp = MarketTime(&mkttime);
		if (ptmTemp == NULL ||
			!_tcsftime(szBuffer, sizeof(szBuffer), "%m/%d/%y %H:%M:%S", ptmTemp))
			szBuffer[0] = '\0';
	}
	return szBuffer;

}

CString MarketTimer::FormatHMS(time_t mkttime)
{
	TCHAR szBuffer[100];

	struct tm* ptmTemp = MarketTime(&mkttime);
	if (ptmTemp == NULL ||
		!_tcsftime(szBuffer, sizeof(szBuffer), "%H:%M:%S", ptmTemp))
		szBuffer[0] = '\0';
	return szBuffer;

}

time_t MarketTimer::GetFirstSecond()
{
	return mfirstSecond;
}

CString MarketTimer::FormatTime(time_t mkttime, const char *format)
{
	TCHAR szBuffer[100];
	if (mkttime==0){
		szBuffer[0]='\0';
	}else {
		struct tm* ptmTemp = MarketTime(&mkttime);
		if (ptmTemp == NULL ||
			!_tcsftime(szBuffer, sizeof(szBuffer),format, ptmTemp))
			szBuffer[0] = '\0';
	}
	return szBuffer;
}

//DEL BOOL MarketTimer::GetMktTime(const time_t ntime, LPSYSTEMTIME psystime)
//DEL {
//DEL 	time_t localtm;
//DEL 	if (ntime<0) return FALSE;
//DEL 	localtm=ntime-mdiff;
//DEL 	tm tm1;
//DEL 	tm1=*localtime(&localtm);
//DEL 
//DEL 
//DEL 	FILETIME ft;
//DEL 	LONGLONG ll = Int32x32To64(localtm, 10000000) + 116444736000000000;
//DEL 	ft.dwLowDateTime = (DWORD) ll;
//DEL 	ft.dwHighDateTime =(unsigned long)(ll >>32);
//DEL 	
//DEL /*	memset(psystime,0,sizeof(SYSTEMTIME));
//DEL 	psystime->wDayOfWeek=tm1.tm_wday;
//DEL 	psystime->wMilliseconds=0;
//DEL 	psystime->wYear=tm1.tm_yday+1900;
//DEL 	psystime->wMonth=tm1.tm_mon+1;
//DEL 	psystime->wDay=tm1.tm_mday;
//DEL 	psystime->wHour=tm1.tm_hour;
//DEL 	psystime->wMinute=tm1.tm_min;
//DEL 	psystime->wSecond=tm1.tm_sec; 
//DEL 
//DEL */
//DEL 	FileTimeToSystemTime(&ft,psystime); 
//DEL 	return TRUE;
//DEL }

time_t MarketTimer::GetLocalTm(time_t nmkttm)
{
	return nmkttm-mdiff;
}
