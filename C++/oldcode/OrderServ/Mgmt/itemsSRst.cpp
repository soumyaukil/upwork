// itemsSRst.cpp: implementation of the srstAccIfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../orderserv.h"
#include "itemsSRst.h"
#include "../OrderDB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern OrderDB theOrderDB;
extern OrderConfig theCfgFile;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstAccIfo::srstAccIfo()
{

}

srstAccIfo::~srstAccIfo()
{

}

//DEL void srstAccIfo::DataTranslate(BOOL update)
//DEL {
//DEL 	if (update==TRUE) { // for update
//DEL 		if (bindflag  &	(1<<10)){ //create time
//DEL 			TranslateDate(&mitem.mc_date,&mc_date,TRUE);  
//DEL 		}
//DEL 		if (bindflag  &	(1<<11)){ //create time
//DEL 			TranslateDate(&mitem.mc_date,&mc_date,TRUE);  
//DEL 		}
//DEL 	} else { // for getdate
//DEL 		if (bindflag  &	(1<<10)){ //create time
//DEL 			TranslateDate(&mitem.mc_date,&mc_date,TRUE);  
//DEL 		}
//DEL 		if (bindflag  &	(1<<11)){ //create time
//DEL 			TranslateDate(&mitem.mc_date,&mc_date,TRUE);  
//DEL 		}
//DEL 	}	
//DEL }



BOOL srstAccIfo::Open()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try{
		ExecuteSql("select accid,name,brid,status,rrno,"
		"linkacc,per_id,add1_id,add2_id,c_date,"
		"m_date,note from maccountifo");
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	mitem.modifyflag =0xffffffff;
	MoveNext();
	return TRUE;
}

BOOL srstAccIfo::OnBind(int flag)
{
	UnBindAll(); 
	if (mitem.modifyflag  & 1 ){ 
		BindLong(flag,1,&mitem.maccid);
	}
	if (mitem.modifyflag & (1<<1)) {
		BindText(flag,2,mitem.mname,sizeof(mitem.mname));
	}
	if (mitem.modifyflag & (1<<2)) {
		BindLong(flag,3,&mitem.mbrid);
	}
	if (mitem.modifyflag & (1<<3)) {
		BindLong(flag,4,&mitem.mstatus);
	}
	if (mitem.modifyflag & (1<<4)) {
		BindText(flag,5,mitem.mrrno,sizeof(mitem.mrrno));
	}
	if (mitem.modifyflag & (1<<5)) {
		BindLong(flag,6,&mitem.mlinkacc);
	}
	if (mitem.modifyflag & (1<<7)) {
		BindLong(flag,7,&mitem.mper_id);
	}
	if (mitem.modifyflag & (1<<8)) {
		BindLong(flag,8,&mitem.maddr1_id); 
	}
	if (mitem.modifyflag & (1<<9)) {
		BindLong(flag,9,&mitem.maddr2_id); 
	}
	if (mitem.modifyflag & (1<<10)){
		if ((flag==bindForUpdate)||(flag==bindForNew)){
			TranslateDate(&mitem.mc_date,&mc_date,TRUE);
		}
		BindTimeStruct(flag,10,&mc_date); 
		if (flag==bindFixUp) {
			TranslateDate(&mitem.mc_date,&mc_date,FALSE);  
		}
	}
	if (mitem.modifyflag & (1<<11)){
		if ((flag==bindForUpdate)||(flag==bindForNew)){
			TranslateDate(&mitem.mm_date,&mm_date,TRUE);
		}
		BindTimeStruct(flag,11,&mm_date); 
		if (flag==bindFixUp) {
			TranslateDate(&mitem.mm_date,&mm_date,FALSE);  
		}
	}
	if (mitem.modifyflag & (1<<12)) {
		BindText(flag,12,mitem.mnote,sizeof(mitem.mnote));
	}
	
	bindflag=mitem.modifyflag;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// srstAcct_Access Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstAcct_Access::srstAcct_Access()
{

}

srstAcct_Access::~srstAcct_Access()
{

}

BOOL srstAcct_Access::OnBind(int flag)
{
	UnBindAll(); 
	if ((flag==bindForRead)||(flag==bindForNew)){
		BindLong(flag,1,&mitem.mtrid);
		BindLong(flag,2,&mitem.maccid);
	}
	BindLong(flag,3,&mitem.mstatus);
	BindLong(flag,4,&mitem.mdealid);
	if ((flag==bindForUpdate)||(flag==bindForNew)){
		time(&mitem.mm_date);
		TranslateDate(&mitem.mm_date,&mm_date,TRUE);
	}
	BindTimeStruct(flag,5,&mm_date);
	if (flag==bindFixUp) {
		TranslateDate(&mitem.mm_date,&mm_date,FALSE);  
	}
	BindLong(flag,6,&mitem.mOpdealid);
	BindLong(flag,7,&mitem.mFtdealid);
	
	return TRUE;
}

BOOL srstAcct_Access::Open(BOOL bRead,long ntrid,long nacid )
{
	char sqlstr[500];
	char *comlst="select trid,accid,status,dealid,m_date,opdealid,ftdealid from macct_access ";
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	try{
		if (bRead==TRUE) {
			SetConCurrency(SQL_CONCUR_READ_ONLY);
			SetCursorType (SQL_CURSOR_FORWARD_ONLY);
			ExecuteSql(comlst);
		}else {
			SetConCurrency(SQL_CONCUR_VALUES);
			SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
			sprintf(sqlstr,"%s where trid=%d and accid= %d",comlst,ntrid,nacid);
			ExecuteSql(sqlstr);
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}

	MoveNext();
	return TRUE;

}

//////////////////////////////////////////////////////////////////////
// srstPosition Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstPosition::srstPosition()
{

}

srstPosition::~srstPosition()
{

}


BOOL srstPosition::OpenToRead()
{
//	char * sqlstr;
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
//	sqlstr=GetColLst()
	try {
		ExecuteSql(GetColLst(FALSE));
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	};
	MoveNext();
	return TRUE;

}

BOOL srstPosition::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.maccid);
		BindText(flag,2,mitem.msecsym,sizeof(mitem.msecsym));
		BindBYTE(flag,3,&mitem.mptype);
		BindLong(flag,4,&mitem.mbrid);
		BindLong(flag,5,&mitem.mInitQty);
		BindDouble(flag,6,&mitem.mInitPrice);
		BindLong(flag,7,&mitem.mqty);
		BindDouble(flag,8,&mitem.mavgcost);
		BindTime_t(flag,9,&mitem.mc_date,&mc_date);
		BindTime_t(flag,10,&mitem.mm_date,&mm_date);
		break;
case bindForUpdate:
		if(mDirtyFlag & (iPosBrid))	BindLong(flag,4,&mitem.mbrid);
		if(mDirtyFlag & (iPosInitQty)) BindLong(flag,5,&mitem.mInitQty);
		if(mDirtyFlag & (iPosInitPric)) BindDouble(flag,6,&mitem.mInitPrice); 
		if(mDirtyFlag & (iPosQty)) BindLong(flag,7,&mitem.mqty);
		if(mDirtyFlag & (iPosAvgPric)) BindDouble(flag,8,&mitem.mavgcost);
		if(mDirtyFlag & (iPosCdat)) BindTime_t(flag,9,&mitem.mc_date,&mc_date);
		if(mDirtyFlag & (iPosMdat)) BindTime_t(flag,10,&mitem.mm_date,&mm_date);
		break;
	}
	return TRUE;
}

