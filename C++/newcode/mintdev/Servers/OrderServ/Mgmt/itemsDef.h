// itemsDef.h: interface for the itemBranch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSDEF_H__885FFDA0_88A9_11D4_BD06_0050041B8FEC__INCLUDED_)
#define AFX_ITEMSDEF_H__885FFDA0_88A9_11D4_BD06_0050041B8FEC__INCLUDED_

#include "Functions/intmap.h"	// Added by ClassView
#include "Functions/FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Functions/PkgBuff.h"

class itemAddress  
{
public:
	int SetPkgInfo(PkgToken *ptok,WORD flag);
	int GetPkgInfo(PkgToken *ptok);
	void CopyItem(itemAddress *pitem,WORD flag);

public:
	BOOL UpdateData();
	void Clear();
	BOOL MakeInfoPkg(char *pbuff,WORD flag);
	int modifyflag;
	long maddid;
	long mbrid;
	char maddress[51];
	char mcity[21];
	char mstate[12];
	char mzip[8];
	char mcountry[21];
	char mphone[51];
	char mfax[51];
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
	itemAddress * pnext;
};

class itemPerson{
public:
	long mperid;
	long mbrid;
	BYTE mentity_type;
	char mtitle[21];
	char mentity_name[51];
	char mfname[21];
	char mmname[4];
	char mlname[26];
	char mSSN[31];
	char mphone[51];
	char memail[51];
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
public:
	BOOL UpdateData();
	int modifyflag;
	itemPerson * pnext;
	void Clear();

	void CopyItem(itemPerson *pitem,WORD flag=0xffff);
	BOOL MakeInfoPkg(char *pbuff,WORD flag);
	int SetPkgInfo(PkgToken *ptok,WORD flag);
	int GetPkgInfo(PkgToken *ptok);
};

class itemBranch  
{
public:
	long mbrid;
	char mshort[5];
	long mstatus;
	char mname[50];
	long mper_id;
	long madd_id;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
public:
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);

	BOOL UpdateData();
	WORD modifyflag;
	itemBranch * pnext;

	inline BOOL IsDeleted(){if ((mstatus&0x2)==2) return TRUE; else return FALSE;};
	inline BOOL IsOpen(){if ((mstatus&0x3)==1) return TRUE; else return FALSE;};

	void Clear();
	BOOL MakeInfoPkg(char * pbuff,WORD flag);
	int SetPkgInfo(PkgToken *ptok,WORD flag);
	int GetPkgInfo(PkgToken *ptok, itemPerson*pperson, itemAddress *padd);
	void CopyItem(itemBranch *pitem,WORD flag=0xffff);
};

// the trader item
class itemTrader 
{
public:
	long mtrid;
	char mtracno[11];
	char mPWD[21];
	long mbrid;
	long mstatus;
	long mstatus1; //extend the status to 128 bit
	long mstatus2;
	long mstatus3;
	long mtype;
	long mPerid;
	long mAdd1ID;
	long mAdd2ID;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
	long m_master_trader; //the master trader for this trader;
public:
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);

	BOOL UpdateData();
	BOOL MakeInfoPkg(char *pbuff,WORD flag);
	int SetPkgInfo(PkgToken *ptok,WORD flag);
	int GetPkgInfo(PkgToken *ptok,itemPerson *pper1,itemAddress *padd1,itemAddress *padd2);
	void CopyItem(itemTrader *pitem, long flag=0xffffffff);
	void Clear();
	int modifyflag;
	inline BOOL IsTrader(){if ((mstatus&0x30)==0) return TRUE; else return FALSE;};
	inline BOOL IsManager(){if ((mstatus&0x10)!=0) return TRUE; else return FALSE;};
	inline BOOL IsAdmin(){if ((mstatus&0x20)!=0) return TRUE; else return FALSE;};
	inline BOOL IsOpen() { if ((mstatus&3)==1) return TRUE; else return FALSE;};
	inline BOOL IsDeleted(){return ((mstatus & 2)==2);};


};

class itemAccIfo
{
public:
	long maccid;
	char mname[10];
	long mbrid; 
	long mstatus;
	char mrrno[8];
	long mlinkacc;
	long mper_id;
	long maddr1_id;
	long maddr2_id;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
//	long mstatus1;
//	long mstatus2;
//	long mstatus3;
public:
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	BOOL SetPkg(PkgToken *ptok, WORD flag);
	WORD GetPkg(PkgToken *ptok);
	BOOL UpdateData();
	void CopyItem(itemAccIfo *pitem , long flag=0xffffffff);
	void Clear();
	int modifyflag;

