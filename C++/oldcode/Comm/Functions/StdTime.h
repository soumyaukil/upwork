// StdTime.h: interface for the StdTime class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STDTIME_H__9C93AF81_93A9_11D4_BD0D_0050041B8FEC__INCLUDED_)
#define AFX_STDTIME_H__9C93AF81_93A9_11D4_BD0D_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class StdTime  
{
	unsigned long mtimeval;
	inline BOOL operator >(StdTime & other)  {return mtimeval> other.mtimeval;};
	inline BOOL operator ==(StdTime & other)  { return mtimeval> other.mtimeval;};
	inline operator =(StdTime & other) { mtimeval= other.mtimeval;};
	inline operator =(unsigned long ntime) { mtimeval=ntime; };
public:
	unsigned long MakeTime( int year, int month, int day, int hour, int min, int sec);
	StdTime( unsigned long ntime);
	StdTime();

	
	virtual ~StdTime();

};

#endif // !defined(AFX_STDTIME_H__9C93AF81_93A9_11D4_BD0D_0050041B8FEC__INCLUDED_)