BOOL srstPosition::OpenToUpdate(BOOL bNewTable,int naccid, char *psecsym, BYTE btype)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where accid=%d and secsym='%s' and ptype=%d "
		,GetColLst(bNewTable),naccid,psecsym,btype);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try {
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	};
	MoveNext();
	return TRUE;

}

LPCSTR srstPosition::GetColLst(BOOL bNewTable)
{
	if (bNewTable) {
		return "select accid,secsym,ptype,brid,InitQty,"
			"InitPrice,qty,AvgCost,c_date,m_date from mNewPosition";
	}else {
		return "select accid,secsym,ptype,brid,InitQty,"
			"InitPrice,qty,AvgCost,c_date,m_date from mPosition";
	}
}

BOOL srstOrders::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.morderid);
		BindLong(flag,2,&mitem.mtrid);
		BindLong(flag,3,&mitem.maccid);
		BindLong(flag,4,&mitem.mentrid);
		BindLong(flag,5,&mitem.morigoid);
		BindLong(flag,6,&mitem.morigtkn); 
		BindLong(flag,7,&mitem.mbrid);
		BindText(flag,8,mitem.mroute,sizeof(mitem.mroute));
		BindText(flag,9,mitem.mbkrsym,sizeof(mitem.mbkrsym));
		BindLong(flag,10,&mitem.mstatus);
		BindBYTE(flag,11,&mitem.mexecnum);
		BindText(flag,12,mitem.mconftxt,sizeof(mitem.mconftxt));
		BindText(flag,13,mitem.mtoken,sizeof(mitem.mtoken));
		BindLong(flag,14,&mitem.mqty);
		BindLong(flag,15,&mitem.mlvsqty);
		BindLong(flag,16,&mitem.mcxlqty);
		BindText(flag,17,mitem.msecsym,sizeof(mitem.msecsym));
		BindBYTE(flag,18,&mitem.mexchange);
		BindDouble(flag,19,&mitem.mprice);
		BindDouble(flag,20,&mitem.mstopprice);
		BindDouble(flag,21,&mitem.mtrailprice);
		BindDouble(flag,22,&mitem.maskprice);
		BindDouble(flag,23,&mitem.mbidprice);
		BindText(flag,24,mitem.mrrno,sizeof(mitem.mrrno));
		BindWORD(flag,25,&mitem.mtmforce);
		BindTime_t(flag,26,&mitem.mc_date,&mc_date);
		BindTime_t(flag,27,&mitem.mm_date,&mm_date);
		BindLong(flag,28,&mitem.ms1);
		BindLong(flag,29,&mitem.mf1);
		BindLong(flag,30,&mitem.mf2);
		BindLong(flag,31,&mitem.mf3);
		break;
	case bindForUpdate:
		if (mDirtyFlag&itemOrder::deforderid) BindLong(flag,1,&mitem.morderid);
		if (mDirtyFlag&itemOrder::deftrid) BindLong(flag,2,&mitem.mtrid);
		if (mDirtyFlag&itemOrder::defaccid) BindLong(flag,3,&mitem.maccid);
		if (mDirtyFlag&itemOrder::defentrid) BindLong(flag,4,&mitem.mentrid);
		if (mDirtyFlag&itemOrder::deforigoid)	BindLong(flag,5,&mitem.morigoid);
		if (mDirtyFlag&itemOrder::deforigtkn)	BindLong(flag,6,&mitem.morigtkn); 
		if (mDirtyFlag&itemOrder::defbrid) BindLong(flag,7,&mitem.mbrid);
		if (mDirtyFlag&itemOrder::defroute) BindText(flag,8,mitem.mroute,sizeof(mitem.mroute));
		if (mDirtyFlag&itemOrder::defbkrsym) BindText(flag,9,mitem.mbkrsym,sizeof(mitem.mbkrsym));
		if (mDirtyFlag&itemOrder::defstatus) BindLong(flag,10,&mitem.mstatus);
		if (mDirtyFlag&itemOrder::defexecnum) BindBYTE(flag,11,&mitem.mexecnum);
		if (mDirtyFlag&itemOrder::defconftxt) BindText(flag,12,mitem.mconftxt,sizeof(mitem.mconftxt));
		if (mDirtyFlag&itemOrder::deftoken) BindText(flag,13,mitem.mtoken,sizeof(mitem.mtoken));
		if (mDirtyFlag&itemOrder::defqty) BindLong(flag,14,&mitem.mqty);
		if (mDirtyFlag&itemOrder::deflvsqty) BindLong(flag,15,&mitem.mlvsqty);
		if (mDirtyFlag&itemOrder::defcxlqty) BindLong(flag,16,&mitem.mcxlqty);
		if (mDirtyFlag&itemOrder::defsecsym) BindText(flag,17,mitem.msecsym,sizeof(mitem.msecsym));
		if (mDirtyFlag&itemOrder::defexchange) BindBYTE(flag,18,&mitem.mexchange);
		if (mDirtyFlag&itemOrder::defprice) BindDouble(flag,19,&mitem.mprice);
		if (mDirtyFlag&itemOrder::defstopprice) BindDouble(flag,20,&mitem.mstopprice);
		if (mDirtyFlag&itemOrder::deftrailprice) BindDouble(flag,21,&mitem.mtrailprice);
		if (mDirtyFlag&itemOrder::defaskprice) BindDouble(flag,22,&mitem.maskprice);
		if (mDirtyFlag&itemOrder::defbidprice) BindDouble(flag,23,&mitem.mbidprice);
		if (mDirtyFlag&itemOrder::defrrno) BindText(flag,24,mitem.mrrno,sizeof(mitem.mrrno));
		if (mDirtyFlag&itemOrder::deftmforce) BindWORD(flag,25,&mitem.mtmforce);
		if (mDirtyFlag&itemOrder::defc_date) BindTime_t(flag,26,&mitem.mc_date,&mc_date);
		if (mDirtyFlag&itemOrder::defm_date) BindTime_t(flag,27,&mitem.mm_date,&mm_date);
		if (mDirtyFlag&itemOrder::defmexts1)  BindLong(flag,28,&mitem.ms1);
		if (mDirtyFlag&itemOrder::defmextf1)  BindLong(flag,29,&mitem.mf1);
		if (mDirtyFlag&itemOrder::defmextf2)  BindLong(flag,30,&mitem.mf2);
		if (mDirtyFlag&itemOrder::defmextf3)  BindLong(flag,31,&mitem.mf3);

		break;
	}
	return TRUE;

}

