// secwatchlst.h: interface for the secwatchlst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SECWATCHLST_H__4BAF98B1_388A_11D4_BCC0_00105A82D2D1__INCLUDED_)
#define AFX_SECWATCHLST_H__4BAF98B1_388A_11D4_BCC0_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Afxmt.h>

class secwatchlst  
{
public:
	int AddVal(LPCSTR str,int val, BOOL deleteifzero=TRUE);
	int SetVal(LPCSTR str,int val);
	void empty();
	BOOL ItemExist(LPCSTR str);
	int DelItem(LPCSTR str);
	int AddItem(LPCSTR str,LPVOID val);
	CCriticalSection mCritical;
	CMapStringToPtr seclist;
	secwatchlst();
	virtual ~secwatchlst();

};

#endif // !defined(AFX_SECWATCHLST_H__4BAF98B1_388A_11D4_BCC0_00105A82D2D1__INCLUDED_)
