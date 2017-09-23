// GTime.h: interface for the GTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GTIME_H__F8908291_37F8_11D5_BD36_0050041B8FEC__INCLUDED_)
#define AFX_GTIME_H__F8908291_37F8_11D5_BD36_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// time for chart server;
class GTime  
{
public:
	DWORD mtime;
	DWORD mdate;
public:
	GTime();
public:
	GTime(DWORD ndate, DWORD ntime);
	inline DWORD GetDate(){return mdate;};
	inline DWORD GetTime(){return mtime;};
	inline __int64 GetDateTime() {return ((__int64)mdate<<32)+mtime;}
	inline DWORD GetYear() { return   mdate>>9;};
	inline DWORD GetMonth()  { return ( mdate>>5) & 0x000f;};
	
	inline DWORD GetDay()			{return (mdate &0x001f);	};
	inline DWORD GetHour()			{return mtime/(3600*1000);	};
	inline DWORD GetMinute()		{return (mtime/60000)%60;	};
	inline DWORD GetSecond()		{return (mtime/1000)%60;	};
	inline DWORD GetTodaySeconds()	{return mtime/1000;			};
	static GTime GetCurrentLocalTime();
	
public:
	BOOL MkTime(int Year,int Mon, int Day, int Hour, int Min, int Sec, int mSec);
	inline void SetYear (int year) {	mdate &= (~(0xffff<<9)); mdate |= ((year&0xffff)<<9);};
	inline void SetMonth(int month) {	mdate &= (~(0xf<<5)); mdate |= ((month&0xf)<<5);};
	inline void SetDay  (int day)	   {	mdate &= (~0x1f);     mdate |= (day&0x1f);	  };

	inline void SetHour(int hour){
//		int h = GetHour();
		int m = (mtime/60000)%60;
		int s = (mtime/1000)%60;
		int ms= mtime/1000;
		mtime = ((hour*60+m)*60+s)*1000+ms;
	};
	void SetMinute(int minute){
		int h = mtime/(3600*1000);
//		int m = GetMinute();
		int s = (mtime/1000)%60;
		int ms= mtime%1000;
		mtime = ((h*60+minute)*60+s)*1000+ms;
	};

	time_t GetTime_t();
	void SetToEmpty();
	inline BOOL IsEmpty() {
		return (GetDateTime()==0);
	}
	const GTime & operator =(__int64 ndatetime);
	const GTime & operator =(long ntime);
	const GTime & operator = (GTime gtm);
	inline BOOL operator>(GTime &gtm) {
		return GetDateTime()>gtm.GetDateTime();
	}
	inline BOOL operator<(GTime &gtm) {
		return GetDateTime()<gtm.GetDateTime();
	}
	inline BOOL operator>=(GTime &gtm) {
		return GetDateTime()>=gtm.GetDateTime();
	}
	inline BOOL operator<=(GTime &gtm) {
		return GetDateTime()<=gtm.GetDateTime();
	}
	inline BOOL operator==(GTime &gtm) {
		return GetDateTime()==gtm.GetDateTime();
	}
};


//time define for client;
struct CChartTime
{
public:
	void SetTime_t(time_t t1);
	BOOL GetMinNum();
	time_t GetTime_t(tm *ptm);
	void SetTime(DWORD ntime);
	DWORD mtime;

	void SetTime(WORD nyear, BYTE nmon, BYTE nday, BYTE nHour, BYTE nMinute);
	inline BOOL operator>(CChartTime &gtm) {
		return mtime>gtm.mtime;
	}
	inline BOOL operator<(CChartTime &gtm) {
		return mtime<gtm.mtime;
	}
	inline BOOL operator==(CChartTime &gtm) {
		return mtime==gtm.mtime;
	}
	inline void operator=(CChartTime &gtm) {
		mtime=gtm.mtime;
	}
	inline void operator=(DWORD ntm) {
		mtime=ntm;
	}
	inline int GetYear(){
		return (mtime>>20)+1900;
	}
	inline int GetMonth() {
		return (mtime>>16)&0x0f;
	}
	inline int GetDay() {
		return (mtime>>11)&0x1f;
	}
	inline int GetHour() {
		return (mtime>>6) & 0x1f;
	}
	inline int GetMinute() {
		return (mtime& 0x3f);
	}
};

#endif // !defined(AFX_GTIME_H__F8908291_37F8_11D5_BD36_0050041B8FEC__INCLUDED_)