	inline BOOL IsDeleted(){return ((mstatus & 2)==2);};
	inline BOOL IsOpen(){
		if ((mstatus&0x3)==1) return TRUE; 
		else return FALSE;};
	inline BOOL IsMargin() {return  (mstatus & (1<<4))==(1<<4);};
	inline BOOL IsTraining() {return (mstatus & (1<<5))==(1<<5);};
	inline BOOL IsProprietary() {return  (mstatus & (1<<6))==(1<<6);};
	inline BOOL IsDVPAccount()  {return  (mstatus & (1<<19))==(1<<19);};

};
/*class itemAcct_Access
{
public:
	long mtrid;
	long maccid;
	long mstatus;
	long mdealid;
	long mm_date;
public:
	itemAcct_Access *pnext;
	
};
*/

#define iPosAcc			0x1
#define iPosSec			0x2
#define iPosType		0x4
#define iPosBrid		0x8
#define iPosInitQty		0x10
#define iPosInitPric	0x20
#define iPosQty			0x40
#define iPosAvgPric		0x80
#define iPosBP			0x100
#define iPosReal		0x200
#define iPosCdat		0x400
#define iPosMdat		0x800
#define iPosHoldQty		0x1000

class itemPosition {
public:
	long maccid;
	char msecsym[10];
	BYTE mptype;
	long mbrid;
	long mInitQty;
	double mInitPrice;
	long mqty;
	long mHoldqty;//the yesterday hold qty for today that didn't touched. didn't count on buying power;
	double mavgcost; //the Average Price for this position
	double mBuyPwer;//how many buying power this position take.
	double mRealize;//how much money already got with this position.

	time_t mc_date;
	time_t mm_date;
public:
	int GetEffectQty();
	double GetYClose();
	void SetToInit();
	static void InitBuff();
	static void PutVoidBuff(itemPosition *pitem);
	static itemPosition * GetVoidBuff();
	void Clear();
	BOOL SendChange(int change);
	BOOL ModifyDB(WORD flag);
	BOOL GetPkg(PkgToken *ptok, WORD *pflag);
	BOOL MakeInfoPkg(char *pbuff,WORD flag);
	void CopyFrom(itemPosition *pitem);
//	itemPosition * pprev; // double link for the position to be easy to 
//	itemPosition * pnext; // 
//	static itemPosition * phashtab[HASHSIZE];
//	static simplelock mlock;
private:
	static FixLenMem mitembuff;
};

