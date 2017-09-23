// PtrLink.h: interface for the CPtrLink class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PTRLINK_H__631AA3F2_6C66_11D4_BCF9_00105A82D2D1__INCLUDED_)
#define AFX_PTRLINK_H__631AA3F2_6C66_11D4_BCF9_00105A82D2D1__INCLUDED_

#include "FixLenMem.h"	// Added by ClassView
#include "synchronal.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct lnkPtr{
	int    mflag;
	void * mptr; // the ptr to the client
	lnkPtr * mpNext;
public:
};

class CPtrLink  
{
public:
	lnkPtr *phead; 
//	simplelock mlock; 
	APICritical mlock; 
private:
	static FixLenMem mWatchlstBuff;
	
public:
	int GetCounter();
	static void GetMemUsage(int &ntotal, int &nvoid);
	BOOL IsExist(void *ptr,int mask=1);
	//if this is the first one return true;
	//else return false;
	BOOL RemovePtr(void *ptr,int mask=1);
	//if this is the first one, return true;
	//else return false;
	BOOL AddPtr(void *ptr,int mask=1);
	CPtrLink();
	virtual ~CPtrLink();
};

#endif // !defined(AFX_PTRLINK_H__631AA3F2_6C66_11D4_BCF9_00105A82D2D1__INCLUDED_)