LPCSTR srstOrders::GetColLst()
{
	return "select OrderID,trid,accid,entrid,origoid,"
		"origtkn,brid,route,bkrsym,status,"
		"execnum,conftxt,token,qty,lvsqty,"
		"cxlqty,secsym,exchange,price,stopprice,"
		"trailer,ask,bid,rrno,timeforce,"
		"c_date,m_date,s1,f1,f2,f3  from mOrders";
}

srstOrders::srstOrders()
{
}

BOOL srstOrders::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstOrders::OpenToUpdate(int orderid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where OrderID=%d "
		,GetColLst(),orderid);
	try{
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
		SetConCurrency(SQL_CONCUR_VALUES);
		SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
		ExecuteSql(sqlstr);
		MoveNext();
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// srstTrade Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstTrade::srstTrade()
{

}

srstTrade::~srstTrade()
{

}

LPCSTR srstTrade::GetColLst()
{
	return "select TradeID,trid,accid,entrid,origAcc,"
		"origtrid,origoid,OrderID,brid,route,"
		"bkrsym,trtype,status,execnum,conftxt,"
		"token,qty,price,secsym,exchange,"
		"ask,bid,rrno,comm,e_date,"
		"c_date,m_date  from mTrades";

}

BOOL srstTrade::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,	1 ,&mitem.mtradeid);
		BindLong(flag,	2 ,&mitem.mtrid);
		BindLong(flag,	3 ,&mitem.maccid);
		BindLong(flag,	4 ,&mitem.mentrid);
		BindLong(flag,	5 ,&mitem.morigacc);
		BindLong(flag,	6 ,&mitem.morigtrid);
		BindLong(flag,	7 ,&mitem.morigoid);
		BindLong(flag,	8 ,&mitem.morderid);
		BindLong(flag,	9 ,&mitem.mbrid);
		BindText(flag,	10,mitem.mroute,sizeof(mitem.mroute));
		BindText(flag,	11,mitem.mbkrsym,sizeof(mitem.mbkrsym));
		BindBYTE(flag,	12,&mitem.mtrtype);  
		BindLong(flag,	13,&mitem.mstatus);
		BindBYTE(flag,	14,&mitem.mexecnum);
		BindText(flag,	15,mitem.mconftxt,sizeof(mitem.mconftxt));
		BindText(flag,	16,mitem.mtoken,sizeof(mitem.mtoken));
		BindLong(flag,	17,&mitem.mqty);
		BindDouble(flag,18,&mitem.mprice);
		BindText(flag,	19,mitem.msecsym,sizeof(mitem.msecsym));
		BindBYTE(flag,	20,&mitem.mexchange);
		BindDouble(flag,21,&mitem.mEcnFee);
		BindDouble(flag,22,&mitem.mTradePL);
		BindText(flag,	23,mitem.mrrno,sizeof(mitem.mrrno));
		BindDouble(flag,24,&mitem.mcommision);
		BindTime_t(flag,25,&mitem.me_date,&me_date);
		BindTime_t(flag,26,&mitem.mc_date,&mc_date);
		BindTime_t(flag,27,&mitem.mm_date,&mm_date);
		break;
	case bindForUpdate:
		if (mDirtyFlag&itemTrade::deftradeid	) BindLong(flag,  1 ,&mitem.mtradeid);
		if (mDirtyFlag&itemTrade::deftrid		) BindLong(flag,  2 ,&mitem.mtrid);
		if (mDirtyFlag&itemTrade::defaccid		) BindLong(flag,  3 ,&mitem.maccid);
		if (mDirtyFlag&itemTrade::defentrid	) BindLong(flag,  4 ,&mitem.mentrid);
		if (mDirtyFlag&itemTrade::deforigacc	) BindLong(flag,  5 ,&mitem.morigacc);
		if (mDirtyFlag&itemTrade::deforigtrid	) BindLong(flag,  6 ,&mitem.morigtrid);
		if (mDirtyFlag&itemTrade::deforigoid	) BindLong(flag,  7 ,&mitem.morigoid);
		if (mDirtyFlag&itemTrade::deforderid	) BindLong(flag,  8 ,&mitem.morderid);
		if (mDirtyFlag&itemTrade::defbrid		) BindLong(flag,  9 ,&mitem.mbrid);
		if (mDirtyFlag&itemTrade::defroute		) BindText(flag,  10,mitem.mroute,sizeof(mitem.mroute));
		if (mDirtyFlag&itemTrade::defbkrsym	) BindText(flag,  11,mitem.mbkrsym,sizeof(mitem.mbkrsym));
		if (mDirtyFlag&itemTrade::deftrtype	) BindBYTE(flag,  12,&mitem.mtrtype);  
		if (mDirtyFlag&itemTrade::defstatus	) BindLong(flag,  13,&mitem.mstatus);
		if (mDirtyFlag&itemTrade::defexecnum	) BindBYTE(flag,  14,&mitem.mexecnum);
		if (mDirtyFlag&itemTrade::defconftxt	) BindText(flag,  15,mitem.mconftxt,sizeof(mitem.mconftxt));
		if (mDirtyFlag&itemTrade::deftoken		) BindText(flag,  16,mitem.mtoken,sizeof(mitem.mtoken));
		if (mDirtyFlag&itemTrade::defqty		) BindLong(flag,  17,&mitem.mqty);
		if (mDirtyFlag&itemTrade::defprice		) BindDouble(flag,18,&mitem.mprice);
		if (mDirtyFlag&itemTrade::defsecsym	) BindText(flag,  19,mitem.msecsym,sizeof(mitem.msecsym));
		if (mDirtyFlag&itemTrade::defexchange	) BindBYTE(flag,  20,&mitem.mexchange);
		if (mDirtyFlag&itemTrade::defecnfee	) BindDouble(flag,21,&mitem.mEcnFee);
		if (mDirtyFlag&itemTrade::deftradepl) BindDouble(flag,22,&mitem.mTradePL);
		if (mDirtyFlag&itemTrade::defrrno		) BindText(flag,  23,mitem.mrrno,sizeof(mitem.mrrno));
		if (mDirtyFlag&itemTrade::defcommision	) BindDouble(flag,24,&mitem.mcommision);
		if (mDirtyFlag&itemTrade::defe_date	) BindTime_t(flag,25,&mitem.me_date,&me_date);
		if (mDirtyFlag&itemTrade::defc_date	) BindTime_t(flag,26,&mitem.mc_date,&mc_date);
		if (mDirtyFlag&itemTrade::defm_date	) BindTime_t(flag,27,&mitem.mm_date,&mm_date);
		break;
	}
	return TRUE;
}

