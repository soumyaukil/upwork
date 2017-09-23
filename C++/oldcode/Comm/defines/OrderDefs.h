#ifndef __STRUCT_DEFINES
#define __STRUCT_DEFINES
#pragma once
/***********************************
This file defines the structure used in DAS tading systmes.

************************************/

enum enMTSType
{
	enMTSNone   =0,
	enMTSMaster =1,
	enMTSSub    =2,
};

class st_Order {
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
	long mqty;
	long mlvsqty;
	long mcxlqty;
	char msecsym[10];
	BYTE mexchange;
	double mprice;
	double mstopprice;
	double mtrailprice;
	double maskprice;
	double mbidprice;
	char mrrno[8];
	WORD mtmforce;
	long mc_date;
	long mm_date;
	long ms1;
	long mf1;
	long mf2;
	long mf3;


public:
	inline BOOL IsBuyOrder() {return (mstatus & (1<<6))==(1<<6);};
	inline BOOL IsShortOrder(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsOpenOrder(){return ((mstatus & (1<<0))==(1<<0));};
	inline BOOL IsCashOrder(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsStopOrder(){return ((mstatus &( 1<<10))==(1<<10));};
	inline BOOL IsBasicStopOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==(1<<10));};
	inline BOOL IsTrailerOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<11)));};
	inline BOOL IsRangeOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<26)));};
	inline BOOL IsPegOrder() {return ((ms1&(1<<6))!=0);};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsPrincipal(){return ((mstatus &(1<<16))==0);};
	inline BOOL IsAgency(){return ((mstatus &(1<<16))==(1<<16));};
	inline BOOL IsGTCOrder() { return (mtmforce==0xFFFE); };
	inline BOOL IsNoRR(){return ((mstatus &(1<<28))==(1<<28));};
	
	inline void SetCashFlag(BOOL flag) {
		if (flag) mstatus|=(1<<8);
		else mstatus &=~(1<<8);
	};
	inline void SetTraningFlag(BOOL flag) {
		if (flag) mstatus|=(1<<24);
		else mstatus &=~(1<<24);
	};
	//side: 0 sell, 1:buy;2 short;
	inline void SetSide(int side){
		mstatus=((mstatus & (~((1<<6)|(1<<7)))) | ((side & 3)<<6));
	}

};


class st_Trade{
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
		defaskprice	=1<<20,
		defbidprice	=1<<21,
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
	BYTE mtrtype;//CURRENT IS NOT USED
	long mstatus;
	BYTE mexecnum;
	char mconftxt[20];
	char mtoken[16];
	long mqty;
	double	mprice;
	char msecsym[10];
	BYTE mexchange;
	double maskprice;//This is EcnFee, don't need askprice;
	double mbidprice;//This is used as P/L, don't need bid price;
	char mrrno[8];
	double mcommision;
	long me_date;
	long mc_date;
	long mm_date;

public:
	inline BOOL IsOpenTrade(){return ((mstatus & (5))==(1));};
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsPrincipal(){return ((mstatus &(1<<16))==0);};
	inline BOOL IsAgency(){return ((mstatus &(1<<16))==(1<<16));};
	inline BOOL IsAddLiquity(){return ((mstatus &(1<<20))==(1<<20));};
	inline BOOL IsRouteOut(){return ((mstatus &(1<<27))==(1<<27));};
	
};

class st_Ticket{
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
	double mcomm1;
	long me_date;
	long mm_date;
public:
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};
	// check the fixed commision
	inline BOOL IsFixedComm() { return ((mstatus & (1<<21))!=0);};
	// check if this ticket is a cover or not.
	inline BOOL IsCoverTicket() { return ((mstatus &( 1<<22))==(1<<22));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsPrincipal(){return ((mstatus &(1<<16))==0);};
	inline BOOL IsAgency(){return ((mstatus &(1<<16))==(1<<16));};
};

class st_Position {
public:
	enum {
		iPosAcc			=0x1,
		iPosSec			=0x2,
		iPosType		=0x4,
		iPosBrid		=0x8,
		iPosInitQty		=0x10,
		iPosInitPric	=0x20,
		iPosQty			=0x40,
		iPosAvgPric		=0x80,
		iPosBP			=0x100,
		iPosReal		=0x200,
		iPosCdat		=0x400,
		iPosMdat		=0x800,
	};
	long maccid;
	char msecsym[10];
	char mptype;
	long mbrid;
	long mInitQty;
	double mInitPrice;
	long mqty;
	double mavgcost;
	double mBuyPwer;//how many buying power this position take.
	double mRealize;//how much money already got with this position.
	long mc_date;
	long mm_date;
};


