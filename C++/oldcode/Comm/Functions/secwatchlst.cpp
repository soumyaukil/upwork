// secwatchlst.cpp: implementation of the secwatchlst class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "riskctrl.h"
#include "secwatchlst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

secwatchlst::secwatchlst()
{
	seclist.RemoveAll();
	seclist.InitHashTable(200);
}

secwatchlst::~secwatchlst()
{

}

int secwatchlst::AddItem(LPCSTR str, LPVOID val)
{
	mCritical.Lock();
	void * tmpval; 
	if(seclist.Lookup(str,(void*&)tmpval)==FALSE) { // does exist 
		seclist.SetAt(str,val);
	}
	mCritical.Unlock();
	return 0;
}

int secwatchlst::DelItem(LPCSTR str)
{
	mCritical.Lock();
	seclist.RemoveKey(str); 
	mCritical.Unlock();
	return 0;
}

BOOL secwatchlst::ItemExist(LPCSTR str)
{
	void * tmpval; 
	BOOL ret;
	ret=seclist.Lookup(str,(void*&)tmpval);
	return ret;
}

void secwatchlst::empty()
{
	seclist.RemoveAll();
}

int secwatchlst::SetVal(LPCSTR str,int val)
{
	mCritical.Lock();
	seclist.SetAt(str,(void*)val);
	mCritical.Unlock();
	return 0;
}

int secwatchlst::AddVal(LPCSTR str, int val, BOOL deleteifzero)
{
	mCritical.Lock();
	int tmpval; 
	
	if(seclist.Lookup(str,(void*&)tmpval)==FALSE) { // does exist 
		tmpval=0;	
	}
	tmpval+=val;
	if ((tmpval==0) &&(deleteifzero==TRUE)){
		seclist.RemoveKey(str); 
	} else {
		seclist.SetAt(str,(void*)tmpval);
	}

	mCritical.Unlock();
	return 0;

}