BOOL srstTrade::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try {
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	};
	MoveNext();
	return TRUE;

}

BOOL srstTrade::OpenToUpdate(int tradeid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where TradeID=%d "
		,GetColLst(),tradeid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	};
	MoveNext();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// srstAccEquity Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstAccEquity::srstAccEquity()
{

}

srstAccEquity::~srstAccEquity()
{

}

BOOL srstAccEquity::OnBind(int flag)
{
	UnBindAll(); 
	BindLong(flag,1,&mitem.maccid);
	BindDouble(flag,2,&mitem.mOpenBP);
	BindDouble(flag,3,&mitem.mOpenNBP);
	BindDouble(flag,4,&mitem.mOpenEQ);
	BindDouble(flag,5,&mitem.mOpenMGEQ);
	BindDouble(flag,6,&mitem.mOpenEQRQ);
	BindDouble(flag,7,&mitem.mCurrBP);
	BindDouble(flag,8,&mitem.mCurrNBP);
	BindDouble(flag,9,&mitem.mCurrHoldBP); 
	BindDouble(flag,10,&mitem.mCurrEQ);
	BindDouble(flag,11,&mitem.mCurrMGEQ); 
	BindDouble(flag,12,&mitem.mCurrEQRQ);
	BindDouble(flag,13,&mitem.mComm);
	BindTime_t(flag,14,&mitem.mm_time,&mm_date);
	return TRUE;
}

LPCSTR srstAccEquity::GetColLst(BOOL bNewTable)
{
	if (bNewTable) {
		return "select accid,OpenBP,OpenNBP,OpenEQ,OpenMGEQ,OpenEQRQ,"
			"CurrBP,CurrNBP,CurrHoldBP,currEQ,CurrMGEQ,CurrEQRQ,Comm,m_date "
			" from mNewAccount";
	}else {
		return "select accid,OpenBP,OpenNBP,OpenEQ,OpenMGEQ,OpenEQRQ,"
			"CurrBP,CurrNBP,CurrHoldBP,currEQ,CurrMGEQ,CurrEQRQ,Comm,m_date "
			" from mAccount";
	}
}

