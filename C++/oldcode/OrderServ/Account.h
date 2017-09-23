// Account.h: interface for the Account class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ACCOUNT_H__2F7F734F_4DC1_11D4_BCDD_00105A82D2D1__INCLUDED_)
#define AFX_ACCOUNT_H__2F7F734F_4DC1_11D4_BCDD_00105A82D2D1__INCLUDED_

#include "FUNCTIONS\intmap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Client.h"
#include "Functions\FixLenMem.h"
//#include "recordset\rstAc_Tr_ifo.h"
#include "MGMT\itemsDef.h"	// Added by ClassView


class AC_TR_relation
{
public:
	long mtrid;		//the trader id
	long maccid;	//the account id
	long mstatus;	//relationship type
	long mdealid;	// the deal with this relation;
	long mOpdealid;
	long mFtdealid;
	time_t mm_date;   // the date for this relation;
	AC_TR_relation *pnexttr;	//the next trader chain
	AC_TR_relation *pnextac;	//the next account chain
public:
	BOOL GetPkgNew(PkgToken *ptok);
	static BOOL Init();
	static BOOL DeleteItem(int trid,int accid);
	static AC_TR_relation * AddItem(AC_TR_relation *pitem);
	static AC_TR_relation * Finditem(long trid, long acid);
	void CopyItem(AC_TR_relation *pitem);

	BOOL DeleteDB();
	BOOL ModifyDB();
	BOOL MakeInfoPkgNew(char *buff,BYTE flag);
	
	static FixLenMem mrelationbuf;
};

class Account  
{
public:
	class stPosInfo 
	{
	public:
		char msecsym[12];
		char moptSym[12];
		long mexpday;
		long mstrikepice;
		BOOL mbCall;
		int mLong;     
		int mShort;
		int m_optLong;
		int m_optShort;
		int m_primshr;
		int moptCLong;
		int moptCWrite;
		int moptPLong;
		int moptPWrite;
		
	};

	// this struct is for caculate the bp and recalate bp;
	struct st_bpifo { 
		int longpos;
		int shrtpos;
//		int buyshare;
//		int sellshare;
		double buybp;
		double sellbp;
		double commionbp;
		inline double maxbp() { return ((buybp>sellbp)?buybp:sellbp);};
	};

private:
	long mLockNum;
public:
	itemAccIfo mitemifo;	
	itemAccEquity mitemEquity;
	itemAccCtrl  mitemCtrl;
	itemAccCtrl_Symb  *mpitemCtrl_Symb;
	itemAccCtrl_Route *mpitemCtrl_Route;
	
	int mdirtyflag;		// mdified but not save to database
	AC_TR_relation *pFistAcc; //the first item in the account-trader relation ship
	int mServerID;		//which server this account is in.
	

public://position
	intlst mLstPositions;
	itemPosition * FindPosition(LPCSTR secsym,char type,intlst *pposlst=NULL);
	itemPosition * ModifyPosition(itemPosition *pitem, int flag);
	BOOL FindPosInfo(LPCSTR secsym, Account::stPosInfo *pifo);

public://trade;
	intlst mLstTrades;
	BOOL AddTrade(itemTrade *ptrade);
	static itemTrade * AddNewTrade(itemTrade *ptrade,BOOL bCalcuateEquity);

public://tickets;
	//ticklist is sort by time;
	intlst mLstTickets;

public:// orders;
	intlst mLstOrders;
	BOOL AddOrder(itemOrder *porder);
	static itemOrder * AddNewOrder(itemOrder *porder);
	BOOL FindOrderHoldIfo(LPCSTR secsym,Account::stPosInfo *pifo);
	int FindOrderHoldQty(LPCSTR secsym,BOOL bBuy);

public://buying power and position,

private:
	void TktPosUpdate(intlst *pposlst,BOOL bSendDiff);
	void TktAddPos(int ntype, itemTicket *pticket, intlst *ptktlst, intlst *pposlst, itemAccEquity *pequity);
	itemTicket * TktCoverPos(int ntype,itemTicket *pticket, intlst *ptktlst, intlst *pposlst, itemAccEquity *pequity);
	BOOL TktTicketAddEffect(itemTicket *pticket,intlst *pTktlst, intlst *pPoslst,itemAccEquity *pEquity);
	BOOL TktCreateTicketFromTrade(itemTicket *pticket, itemTrade *ptrade);
	BOOL TktCombineTicket(itemTicket *pticket,itemTrade *ptrade);
	BOOL TktAddTrade(intlst *ticklst,itemTrade *ptrade);
	void TktPosRefresh(time_t lastmidify);
	void TktTicketUpdate(intlst *pticket);
public:
	void TktEquityUpdate(itemAccEquity *pequity,BOOL bSendDiff);
	BOOL TktRecaluate(BOOL bSendPos, BOOL bSendEquity);
	BOOL TktNeedRecalcuate(itemTrade *ptrade);
	BOOL TktPatchAddTrade(itemTrade *ptrade, BOOL bSendPos,BOOL bSendQeuity);
	int FindPositionNum(LPCSTR secsym);
public:
	BOOL IsUnwindOrder(itemOrder *porder);
	void CancelAllUnwindOrder(itemOrder *pord);
	int GetRoundTripTrades();
	static BOOL LoadBP_NBLT_GAON(LPCSTR filename);
	int AutoCoverPosition(LPCSTR symbol,int trid,double dprice,BYTE primexch);
	double ReCalOrderHoldBP_Ext();