class Account;
class itemOrder {
public:
	enum {
		deforderid	=0x1,
		deftrid		=(1<<1),
		defaccid	=(1<<2),
		defentrid	=(1<<3),
		deforigoid	=(1<<4),
		deforigtkn	=(1<<5),
		defbrid		=(1<<6),
		defroute	=(1<<7),
		defbkrsym	=(1<<8),
		defstatus	=(1<<9),
		defexecnum	=(1<<10),
		defconftxt	=(1<<11),
		deftoken	=(1<<12),
		defqty		=(1<<13),
		deflvsqty	=(1<<14),
		defcxlqty	=(1<<15),
		defsecsym	=(1<<16),
		defexchange	=(1<<17),
		defprice	=(1<<18),
		defstopprice=(1<<19),
		deftrailprice=(1<<20),
		defaskprice	=(1<<21),
		defbidprice	=(1<<22),
		defrrno		=(1<<23),
		deftmforce	=(1<<24),
		defc_date	=(1<<25),
		defm_date	=(1<<26),
		defmexts1	=(1<<27),
		defmextf1  =(1<<28),
		defmextf2  =(1<<29),
		defmextf3  =(1<<30),
		
	};
	long morderid;
	long mtrid;
	long maccid;
	long mentrid;
	long morigoid;
	long morigtkn;
	long mbrid;
	char mroute[10];
	char mbkrsym[5];
	long mstatus;
	BYTE mexecnum;
	char mconftxt[20];
	char mtoken[16];
	long mqty;			//orignal qty;
	long mlvsqty;		//live qty;
	long mcxlqty;		//cancel qty;
	char msecsym[10];
	BYTE mexchange;
	double mprice;
	double mstopprice;
	double mtrailprice;
	double maskprice;
	double mbidprice;
	char mrrno[8];
	WORD mtmforce;
	time_t mc_date;
	time_t mm_date;
	long ms1;			//the extend status;
	long mf1;			//for the reserve order, this is how much we show
	long mf2;			//this is price we show on the floor *10000
	long mf3;
public:
	int GetOpenBpQty();
	void Clear();
	static BOOL SaveAllToDB();
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff,int bufflen, DWORD flag);
	BOOL CheckNewOrder(DWORD flag,char *pmsg);


	inline BOOL IsBuyOrder()	{return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortOrder()	{return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsOpenOrder()	{return ((mstatus & (1<<0))==(1<<0));};
	inline BOOL IsMarketOrder()	{return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsCashOrder()	{return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsTestOrder()	{return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsPhoneTrade()  {return ((mstatus &( 1<<13))==(1<<13));};
	inline BOOL IsOption()      {return (msecsym[0]=='+'); };
	inline BOOL IsFuture()		{return (msecsym[0]=='#'); };

	inline BOOL IsStopOrder(){return ((mstatus &( 1<<10))==(1<<10));};
	inline BOOL IsBasicStopOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==(1<<10));};
	inline BOOL IsTrailerOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<11)));};
	inline BOOL IsRangeOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<26)));};
	inline BOOL IsGTCOrder() { return (mtmforce==0xFFFE); };


	double GetHoldBPRate(Account *pacc);
	static void InitBuff();
	BOOL SetOrderOpenQty(int Qty);
	//this function move the order to the right list;
	static itemOrder * FindActiveOrder(int orderid);
	static BOOL CancelOrder(int norderid,int nlvqty,char *msg);
	BOOL SetInfoPkg(char * pbuff,DWORD flag);
	BOOL SendChange(DWORD change);
	BOOL ModifyDB(DWORD flag);

	//new order will create an order and put in the open order list.
	//if errmsg !=NULL then it have to be biger enought(>500) to save the error messages;
	//flag: items mask
	//flagex: bit 0: new order from customer;

	BOOL NewOrder(DWORD flag,char *pclient,char* errmsg,int flagex=0);
	//SendOrder just send, don't do anthing else.
	BOOL SendOrder();

	BOOL SetPkg(PkgToken *ptok, DWORD flag);
	BOOL GetPkg(PkgToken *ptok, DWORD *pflag);
	void CopyFrom(itemOrder *pitem);
	static intmap sMapOrderID;
//	static intmap sOpenOrders;
	static FixLenMem sitembuff;
};

class itemDeal; 
class itemTrade{
public:
	enum {
		deftradeid	=1,
		deftrid		=1<<1,
		defaccid	=1<<2,
		defentrid	=1<<3,
		deforigacc	=1<<4,
		deforigtrid	=1<<5,
		deforigoid	=1<<6,
		deforderid	=1<<7,
		defbrid		=1<<8,
		defroute	=1<<9,
		defbkrsym	=1<<10,
		deftrtype	=1<<11,
		defstatus	=1<<12,
		defexecnum	=1<<13,
		defconftxt	=1<<14,
		deftoken	=1<<15,
		defqty		=1<<16,
		defprice	=1<<17,
		defsecsym	=1<<18,
		defexchange	=1<<19,
		defecnfee	=1<<20,
		deftradepl  =1<<21,
		defrrno		=1<<22,
		defcommision=1<<23,
		defe_date	=1<<24,
		defc_date	=1<<25,
		defm_date	=1<<26,
	};

	long mtradeid;
	long mtrid;
	long maccid;
	long mentrid;
	long morigacc;
	long morigtrid;
	long morigoid;
	long morderid;
	long mbrid;
	char mroute[10];
	char mbkrsym[5];
	//TradeType:  this may change according to the route, for INET:
	// 'A' Add liquidity, 
	// 'R' Remove Liquidity;
	// 'X' Route Out;
	// 'D' DOT order, 
	BYTE mtrtype;
	long mstatus;
	BYTE mexecnum;
	char mconftxt[20];
	char mtoken[16];
	long mqty;
	double	mprice;
	char msecsym[10];
	BYTE mexchange;
	double mEcnFee; //orignal is the mAskPrice;
	double mTradePL;
	char mrrno[8];
	double mcommision;
	time_t me_date;
	time_t mc_date;
	time_t mm_date;
public:
	double GetEcnFee(itemDeal *pdeal);
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	void PutToBuff();
	inline BOOL IsOpen(){return ((mstatus & 5)==1);};
	inline BOOL IsOrignalTrade(){return ((mstatus & (3))==(1));}; //open, but not sub trade.
	
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsAddLiquity(){return ((mstatus &(1<<20))==(1<<20));};
	inline BOOL IsRouteOut(){return ((mstatus &(1<<27))==(1<<27));};

