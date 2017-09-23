// ClientPortDaemon.cpp: implementation of the CClientPortDaemon class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderServ.h"
#include "ClientPortDaemon.h"
#include "Client.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClientPortDaemon::CClientPortDaemon()
{

}

CClientPortDaemon::~CClientPortDaemon()
{

}

int CClientPortDaemon::Listen(u_short portnu)
{
	int ret;
	ret=Msocket::ListenFrom(portnu);
	if (ret!=0) {
		AfxMessageBox( "Listen Client error ");
	}
	return ret;
}



int CClientPortDaemon::onAccept(DWORD nerrno,M_OVBUF *pOvl)
{
	if (nerrno==0){
		CClient * tmpclient;
		tmpclient=new(CClient);
	//	tmpclient->mSocket=socket(AF_INET,SOCK_STREAM,0);
		DoAccept( pOvl,tmpclient);
	}
	AcceptNew();
	return (0);
}
