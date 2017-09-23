// ClientPortDaemon.h: interface for the CClientPortDaemon class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTPORTDAEMON_H__E245BFB1_4C3F_11D4_BCDB_00105A82D2D1__INCLUDED_)
#define AFX_CLIENTPORTDAEMON_H__E245BFB1_4C3F_11D4_BCDB_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MSOCKET\Msocket.h"

class CClientPortDaemon : public Msocket  
{
	public:
	int onAccept(DWORD nerrno,M_OVBUF *pOvl);
	int Listen(u_short portnu);
	CClientPortDaemon();
	virtual ~CClientPortDaemon();

};

#endif // !defined(AFX_CLIENTPORTDAEMON_H__E245BFB1_4C3F_11D4_BCDB_00105A82D2D1__INCLUDED_)