BOOL srstAccEquity::Open(BOOL bNewTable)
{

	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(GetColLst(bNewTable));
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstAccEquity::OpenToUpdate(BOOL bNewTable,int accid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where accid=%d "
		,GetColLst(bNewTable),accid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}




srstAccCtrl::srstAccCtrl()
{

}

srstAccCtrl::~srstAccCtrl()
{

}

BOOL srstAccCtrl::OnBind(int flag)
{
	UnBindAll(); 
	BindLong(flag,1,&mitem.maccid);
	BindLong(flag,2,&mitem.mflag);
	BindLong(flag,3,&mitem.mflagex);
	BindDouble(flag,4,&mitem.mMaxLoss);
	BindLong(flag,5,&mitem.maxshare);
	BindLong(flag,6,&mitem.maxtotalshare);
	BindLong(flag,7,&mitem.maxsymbshare);
	BindLong(flag,8,&mitem.maxsymbs);
	BindDouble(flag,9,&mitem.maxUnrealizedLoss);
	BindLong(flag,10,&mitem.mstarttradetime);
	BindLong(flag,11,&mitem.mstoptradetime);
	return TRUE;
}

LPCSTR srstAccCtrl::GetColLst()
{
	return "select accid,flag,flagex,maxloss,maxshare,maxtotalshare,maxsymbshare,"
			"maxsymbs,maxunrealizedloss,starttradetime,stoptradetime "
			" from mAccCtrlEx";
}

BOOL srstAccCtrl::Open()
{

	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstAccCtrl::OpenToUpdate(int accid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where accid=%d "
		,GetColLst(),accid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}




BOOL srstDeal::OnBind(int flag)
{
	UnBindAll(); 
	BindLong(flag,1,&mitem.mdealid);
	BindText(flag,2,mitem.mname,sizeof(mitem.mname));
	BindLong(flag,3,&mitem.mdefaultdeal);
	BindLong(flag,4,&mitem.msharelmt);
	BindDouble(flag,5,&mitem.mshareadj);
	BindDouble(flag,6,&mitem.mlimitadj);
	BindDouble(flag,7,&mitem.mmktadj);
	BindLong(flag,8,&mitem.mbaseshare); 
	BindDouble(flag,9,&mitem.mbasecharge); 
	BindLong(flag,10,&mitem.msharecap);
	BindDouble(flag,11,&mitem.mcommcap);
	BindDouble (flag,12,&mitem.mmincomm); 
	BindText(flag,13,mitem.mnote,sizeof(mitem.mnote));
	BindTime_t(flag,14,&mitem.mm_date,&mm_date);
	BindLong(flag,15,&mitem.mbrid);
	BindLong(flag,16,&mitem.mflag);
	BindDouble(flag,17,&mitem.mvaladj);
	BindLong(flag,18,&mitem.msharecap1);
	BindDouble(flag,19,&mitem.mshareadj1); 
	BindDouble(flag,20,&mitem.msymbolfee);
	return TRUE;

}

LPCSTR srstDeal::GetColLst()
{
	return "select dealid,name,defdeal,sharelimit,shareadj,"
		"limitadj,marketadj,baseshare,basecharge,sharecap,commcap,mincomm,note,m_date,"
		"brid,flag,valueadj,sharecap1,shareadj1,symbolfee"
		" from mdeal";

}

BOOL srstDeal::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try {
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstPassthruFee::OnBind(int flag)
{
	UnBindAll(); 
	BindLong(flag,1,&mdealid);
	BindText(flag,2,mitem.mpath,sizeof(mitem.mpath));
	BindDouble(flag,3,&mitem.costpertkt);
	BindDouble(flag,4,&mitem.costperexe);
	BindDouble(flag,5,&mitem.costpershr);
	BindDouble(flag,6,&mitem.costpershrA);
	BindLong(flag,7,&mitem.mflag);
	return TRUE;


}

LPCSTR srstPassthruFee::GetColLst()
{
	return "select dealid,path,"
		"costpertkt,costperexe,costpershr,costpercxl,flag "
		" from mpassthrufee ";
}

BOOL srstPassthruFee::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try {
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr ) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstDeal::OpenToUpdate(int dealid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where dealid=%d "
		,GetColLst(),dealid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}

BOOL srstPassthruFee::OpenToUpdate(int dealid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where dealid=%d "
		,GetColLst(),dealid);
	mdealid=dealid;
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}



LPCSTR srstAddress::GetColLst()
{
	return "select Add_id,brid,phone,fax,address,"
			"city,state,zip,country,c_date,"
			"m_date,note from mAddress ";

}

BOOL srstAddress::OnBind(int flag)
{
	long dirtyflag;
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.maddid);
		BindLong(flag,2,&mitem.mbrid);
		BindText(flag,3,mitem.mphone,sizeof(mitem.mphone));
		BindText(flag,4,mitem.mfax,sizeof(mitem.mfax));
		BindText(flag,5,mitem.maddress,sizeof(mitem.maddress));
		BindText(flag,6,mitem.mcity,sizeof(mitem.mcity));
		BindText(flag,7,mitem.mstate,sizeof(mitem.mstate));
		BindText(flag,8,mitem.mzip,sizeof(mitem.mzip));
		BindText(flag,9,mitem.mcountry,sizeof(mitem.mcountry));
		BindTime_t(flag,10,&mitem.mc_date,&mc_date);
		BindTime_t(flag,11,&mitem.mm_date,&mm_date);
		BindText(flag,12,mitem.mnote,sizeof(mitem.mnote));
		break;
	case bindForUpdate:
		dirtyflag=mitem.modifyflag; 
		if (dirtyflag&1)		BindLong(flag,1,&mitem.maddid);
		if (dirtyflag& (1<<1))	BindLong(flag,2,&mitem.mbrid);
		if (dirtyflag& (1<<2))	BindText(flag,3,mitem.mphone,sizeof(mitem.mphone));
		if (dirtyflag& (1<<3))	BindText(flag,4,mitem.mfax,sizeof(mitem.mfax));
		if (dirtyflag& (1<<4))	BindText(flag,5,mitem.maddress,sizeof(mitem.maddress));
		if (dirtyflag& (1<<5))	BindText(flag,6,mitem.mcity,sizeof(mitem.mcity));
		if (dirtyflag& (1<<6))	BindText(flag,7,mitem.mstate,sizeof(mitem.mstate));
		if (dirtyflag& (1<<7))	BindText(flag,8,mitem.mzip,sizeof(mitem.mzip));
		if (dirtyflag& (1<<8))	BindText(flag,9,mitem.mcountry,sizeof(mitem.mcountry));
		if (dirtyflag& (1<<10))	BindTime_t(flag,10,&mitem.mc_date,&mc_date);
		if (dirtyflag& (1<<11))	BindTime_t(flag,11,&mitem.mm_date,&mm_date);
		if (dirtyflag& (1<<12))	BindText(flag,12,mitem.mnote,sizeof(mitem.mnote));
		break;
	}
	return TRUE;
}