	static BOOL LoadSODFile(LPCSTR filename);
	static BOOL LoadSODQuest(LPCSTR filename);
	static BOOL LoadPenson044(LPCSTR filename);
	static BOOL LoadNorthAmerica002(LPCSTR filename);
	static BOOL LoadCCSBPFile(LPCSTR filename);
	static BOOL RemoveAllOrderTrades();
	static BOOL RecoverFromLog(char *plogfile);
	static BOOL CancelAllOpenOrders(LPCSTR proute);
	static BOOL CountAllTrades(int &numtotal,int &numreal,int &numadd,int &numcanced);
	static BOOL CountAllOrders(int &numtotal,int &numreal, int &numopen,int &numexed,int &numexecs);
	static BOOL CountAllTickets(int &numtotal,int &numreal,double &comm);

	BOOL CheckOrderBP(itemOrder *porder,char *errmsg);
	BOOL CheckOrderBP_Ext(itemOrder *porder,char *pmsg);
	void Lock();
	void UnLock();
	static BOOL LoadEquityInfo();
	static BOOL SaveAllEquityInfo(BOOL bNewTable=TRUE);
	static BOOL SaveAllTickets();
	static BOOL SaveAllPosition();
	static BOOL SaveAllData();
	static BOOL SaveAllPositionToFile(LPCSTR fname);

	BOOL SavePositionLst(intlst *pposlst,BOOL binit=FALSE);
	double ReCalOrderHoldBP();
	BOOL MakePkgByEquity(char *pkgbuff, WORD flag);
//	BOOL ReCalculatePosition();
	BOOL MakePkgByTime(char *pkgbuff, time_t ntime);
	static intmap sMapAccid;
//	int CopyItem(itemAccIfo *pitem, int flag, BOOL bGet=TRUE);
	static Account * FindAcc(int naccid);
	static Account * FindAccByName(LPCSTR accname);
	static int init(int accountnumber);

	//	static CMapPtrToPtr mapid2account; // the map from id to account
	Account();
	virtual ~Account();
};



class CClient;
#define ADM_MAX_BRANCH 10
class Trader  
{
public:// the items in the data base
	itemTrader minfo;
	int mBranchList[ADM_MAX_BRANCH]; // the extra branch this user need to manager
	int mBranchPerm[ADM_MAX_BRANCH]; // the permmission for this user manage this branch;
public:
	CClient *mpClient;
	AC_TR_relation *pFistTrader; //the first item in the account-trader relation ship
	BYTE mServerId;			//which server this trader is in. this is for multipule server.
public:
	BOOL UpdateExBranchLst();
	static BOOL SPKGTraderAll(char *pkg);
	int LogItOut(LPCSTR pmsg);
	static BOOL DispatchMessage(itemMessage *pitem,itemTrader *ptrader);
	static BOOL FindTraderAccount(int ntrid, long *pacc, int * num);
	static BOOL SPKGTrader(char *pkg,long trid);
	static BOOL SPKGTrader(char *pkg, long trid,long brid,long watchflag);
	//Send a package to the trader which can watch this account and administrator and manager.
	//the watch flag is that flag
	static BOOL SPKGAcc(char *pkg,long accid, long brid, int watchflag);
	inline BOOL IsTrader(){return minfo.IsTrader();};
	inline BOOL IsManager(){return minfo.IsManager();};
	inline BOOL IsAdmin()	{return minfo.IsAdmin();}

	// the map from id to account
	static intmap sMapTrid; 
	// this is the trader online.
	static intmap mOnlineTrader;
	// this is the admin and managers online.
	static intmap mOnlineAdmin;

	inline BOOL IsOpen() { return minfo.IsOpen();};
	inline BOOL IsCheckShrtLst() { if (minfo.mstatus&(1<<16)) return TRUE; else return FALSE;};
	inline BOOL IsCheckBP(){if ((minfo.mstatus&(1<<18))!=0) return TRUE; else return FALSE;};
	static CClient * FindRefClient(int trid);
	static Trader * FindTrader(LPCSTR ptracno);
	static Trader * FindTrader(int ntrid);
	static BOOL Init(int tradernumber);
	Trader();
	virtual ~Trader();
};

#endif // !defined(AFX_ACCOUNT_H__2F7F734F_4DC1_11D4_BCDD_00105A82D2D1__INCLUDED_)
