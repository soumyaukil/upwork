#pragma once

#define MaxTMSellNum 30


struct toplstitem
{
	char secsym[10];
};

///Time@sale information;
struct itemTimeSale{
	double price;
	long   volume;
	WORD   condition;
	time_t msgtime;
//	itemTimeSale *pNext;
//	itemTimeSale *pPrev;
};


//L2 quote information;
struct itemQuote{
	char MMID[8]; 
	//bit 0xFF, the quote condition;
	//bit 0xFF00, the change flag 
	//		100: is ask qutoe,
	//		200:price +, 
	//		400 size + 
	//		800: price changed
	//     1000: size changed, 
	//	   2000:condition changd, 
	//	   4000, time changed, 
	//	   8000: all changd.
	//bit 0xFFFF0000: reserved:
	//    10000; removed;
	int    QuoteFlag; // include PMM, Quoteflag,etc.
	double Price;
	int	   Size;
	time_t QuoteTime;
public:
	inline BOOL IsAskQuote(){ return (QuoteFlag&0x100);};
	inline BOOL IsActiveQuote(){ return ((QuoteFlag& 0xFF)!='C');};
};



class itemsyminfo
{
private:
	//internal use.
	itemTimeSale *mpTmsallHead;
	itemTimeSale *mpTmsallTail;
	int   mnumTimeSellItem;

public:
	char msecsym[20];
	char mprimexch;
	int mstatus;
	double	m_l1_BidPrice;
	int		m_l1_BidSize;
	double	m_l1_AskPrice;
	int		m_l1_AskSize;
	time_t	m_l1_QuoteTime;
	double	m_l1_lastPrice;
	double  m_l1_todayhigh;
	double  m_l1_todaylow;
	double  m_l1_todayclose;
	int		m_l1_volume;
	double	m_l1_yclose;
	double	m_l1_todayopen;
	double	m_l1_LastBidPrice;// this is used for show up/down arrow for NASDAQ NMS stocks;
	double  m_l1_lastPriceOld; // for internal use.

};