	inline void SetOpenFlag(BOOL bOpen){
		if (bOpen) mstatus |= 1;
		else mstatus &= ~(1);
	};

	inline void SetShortTrade(BOOL bshort) {
		if (bshort) mstatus |= (1<<7);
		else mstatus &= ~(1<<7);
	};
	inline void SetCashTrade(BOOL bcash) {
		if (bcash) mstatus |= (1<<8);
		else mstatus &= ~(1<<8);
	};

	//copy some basic information form the order,for trade execute;
	BOOL CopyFromOrder(itemOrder *porder);
	void CopyFrom(itemTrade *ptrade, DWORD flag);
	BOOL SendChange(DWORD change);
	void CopyFrom(itemTrade *ptrade);
	BOOL SetInfoPkg(char *pbuff);
	BOOL SetPkg(PkgToken *ptok, DWORD flag);
	BOOL GetPkg(PkgToken *ptok, DWORD *pflag);
	BOOL ModifyDB(DWORD flag);
	//the trades map which maped by tradeid;
//	the Index by Accountid;
//	static intmap sMapAccID;
	void Clear();

	static void InitBuff();
	static BOOL SaveAllToDB();
	static itemTrade * GetVoidBuff();
	static itemTrade * FindTrade(long ntradeid);
	static itemTrade * FindTradeByExe(long accid, long orderid, BYTE exeseq);
	static intmap sMapTradeID;
private:
	static FixLenMem sitembuff;
};

class itemAccEquity{
public:
	enum {
		defaccid		=1,
		defOpenBP		=1<<1,
		defOpenNBP		=1<<2,
		defOpenEQ		=1<<3,
		defOpenMGEQ		=1<<4,
		defOpenEQRQ		=1<<5,
		defCurrBP		=1<<6,
		defCurrNBP		=1<<7,
		defCurrHoldBP	=1<<8,
		defCurrEQ		=1<<9,
		defCurrMGEQ		=1<<10,
		defCurrEQRQ		=1<<11,
		defComm			=1<<12,
		deftktnum		=1<<13,
		defRealized		=1<<14,
		defm_time		=1<<15,
	};
	long   maccid;
	double mOpenBP;		// open buying powering
	double mOpenNBP;	// open Night buying powing
	double mOpenEQ;		// open equity
	double mOpenMGEQ;	// open margin equity
	double mOpenEQRQ;	// open Equity require;
	double mCurrBP;		// current buying poweing 
	double mCurrNBP;	// current overnight buying power,
	double mCurrHoldBP;	// current Order Hold Buying power;
	double mCurrEQ;		// current EQutiy
	double mCurrMGEQ;	// current Margin EQuity;
	double mCurrEQRQ;	// current EQurity Require;
	double mComm;		// commision of today.
	long   mtktnum;	    // today's ticket number;
	long   mtotalshares;//today's traded shares;
	double mRealized;	// today's Realized P/L;
	time_t   mm_time;		// the last modify time;
public:
	void SetOpenMarginEQ(double margeq);
	void SetOpenTotalEQ(double totaleq);
	void SetOpenNightBP(double bp);
	void SetOpenDayBP(double bp);
	BOOL UpdateDB();
	WORD CompareItem(itemAccEquity *pitem);
	void CopyFrom(itemAccEquity *pitem);
	void SetToInit();
	BOOL SetPkg(PkgToken *ptok, WORD flag);
	WORD GetPkg(PkgToken *ptok);
	void Clear();
};


//AccCtrl is to control the account, such as max loss for a day.
// max share to trade etc. 
// this item is in the order server, will not dynamic updated to client. only reqest snapshot from client;

class itemAccCtrl{ 
public:
	long maccid;
	long mflag;
	long mflagex;
	double mMaxLoss;
	long maxshare;//how many share this trade can trade, 0 is no limit;
	long maxtotalshare; //how many total share open, (position expose)
	long maxsymbshare;	//how many total share per symbol open;(position expose); 

