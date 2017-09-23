// QuoteLst.h: interface for the QuoteLst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QUOTELST_H__C675B031_0803_11D5_BD29_0050041B8FEC__INCLUDED_)
#define AFX_QUOTELST_H__C675B031_0803_11D5_BD29_0050041B8FEC__INCLUDED_

#include "..\FUNCTIONS\FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct QuoteIfo{
	DWORD MMID; // MMID should
	//bit 0xFF, the quote condition;
	//bit 0xFF00, the change flag 
	//         100: ask, 200:price +, 400 size + 800: price changed
	//         1000: size changed, 2000:condition changd, 4000, time changed, 8000: all changd.
	//bit 0xFFFF0000: reserved:
	//      10000; removed;
	int    QuoteCondition; // include PMM, Quoteflag,etc.
	double Price;
	int	   Size;
	time_t QuoteTime;
	struct QuoteIfo * pnext;
	struct QuoteIfo * pprev;
public:
	inline BOOL IsAskQuote(){ return (QuoteCondition &0x100);};
	inline BOOL IsActiveQuote(){ return ((QuoteCondition & 0xFF)!='C');};
};

class QuoteLst  
{
public:
	QuoteIfo * FindQuoteByMMID( DWORD MMID);
	int MarkRemove(BOOL bless,double dprice, int currtime, int second);
	QuoteIfo * RmAllPrice(BOOL bless,double dprice,int currtime,int seconds);
	BOOL FindBestPrice(double *hi, int *hisize, double *lo, int *losize);
	QuoteIfo * QSevInsertQuote(BOOL bAscend, DWORD nMMID, DWORD &nQuoteCondition, double dprice, int nSize, time_t ntime);
//	QuoteIfo *SortInsertQuote(BOOL bAscend, DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime);
	inline QuoteIfo *GetHead(){ return mphead;};
	int GetCount();
	QuoteIfo *InsertQuote(BOOL bAscend, DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime);
	QuoteIfo * AddQuote(BOOL bAscend,DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime,int flag=0xff);
	void RemoveAll();
	QuoteIfo * RmoveQuote(DWORD nMMID);
	QuoteLst();
	virtual ~QuoteLst();
	static void GetMemUsage(int &ntotal,int &nvoid);
	static void ReleaseItemBuff(QuoteIfo *pitem);
	static void ReleaseItemsLst(QuoteIfo *phead);
	static void Init();
	static QuoteIfo * GetItemBuff();
	void Lock() { mLock.Lock(); };
	void UnLock() {mLock.Unlock(); };
private:
	BOOL InsertNewQuote(BOOL bAscend, QuoteIfo *pifo);
	simplelock mLock;
	QuoteIfo *mphead;// no tail, only keep head;
static	FixLenMem sQuoteInfoBuff;

};

#endif // !defined(AFX_QUOTELST_H__C675B031_0803_11D5_BD29_0050041B8FEC__INCLUDED_)