BOOL srstAddress::OpenToUpdate(int addid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where add_id=%d "
		,GetColLst(),addid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}

//////////////////////////////////////////////////////////////////////
// srstBranch Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstBranch::srstBranch()
{

}

srstBranch::~srstBranch()
{

}

LPCSTR srstBranch::GetColLst()
{
	return "select "
		"brid,short,status,name,"
		"mPer_ID,Add1_id,c_date,m_date,note"
		" from mbranch";
}


BOOL srstBranch::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.mbrid);
		BindText(flag,2,mitem.mshort,sizeof(mitem.mshort));
		BindLong(flag,3,&mitem.mstatus);
		BindText(flag,4,mitem.mname,sizeof(mitem.mname));
		BindLong(flag,5,&mitem.mper_id);
		BindLong(flag,6,&mitem.madd_id);
		BindTime_t(flag,7,&mitem.mc_date,&mc_date);
		BindTime_t(flag,8,&mitem.mm_date,&mm_date);
		BindText(flag,9,mitem.mnote,sizeof(mitem.mnote));
		break;
	case bindForUpdate:
		if (mDirtyFlag&1)		BindLong(flag,1,&mitem.mbrid);
		if (mDirtyFlag& (1<<1))	BindText(flag,2,mitem.mshort,sizeof(mitem.mshort));
		if (mDirtyFlag& (1<<2))	BindText(flag,4,mitem.mname,sizeof(mitem.mname));
		if (mDirtyFlag& (1<<3))	BindLong(flag,3,&mitem.mstatus);
		if (mDirtyFlag& (1<<4))	BindLong(flag,5,&mitem.mper_id);
		if (mDirtyFlag& (1<<5))	BindLong(flag,6,&mitem.madd_id);
		if (mDirtyFlag& (1<<6))	BindTime_t(flag,7,&mitem.mc_date,&mc_date);
		if (mDirtyFlag& (1<<7))	BindTime_t(flag,8,&mitem.mm_date,&mm_date);
		if (mDirtyFlag& (1<<8)) BindText(flag,9,mitem.mnote,sizeof(mitem.mnote));
		break;
	}
	return TRUE;
}

BOOL srstBranch::OpenToUpdate(int nbrid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where brid=%d "
		,GetColLst(),nbrid);

	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstBranch::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try {
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// srstTrader Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstTrader::srstTrader()
{

}

srstTrader::~srstTrader()
{

}

LPCSTR srstTrader::GetColLst()
{
	return "select trid,tracno,pwd,brid,status,"
			"per_id,add1_id,add2_id,c_date,m_date,note,status1,status2,status3 "
			"from mtrader ";
}

BOOL srstTrader::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try {
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}

BOOL srstTrader::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.mtrid);
		BindText(flag,2,mitem.mtracno,sizeof(mitem.mtracno));
		BindText(flag,3,mitem.mPWD,sizeof(mitem.mPWD));
		BindLong(flag,4,&mitem.mbrid);
		BindLong(flag,5,&mitem.mstatus);
		BindLong(flag,6,&mitem.mPerid);
		BindLong(flag,7,&mitem.mAdd1ID);
		BindLong(flag,8,&mitem.mAdd2ID);
		BindTime_t(flag,9,&mitem.mc_date,&mc_date);
		BindTime_t(flag,10,&mitem.mm_date,&mm_date);
		BindText(flag,11,mitem.mnote,sizeof(mitem.mnote) );
		BindLong(flag,12,&mitem.mstatus1);
		BindLong(flag,13,&mitem.mstatus2);
		BindLong(flag,14,&mitem.mstatus3);
		break;
	case bindForUpdate:
		if (mDirtyFlag & (1<<0)) BindLong(flag,1,&mitem.mtrid);
		if (mDirtyFlag & (1<<1)) BindText(flag,2,mitem.mtracno,sizeof(mitem.mtracno));
		if (mDirtyFlag & (1<<2)) BindText(flag,3,mitem.mPWD,sizeof(mitem.mPWD));
		if (mDirtyFlag & (1<<3)) BindLong(flag,4,&mitem.mbrid);
		if (mDirtyFlag & (1<<4)) BindLong(flag,5,&mitem.mstatus);
		if (mDirtyFlag & (1<<5)) BindLong(flag,6,&mitem.mPerid);
		if (mDirtyFlag & (1<<6)) BindLong(flag,7,&mitem.mAdd1ID);
		if (mDirtyFlag & (1<<7)) BindLong(flag,8,&mitem.mAdd2ID);
		if (mDirtyFlag & (1<<8)) BindTime_t(flag,9,&mitem.mc_date,&mc_date);
		if (mDirtyFlag & (1<<9)) BindTime_t(flag,10,&mitem.mm_date,&mm_date);
		if (mDirtyFlag & (1<<10)) BindText(flag,11,mitem.mnote,sizeof(mitem.mnote) );
		if (mDirtyFlag & (1<<14)) {
			BindLong(flag,12,&mitem.mstatus1);
			BindLong(flag,13,&mitem.mstatus2);
			BindLong(flag,14,&mitem.mstatus3);
		}

		break;
	}
	return TRUE;

}

//////////////////////////////////////////////////////////////////////
// srstPerson Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//DEL srstPerson::srstPerson()
//DEL {
//DEL 
//DEL }

//DEL srstPerson::~srstPerson()
//DEL {
//DEL 
//DEL }

LPCSTR srstPerson::GetColLst()
{
	return "select Per_id,brid,entity_type,title,entity_name,"
			"fname,mname,lname,SSN,phone,"
			"email,c_date,m_date,note "
			"from mCustomer ";
}

