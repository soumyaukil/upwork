#pragma once

#include "..\Functions\Pkgbuff.h"

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
	long mc_date;
	long mm_date;
	long ms1;			//the extend status;
	long mf1;
	long mf2;
	long mf3;
public:
	BOOL SetPkg(PkgToken *ptok, DWORD flag);
	int  GetPkg(PkgToken *ptok, DWORD *pflag);
	void CopyFrom(itemOrder *pitem, DWORD flag);
	void CopyFrom(itemOrder *pitem);


	const char * GetStatusStr();
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	void Clear() {	memset(this,0,sizeof(itemOrder));}
//	DWORD LoadStr(char *buff);
	inline BOOL IsBuyOrder()	{return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortOrder()	{return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsOpenOrder()	{return ((mstatus & (1<<0))==(1<<0));};
	inline BOOL IsMarketOrder()	{return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsCashOrder()	{return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsTestOrder()	{return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsPhoneTrade()  {return ((mstatus &( 1<<13))==(1<<13));};
	inline BOOL IsOption()      {return (msecsym[0]=='+'); };
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};

	inline BOOL IsStopOrder(){return ((mstatus &( 1<<10))==(1<<10));};
	inline BOOL IsBasicStopOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==(1<<10));};
	inline BOOL IsTrailerOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<11)));};
	inline BOOL IsRangeOrder(){return ((mstatus &(( 1<<10)|(1<<11)|(1<<26)))==((1<<10)|(1<<26)));};
	inline BOOL IsGTCOrder() { return (mtmforce==0xFFFE); };

	//this function move the order to the right list;

};

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
	BYTE mtrtype; //CURRENT NOT USED.
	long mstatus;
	BYTE mexecnum;
	char mconftxt[20];
	char mtoken[16];
	long mqty;
	double	mprice;
	char msecsym[10];
	BYTE mexchange;
	double maskprice;
	double mbidprice;
	char mrrno[8];
	double mcommision;
	long me_date;
	long mc_date;
	long mm_date;
public:
	void Clear() {memset(this,0,sizeof(itemOrder)); };
	void CopyFrom(itemTrade *ptrade){memmove(this,ptrade,sizeof(itemTrade));};
	void CopyFrom(itemTrade *pitem, DWORD flag);

	BOOL GetPkg(PkgToken *ptok, DWORD *pflag);

	DWORD LoadStr(char *buff);

	inline BOOL IsOpen(){return ((mstatus & 1)==1);};
//	inline BOOL IsOpenTrade(){return ((mstatus & 1)==(1));}; //open, but not allocated trade.
//	inline BOOL IsOrignalTrade(){return ((mstatus & 1)==(1));}; //open, but not sub trade.
	inline BOOL IsOpenTrade(){return ((mstatus & (5))==(1));}; //open, but not allocated trade.
	inline BOOL IsOrignalTrade(){return ((mstatus & (3))==(1));}; //open, but not sub trade.
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsTraining(){return ((mstatus & (1<<24))==(1<<24));};
	inline BOOL IsAddLiquity(){return ((mstatus &(1<<20))==(1<<20));};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};


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
};
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
	long me_date;
	long mm_date;
	double mcomm1;
