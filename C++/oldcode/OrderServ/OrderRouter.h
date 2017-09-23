// OrderRouter.h: interface for the OrderRouter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERROUTER_H__27CBA7C1_CD13_11D4_BD27_0050041B8FEC__INCLUDED_)
#define AFX_ORDERROUTER_H__27CBA7C1_CD13_11D4_BD27_0050041B8FEC__INCLUDED_

#include "FUNCTIONS\intmap.h"	// Added by ClassView
#include "MGMT\itemsDef.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MSOCKET\Msocket.h"
#include "Functions\PkgBuff.h"
#include "..\DEFINE\OrderDef.h"	// Added by ClassView

class OrderRouter : public Msocket  
{

	dabfchain mChainInBuf;
	dabfchain mChainOutBuf;
	dabuff *mInBuf;
	dabuff *mOutBuf;
	BOOL mbShutdowAfterSend;
	CFile plogfile;
	BOOL  bLogFlag;
	
	time_t mlastbeat;
	long mInCloseCount; // the inclose exclusive counter;
public:// server information
	int mtmconnect;		// the time to start connect; minutes of today
	int mtmdisconnect;	// the time to disconnect;	  minutes of today
	int mlsttmconnect;  // the time for last time to connect; time_t
	int mconnectfailnum; // the number of fail connection;
	int mautoconnect;   // automatic connect to server if disconnect; 1 yes, 0 no;
	int mCxlReplaceEnable; //enable cancel replace;
	
	char  mRouteName[10];
	DWORD mServerIP;
	WORD mServerPort;
	char mUserID[10];
	char mUserPWD[20];
	// order information
	BOOL mServerEnable;
	//this value return from the route which tells what order type supported by the route.

	long mServerOrderTypes;
	// control if this route enable to new order;
	BOOL mNewOrderEnable;

	BOOL mbTestRoute;
	
	//this map is for check if certern account is able to trade for this route:
	//key: account id, val: bit 0: enable, bit 1: disable.
	intmap mAuthLst;
	//if account is not in the list, shoule this account tradable?
	BOOL   mdeftradable;
	long   mServerVersion;

	DWORD mlastmessagetime;
	DWORD mlastheartbeatSendtime;

	//this is the bit flag which shows the exchange been disabled for this route;
	//start from bit 0:NAQUuOF
	int   mDisableExch;

	BOOL m_bHideRoute;				//don't send this route status to customer
	BOOL m_bDisableCustomerOrder;	//will disable the order for customer;

public:
	int PkgAccountPosition(char *pkgbuff);
	int PkgAccountBalance(char *pkgbuff);
	int SendPkg(int type, const char *pbuf,int len);
	void GetDisableExch(char *buff);
	BOOL IsExchDisable(char *symbol, char nexchange);
	void SetDisableExch(LPCSTR str);
	BOOL SendHeartBeat();
	int PkgDirectOrder(char *pkgbuff);
	int PkgTrailerPriceUpdate(char *pbuff);
	int PkgReplaceAction(char *pkgbuff);
	BOOL SendOrderReplace(itemOrder *porder, itemOrder *pnew,char *msg);
	int PkgOrderExecute_Cancel(char *pkgbuff);
	int Pkg_SMT_NewOrder(char *pkgbuff);
	int GetRouteStatusInt();
	int GetRouteStatus();
	BOOL UpdateStatus();
	BOOL ResetStatus();
	BOOL DoConfig();
	BOOL IsConnected();
	BOOL IsEnabled();
	inline BOOL IsAutoRoute() { return ((mServerOrderTypes&(1<<15))!=0);};
	int PkgHeartBeat(char *pkgbuff);
//	inline BOOL IsEnabled() { return (mServerEnable==TRUE);};
	int PkgOrderExecute(char *pkgbuff);
	int PkgOrderAction(char *pkgbuff);
	BOOL SendCancelOrder(itemOrder *porder, int lvsqty,char * msg);
	BOOL SendNewOrder(itemOrder * porder,char *porderpkg);
	BOOL PkgStatusIfo(char *pkgbuff);
	BOOL pkgStatusReq(int type);
	BOOL PkgTextInfo(char *pkgbuff);
	int PkgLoginReply(char *pkgbuff);
	BOOL pkgLogin();
	BOOL DisconnectServer();
	BOOL ConnectToServer(int mnewthread=1);
	int onConnecting(M_OVBUF *pOvl,SOCKADDR_IN *localaddr,SOCKADDR_IN *rmoteaddr);
	int ProcessPkg(char *pbuff);
	int PutPendingWrite();
	int PutPendingRead();
	int SendPkg(char *pbuf);
	int onClose();
	int onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	int onReading(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	OrderRouter();
	virtual ~OrderRouter();

};

#endif // !defined(AFX_ORDERROUTER_H__27CBA7C1_CD13_11D4_BD27_0050041B8FEC__INCLUDED_)