BOOL srstPerson::OnBind(int flag)
{
	long dirtyflag;
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindLong(flag,1,&mitem.mperid);
		BindLong(flag,2,&mitem.mbrid);
		BindBYTE(flag,3,&mitem.mentity_type);
		BindText(flag,4,mitem.mtitle,sizeof(mitem.mtitle));
		BindText(flag,5,mitem.mentity_name ,sizeof(mitem.mentity_name));
		BindText(flag,6,mitem.mfname,sizeof(mitem.mfname));
		BindText(flag,7,mitem.mmname,sizeof(mitem.mmname ));
		BindText(flag,8,mitem.mlname,sizeof(mitem.mlname));
		BindText(flag,9,mitem.mSSN,sizeof(mitem.mSSN));
		BindText(flag,10,mitem.mphone,sizeof(mitem.mphone));
		BindText(flag,11,mitem.memail,sizeof(mitem.memail));
		BindTime_t(flag,12,&mitem.mc_date,&mc_date);
		BindTime_t(flag,13,&mitem.mm_date,&mm_date);
		BindText(flag,14,mitem.mnote,sizeof(mitem.mnote));
		break;
	case bindForUpdate:
		dirtyflag=mitem.modifyflag; 
		if (dirtyflag & 1)		BindLong(flag,1,&mitem.mperid);
		if (dirtyflag & 1<<1)	BindLong(flag,2,&mitem.mbrid);
		if (dirtyflag & 1<<2)	BindBYTE(flag,3,&mitem.mentity_type);
		if (dirtyflag & 1<<3)	BindText(flag,4,mitem.mtitle,sizeof(mitem.mtitle));
		if (dirtyflag & 1<<4)	BindText(flag,5,mitem.mentity_name ,sizeof(mitem.mentity_name));
		if (dirtyflag & 1<<5)	BindText(flag,6,mitem.mfname,sizeof(mitem.mfname));
		if (dirtyflag & 1<<6)	BindText(flag,7,mitem.mmname,sizeof(mitem.mmname ));
		if (dirtyflag & 1<<7)	BindText(flag,8,mitem.mlname,sizeof(mitem.mlname));
		if (dirtyflag & 1<<8)	BindText(flag,9,mitem.mSSN,sizeof(mitem.mSSN));
		if (dirtyflag & 1<<9)	BindText(flag,10,mitem.mphone,sizeof(mitem.mphone));
		if (dirtyflag & 1<<10)	BindText(flag,11,mitem.memail,sizeof(mitem.memail));
		if (dirtyflag & 1<<11)	BindTime_t(flag,12,&mitem.mc_date,&mc_date);
		if (dirtyflag & 1<<12)	BindTime_t(flag,13,&mitem.mm_date,&mm_date);
		if (dirtyflag & 1<<13)	BindText(flag,14,mitem.mnote,sizeof(mitem.mnote));
		break;
	}
	return TRUE;

}

