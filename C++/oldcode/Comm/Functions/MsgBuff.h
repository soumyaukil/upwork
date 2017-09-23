// MsgBuff.h: interface for the MsgBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSGBUFF_H__E22C8221_57F5_11D4_BCEE_00105A82D2D1__INCLUDED_)
#define AFX_MSGBUFF_H__E22C8221_57F5_11D4_BCEE_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "pkgbuff.h"

class MsgBuff  
{
public:
	dabuff * GetPkg(WORD type,WORD flag, BOOL removed=TRUE);
	int PutDabuff(dabuff *pbf);
	CCriticalSection mCriticalLock;
	MsgBuff();
	virtual ~MsgBuff();
	long dealmsgs;
private:
	dabuff *phead;
//	dabuff *pend;
	long pkgnu;
};

#endif // !defined(AFX_MSGBUFF_H__E22C8221_57F5_11D4_BCEE_00105A82D2D1__INCLUDED_)
