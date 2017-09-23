// CDasApi.h: interface for the CDasApi class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDasApi_H__C70A243B_812B_4617_8AA8_A721A4171915__INCLUDED_)
#define AFX_CDasApi_H__C70A243B_812B_4617_8AA8_A721A4171915__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "QuoteDefs.h"
#include "OrderDefs.h"


class CDasApi  
{
	int m_EventMask;
public:
	//0x0001: Level 1 quotes;
	//0x0002: Time & Sale.
	//0x0004: Level 2 quotes
	//0x0100: Order change events;
	//0x0200: Trade change events;
	CDasApi(int eventmask);
	CDasApi();
	virtual ~CDasApi();

public :
	//login in to the sever, firm id can be set as NULL
	void Login(char *FirmId, char * UserId, char *Password);
	void ShowMainFrame();
	//Verify Trader's username and password.
	//If this trader is able to trade, return trader id.
	//Otherwise return 0;
	int  VerifyTrader(char *UserID, char *Password);
	
	//change password for the trader;
	//return: 0: successed; otherwise error code. GetErrorMessage() to get the error message;
	int ChangePassword(char *UserID, char *OldPassword, char *NewPassword);

	
	//this fnction return the account informaton and equity for the specific traderid
	//pos  : this is the iterator, first time call this function, Set the iterator to 0
	//trid : the trader id, which return by VerifyTraer.
	//return: if find accounts, return non-zero, you can call this functon again to get more account
	//		  if return zero, means can't find more account.
	int EnumAccounts(int &pos,int trid,st_AccIfo* &pacc,st_AccEquity* &pequity);
	//return 1 if success and pacc and pequity are seted.
	//return 0 if fail.
	//accid is the account id.
	int GetAccount(int accid,st_AccIfo* &pacc,st_AccEquity* &pequity);
	
	//Translate the account name to account id;
	//if account exist, return account id, otherwise return 0; 
	int GetAccountID(char *account);

	//Get All the orders of today, include open and canceled order
	int EnumOrders(int &pos, int trid, st_Order* &porder);
	//Get All the Executions of today.
	int EnumTrades(int &pos, int trid, st_Trade* &ptrade);
	int EnumPosition(int &pos, int accid, st_Position* &position);

	//the order information is in the orders, 
	//errmsg has to be allocaed and big enough!!  char errmsg[200].
	//return 0 successed, otherwise return the errorcode 
	// when order send successed, the porder->orderid will be the order id
	//			if bWait set to TRUE
	int SendOrder(st_Order *porder, char *errmsg,BOOL bWait=TRUE);


	st_Order * GetOrder(int orderid);
	int CancelOrder(int orderid);
	//this function replace the order. right now we only allow replace the qty and price;
	//return 0 success, otherwise use GetErrorMessage to get the error message.
	int ReplaceOrder(int orderid, int newqty, double newprice);

public:
	//Subscrib or unsubscrib data feed, feed type is a integer and defined as following
	//1: L1
	//2: L2
	//3: Time & sale 
	//4: ISLD
	//5: OpenBook
	//6: Arca
	//7: Inca
	//8: Brut
	void SubScribFeed(char *psymbol, int feed, BOOL bSubScrib);

	//this function can be replaced by SubScribFeed with feed set to 1
	void SubScribL1Quotes( char * psymbol,BOOL bSubScrib);
	//this function can be replaced by SubScribFeed with feed set to 3
	void SubScribTimeSale(char *psymbol, BOOL bSubScrib);

	//Get snapshot of lv1 quotes.
	st_L1Quotes  * GetLv1Quote(char *psymbol);
	st_OptQuotes * GetOptionQuote(char *psymbol);
	int IsOrderServerLogin();

public:
	//When time&sale comes, this virtual function will get called.
	virtual int OnTimeSale(char*Symb, double price, int size, int tradetime,char mktcenter);
	//When Level1 quotes changes, this virtual function will get called.
	virtual int OnQuote(st_L1Quotes *pQuote);
	//When Level2 quotes changes, this virtual function will get called.
	virtual int OnLv2Quote(st_L2Quote *pst);

	virtual int OnGetOrder(st_Order *pOrder,DWORD nflag);
	virtual int OnGetTrade(st_Trade *pTrade,DWORD nflag);


public:
	void SetServerAddr(char *orderserver, int orderport, char * quoteserver,int quoteport);
	const char *GetErrorMessage(int errid);
	static BOOL PreTranslateMessage(MSG* pMsg);
};



extern "C" {
//this function get the snapshot of the level2 quotes which better than dprice 
// the quotes in stored in the pquotes array;
// returns the numbers of the quotes 
int DAS_GetL2Quotes(const char *psymbol, BOOL bBid, st_L2Quote *pQuotes, int MaxQuotes, double dprice);

//this function get the inet book quotes which is bettern the the dprice;
//the return structure will only use the price,size,time;
int DAS_GetInetBookQuotes(const char *psymbol, BOOL bBid,st_L2Quote *pQuotes, int MaxQuotes, double dprice);

st_Trader *DAS_GetTraderInfo(int trid);

}

#endif // !defined(AFX_CDasApi_H__C70A243B_812B_4617_8AA8_A721A4171915__INCLUDED_)