//if qtype==1, by specific perid;
//if qtype==2, by specific branch,
//if qtype==3, by all traders 
BOOL srstPerson::OpenToRead(BYTE qtype, long qmodify, long qid,int *brlst)
{
	char sqlstr[1000];
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	switch (qtype){
	case 1:
		sprintf(sqlstr,"%s where Per_id=%d",GetColLst(),qid);
		break;
	case 2:
		if (brlst==NULL) sprintf(sqlstr,"%s where brid=%d",GetColLst(),qid);
		else {
			int len=sprintf(sqlstr,"%s where brid in (%d",GetColLst(),qid);
			for (int i=0;i<ADM_MAX_BRANCH;i++) {
				if (brlst[i]==0) break;
				len+=sprintf(sqlstr+len,",%d",brlst[i]);
			}
			sprintf(sqlstr+len,")");
		}
		break;
	case 3:
		sprintf(sqlstr,"%s ",GetColLst());
		break;
	default:
		return FALSE;
	}
	try {
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
	

}

BOOL srstAddress::OpenToRead(BYTE qtype, long qmodify, long qid,int *brlst)
{
	char sqlstr[1000];
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	switch (qtype){
	case 1:
		sprintf(sqlstr,"%s where Add_id=%d",GetColLst(),qid);
		break;
	case 2:
		if (brlst==NULL) sprintf(sqlstr,"%s where brid=%d",GetColLst(),qid);
		else {
			int len=sprintf(sqlstr,"%s where brid in (%d",GetColLst(),qid);
			for (int i=0;i<ADM_MAX_BRANCH;i++) {
				if (brlst[i]==0) break;
				len+=sprintf(sqlstr+len,",%d",brlst[i]);
			}
			sprintf(sqlstr+len,")");
		}
		break;
	case 3:
		sprintf(sqlstr,"%s ",GetColLst());
		break;
	default:
		return FALSE;
	}
	try {
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstPerson::OpenToUpdate(int perid)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where Per_id =%d "
		,GetColLst(),perid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

BOOL srstTicket::OnBind(int flag)
{
	UnBindAll();
	BindLong(flag,1,&mitem.mticketid);
	BindLong(flag,2,&mitem.mtrid);
	BindLong(flag,3,&mitem.maccid);
	BindLong(flag,4,&mitem.mentrid);
	BindLong(flag,5,&mitem.mtradeid);
	BindLong(flag,6,&mitem.morderid);
	BindLong(flag,7,&mitem.mbrid);
	BindText(flag,8,mitem.mroute,sizeof(mitem.mroute));
	BindText(flag,9,mitem.mbkrsym,sizeof(mitem.mbkrsym));
	BindLong(flag,10,&mitem.mstatus);
	BindBYTE(flag,11,&mitem.mexecnum);
	BindLong(flag,12,&mitem.mqty);
	BindDouble(flag,13,&mitem.mprice);
	BindText(flag,14,mitem.msecsym,sizeof(mitem.msecsym));
	BindBYTE(flag,15,&mitem.mexchange);
	BindText(flag,16,mitem.mrrno,sizeof(mitem.mrrno));
	BindDouble(flag,17,&mitem.mcommision);
	BindTime_t(flag,18,&mitem.me_date,&me_date);
	BindTime_t(flag,19,&mitem.mm_date,&mm_date);
	return TRUE;
}

BOOL srstTicket::Open()
{
	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

LPCSTR srstTicket::GetColLst()
{
	return "select TicketID,trid,accid,entrid,tradeid,"
				  "orderid,brid,route,bkrsym,status,"
				  "execnum,qty,price,secsym,exchange,"
				  "rrno,comm,e_date,m_date"
				  " from mTickets";

}

BOOL srstMessage::OnBind(int flag)
{

	BindText(flag,1,mitem.mFr,sizeof(mitem.mFr));
	BindText(flag,2,mitem.mTo,sizeof(mitem.mTo));
	BindLong(flag,3,&mitem.mid);
	BindText(flag,4,mitem.mTitle,sizeof(mitem.mTitle));  
	BindLong(flag,5,&mitem.mflag); 
	BindBYTE(flag,6,&mitem.mPrior);
	BindText(flag,7,mitem.mtxt1,sizeof(mitem.mtxt1));
	BindText(flag,8,mitem.mtxt2,sizeof(mitem.mtxt2));
	BindText(flag,9,mitem.mtxt3,sizeof(mitem.mtxt3));
	BindText(flag,10,mitem.mtxt4,sizeof(mitem.mtxt4));
	BindTime_t(flag,11,&mitem.ms_date,&ms_date); 
	return TRUE;
}

LPCSTR srstMessage::GetColLst()
{
	return "Select mFr,mTo,mid,title,flag,"
		"prior,txt1,txt2,txt3,txt4,"
			"s_date from mMsg"; 
}

BOOL srstMessage::OpenToAdd()
{
	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_DYNAMIC);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}

BOOL srstMessage::AddToDb(itemMessage *pitem)
{
	BOOL ret;
	if (bopenforadd==FALSE) {
		ret=OpenToAdd();
		if (!ret) return FALSE;
		bopenforadd=TRUE;
	}
	try{
		memcpy(&mitem,pitem,sizeof(itemMessage));
		AddNew(); 
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;
}

srstMessage::srstMessage()
{
	bopenforadd=FALSE;
}

BOOL srstMessage::OpenToRead(int id, time_t tafter)
{
	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_DYNAMIC);
	char sqlstr[1000];
	char tmstr[100];
	tm *t1,t;
	t1=localtime(&tafter);
	if (t1!=NULL){
		t=*t1;
	}else {
		return FALSE;
	}
	sprintf(tmstr,"%d/%d/%d %d:%d:%d",
		t.tm_mon+1,t.tm_mday,t.tm_year+1900,  
		t.tm_hour,t.tm_min,t.tm_sec);
	sprintf(sqlstr,"%s where mid=%d and datediff(second,'%s',s_date)>0",
		GetColLst(),id,tmstr);

	try{
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC read messages %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
	


}

int srstTrader::OpenToUpdate(int trid)
{
	char sqlstr[1000];

	sprintf(sqlstr,"%s where trid=%d",GetColLst(),trid);
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try {
		ExecuteSql(sqlstr);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

LPCSTR srstOrderActive::GetColLst()
{
	return "select trid,accid,orderid,flag,secsym,route,"
		"price,qty,logtime,note"
		" from mLOA ";
}

BOOL srstOrderActive::OnBind(int flag)
{
	UnBindAll(); 
	BindLong(flag,1,&mitem.mtrid);
	BindLong(flag,2,&mitem.maccid);
	BindLong(flag,3,&mitem.morderid);
	BindLong(flag,4,&mitem.mflag);
	BindText(flag,5,mitem.msecsym,sizeof(mitem.msecsym));
	BindText(flag,6,mitem.mroute,sizeof(mitem.mroute));
	BindDouble(flag,7,&mitem.mprice);
	BindLong(flag,8,&mitem.mqty);
	BindTime_t(flag,9,&mitem.mlogtime,&mlog_date);
	BindText(flag,10,mitem.mnote,sizeof(mitem.mnote));
	return TRUE;
}

BOOL srstOrderActive::OpenToAdd()
{
	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_DYNAMIC);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}

LPCSTR srstOptsym::GetColLst()
{
	return "select optsymb,expday,strikepice,undsymb,"
			"pmundlyshr,m_date "
			" from mOptSym";
}

BOOL srstOptsym::OnBind(int flag)
{
	UnBindAll(); 
	switch (flag) {
	case bindForRead:
	case bindForNew:
	case bindFixUp:
		BindText(flag,1,mitem.optsymb,sizeof(mitem.optsymb));
		BindTime_t(flag,2,&mitem.expday,&mexp_date);
		BindLong(flag,3,&mitem.strikepice);
		BindText(flag,4,mitem.undsymb,sizeof(mitem.undsymb));
		BindLong(flag,5,&mitem.pmundlyshr);
		BindTime_t(flag,6,&mitem.m_date,&m_date);
		break;
	case bindForUpdate:
		BindText(flag,1,mitem.optsymb,sizeof(mitem.optsymb));
		BindTime_t(flag,2,&mitem.expday,&mexp_date);
		BindLong(flag,3,&mitem.strikepice);
		BindText(flag,4,mitem.undsymb,sizeof(mitem.undsymb));
		BindLong(flag,5,&mitem.pmundlyshr);
		BindTime_t(flag,6,&mitem.m_date,&m_date);
		break;
	}
	return TRUE;

}

BOOL srstOptsym::OpenToUpdate(char *optsymb)
{
	char sqlstr[500];
	sprintf(sqlstr,"%s where optsymb='%s' "
		,GetColLst(),optsymb);
	try{
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
		SetConCurrency(SQL_CONCUR_VALUES);
		SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
		ExecuteSql(sqlstr);
		MoveNext();
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;

}

BOOL srstOptsym::OpenToRead()
{
	if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_READ_ONLY);
	SetCursorType (SQL_CURSOR_FORWARD_ONLY);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;
}
