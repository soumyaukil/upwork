// StdTime.cpp: implementation of the StdTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FeedReceiver.h"
#include "StdTime.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

StdTime::StdTime()
{
	mtimeval=0;
}

StdTime::~StdTime()
{

}

StdTime::StdTime(unsigned long ntime)
{
	mtimeval=ntime;
}

unsigned long StdTime::MakeTime(int year, int month, int day, int hour, int min, int sec)
{

	return mtimeval;
}
