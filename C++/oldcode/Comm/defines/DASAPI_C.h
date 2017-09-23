
#ifndef _DAS_C_API_H
#define _DAS_C_API_H

#include "QuoteDefs.h"
#include "OrderDefs.h"


extern "C" {  // ** To turn off name mangling

// ** In order to keep all fuction names unique across the project, prefix "DAS_" is added

//--------------------------------------------------------------------------------------
//Misc functions;
void DAS_InitAPI(int eventmask);
void DAS_FreeAPI();
BOOL DAS_PreTranslateMessage(MSG* pMsg);

//--------------------------------------------------------------------------------------
//Login and connections;
void DAS_SetServerAddr(char *orderserver, int orderport, char * quoteserver,int quoteport);
void DAS_Login(char *FirmId, char *UserId, char *Password);
void DAS_ShowMainFrame();
//Verify Trader return the trid of the user;
//trid is the unique id for this User;
int  DAS_VerifyTrader(char *UserID, char *Password);
//trid is returned by Verify User;
st_Trader *DAS_GetTraderInfo(int trid);

int  DAS_ChangePassword(char *UserID, char *OldPassword, char *NewPassword);
	// Return 0, not ready, return 1, status is connected and ready.
	// ServerType  0: order server, 1: Quoteservers;
int  DAS_ServerStatus(int servertype);
const char *DAS_GetErrorMessage(int errid);

//--------------------------------------------------------------------------------------
//Account  Order , Trade information;
int  DAS_EnumAccounts(int &pos, int trid, st_AccIfo* &pacc, st_AccEquity* &pequity);
int  DAS_GetAccount(int accid, st_AccIfo* &pacc, st_AccEquity* &pequity);
int  DAS_GetAccountID(const char *account);
int  DAS_EnumOrders(int &pos, int trid, st_Order* &porder);
int  DAS_EnumPosition(int &pos, int accid, st_Position* &position);
int  DAS_EnumTrades(int &pos, int trid, st_Trade* &ptrade);


int  DAS_SendOrder(st_Order *porder, char *errmsg, BOOL bWait);
int  DAS_ReplaceOrder(int orderid, int newqty, double newprice);
st_Order * DAS_GetOrder(int orderid);
int  DAS_CancelOrder(int orderid);

//--------------------------------------------------------------------------------------
//Feed subscrib/desubscrib functions:
	//Subscrib or unsubscrib data feed, feed type is a integer and defined as following
	//1: L1
	//2: L2
	//3: Time & sale 
	//4: ISLD
	//5: OpenBook
	//6: Arca
	//7: Inca
	//8: Brut
void DAS_SubScribFeed(char *psymbol, int feed, BOOL bSubScrib);
void DAS_SubScribL1Quotes( char * psymbol, BOOL bSubScrib);
void DAS_SubScribTimeSale(char *psymbol, BOOL bSubScrib);
st_L1Quotes * DAS_GetLv1Quote(char *psymbol);
st_OptQuotes *DAS_GetOptionQuote(char *psymbol);
int DAS_IsOrderServerLogin();


//More feed functions:

//this function get the snapshot of the level2 quotes which better than dprice 
// the quotes in stored in the pquotes array;
// returns the numbers of the quotes 
int DAS_GetL2Quotes(const char *psymbol, BOOL bBid, st_L2Quote *pQuotes, int MaxQuotes, double dprice);

//this function get the inet book quotes which is bettern the the dprice;
//the return structure will only use the price,size,time;
int DAS_GetInetBookQuotes(const char *psymbol, BOOL bBid,st_L2Quote *pQuotes, int MaxQuotes, double dprice);

//--------------------------------------------------------------------------------------
//Callback functions;
//The follow is the call back functions for quotes and orders.
typedef int (*FpOnTimeSale)(char*Symb, double price, int size, int tradetime,char mktcenter);
typedef int (*FpOnQuote)(st_L1Quotes *pQuote,DWORD nflag);
typedef int (*FpOnLv2Quote)(st_L2Quote *pst);
typedef int (*FpOnGetOrder)(st_Order *pOrder, DWORD nflag);
typedef int (*FpOnGetTrade)(st_Trade *pTrade, DWORD nflag);

//This call back function is used to translate the order token id to order id;
//when customer send an order to server, give an tokenid, and the order server allocated an unique order id
//to that order, then we will know which order is related to the order you send to the server.
typedef int (*FpOnGetOrderToken)(int stat,int tokid,int orderid);


//when an order status changes. this function will get called.
typedef int (*FpOnGetOrderActive) (st_Order_Active *orderactive);

//when an InetBook have an items, this function will get called.
typedef int (*FpOnGetInetBook) (st_InetBook *pinetbookitem);

void DAS_SetCB_TimeSale(FpOnTimeSale fp);
void DAS_SetCB_Quote(FpOnQuote fp);
void DAS_SetCB_OnLv2Quote(FpOnLv2Quote fp);
void DAS_SetCB_OnGetOrder(FpOnGetOrder fp);
void DAS_SetCB_OnGetTrade(FpOnGetTrade fp);

void DAS_SetCB_OnGetOrderToken(FpOnGetOrderToken fp);
void DAS_SetCB_OnGetOrderActive(FpOnGetOrderActive);

}


#endif  // ** _DAS_C_API_H
