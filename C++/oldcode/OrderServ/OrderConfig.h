// OrderConfig.h: interface for the OrderConfig class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERCONFIG_H__8A880701_8354_11D4_BD01_0050041B8FEC__INCLUDED_)
#define AFX_ORDERCONFIG_H__8A880701_8354_11D4_BD01_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define MaxServerNumber 50

#include "FUNCTIONS\Config.h"
#include "LOG\Log.h"	// Added by ClassView
#include "OrderRouter.h"	// Added by ClassView
#include "FUNCTIONS\FixLenMem.h"	// Added by ClassView
#include "IssueIfo.h"
#include ".\Mgmt\ItemPool.h"
class OrderConfig : public CConfig  
{
public:
	//The MktOpen and MktClose decide the market hours.
	time_t mtmMktOpen;
	time_t mtmMktClose;

	time_t mtmFirstSecond;// today's first second;
	time_t mtmQuit;
	time_t mtmClose; //the time to close today's trade. time t
	long   mIsClosed; //0; not closed, 1: closing, 2: closed
	char   mlicense[100];
	char   mlicenseDeCode[100];
public:
	CString mdbConnectString;
	char mSystemName[50];
	WORD mServerPort1;
	WORD mServerPort2;
	WORD mServerPort3;
	char mCompanyID[12];  //give up
	char mCompanyIDEx[12];//subid of a giveup
	BOOL mCompanyIDCheck;
	char mQSVToken[20];
	int  mServerNumber ;
	BYTE mServerIDlst[MaxServerNumber];
	DWORD mServerIPlst[MaxServerNumber];
	WORD mServerPortlst[MaxServerNumber];
	CString mWelcomeMsg;
	BYTE mOrderServerID;
	int mBranchNumber;// possible branch number for harsh table
	int mAccountNumber;//possible Account number
	int mTraderNumber;//possible Trader Number
private:	
	DWORD mCurrOrderID;// the current orderid of this order server;
	DWORD mCurrTradeID;//the current Trade ID, the hi byte is server id;
	DWORD mCurrTicketID; //the new ticket id,
	
	BOOL  mbResetOrderID;//every day restart will reset the orderid. default is TRUE;
public:
	strmap mECNLST;
	strmap mETFLST;

	strmap mLogonAllow;
	strmap mLogonReject;
	intmap mLogonSysidBlock;

	intlst mScriptTask;

	strmap mScriptLogon; //only the user in this map able to run script;
	
//	intmap mDemoSysIDMap; //this map is used as control the demo

	char   mrouteboxlst[10][256];//this is for client to retrive, the trading window route dropdown box.
	int    mrouteboxlstidx;

public:// order handle	
	double mMKTOrderBPHoldRate;
	
	//mMKTHoldSWCHPrice: this is for the orders. how much we should full requirement.
	//usually this value will be little bigger than mFullRequireSwitch.
	//mFullRequireSwitch is NASD rule. but mMKTHoldSWCHprice if for broker 
	//who want more hold buying power for orders incase order got executed
	
	double mMKTHoldSWCHPrice;
	double mCommBPPreHold;

	//bit 0: shortable list 0, default shortable;
	//bit 1: shortable list 1: default shortable;
	int mbDefShortAble;//if TRUE, for security is shortable if not in shortlist;
						//if FALSE for security is not shortable if not in shortlist;
	BOOL mbEnableOption;
	double mShortPriceSwitch;
	double mShortPriceSwitch1;

	int mDayMargBPRate;
	int mNightMargBPRate;

	BOOL mbRecvBPForCash;//if TRUE, for cash position, don't recover the buyingpower

	//#Full require buying power for margin position.
	//When lower than this price, even margin position require full cash buyingpower
	double mFullRequireSwitch;

	// how many times Order bp can excess the position bp for the SPECIAL ACCOUNT
	// In Account infor, status bit 17, the position BP control;
	int mOrderBpCtrlRatio;

	WORD m_VersionBlock;
	WORD m_VersionUpGrade;
	CString m_msgBlock;
	CString m_msgUpGrade;
	
	char mMMRoute[10];
	BOOL mMMRouteEnable;
	BOOL mAutoConnectTest;

	int msqlInitTimeOut;
	int msqlTimeOut;
			// if this set to true, we will do the automatic database write everytime data come back.
			// otherwise only write to log file.
			// and when day close, we write it to database.
	BOOL mRealTimeDBWrite;

	BOOL mbEcnFeeInCap; //for the deal, if the ecn fee not in cap, max/per ticket not count ECN Fee
	BOOL mbTradeEcnFee; //Caculate the ecn fee for each Trade; set to false may speed trading.
	BOOL mbDVPKeepPosition;//if this is seted, the dvp account will not erase position;
	BOOL mbARCAUNoSpecialCharge;//for NBLE and NBLT, ARCAU charge will be the same under 1$
	int mTSXByBasisPoint; //if this is not 0,we charge TSX ECN fee by basis points.---for SUSA

	BOOL mbRecOrderActiveToLog; //Record the order active to log file. 
public: //route config
/*	
	OrderRouter mRouteISLD;
	OrderRouter mRouteSOES;
	OrderRouter mRouteDOT;
	OrderRouter mRouteARCA;
	OrderRouter mRouteHRZG;
	OrderRouter mRouteREDI;
	OrderRouter mRouteBRUT;
	OrderRouter mRouteNITE;
	OrderRouter mRouteTEST;
	OrderRouter mRouteGNET;
	OrderRouter mRouteBAMM;
*/
	strmap mRoutes;
public:
	DWORD GetNewTicketID();
	DWORD GetNewTradeID();
	DWORD GetNewOrderID();
	BOOL IsMktHours(time_t t1);
	void LoadRouteCtrl();
	void DoDayClose();
	BOOL AutoConnectRoute();
	int GetSecType(char *symbol, char nexchange);
	BOOL ConnectAllRoute();
	time_t GetExpTime();
	void SetMaxTradeID(DWORD tid);
	void SetMaxOrderID(DWORD moid);
	BOOL DisConnectAllRoute();
	OrderRouter * GetRoute(LPCSTR proute);
	BOOL IsTestSecurity(char *secsym);
	int GetLoginCtrl(LPCSTR id,DWORD ip);
	void GetLogonCtrlItem(LPCSTR id,LPCSTR subid,LPCSTR value);
	void LoadLogonCtrl();
	BOOL SetBalance(BYTE ServerID, DWORD ServerIp,WORD ServerPort);
	BOOL GetMarginAble(char *secsym,BYTE cIssueMkt);
	BOOL IsECN(char *pmmid);
	int GetShortAbleSize(char * secsym,BYTE cIssueMkt,int nlist);
	ItemPool * mpItemPool;
	IssueSet * mpIssueData;
	BOOL Exit();
	BOOL init();
	OrderRouter * FindRouter(itemOrder *porder);
	CStreamLog mlog;
	CStreamLog mLoginLog;
	CString	   mPathLog;	
	int OnGetCfg(LPCSTR id, LPCSTR subid, LPCSTR value);
	OrderConfig();
	virtual ~OrderConfig();	
};

#endif // !defined(AFX_ORDERCONFIG_H__8A880701_8354_11D4_BD01_0050041B8FEC__INCLUDED_)
