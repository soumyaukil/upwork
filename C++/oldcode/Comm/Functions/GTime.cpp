// GTime.cpp: implementation of the GTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GTime::GTime()
{
	mdate=0;
	mtime=0;
}


const GTime & GTime::operator =(__int64 ndatetime)
{
	mdate=*(DWORD*)(((char*)&ndatetime)+4);
	mtime=*(DWORD*)(&ndatetime);
	return *this;
}

const GTime & GTime::operator = (GTime gtm)
{
	mdate=gtm.mdate;
	mtime=gtm.mtime;
	return *this;
}


const GTime & GTime::operator =(long ntime)
{
	time_t inTime = ntime;
	tm *ptm1;
	ptm1 = localtime(&inTime);
	if (ptm1==NULL){
		mdate=0;
		mtime=0;
	}else {
		mtime=(((ptm1->tm_hour*60+ptm1->tm_min)*60+ptm1->tm_sec))*1000;
		mdate=((ptm1->tm_year+1900)<<9) +((ptm1->tm_mon+1)<<5) + (ptm1->tm_mday);
	}
	return *this;
}


GTime::GTime(DWORD ndate, DWORD ntime)
{
	mdate=ndate;
	mtime=ntime;
}



GTime GTime::GetCurrentLocalTime()
{
	GTime t1;
	time_t tm1;
	time(&tm1);
	t1=tm1;
	return t1;
}

void GTime::SetToEmpty()
{
	mdate=0;
	mtime=0;
}

time_t GTime::GetTime_t()
{
	struct tm tm1;
	tm1.tm_year=(mdate>>9)-1900;
	tm1.tm_mon=(mdate>>5)&0xf -1;
	tm1.tm_mday=mdate&0x1f;
	tm1.tm_hour=mtime/3600;
	tm1.tm_min=mtime/60%60;
	tm1.tm_sec=mtime%60;
	tm1.tm_isdst=-1;
	return mktime(&tm1);
}


void CChartTime::SetTime(WORD nyear, BYTE nmon, BYTE nday, BYTE nHour, BYTE nMinute)
{
	if ((nyear<1900) ||(nyear>2100)) {
		mtime=0;
		return;
	}
	mtime=((((((((nyear-1900)<<4)+nmon)<<5)+nday)<<5)+nHour)<<6)+ nMinute;
}

void CChartTime::SetTime(DWORD ntime)
{
	mtime=ntime;
}

time_t CChartTime::GetTime_t(tm *ptm)
{
	tm tm1, *ptm1;
	if (ptm==NULL) ptm1=&tm1;else ptm1=ptm;
	ptm1->tm_year=GetYear()-1900; 
	ptm1->tm_mon=GetMonth()-1;
	ptm1->tm_mday=GetDay();
	ptm1->tm_hour =GetHour();
	ptm1->tm_min =GetMinute();
	ptm1->tm_sec=0;
	ptm1->tm_isdst=-1; 
	return(mktime(ptm1)); 
}

BOOL CChartTime::GetMinNum()
{
	return (((mtime>>6)&0x1f)*60+(mtime&0x3f));
}


void CChartTime::SetTime_t(time_t t1)
{
	tm tm1, *ptm1;
	ptm1=localtime(&t1);
	if (ptm1==NULL){
		mtime=0;
		return;
	}else {
		tm1=*ptm1;
	}
	CChartTime tt;
	tt.SetTime(tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday,
		tm1.tm_hour,tm1.tm_min);
	mtime=tt.mtime;
}


BOOL GTime::MkTime(int Year, int Mon, int Day, int Hour, int Min, int Sec, int mSec)
{

	mdate=((Year&0xffff)<<9)|((Mon&0xf)<<5) |(Day&0x1f);
	mtime = ((Hour*60+Min)*60+Sec)*1000+mSec;
	return TRUE;
}
