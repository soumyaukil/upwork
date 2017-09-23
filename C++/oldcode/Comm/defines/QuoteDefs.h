#ifndef __QUOTESTRUCT_DEFS
#define __QUOTESTRUCT_DEFS
#pragma once
/***********************************
This file defines the structure used in DAS tading systmes.

************************************/

class st_L1Quotes
{
public:
	char secsym[10];
	char PrimExch;
	int mstatus;
	double	l1_BidPrice;
	int		l1_BidSize;
	double	l1_AskPrice;
	int		l1_AskSize;
	time_t	l1_QuoteTime;
	double	l1_lastPrice;
	double  l1_todayhigh;
	double  l1_todaylow;
	double  l1_todayclose;
	int		l1_volume;
	double	l1_yclose;
	double	l1_todayopen;
	double	l1_LastBidPrice;// this is used for show up/down arrow for NASDAQ NMS stocks;
	

};



// QuoteCondition (0x100) Is ask;
//				  (0x10000) marked as removed.
class st_L2Quote {
public:
	char secsym[12];
	char MMID[8];
	int QuoteCondition;
	double Price;
	int Size;
	int time;
};

class st_InetBook{
public:
	char secsym[12];
	int action; //A, add, C, cancel, E, execution;
	int flag;    //bit 1: buy/sell
	int nrefnu;	 //reference number,
	double price;
	int    mshare;  //how many share in this level;
	int    mchange; //how many shares change; 
	int    ntime;
};


class st_OptQuotes{
public:
	char moptsymb[12];
	char munderly[12];
	long mstrickprice; // actuall price /1000
	char mPrimExch;
	DWORD mPrimUndShr;
	long mexpday;
	DWORD mopenint;
	double mask;
	long   masksz;
	char   maskex;
	double mbid;
	long   mbidsz;
	char   mbidex;
	double mlast;
	DWORD mvolume;
	double myclose;
	double mopen;
	double mclose;
	double mhi;
	double mlo;
};

#endif