	long maxsymbs;			//max # of open positions in the same time;
	double maxUnrealizedLoss; //the max unrealized loss for this account.
	long  mstarttradetime;	//the second begin of today, enable them to trade;  0 no control.
	long  mstoptradetime;	//the second of today, enable them to trade;		0 no control.

public:
	BOOL MakePkg(char *buffer);
	void CopyFrom(itemAccCtrl *pitem);
};

class itemAccCtrl_Symb{
public:
	char msymbol[12];
	long maxpos;	//the max position can have for this symbol;
	double maxloss;	//the max for this position, if exceed that, only allow cover;
	long maxorder;  //the max shares of orders for this symbol;
	itemAccCtrl_Symb *pnext;
	static BOOL MakePkg_Sym(int accid, itemAccCtrl_Symb *psym, char *buffer);

};

class itemAccCtrl_Route{
public:
	char mroute[8]; //the route flag;
	long mflag;		//how to control this route;
	itemAccCtrl_Route *pnext;
	static BOOL MakePkg_Route(int accid, itemAccCtrl_Route *proute, char *buffer);

};

class itemPassthrufee {
public:
	long mflag;
	char mpath[10];
	double costpertkt;
	double costperexe;
	double costpershr;
	double costpershrA;
public:
	void FixFlag();
	void CopyFrom(itemPassthrufee *pitem);
	itemPassthrufee *pnext;
	static FixLenMem sitemPassTFbuff;

};

class itemDeal {
public:
	long mdealid;
	long  mbrid;			//new;
	long  mflag;			//new;
	char mname[10];
	long mdefaultdeal;
	long msharelmt;
	double mshareadj;
	double mlimitadj;
	double mmktadj;
	double mvaladj;		//the value adj for the ticket.
	long mbaseshare;
	double mbasecharge;
	long msharecap;
	long msharecap1;	//the share cap 1 which have second charge level;
	double mshareadj1;	//the share adj 1 which have second charge level.
	double mcommcap;
	double mmincomm;	//the miniumn commision;
	char mnote[256];
	time_t mm_date;
	double msymbolfee;
	itemPassthrufee *ppathfee; 
public:
	BOOL SetPkgInfoNew(PkgToken *ptok,int ver);
	BOOL SaveDealToDB();
	BOOL GetPkgInfoNew(PkgToken *ptok,int ver);
	void CopyDeal(itemDeal *pdeal);
	void FixFlag();
	BOOL GetPathFeeValue(int flag,LPCSTR ppath,itemPassthrufee *ppathfee);
	BOOL GetDealValue(int flag,itemDeal *pdeal);

	itemPassthrufee * FindPathFee(LPCSTR ppath);
	static itemDeal * FindDeal(long dealid);
	//this function get the information and modify it in database

	BOOL Clear();
	void CopyFrom(itemDeal *pdeal);
	BOOL Add_Modify_Map();

	static itemDeal * GetVoidDeal();
	static BOOL PutVoidDeal(itemDeal *pdeal);
	static void Init();
	static intmap sMapDealID;
	static FixLenMem sitemDealbuff;
};
			