public:
	DWORD LoadStr(char *buff);
	BOOL  SaveStr(char *buff, int bufflen,DWORD flag);
	inline BOOL IsBuyTrade(){return ((mstatus & (1<<6))==(1<<6));};
	inline BOOL IsShortTrade(){return ((mstatus & (1<<7))==(1<<7));};
	inline BOOL IsCashTrade(){return ((mstatus & (1<<8))==(1<<8));};
	inline BOOL IsMarketOrder(){return ((mstatus &( 1<<9))==(1<<9));};
	inline BOOL IsPhoneTrade() {return ((mstatus &( 1<<13))==(1<<13));};
	inline BOOL IsSolicit(){return ((mstatus &( 1<<15))==(1<<15));};
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
public:
	int GetPkg(PkgToken *ptok,DWORD *pflag);
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
	void itemAccIfo::CopyFrom(itemAccIfo *pitem);
	void itemAccIfo::CopyFrom(itemAccIfo *pitem, DWORD flag);
	inline BOOL IsDeleted(){return ((mstatus & 2)==2);};
	inline BOOL IsOpen(){
		if ((mstatus&0x3)==1) return TRUE; 
		else return FALSE;};
	inline BOOL IsMargin() {return  (mstatus & (1<<4))==(1<<4);};
	inline BOOL IsTraining() {return (mstatus & (1<<5))==(1<<5);};
	inline BOOL IsProprietary() {return  (mstatus & (1<<6))==(1<<6);};

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
	double mComm;		// commision;
	long   mtktnum;	    // today's ticket number;
	double mRealized;	// today's Realized P/L;
	long   mm_time;		// the last modify time;
public:
//	WORD CompareItem(itemAccEquity *pitem);
	int GetPkg(PkgToken *ptok,DWORD *pflag);
	void CopyFrom(itemAccEquity *pitem,DWORD flag);
	void CopyFrom(itemAccEquity *pitem);
	BOOL SetPkg(PkgToken *ptok, DWORD flag);
	void Clear();
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
	inline BOOL IsDeleted(){if ((mstatus&0x2)==2) return TRUE; else return FALSE;};
	inline BOOL IsOpen(){if ((mstatus&0x3)==1) return TRUE; else return FALSE;};
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
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
	long mtype;
	long mPerid;
	long mAdd1ID;
	long mAdd2ID;
	time_t mc_date;
	time_t mm_date;
	char mnote[256];
public:
	inline BOOL IsTrader(){if ((mstatus&0x30)==0) return TRUE; else return FALSE;};
	inline BOOL IsManager(){if ((mstatus&0x10)!=0) return TRUE; else return FALSE;};
	inline BOOL IsAdmin(){if ((mstatus&0x20)!=0) return TRUE; else return FALSE;};
	inline BOOL IsOpen() { if ((mstatus&3)==1) return TRUE; else return FALSE;};
	inline BOOL IsDeleted(){return ((mstatus & 2)==2);};
	DWORD LoadStr(char *buff);
	BOOL SaveStr(char *buff, int bufflen, DWORD flag);
};



class itemPosition {
public:
	enum {
		iPosAcc		=0x1,
		iPosSec		=0x2,
		iPosType	=0x4,
		iPosBrid	=0x8,
		iPosInitQty	=0x10,
		iPosInitPric=0x20,
		iPosQty		=0x40,
		iPosAvgPric	=0x80,
		iPosBP		=0x100,
		iPosReal	=0x200,
		iPosCdat	=0x400,
		iPosMdat	=0x800,
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
//for the risk ctrl
public:
	void Clear(BOOL clearall=TRUE);
	static char GetTypeVal(LPCSTR ptypename);
	char* GetTypeStr();
	void CopyFrom(itemPosition *pitem, DWORD flag);
	void CopyFrom(itemPosition *pitem){
		memmove(this,pitem,sizeof(itemPosition));
	};
	int GetPkg(PkgToken *ptok,DWORD *pflag);
};


class itemAcct_Link {
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
	BOOL GetStatusStr(char *pbuff);
//	BOOL CompareItem(itemAcct_Link *pitem);
	//flag 1:query, 2:add/modify, 3:delete;
	
	void Clear();
	void CopyItem(itemAcct_Link *pitem);
};

class itemOrderTokenReply
{
public:
	BYTE stat;
	int tokid;
	int ordid;
};



//the order activatity flag
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
	char * GetSide(char *st);
	char * GetTypeName(char *st);
	char * GetTypeStr(char *st);
	BYTE GetType();
	BOOL SetPkg(PkgToken *ptok);
	BOOL GetPkg(PkgToken *ptok);
	void Clear();
};
