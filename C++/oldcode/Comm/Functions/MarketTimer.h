// MarketTimer.h: interface for the MarketTimer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MARKETTIMER_H__A1760A11_3B15_11D5_BD37_0050041B8FEC__INCLUDED_)
#define AFX_MARKETTIMER_H__A1760A11_3B15_11D5_BD37_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MarketTimer  
{
public:
	time_t GetLocalTm(time_t nmkttm);
	CString FormatTime(time_t mkttime, const char* format);
	time_t GetFirstSecond();
	CString FormatHMS(time_t mkttime);
	CString FormatMDYHMS(time_t mkttime);
	BOOL AdjTime(time_t markettime);
	struct tm * MarketTime( const time_t *timer);
	time_t MkMktTime(int year,int mon, int day, int hour,int min, int sec);
	time_t GetMktTime();
	BOOL SetDiff(time_t ntime, int year, int mon, int day, int hour, int min,int sec);
	MarketTimer();
	virtual ~MarketTimer();
	int mdiff; //this is the time zorn different;
	int mclockdiff;// this id the clockdifferent;
	int mfirstSecond;
};
 
#endif // !defined(AFX_MARKETTIMER_H__A1760A11_3B15_11D5_BD37_0050041B8FEC__INCLUDED_)