class Account;
class itemTicket{
public:
	long mticketid;
	long mtrid;
	long maccid;
	long mentrid;
	long mtradeid;
	long morderid;
	long mbrid;
	char mroute[10];
	char mbkrsym[5];
	long mstatus;
	BYTE mexecnum;
	long mqty;
	double mprice;
	char msecsym[10];
	BYTE mexchange;
	char mrrno[8];
	double mcommision;
	time_t me_date;
	time_t mm_date;
	double mcomm1;
public:
	DWORD LoadStr(char *buff);
	BOOL  SaveStr(char *buff, int bufflen,DWORD flag);
	void CopyFrom(itemTicket *pticket);
	BOOL SetPkg(PkgToken *ptok, DWORD flag);
	BOOL SetInfoPkg(char *pbuff);
	double GetCommision(Account *pacc,intlst *ptktlst);
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsPhoneTrade() {return ((mstatus &( 1<<13))==(1<<13));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsOpen(){return (mqty!=0);};


	inline void SetShortTrade(BOOL bshort) {
		if (bshort) mstatus |= (1<<7);
		else mstatus &= ~(1<<7);
	};
	inline void SetCashTrade(BOOL bCash) {
		if (bCash) mstatus |= (1<<8);
		else mstatus &= ~(1<<8);
	};

	// check the fixed commision
	inline BOOL IsFixedComm() { return ((mstatus & (1<<21))!=0);};
	//Set or clear the fixed commision flag
	inline void SetFixedComm(BOOL nval) {
		if (nval) mstatus|=(1<<21); 
		else mstatus&= ~(1<<21);
	};
	
	inline BOOL IsCoverTicket() { return ((mstatus &( 1<<22))==(1<<22));};
	inline void SetCoverTicket(BOOL nval) {
		if (nval) mstatus|=(1<<22); 
		else mstatus&= ~(1<<22);
	}

	void PutToBuff();
	static itemTicket * GetVoidBuff();
	static void InitBuff();
private:
	static FixLenMem sitembuff;
};

class itemMessage
{
public:
	void Clear();
	int SetPkgInfo(PkgToken *ptok);
	int GetPkgInfo(PkgToken *ptok);
	char mFr[20];
	char mTo[20];
	long mid;
	char mTitle[50];
	long mflag;
	BYTE mPrior;
	char mtxt1[256];
	char mtxt2[256];
	char mtxt3[256];
	char mtxt4[256];
	time_t ms_date;
};

enum enManualOrderType{
	enMOOrder=0,
	enMOAccept,
	enMOReject,
	enMOCancel,
	enMOCanceled,
	enMOCancelrej,
	enMOExecution
};

class itemManualOrder
{
public:
	int id; //orderid;
	int userid;
	int accid;
	int ntype;// 0: order, 1: accept,2:reject,3:cancel request, 4: canceled, 2:execution
	int origoid;
	int status;
	char equitytype[12];
	char exchange[12];
	int  share;
	int	 left_share;
	char price[12];
	char symbol[12];
	char currency[8];
	char note[256];
	int  acceptby; //the user who accept this manual order;
	int  c_time; //create time;
	int  m_time; //last modify time;
	static intmap smapOrderid;
	static FixLenMem sOrderBuffer; 
public:
	BOOL SetInfoPkg(char *pbuff,DWORD flag);
	BOOL Save();
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	BOOL IsAccepted() { return ((status & (1<<4))!=0); }
	static itemManualOrder * Find(int orderid);
	BOOL GetPkg(PkgToken *ptok, DWORD *pflag);
	BOOL SetPkg(PkgToken *ptok, DWORD flag);
};


//order active type, used in mflag last 8 bit
#define LOASending		1
#define LOASendRej		3
#define LOAAccept		2

#define LOACanceling	4	
#define LOACanceled		5
#define LOACancelRej	6
#define LOAClose		7
#define LOATimeout		8

#define LOAExecuted		9
#define LOAReplaced		10
#define LOAReplacing	11	
#define LOAReplaceRej	12	

class itemOrderActive
{
public:
	long mtrid;
	long maccid;
	//low byte is the type;
	//0x100: is buy order;
	//0x200: is short order;
	//0x300: is stop order, then price is the trigger price;
	long mflag;
	long morderid;
	char msecsym[10];
	char mroute[10];
	double mprice;
	long mqty;
	time_t mlogtime;
	char mnote[20];
public:
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	BOOL SendActive(char *pclient);
	BOOL SaveToDB();
	BOOL SetType(BYTE btype);
	BOOL FillInfo(itemOrder *porder);
	BOOL SendActive(int brid);
	BOOL SetPkg(PkgToken *ptok);
	BOOL GetPkg(PkgToken *ptok);
	void Clear();
};

class itemOptsym
{
public:
	BOOL SaveStr(char *buff, int bufflen);
	DWORD LoadStr(char *buff);
	BOOL IsCall();
	static itemOptsym * FindInfo(LPCSTR psymb);
	void CopyFrom(itemOptsym *pitem);
	static itemOptsym * FindAddInfo(itemOptsym *psym,BOOL bAdd,BOOL bAddDB);
	static void InitBuff();
	char optsymb[10];
    time_t expday;
    long strikepice;
    char undsymb[10];
    long  pmundlyshr;
	long mcurrundprice;
    time_t m_date;
	static strmap sMapSymb;
	static FixLenMem sitembuff;
};

#endif // !defined(AFX_ITEMSDEF_H__885FFDA0_88A9_11D4_BD06_0050041B8FEC__INCLUDED_)
