// Client.h: interface for the CClient class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENT_H__8DAE9762_F9C7_11D3_BC66_00105A82D2D1__INCLUDED_)
#define AFX_CLIENT_H__8DAE9762_F9C7_11D3_BC66_00105A82D2D1__INCLUDED_

#include "Account.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ClientObject.h"
#include "MSOCKET\Msocket.h"
#include "Functions\PkgBuff.h"
#include "Account.h"

class Trader;
class CClient : public Msocket  
{
public:
	BYTE mHeartBeatInt;
//	char mLogonCompanyID[10];
//	char mLogonUserID[10];
	long mtrid;
	WORD m_Version;
	
	DWORD mClientAddress;//this is the IP they report to us;

	time_t  mlogintime;

	// if (logon) then can process package;
	//else only wait for login;
	BOOL mbLogon;
				
	//watch flag meains which item is dynamic refresh.
	//maybe manage or administrator need to see the order change
	//information or position information.
	
	//bit 0 Account.
	//bit 1 Trader
	//bit 2 Order
	//bit 3 trades;
	//bit 4 position
	int mWatchFlag;

	DWORD mClientSysID;
	
	//bit 0: open order only;
	int mLoginFlag;

public:
	int PkgManualOrders_Query(PkgToken *ptok);
	int PkgManualOrder(char *pbuff,int len);
	int PkgRouteBoxLst(char *pbuff);
	int PkgExchAgree(char *pbuff);
	int PkgOMCmd(char *currbuff);
	int PkgLOARequest(char *pbuff);
	int PkgAccCtrl(char *pbuff);
	int PkgOnlineTraders(char *pbuff);
	int PkgOrders_Replace(PkgToken *ptok);
	int PkgFuturesInfo(char *currbuff);
	int OnLogonSuccess();
	BOOL SendEventTextMsg(LPCSTR pstr,WORD nid=0,WORD nval=0);
	int PkgDealNew(char *currbuff,int ver);
	int PkgOptInfo(char *pbuff);
	int PkgUntradable(char *currbuff);
	int onFinish();
	BOOL SendOrderTknRply(BYTE stat,long origtkn,long  orderid);
	CString GetClientIPString();
	int GetWriteBuffSize();
	int PkgHeartBeat(char *currbuff);
	int PkgSecIfoChange(char *pbuff);
	BOOL IsWatching(int brid,int watchflag);
	int PkgTrader_Modify(PkgToken *ptok);
	int PkgTrader_Query(PkgToken *ptok);
	int PkgServersReq(char *currbuff);
	int PkgMsgQuery(char *pbuff);
	int SendMsg(itemMessage *pitem);
	int PkgNewMsg(char *currbuff);
	int PkgAddIfo_Query(PkgToken *ptok);
	int PkgPerIfo_Query(PkgToken *ptok);

	Trader *mptrifo;
	int SendPkg(char * pbuf);
	BOOL mbShutdowAfterSend;
	int Shutdown();
	int SendPkg(dabuff *pdapkg);
	CClient();
	virtual ~CClient();


private:
	dabfchain mChainInBuf;
	dabfchain mChainOutBuf;// only for count buff;

	dabuff *mInBuf;
	dabuff *mOutBuf;

	int onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	int onReading(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	int PutPendingRead();
	int PutPendingWrite();
	int onConnecting(M_OVBUF *pOvl,SOCKADDR_IN *localaddr,SOCKADDR_IN *rmoteaddr);
	int onClose();
	int ProcessPkg(char * pbuff);
private:
	
	
	int PkgOrders_Close(PkgToken *ptok);
	int PkgOrders_Cancel(PkgToken *ptok);
	int PkgOrders_Query(PkgToken *ptok);
	int PkgOrders(char *currbuff);

	int PkgTrades(char *currbuff);
	int PkgTrades_Modify(PkgToken *ptok);
	int PkgTrades_Query(PkgToken *ptok);

	int PkgPosition( char * currbuff);
	int PkgPosition_Query(PkgToken *tok);

	int PkgTickets(char *currbuff);
	int PkgTickets_Query(PkgToken *ptok);
	int PkgSecIfoQuery(char *pbuff);

	int PkgBrIfo(char * currbuff);

	int PkgTrader(char * currbuff);
	int PkgAccount(char *currbuff);
	int PkgAccount_Modify(PkgToken *ptok);
	int PkgAccount_Query(PkgToken *ptok);
	int PkgAcc_Access(char *currbuff);
	int PkgAcc_Access_Query(PkgToken *ptok);

	int PkgAddIfo(char *currbuff);
	int PkgPerIfo(char * currbuff);

	int PkgLogin(char *currbuff);
	int PkgLogout(char * currbuff);
	
protected:
};

#endif // !defined(AFX_CLIENT_H__8DAE9762_F9C7_11D3_BC66_00105A82D2D1__INCLUDED_)