class st_Trader {
public:
	int mtrid;
	char mtracno[10];
	char mPWD[20];
	int mbrid;
	int mstatus;
	long mstatus1; //extend the status to 128 bit
	long mstatus2;
	long mstatus3;
	int mper_id;
	int maddr1_id;
	int maddr2_id;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
	int m_master_trader;//the Master Trder for this logon
public:
	inline BOOL IsOpen(){if ((mstatus&0x3)==1) return TRUE; else return FALSE;};
	inline BOOL IsDeleted(){if ((mstatus&0x2)==2) return TRUE; else return FALSE;};
	inline BOOL IsTrader(){if ((mstatus&0x30)==0) return TRUE; else return FALSE;};
	inline BOOL IsManager(){if ((mstatus&0x10)!=0) return TRUE; else return FALSE;};
	inline BOOL IsAdmin(){if ((mstatus&0x20)!=0) return TRUE; else return FALSE;};
	inline BOOL IsCheckBP(){if ((mstatus&(1<<18))!=0) return TRUE; else return FALSE;};
	inline BOOL IsCheckPosition(){if ((mstatus&(1<<17))!=0) return TRUE; else return FALSE;};
	inline BOOL IsCheckShtlst(){if ((mstatus&(1<<16))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetFutureFeed(){if ((mstatus&(1<<11))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetOptFeed(){if ((mstatus&(1<<12))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetNewsFeed(){if ((mstatus&(1<<13))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetNewsFeed1(){if ((mstatus&(1<<14))!=0) return TRUE; else return FALSE;};
	inline BOOL IsMgrViewOnly(){if ((mstatus&(1<<19))!=0) return TRUE; else return FALSE;};

	inline BOOL IsGetArcaBook(){if ((mstatus&(1<<25))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetIncaBook(){if ((mstatus&(1<<26))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetBrutBook(){if ((mstatus&(1<<27))!=0) return TRUE; else return FALSE;};
	inline BOOL IsGetIsldBook(){if ((mstatus&(1<<28))!=0) return TRUE; else return FALSE;};
	inline enMTSType GetMTSType()  {return (enMTSType)((mstatus>>2)&(0x3));}
};

class st_AccIfo {
public:
	int maccid;
	char mname[10];
	int mbrid;
	int mstatus;
	char mrrno[8];
	int mlinkacc; //the Master Account for this account
	int mper_id;
	int maddr1_id;
	int maddr2_id;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
	long mstatus1; 

public:
	inline BOOL IsOpen(){if ((mstatus&0x3)==1) return TRUE; else return FALSE;};
	inline BOOL IsDeleted(){if ((mstatus&0x2)==2) return TRUE; else return FALSE;};
	inline BOOL IsMargin() {return  (mstatus & (1<<4))==(1<<4);};
	inline BOOL IsTraining() {return (mstatus & (1<<5))==(1<<5);};
	inline BOOL IsProprietary() {return  (mstatus & (1<<6))==(1<<6);};
	inline BOOL IsDVPAccount() { return  (mstatus & (1<<19))==(1<<19);};
	inline BOOL IsAPIShort()   { return  (mstatus & (1<<22))==(1<<22);};
	inline void SetMargin(BOOL bmargin) { 
		if (bmargin) mstatus|=(1<<4);
		else mstatus &= ~(1<<4);
	};
	inline void SetTraining(BOOL btraining) { 
		if (btraining) mstatus|=(1<<5);
		else mstatus &= ~(1<<5);
	};
	inline void SetProprietary(BOOL bproprietary) { 
		if (bproprietary) mstatus|=(1<<6);
		else mstatus &= ~(1<<6);
	};
	inline enMTSType GetMTSType()  {return (enMTSType)((mstatus>>2)&(0x3));}
};

class st_AccEquity{
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
	double mComm;		// commision;
	long   mtktnum;	    // today's ticket number;
	double mRealized;	// today's Realized P/L;
	long   mm_time;		// the last modify time;
};

class st_Acct_Link {
public:
	long mtrid;
	long maccid;
	long mstatus;
	long mdealid;
	long mOpdealid;
	long mFtdealid;
	long mm_date;
public:
	inline BOOL IsTradeAble() {
		if ((mstatus&9)==9) return TRUE;
		else return FALSE;
	}
};


//order active flag define.
//the flag define;
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

class st_Order_Active
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
};

#endif