// Account.cpp: implementation of the Account class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderServ.h"
#include "Account.h"
//#include ".\recordset\rstbranch.h"
#include "defines\PkgIDDef.h"
#include "OrderDB.h"
#include "MGMT\itemsSRst.h"	// Added by ClassView
#include "Functions\strfuncs.h"
#include "Functions\Perf.h"
#include "Functions\encrypt.h"
 
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FixLenMem AC_TR_relation::mrelationbuf(sizeof(AC_TR_relation),2044);
//CMapPtrToPtr Account::mapid2account; // the map from id to account
//CMapPtrToPtr BranchItem::mapbrlst; //the map for the barnch;  
intmap Account::sMapAccid;


extern COrderServApp theApp;
extern OrderDB theOrderDB;

intmap Trader::sMapTrid; // the map from id to account
intmap Trader::mOnlineTrader;
intmap Trader::mOnlineAdmin;


#include "OrderConfig.h"
extern OrderConfig theCfgFile; 

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Account::Account()
{
/*	maccid=0;
	mbrid=0;
	mtype=0;
	mstatus=0;
	mrrno=0;
	mper_id=0;
	madd1_id=0;
	madd2_id=0;
	mc_date=0;
	mm_date=0;
*/
	mitemifo.Clear();
	mitemEquity.Clear();
	memset(&mitemCtrl,0,sizeof(mitemCtrl));
	mdirtyflag=0;		// mdified but not save to database
	pFistAcc=NULL; //the first item in the account-trader relation ship
	mServerID=0;		//which server this account is in.
	mLockNum=0;	// clear the lock. 
	memset(&mitemCtrl,0,sizeof(mitemCtrl)); 
	mpitemCtrl_Symb=NULL;
	mpitemCtrl_Route=NULL;
}

Account::~Account()
{
	while(mpitemCtrl_Symb) {
		itemAccCtrl_Symb *p1;
		p1=mpitemCtrl_Symb;
		mpitemCtrl_Symb=mpitemCtrl_Symb->pnext;
		delete p1;
	}
	while (mpitemCtrl_Route) {
		itemAccCtrl_Route *p1;
		p1=mpitemCtrl_Route;
		mpitemCtrl_Route=mpitemCtrl_Route->pnext;
		delete p1;
	}
}

//////////////////////////////////////////////////////////////////////
// BranchItem Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
BranchItem::BranchItem()
{
/*	maddid=0;
	mbrid=-1;
	mbrname[0]=0;
	mc_date =0;
	mm_date=0;
	mflag=0;
	mperid=0;
	mshort[0]=0;
	mstatus =0;
	note[0]=0;
*
}

BranchItem::~BranchItem()
{
	POSITION pos;
	pos=mapbrlst.GetStartPosition();
	int brid;
	BranchItem * pbritem;
	while(pos!=NULL) {
		mapbrlst.GetNextAssoc(pos,(void *&)brid,(void*&)pbritem);
		delete pbritem;
	}
}
*/
//DEL BranchItem * BranchItem::GetBranch(long brid)
//DEL {
//DEL 	BranchItem *pbr;
//DEL 	BOOL ret;
//DEL 	ret=mapbrlst.Lookup((void*)brid,(void*&)pbr);
//DEL 	if (ret==TRUE) return pbr;
//DEL 	else return NULL;
//DEL }

//////////////////////////////////////////////////////////////////////
// Trader Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Trader::Trader()
{
/*	mtrid=0;
	memset(mtracno,0,sizeof(mtracno));
	memset(mPWD,0,sizeof(mtracno));
	mbrid=0;
	mstatus=0;
	mPerid=0;
	mAdd1ID=0;
	mAdd2ID=0;
	mc_date=0;
	mm_date=0;
*/
	mpClient=NULL;
	minfo.Clear(); 
	pFistTrader=0; //the first item in the account-trader relation ship
	mServerId=0;			//which server this trader is in. this is for multipule server.

	for (int i=0;i<ADM_MAX_BRANCH;i++) {
		mBranchList[i]=0; // the extra branch this user need to manager
		mBranchPerm[i]=0; // the permmission for this user manage this branch;
	}

}

Trader::~Trader()
{

}

// this function is static, is used to load all the information from the database.
int Account::init(int accountno)
{
	BOOL ret;
	sMapAccid.init(accountno); 
	
	srstAccIfo rstaccifo;
	ret=rstaccifo.Open();
	ASSERT(ret==TRUE);
	Account *pacc;
	while(!rstaccifo.IsEOF()) {
		if (!rstaccifo.mitem.IsDeleted()){
			pacc=new Account;
			pacc->mitemifo.CopyItem (&rstaccifo.mitem);
			pacc->mitemEquity.maccid=pacc->mitemifo.maccid;
			sMapAccid.AddNoDup(pacc->mitemifo.maccid,(long)pacc);
		}
		rstaccifo.MoveNext(); 
	}
	rstaccifo.Close();

	srstAccEquity rstAccEquiy;
	ret=rstAccEquiy.Open(FALSE);
	ASSERT(ret==TRUE);
	while(!rstAccEquiy.IsEOF()) {
		pacc=FindAcc(rstAccEquiy.mitem.maccid);
		if (pacc!=NULL) {
			pacc->mitemEquity.CopyFrom(&rstAccEquiy.mitem);
			//memcpy(&pacc->mitemEquity,&rstAccEquiy.mitem,sizeof(itemAccEquity));
		};
		rstAccEquiy.MoveNext();
	}
	rstAccEquiy.Close();


//load extra account control;	
	srstAccCtrl rstAccCtrl;
	ret=rstAccCtrl.Open();
	ASSERT(ret==TRUE);
	while(!rstAccCtrl.IsEOF()) {
		pacc=FindAcc(rstAccCtrl.mitem.maccid);
		if (pacc!=NULL) {
			pacc->mitemCtrl.CopyFrom(&rstAccCtrl.mitem);
			//memcpy(&pacc->mitemEquity,&rstAccEquiy.mitem,sizeof(itemAccEquity));
		};
		rstAccCtrl.MoveNext();
	}
	rstAccCtrl.Close();
	
//load symbol control;
	CRecordset rec;
	rec.m_pDatabase=&theOrderDB.mdb;
	rec.Open(CRecordset::forwardOnly,"select accid,symbol,maxpos,maxloss,maxorder from mAccCtrlEx_Symb" ,CRecordset::readOnly);
	while (!rec.IsEOF()){
		CDBVariant v1;
		int accid;
		v1.m_lVal=0;
		rec.GetFieldValue("accid",v1,SQL_C_SLONG);
		accid=v1.m_lVal;
		pacc=FindAcc(accid);
		if (pacc!=NULL) {
			itemAccCtrl_Symb *psym;
			psym=new itemAccCtrl_Symb;
			CString stsecsym;
			rec.GetFieldValue("symbol",stsecsym);
			strncpy(psym->msymbol,stsecsym,sizeof(psym->msymbol)-1);
			v1.m_lVal=0;
			rec.GetFieldValue("maxpos",v1);
			psym->maxpos=v1.m_lVal;
			v1.m_dblVal=0;
			rec.GetFieldValue("maxloss",v1);
			psym->maxloss=v1.m_dblVal; 
			v1.m_lVal=0;
			rec.GetFieldValue("maxorder",v1);
			psym->maxloss=v1.m_dblVal; 
			psym->maxorder=v1.m_lVal;
			psym->pnext=pacc->mpitemCtrl_Symb;
			pacc->mpitemCtrl_Symb=psym;
		};
		rec.MoveNext(); 
	}
	rec.Close();
//---load route control;
	rec.m_pDatabase=&theOrderDB.mdb;
	rec.Open(CRecordset::forwardOnly,"select accid,mroute,mflag from mAccCtrlEx_Route" ,CRecordset::readOnly);
	while (!rec.IsEOF()){
		CDBVariant v1;
		int accid;
		v1.m_lVal=0;
		rec.GetFieldValue("accid",v1,SQL_C_SLONG);
		accid=v1.m_lVal;
		pacc=FindAcc(accid);
		if (pacc!=NULL) {
			itemAccCtrl_Route *proute;
			proute=new itemAccCtrl_Route;
			CString stroute;
			rec.GetFieldValue("mroute",stroute);
			strncpy(proute->mroute ,stroute,sizeof(proute->mroute)-1);
			v1.m_lVal=0;
			rec.GetFieldValue("mflag",v1);
			proute->mflag=v1.m_lVal;
			proute->pnext=pacc->mpitemCtrl_Route;
			pacc->mpitemCtrl_Route=proute;
		};
		rec.MoveNext(); 
	}
	rec.Close();


	//init position:
	itemPosition::InitBuff();
	srstPosition rstposition;
	rstposition.mitem.Clear();

	rstposition.OpenToRead();
	itemPosition *pitem;
	while(rstposition.IsEOF()==FALSE) {
		pitem=itemPosition::GetVoidBuff();
		pitem->CopyFrom(&rstposition.mitem);
		pitem->SetToInit();
		pacc=Account::FindAcc(pitem->maccid);
		if (pacc==NULL){
			theCfgFile.mlog.AddLogStr("No Account for this position!");
			itemPosition::PutVoidBuff(pitem);
		}else {
			if (theCfgFile.mbDVPKeepPosition){
				pacc->mLstPositions.AddTail(0,(long)pitem); 
			}else if (pacc->mitemifo.mstatus&(1<<19)) {//DVP account discard the position every morning.
				itemPosition::PutVoidBuff(pitem);
			}else {
				pacc->mLstPositions.AddTail(0,(long)pitem); 
			}
		}
		rstposition.MoveNext(); 
	}
	rstposition.Close();
	

//init the Trades;
	itemTrade::InitBuff();
	itemTicket::InitBuff();
	srstTrade rstTrade;
	try{
		rstTrade.OpenToRead();
		while(rstTrade.IsEOF()==FALSE) {
			theCfgFile.SetMaxTradeID(rstTrade.mitem.mtradeid); 
/*			if ((BYTE)(DWORD(rstTrade.mitem.mtradeid)>>24)==theCfgFile.mOrderServerID) {
				if (theCfgFile.mCurrTradeID<(DWORD)rstTrade.mitem.mtradeid) {
					theCfgFile.mCurrTradeID=(DWORD)rstTrade.mitem.mtradeid;
				}
			}
*/
			Account *pacc;
			pacc=Account::FindAcc(rstTrade.mitem.maccid);
			if (pacc==NULL) {
				theCfgFile.mlog.AddLogStr("ERROR:34876 the Trade can't find account");
			}else {
				itemTrade *pitem=itemTrade::GetVoidBuff();
				ASSERT(pitem!=NULL);
				pitem->CopyFrom(&rstTrade.mitem);
				pacc->mLstTrades.AddAscent(pitem->me_date,(long)pitem);
				itemTrade::sMapTradeID.Add(pitem->mtradeid,(long)pitem); 
			}
			rstTrade.MoveNext(); 
		}
		rstTrade.Close();
	} catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
	}


//init the orders
	itemOrder::InitBuff();
	itemManualOrder::smapOrderid.init(100);
	itemManualOrder::sOrderBuffer.init(sizeof(itemManualOrder),2020);  

	srstOrders rstOrders;
	try{
		rstOrders.OpenToRead();
		while(rstOrders.IsEOF()==FALSE) {
			theCfgFile.SetMaxOrderID( rstOrders.mitem.morderid);
/*
			if ((BYTE)(DWORD(rstOrders.mitem.morderid)>>24)==theCfgFile.mOrderServerID) {
				if (theCfgFile.mCurrOrderID<(DWORD)rstOrders.mitem.morderid) {
					theCfgFile.mCurrOrderID=(DWORD)rstOrders.mitem.morderid;
				}
			}
*/
			AddNewOrder(&rstOrders.mitem);
			rstOrders.MoveNext(); 
		}
		rstOrders.Close();
	} catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
	}
//init the optsymbol 
	itemOptsym::InitBuff();
	srstOptsym rstoptsym;
	try{
		rstoptsym.OpenToRead();
		while(rstoptsym.IsEOF()==FALSE) {
			itemOptsym::FindAddInfo(&rstoptsym.mitem,TRUE,FALSE);	
			rstoptsym.MoveNext();
		}
		rstoptsym.Close();
		
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
	}

	return 0;
}

/*
BOOL BranchItem::Init(int brnumber)
{
	mapbrlst.InitHashTable(brnumber);
	rstbranch rst(&theOrderDB.mdb) ;
	rst.Open();
	BranchItem *pbr;
	while (!rst.IsEOF()){
		pbr=new BranchItem;
		pbr->mData.CopyItem(&rst.mData); 
/*		pbr->mbrid=rst.m_brid;
		pbr->mflag=rst.m_flag;
		strcpy(pbr->mshort,rst.m_short);
		strcpy(pbr->mbrname,rst.m_name);
		pbr->mstatus =rst.m_status;
		pbr->mperid=rst.m_mPer_ID;
		pbr->maddid= rst.m_Add1_id;
		pbr->mc_date =rst.m_c_date.GetTime();
		pbr->mm_date =rst.m_m_date.GetTime();
		strcpy(pbr->note,rst.m_note);
*
		
		mapbrlst.SetAt((void*)pbr->mData.mbrid,(void*)pbr);
		rst.MoveNext(); 
	}
	rst.Close();

	return TRUE;
}
*/
//DEL BOOL BranchItem::ModifyBranchItem(char *pbuff, char *retPkg,char * infoPkg)
//DEL {
//DEL //	int i;
//DEL //	BOOL Loopflag=TRUE;
//DEL 	BranchItem * pbitem=NULL;
//DEL 	PkgToken tok;
//DEL //	PkgToken rettok;
//DEL //	CString tmpstr;
//DEL 	int modifynu=0;
//DEL //int brid;
//DEL 	tok.InitToGet(pbuff,5); 
//DEL //	rettok.InitToSet(retPkg);
//DEL //	rettok.setWORD(1004);
//DEL //	BYTE fieldtype;
//DEL 	itemBranch itembr;
//DEL 	itemPerson itemper;
//DEL 	itemAddress itemadd;
//DEL 	itemper.modifyflag=itemadd.modifyflag =itembr.modifyflag=0;
//DEL 	itembr.GetPkgInfo(&tok,&itemper,&itemadd);
//DEL 	if (itemadd.modifyflag !=0) {
//DEL 		rstaddress rst;
//DEL 		rst.UpdateData(&itemadd);
//DEL 		if (itembr.madd_id !=itemadd.maddid){
//DEL 			itembr.madd_id =itemadd.maddid;
//DEL 			itembr.modifyflag|=1<<6;
//DEL 		}
//DEL 	}
//DEL 	if (itemper.modifyflag !=0) {
//DEL 		rstperson rst;
//DEL 		rst.UpdateData(&itemper);
//DEL 		if (itembr.mper_id!=itemper.mperid){
//DEL 			itembr.mper_id=itemper.mperid;
//DEL 			itembr.modifyflag|=1<<5;
//DEL 		}
//DEL 	}
//DEL 	rstbranch rbranch;//(&theOrderDB.mdb);
//DEL 	rbranch.m_pDatabase= &theOrderDB.mdb;
//DEL 	rbranch.UpdateData(&itembr);
//DEL 	pbitem=GetBranch(itembr.mbrid);
//DEL 	if (pbitem!=NULL) {
//DEL 		pbitem->mData.CopyItem(&itembr);
//DEL 		pbitem->PrepareInfoPkg(retPkg);
//DEL 		tok.setmsgpkg(infoPkg,DP_TEXT,DT_BR_MODIFY,0,"branch modified!");
//DEL 	}else {
//DEL 		BranchItem * pbitem=new BranchItem;
//DEL 		pbitem->mData.CopyItem(&itembr); 
//DEL 		pbitem->PrepareInfoPkg(retPkg);
//DEL 		mapbrlst.SetAt((void*)itembr.mbrid,(void*)pbitem);
//DEL 		tok.setmsgpkg(infoPkg,DP_TEXT,DT_BR_NEW,0,"add branch successed!");
//DEL 	}
//DEL 	return TRUE;
//DEL }




//DEL BOOL BranchItem::PrepareInfoPkg(char *pbuff)
//DEL {
//DEL 	PkgToken tokret;
//DEL 	tokret.InitToSet(pbuff);
//DEL 	tokret.setWORD(1004);
//DEL 	mData.SetPkgInfo(&tokret);
//DEL 	tokret.setBYTE(0);
//DEL 	tokret.finalizeSet();
//DEL 	return TRUE;
//DEL }

Account * Account::FindAcc(int naccid)
{
/*	Account *pacc;
	if (mapid2account.Lookup((void*)naccid,(void *&)pacc)==TRUE) {
		return pacc;
	} else {
		return NULL;
	}
	*/
	intitem *pintitem;
	pintitem=sMapAccid.FindItem(naccid);
	if (pintitem==NULL) return NULL;
	return (Account*)(pintitem->val);
}

BOOL Trader::Init(int tradernumber)
{
	sMapTrid.init(tradernumber);
	mOnlineAdmin.init(100);
	mOnlineTrader.init(tradernumber); 
	srstTrader srst;
	srst.OpenToRead();
	Trader *ptr;
	while(!srst.IsEOF()) {
		if (!srst.mitem.IsDeleted()){ 
			ptr=new Trader;
			ptr->minfo.CopyItem(&srst.mitem);
/* for debug
			if (strcmp(ptr->minfo.mtracno,"T3")==0) {
				theCfgFile.mLoginLog.AddLogStrFmt("t3 epwd:>%s<",ptr->minfo.mPWD); 
				encrypt  en;
				char dpw[100];
				en.DecryptPWD((unsigned char*)ptr->minfo.mPWD,(unsigned char*)dpw);
				theCfgFile.mLoginLog.AddLogStrFmt("t3 dpwd:>%s<",dpw); 
			}
*/
 
			sMapTrid.Add(ptr->minfo.mtrid,(long)ptr);
		}
		srst.MoveNext(); 
	
	}
	srst.Close(); 
	
	CRecordset rec;
	rec.m_pDatabase=&theOrderDB.mdb;
	rec.Open(CRecordset::forwardOnly,"select * from mAdmBranchList",CRecordset::readOnly);
	while(!rec.IsEOF()) {
		int trid,brid,status;
		CDBVariant v1;
		v1.m_lVal=0;
		rec.GetFieldValue("trid",v1,SQL_C_SLONG);
		trid=v1.m_lVal;
		v1.m_lVal=0;
		rec.GetFieldValue("brid",v1,SQL_C_SLONG);
		brid=v1.m_lVal;
		v1.m_lVal=0;
		rec.GetFieldValue("stat1",v1,SQL_C_SLONG);
		status=v1.m_lVal;
		ptr=Trader::FindTrader(trid);
		if (ptr) {
			int i;
			for (i=0;i<ADM_MAX_BRANCH;i++) {
				if ((ptr->mBranchList[i]==0)||(ptr->mBranchList[i]==brid)) {
					ptr->mBranchList[i]=brid;
					ptr->mBranchPerm[i]=status;
					break;
				}
			}
		}
		rec.MoveNext();			
	}
	rec.Close();



	/*
	rstTrader rst;
	rst.m_pDatabase =&theOrderDB.mdb;
	rst.Open();
	Trader *ptr;
	while(!rst.IsEOF()){
		ptr=new Trader;
		ptr->mtrid=rst.m_trid;
		if (!rst.m_tracno.IsEmpty()) memccpy(ptr->mtracno,rst.m_tracno,0,sizeof(ptr->mtracno));   
		if (!rst.m_PWD.IsEmpty()) memccpy(ptr->mPWD,rst.m_PWD,0,sizeof(ptr->mPWD));  
		ptr->mstatus =rst.m_status;
		ptr->mpClient=NULL;
		ptr->pFistTrader=NULL;
		ptr->mbrid =rst.m_brid;
		ptr->mAdd1ID=rst.m_add1_id;
		ptr->mAdd2ID=rst.m_add2_id;
		ptr->mPerid=rst.m_per_id;
		ptr->mc_date=rst.m_c_date.GetTime();
		ptr->mm_date=rst.m_m_date.GetTime();
		ptr->mnote =rst.m_note; 
		sMapTrid.Add(ptr->mtrid,(long)ptr);
		rst.MoveNext(); 
	}
	rst.Close();
	*/
	return TRUE;
}

Trader * Trader::FindTrader(int ntrid)
{
	intitem *pintitem;
	pintitem=sMapTrid.FindItem(ntrid);
	if (pintitem==NULL) return NULL;
	else return (Trader*)pintitem->val; 
}

Trader * Trader::FindTrader(LPCSTR ptracno)
{
	Trader *ptrader;
	intitem *pintitem;
	Trader *pret;
	pret=NULL;
	sMapTrid.LockMap();
	sMapTrid.EnumFirst(&pintitem); 
	while (pintitem!=NULL) {
		ptrader=(Trader*)pintitem->val;
		ASSERT(ptrader!=NULL);
		if (!ptrader->minfo.IsDeleted()) { 
			if(strcmp(ptrader->minfo.mtracno,ptracno)==0){
				pret=ptrader;
				break;
			}
		}
		sMapTrid.EnumNext(&pintitem);
	}
	sMapTrid.UnlockMap();
	return pret;
}

//Query Trader;
//DEL BOOL Trader::PrepareInfoPkg(char *pbuff)
//DEL {
//DEL 	PkgToken tokret;
//DEL 	tokret.InitToSet(pbuff);
//DEL 	tokret.setWORD(1000);
//DEL 	tokret.setBYTE(1);//Trid
//DEL 	tokret.setlong(mtrid);
//DEL 	tokret.setBYTE(2);//trader account no, login name
//DEL 	tokret.setVarStr(mtracno); 
//DEL 	tokret.setBYTE(4);//PWD
//DEL 	tokret.setVarStr(mPWD);
//DEL 	tokret.setBYTE(5);//Brid
//DEL 	tokret.setDWORD(mbrid );
//DEL 	tokret.setBYTE(7);//status
//DEL 	tokret.setDWORD(mstatus);
//DEL 	tokret.setBYTE(8);//person id
//DEL 	tokret.setDWORD(mPerid);
//DEL 	tokret.setBYTE(9);//Add1_id;
//DEL 	tokret.setDWORD(mAdd1ID);
//DEL 	tokret.setBYTE(10);//Add2_id;
//DEL 	tokret.setDWORD(mAdd2ID);
//DEL 	tokret.setBYTE(11);//Create date;
//DEL 	tokret.setlong(mc_date);
//DEL 	tokret.setBYTE(12);//modify date;
//DEL 	tokret.setlong(mm_date);
//DEL 
//DEL 	tokret.setBYTE(13);//note
//DEL 	tokret.setVarStr(mnote);
//DEL 	tokret.setBYTE(0);
//DEL 	tokret.finalizeSet();
//DEL 	return TRUE;
//DEL 
//DEL 
//DEL }


//DEL BOOL AC_TR_relation::GetPkg(PkgToken *ptok)
//DEL {
//DEL 	mtrid=ptok->getlong();
//DEL 	maccid=ptok->getlong();
//DEL 	mstatus=ptok->getlong();
//DEL 	mdealid=ptok->getlong();
//DEL 	mm_date=ptok->getlong();
//DEL 	return TRUE;
//DEL }

//DEL BOOL AC_TR_relation::MakeInfoPkg(char *buff, BYTE flag)
//DEL {
//DEL 	PkgToken tokret;
//DEL 	tokret.InitToSet(buff);
//DEL 	tokret.setWORD(1003);
//DEL 	tokret.setBYTE(flag);
//DEL 	tokret.setlong(mtrid);
//DEL 	tokret.setlong(maccid);
//DEL 	tokret.setlong(mstatus);
//DEL 	tokret.setlong(mdealid);
//DEL 	tokret.setlong(mm_date);
//DEL 	tokret.setBYTE(0);
//DEL 	tokret.finalizeSet();
//DEL 	return TRUE;
//DEL 	
//DEL }

BOOL AC_TR_relation::ModifyDB()
{
	if ((mtrid==0)||(maccid==0)) return FALSE;
	srstAcct_Access rst;
	rst.Open(FALSE,mtrid,maccid);
	rst.mitem.CopyItem(this);
	BOOL ret;
	if (rst.IsEOF()) {
		ret=rst.AddNew(); 
	}else {
		ret=rst.Update();
	}
	rst.Close();
	mm_date =rst.mitem.mm_date;
	return ret;

}

void AC_TR_relation::CopyItem(AC_TR_relation *pitem)
{
	mtrid=pitem->mtrid;
	maccid=pitem->maccid;
	mstatus=pitem->mstatus;
	mdealid=pitem->mdealid;
	mOpdealid=pitem->mOpdealid; 
	mFtdealid=pitem->mFtdealid;
	mm_date =pitem->mm_date; 
}

AC_TR_relation * AC_TR_relation::Finditem(long trid, long acid)
{
	Trader *ptr;
	AC_TR_relation *ptmp;
	ptr=Trader::FindTrader(trid);
	if (ptr==NULL) return NULL;
	ptmp=ptr->pFistTrader;
	while(ptmp!=NULL) {
		if (ptmp->maccid==acid){
			return ptmp;
		}
		ptmp=ptmp->pnexttr;
	};
	return NULL;
}

AC_TR_relation * AC_TR_relation::AddItem(AC_TR_relation *pitem)
{
	Trader *ptr;
	Account *pacc;
	AC_TR_relation *ptmp;
	ptr=Trader::FindTrader(pitem->mtrid);
	pacc=Account::FindAcc(pitem->maccid);
	if ((ptr==NULL)||(pacc==NULL))return NULL;

	ptmp=(AC_TR_relation *)mrelationbuf.GetVoidBuff();

	ASSERT(ptmp!=NULL);
	ptmp->CopyItem(pitem);
	ptmp->pnexttr=ptr->pFistTrader;
	ptr->pFistTrader=ptmp;
	ptmp->pnextac=pacc->pFistAcc;
	pacc->pFistAcc=ptmp;
	return ptmp;
}


BOOL AC_TR_relation::DeleteItem(int trid, int accid)
{
	Trader *ptr;
	Account *pacc;
	AC_TR_relation *pitemtr,*pitemtr1;
	AC_TR_relation *pitemac,*pitemac1;

	ptr=Trader::FindTrader(trid);
	pacc=Account::FindAcc(accid);
	if ((ptr==NULL)||(pacc==NULL))return NULL;
	pitemtr=pitemtr1=ptr->pFistTrader;
	while(pitemtr!=NULL) {
		if (pitemtr->maccid==accid) break;
		pitemtr1=pitemtr;
		pitemtr=pitemtr->pnexttr;

	}

	pitemac1=pitemac =pacc->pFistAcc;
	while(pitemac!=NULL) {
		if (pitemac->mtrid ==trid) break;
		pitemac1=pitemac;
		pitemac=pitemac->pnextac;
	}

	ASSERT(pitemac==pitemtr);
	
	if (pitemac==NULL) return FALSE;
	if (pitemtr==ptr->pFistTrader) {
		ptr->pFistTrader =pitemtr->pnexttr;
	}else {
		pitemtr1->pnexttr=pitemtr->pnexttr;
	}

	if (pitemac==pacc->pFistAcc) {
		pacc->pFistAcc=pitemtr->pnextac; 
	}else {
		pitemac1->pnextac=pitemtr->pnextac;
	}
	
	mrelationbuf.PutVoidBuff((char *)pitemtr); 
	return TRUE;
}

BOOL AC_TR_relation::DeleteDB()
{
	if ((mtrid==0)||(maccid==0)) return FALSE;
	srstAcct_Access rst;
	rst.Open(FALSE,mtrid,maccid);
	BOOL ret;
	try {
		if (rst.IsEOF()) {
			ret=FALSE; 
		}else {
			ret=rst.Delete();
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	rst.Close();
	return ret;
}

BOOL AC_TR_relation::Init()
{
	srstAcct_Access rst;
	rst.Open(TRUE);
	while(!rst.IsEOF()) {
		AC_TR_relation::AddItem(&rst.mitem);
		rst.MoveNext(); 
	}
	rst.Close();
	return TRUE;
}



//DEL BOOL Trader::SendPositionChange(itemPosition *pitem, int changmask)
//DEL {
//DEL 
//DEL 	//----make package;
//DEL 	char pkg[100];
//DEL 	BOOL ret;
//DEL 	ret=pitem->MakeInfoPkg(pkg,changmask);
//DEL 	ASSERT(ret !=NULL);
//DEL 	//----find to send;
//DEL 	Account *pacc;
//DEL 	pacc=Account::FindAcc(pitem->maccid);
//DEL 	ASSERT(pacc!=NULL);
//DEL 	AC_TR_relation *prelation;
//DEL 	prelation=pacc->pFistAcc;
//DEL 	CClient *pclient;
//DEL 	intitem *pclientitem;
//DEL 	while (prelation!=NULL) {
//DEL 		pclientitem=mOnlineTrader.FindItem(prelation->mtrid);
//DEL 		if (pclientitem!=NULL) {
//DEL 			pclient=(CClient*)pclientitem->val;
//DEL 			pclient->SendPkg(pkg);
//DEL 		}
//DEL 		prelation=prelation->pnextac;
//DEL 	}
//DEL 
//DEL //find all administrator to send the position out.
//DEL 	mOnlineAdmin.LockMap();
//DEL 	pclientitem=mOnlineAdmin.EnumFirst();
//DEL 	
//DEL 	while(pclientitem!=NULL) {
//DEL 		pclient=(CClient*)pclientitem->val;
//DEL 		if ((pclient->mptrifo->mbrid==pitem->mbrid) || (pclient->mptrifo->IsAdmin())) {
//DEL 
//DEL //			if(pclient->mWatchFlag & 1<<4) { // dynamic watch the position change 
//DEL 				pclient->SendPkg(pkg);
//DEL //			}
//DEL 		}
//DEL 		pclientitem=mOnlineAdmin.EnumNext(pclientitem); 
//DEL 	}
//DEL 	mOnlineAdmin.UnlockMap();
//DEL 	return TRUE;
//DEL }

/* 
   This function send a package to online trader 
   which this account belong this trader or
   admin or branch manager which has this watch flag.
*/
BOOL Trader::SPKGAcc(char *pkg, long accid, long brid, int watchflag)
{
	Account *pacc;
	//RISK? the relation changed when this happens?
	pacc=Account::FindAcc(accid);
	ASSERT(pacc!=NULL);
	AC_TR_relation *prelation;
	prelation=pacc->pFistAcc;
	CClient *pclient;
	intitem *pclientitem;
	intlst senlst;
	
	mOnlineTrader.LockMap();

	while (prelation!=NULL) {
		pclientitem=mOnlineTrader.FindItem(prelation->mtrid);
		if (pclientitem!=NULL) {
			pclient=(CClient*)pclientitem->val;
			senlst.AddTail(0,pclientitem->val);
			pclient->RefInc();
//			pclient->SendPkg(pkg);
//			pclient->AsyncSelect(FD_WRITE); 
		}
		prelation=prelation->pnextac;
	}

	mOnlineTrader.UnlockMap();


//find all administrator to send the position out.
	mOnlineAdmin.LockMap();
	pclientitem=mOnlineAdmin.EnumFirst();
	
	while(pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		if (pclient->IsWatching(brid,watchflag)) {
//			if(pclient->mWatchFlag & 1<<4) { // dynamic watch the position change 
				senlst.AddTail(0,pclientitem->val);
				pclient->RefInc();
				//pclient->SendPkg(pkg);
				//pclient->AsyncSelect(FD_WRITE);
//			}
		}
		pclientitem=mOnlineAdmin.EnumNext(pclientitem); 
	}
	mOnlineAdmin.UnlockMap();

	int nkey,nval;
	while (senlst.GetHead(&nkey,&nval)) {
		CClient *pclient=(CClient *)nval;
		pclient->SendPkg(pkg);
		pclient->AsyncSelect(FD_WRITE);
		pclient->RefDec();
	}


	return TRUE;

}

BOOL Trader::SPKGTrader(char *pkg, long trid,long brid,long watchflag)
{
	BOOL msended;
	msended=FALSE;
	CClient *pclient;
	intitem *pclientitem;
	intlst senlst;

	mOnlineTrader.LockMap();
	pclientitem=mOnlineTrader.FindItem(trid);
	if (pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		senlst.AddTail(0,pclientitem->val);
		pclient->RefInc();

//		pclient->SendPkg(pkg);
//		pclient->AsyncSelect(FD_WRITE); 
		msended=TRUE;
	}
	mOnlineTrader.UnlockMap(); 
//	if (msended==TRUE) return TRUE;
	
	mOnlineAdmin.LockMap();
	pclientitem=mOnlineAdmin.EnumFirst();
	while (pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		if (pclient->IsWatching(brid,watchflag)) {
			senlst.AddTail(0,pclientitem->val);
			pclient->RefInc();
//			pclient->SendPkg(pkg);
//			pclient->AsyncSelect(FD_WRITE); 
			msended=TRUE;
		}
		mOnlineAdmin.EnumNext(&pclientitem); 
	}
	mOnlineAdmin.UnlockMap(); 

	int nkey,nval;
	while (senlst.GetHead(&nkey,&nval)) {
		CClient *pclient=(CClient *)nval;
		pclient->SendPkg(pkg);
		pclient->AsyncSelect(FD_WRITE);
		pclient->RefDec();
	}


	return TRUE;
}


BOOL Account::SaveAllEquityInfo(BOOL bNewTable)
{
	char sttm[30];
	char stst[50];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Save Equit Info,%s",sttm);
	theCfgFile.mlog.AddLogStr(stst);

	sMapAccid.LockMap();
	srstAccEquity rstaccequity;
	if (bNewTable) { 
		theOrderDB.mdb.ExecuteSQL("delete mNewAccount"); 
		theCfgFile.mlog.AddLogStr("deleted mNewAccount"); 
	}else {
		theOrderDB.mdb.ExecuteSQL("delete mAccount"); 
		theCfgFile.mlog.AddLogStr("deleted mAccount"); 
	}
//	rstaccequity.ExecuteSql("delete mAccount");
	rstaccequity.init(&theOrderDB.mdb);
	
	rstaccequity.Open(bNewTable);
	intitem *pintitem;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	try{
		while(pintitem!=NULL) {
			pacc=(Account*)pintitem->val;
			ASSERT(pacc!=NULL);
			memcpy(&rstaccequity.mitem,&pacc->mitemEquity,sizeof(itemAccEquity));
			rstaccequity.AddNew();

			sMapAccid.EnumNext(&pintitem);
		}
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
	}
	sMapAccid.UnlockMap();
	rstaccequity.Close();
	return TRUE;
}

BOOL Account::LoadEquityInfo()
{
	BOOL ret;
	Account *pacc;

	srstAccEquity rstAccEquiy;
	rstAccEquiy.mitem.Clear();
	ret=rstAccEquiy.Open(FALSE);
	ASSERT(ret==TRUE);
	while(!rstAccEquiy.IsEOF()) {
		pacc=FindAcc(rstAccEquiy.mitem.maccid);
		if (pacc!=NULL) {
			pacc->mitemEquity.CopyFrom(&rstAccEquiy.mitem);
			//memcpy(&pacc->mitemEquity,&rstAccEquiy.mitem,sizeof(itemAccEquity));
		};
		rstAccEquiy.mitem.Clear();
		rstAccEquiy.MoveNext();
	}
	rstAccEquiy.Close();
	return ret;

	/*
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Equity and BuyingPower file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return FALSE; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return FALSE;
	}; 
	CArchive ar(&fp,CArchive::load);
	char st[400];
	char *starry[6];
	Account *pacc;
	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		if (st[0]=='#') continue;
		int i;
		starry[0]=strtok(st,",");
		for (i=1;i<6;i++){
			starry[i]=strtok(NULL,",");
			if (starry[i]==NULL) {
				starry[0]=0;break;
			}
		};

		if (starry[0]==NULL) {
			AfxMessageBox("Data formate error!");
			break;
			
		}
		pacc=Account::FindAccByName(starry[0]);
		if (pacc==NULL) continue;
		if (starry[2]) {
			pacc->mitemEquity.mOpenEQ=atof(starry[2]);
		}
		if (starry[3]) {
			pacc->mitemEquity.mOpenMGEQ=atof(starry[3]);
		}
		if (starry[4]) {
			pacc->mitemEquity.mOpenBP=atof(starry[4]);
		}
		if (starry[5]) {
			pacc->mitemEquity.mOpenNBP=atof(starry[5]);
		}
		time(&(pacc->mitemEquity.mm_time));
	}
	ar.Close();
	fp.Close();
	*/
	return TRUE;
}

Account * Account::FindAccByName(LPCSTR accname)
{
	intitem *pintitem;
	Account *pacc,*pret;
	sMapAccid.LockMap();
	sMapAccid.EnumFirst(&pintitem);
	pret=NULL;
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		if (!pacc->mitemifo.IsDeleted()) {
			if (strcmp(pacc->mitemifo.mname,accname)==0) {
				pret=pacc;
				break;
			}
		}
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	return pret;
}

BOOL Account::MakePkgByTime(char *pkgbuff, time_t ntime)
{
	BYTE flag;
	flag=0;
	if (mitemifo.mm_date>ntime) {
		flag |=1;
	}
	if (mitemEquity.mm_time>ntime) {
		flag |=1<<4;
	}
	if (flag==0) return FALSE;
	PkgToken tok;
	tok.InitToSet(pkgbuff);
	tok.setWORD(1001);
	tok.setBYTE(flag);
	if (flag&(1<<0)) {
		mitemifo.SetPkg(&tok,0xffff); 
	}
	if (flag&(1<<4)) {
		mitemEquity.SetPkg(&tok,0xffff);
	}
	tok.setBYTE(0);
	tok.finalizeSet();
	return TRUE;
}

BOOL Account::AddTrade(itemTrade *ptrade)
{
	ASSERT(ptrade!=NULL); 
	mLstTrades.AddAscent(ptrade->me_date,(long)ptrade);
	return TRUE;
}

itemPosition * Account::ModifyPosition(itemPosition *pitem, int flag)
{
	itemPosition *ppos;
	ASSERT(pitem!=NULL);
	mLstPositions.LockLst();
	ppos=FindPosition(pitem->msecsym,pitem->mptype);
	if (ppos==NULL) {
		ppos=itemPosition::GetVoidBuff();
		ASSERT(ppos!=NULL);
		ppos->CopyFrom(pitem);
		mLstPositions.AddTail(0,(long)ppos); 
	}else {
/*
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
*/

	if (flag& iPosBrid) ppos->mbrid=pitem->mbrid ;
		if (flag& iPosInitQty) ppos->mInitQty=pitem->mInitQty;
		if (flag& iPosInitPric) ppos->mInitPrice=pitem->mInitPrice;
		if (flag& iPosQty) ppos->mqty=pitem->mqty ;
		if (flag& iPosAvgPric) ppos->mavgcost=pitem->mavgcost;
		if (flag& iPosCdat) ppos->mc_date=pitem->mc_date; 
		if (flag& iPosMdat) ppos->mm_date=pitem->mm_date;
		if (flag& iPosHoldQty) ppos->mHoldqty=pitem->mHoldqty;
	}
	mLstPositions.UnlockLst();
	return pitem;

}

itemPosition * Account::FindPosition(LPCSTR secsym, char type,intlst *pPoslst)
{
	intitem *pintitem;
	itemPosition *pitem; 
	itemPosition *pret=NULL;
	intlst *plst;
	if (pPoslst==NULL) plst=&mLstPositions;
	else plst=pPoslst;
	plst->LockLst();
	plst->EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pitem=(itemPosition*)pintitem->val;
		ASSERT(pitem!=NULL);
		ASSERT(pitem->maccid==mitemifo.maccid); 
		if ((pitem->mptype ==type) &&
			(strcmp(pitem->msecsym,secsym)==0)) {
			pret =pitem;
			break;
		}
		plst->EnumNext(&pintitem); 
	}
	plst->UnlockLst();
	return  pret;
}



itemTrade * Account::AddNewTrade(itemTrade *ptrade,BOOL bCalcuateEquity)
{
	itemTrade *pitem;
	ASSERT(ptrade!=NULL);
	if (ptrade->mtradeid==0) return NULL;
	if (ptrade->maccid==0) return NULL;
	Account *pacc;
	pacc=Account::FindAcc(ptrade->maccid);
	if (pacc==NULL) {
		ASSERT(FALSE);
		theCfgFile.mlog.AddLogStr("ERROR:the Trade can't find account");
	}else {
		pitem=itemTrade::GetVoidBuff();
		pitem->CopyFrom(ptrade);
		pacc->AddTrade(pitem);
		intitem *pintitem;
		pintitem=itemTrade::sMapTradeID.Add(pitem->mtradeid,(long)pitem); 
/*		if (bCalcuateEquity){//if we calcuate the equity, do the following;
			if (pintitem->pnext=NULL) {
				itemAccEquity oldequity;
				oldequity.CopyFrom(&pacc->mitemEquity); 
				pacc->TradeAddEffect(pitem);
				WORD eqdiff;
				eqdiff=oldequity.CompareItem(&pacc->mitemEquity);
				if (eqdiff) {
					char pkg[200];
					pacc->MakePkgByEquity(pkg,eqdiff);
					Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
				}

			}else {
				pacc->ReCalculatePosition();
				pacc->mitemEquity.mCurrHoldBP=pacc->ReCalOrderHoldBP(); 
			}
		}
*/	
	}

	return pitem;
}

itemOrder * Account::AddNewOrder(itemOrder *porder)
{
	itemOrder *pitem;
	ASSERT(porder!=NULL);
	if (porder->morderid==0) return NULL;
	if (porder->maccid==0) return NULL;
	Account *pacc;
	pacc=Account::FindAcc(porder->maccid);
	if (pacc==NULL) {
//		ASSERT(FALSE);
		theCfgFile.mlog.AddLogStr("ERROR:the Order can't find account");
	}else {
		pitem=(itemOrder*)itemOrder::sitembuff.GetVoidBuff();
		pitem->CopyFrom(porder);
		pacc->AddOrder(pitem);
		itemOrder::sMapOrderID.Add(pitem->morderid,(long)pitem); 
	}
	return pitem;
}

BOOL Account::AddOrder(itemOrder *porder)
{
	ASSERT(porder!=NULL); 
	mLstOrders.AddTail(0,(long)porder);
	return TRUE;

}
/*  this  function is used to add position and change the buying power for these
	trade. when add a new trade or change the trade, must call this function to
	get the buypowering cut or back
*/
//DEL BOOL Account::TradeAddEffect(itemTrade *ptrade)
//DEL {
//DEL 	long qty;
//DEL 	qty=ptrade->mqty;
//DEL 	
//DEL 	if (ptrade->IsBuyTrade()) {// buy trade;
//DEL 		qty-=CoverPosType3(ptrade->msecsym,ptrade->mprice,qty);
//DEL 		if (qty!=0) {
//DEL 			if (ptrade->IsCashTrade()) { 
//DEL 				AddPosShare(ptrade->msecsym,ptrade->mprice,qty,1);
//DEL 			}else {
//DEL 				AddPosShare(ptrade->msecsym,ptrade->mprice,qty,1);
//DEL 			}
//DEL 		}
//DEL 	}else {// sell trade;
//DEL 		if (ptrade->IsCashTrade()&&(!ptrade->IsShortTrade())) {// type 1;
//DEL 			qty-=CoverPosType1(ptrade->msecsym,ptrade->mprice,qty);
//DEL 			if(qty!=0) {
//DEL 				qty-=CoverPosType2(ptrade->msecsym,ptrade->mprice,qty); 
//DEL 			}
//DEL 		}else {// type 2 , type 3;
//DEL 			qty-=CoverPosType2(ptrade->msecsym,ptrade->mprice,qty);
//DEL 			if (qty!=0) {
//DEL 				qty-=CoverPosType1(ptrade->msecsym,ptrade->mprice,qty);
//DEL 			}
//DEL 		}
//DEL 		if (qty!=0) {// still can't finish all the trade;
//DEL 			AddPosShare(ptrade->msecsym,ptrade->mprice,qty,3);
//DEL 		}
//DEL 
//DEL 	}
//DEL 	return TRUE;
//DEL }

//DEL int Account::CoverPosType3(LPSTR secsym,double price,int maxqty)
//DEL {
//DEL 	int coverqty;
//DEL 	int newqty;
//DEL 	itemPosition *ppos;
//DEL 	double recovDayBP;
//DEL 	double recovNBP;
//DEL 	ppos=FindPosition(secsym,3);
//DEL 	if (ppos==NULL) return 0;
//DEL 
//DEL 	if (maxqty > ppos->mqty ) {
//DEL 		coverqty= ppos->mqty ;
//DEL 
//DEL 	}else {
//DEL 		coverqty= maxqty;
//DEL 
//DEL 	};
//DEL 	newqty=ppos->mqty -coverqty;
//DEL 	if (ppos->mqty>ppos->mInitQty) {
//DEL 		if (newqty >ppos->mInitQty) {
//DEL 			recovNBP=0;
//DEL 			double bpshare;
//DEL //			bpshare=((ppos->mavgcost*ppos->mqty)-ppos->mInitPrice*ppos->mInitQty)/(ppos->mqty-ppos->mInitQty);
//DEL //			ppos->mavgcost=(ppos->mInitPrice*ppos->mInitQty+bpshare*(newqty-ppos->mInitPrice))/newqty;
//DEL 			bpshare=ppos->mBuyPwer/(ppos->mqty-ppos->mInitQty);
//DEL 			recovDayBP=bpshare*coverqty;
//DEL 			ppos->mBuyPwer-=recovDayBP; 
//DEL 			ppos->mavgcost=(ppos->mInitPrice*ppos->mInitQty *ppos->mBuyPwer)/newqty;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 		}else {
//DEL 			recovDayBP=ppos->mBuyPwer;
//DEL 			recovNBP=ppos->mInitPrice*(ppos->mInitQty-newqty);
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 			ppos->mBuyPwer=0;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 			mitemEquity.mCurrNBP+=recovNBP;
//DEL 		}
//DEL 	}else {
//DEL 		recovNBP=ppos->mInitPrice*(coverqty);
//DEL 		mitemEquity.mCurrNBP+=recovNBP;
//DEL 	}
//DEL 
//DEL 	ppos->mqty = newqty;
//DEL 	return coverqty;
//DEL }


//DEL int Account::CoverPosType1(LPSTR secsym, double price, int maxqty)
//DEL {
//DEL 	int coverqty;
//DEL 	int newqty;
//DEL 	itemPosition *ppos;
//DEL 	double recovDayBP;
//DEL 	double recovNBP;
//DEL 	ppos=FindPosition(secsym,1);
//DEL 	if (ppos==NULL) return 0;
//DEL 
//DEL 	if (maxqty > ppos->mqty ) {
//DEL 		coverqty= ppos->mqty ;
//DEL 
//DEL 	}else {
//DEL 		coverqty= maxqty;
//DEL 
//DEL 	};
//DEL 	newqty=ppos->mqty -coverqty;
//DEL 	if (ppos->mqty>ppos->mInitQty) {
//DEL 		if (newqty >ppos->mInitQty) {
//DEL 			recovNBP=0;
//DEL 			double bpshare;
//DEL 			bpshare=ppos->mBuyPwer/(ppos->mqty-ppos->mInitQty);
//DEL 			recovDayBP=bpshare*coverqty;
//DEL 			ppos->mBuyPwer-=recovDayBP; 
//DEL 			ppos->mavgcost=(ppos->mInitPrice*ppos->mInitQty *ppos->mBuyPwer)/newqty;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 		}else {
//DEL 			recovDayBP=ppos->mBuyPwer;
//DEL 			recovNBP=ppos->mInitPrice*(ppos->mInitQty-newqty)*2;
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 			ppos->mBuyPwer=0;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 			mitemEquity.mCurrNBP+=recovNBP;
//DEL 		}
//DEL 	}else {
//DEL 		recovNBP=ppos->mInitPrice*(coverqty)*2;
//DEL 		mitemEquity.mCurrNBP+=recovNBP;
//DEL 	}
//DEL 
//DEL 	ppos->mqty = newqty;
//DEL 	return coverqty;
//DEL }


//DEL int Account::CoverPosType2(LPSTR secsym, double price, int maxqty)
//DEL {
//DEL 	int coverqty;
//DEL 	int newqty;
//DEL 	itemPosition *ppos;
//DEL 	double recovDayBP;
//DEL 	double recovNBP;
//DEL 	ppos=FindPosition(secsym,3);
//DEL 	if (ppos==NULL) return 0;
//DEL 	if (maxqty > ppos->mqty ) {
//DEL 		coverqty= ppos->mqty ;
//DEL 	}else {
//DEL 		coverqty= maxqty;
//DEL 	};
//DEL 
//DEL 	newqty=ppos->mqty -coverqty;
//DEL 	if (ppos->mqty>ppos->mInitQty) {
//DEL 		if (newqty >ppos->mInitQty) {
//DEL 			recovNBP=0;
//DEL 			double bpshare;
//DEL 			bpshare=ppos->mBuyPwer/(ppos->mqty-ppos->mInitQty);
//DEL 			recovDayBP=bpshare*coverqty;
//DEL 			ppos->mBuyPwer-=recovDayBP; 
//DEL 			ppos->mavgcost=(ppos->mInitPrice*ppos->mInitQty *ppos->mBuyPwer)/newqty;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 		}else {
//DEL 			recovDayBP=ppos->mBuyPwer;
//DEL 			recovNBP=ppos->mInitPrice*(ppos->mInitQty-newqty);
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 			ppos->mBuyPwer=0;
//DEL 			mitemEquity.mCurrBP+=recovDayBP;
//DEL 			mitemEquity.mCurrNBP+=recovNBP;
//DEL 		}
//DEL 	}else {
//DEL 		recovNBP=ppos->mInitPrice*(coverqty);
//DEL 		mitemEquity.mCurrNBP+=recovNBP;
//DEL 	}
//DEL 	ppos->mqty = newqty;
//DEL 	return coverqty;
//DEL }

//DEL BOOL Account::AddPosShare(LPSTR secsym, double price, int share, char type)
//DEL {
//DEL 	itemPosition * ppos;
//DEL 	double takeDayBP,takeNBP;
//DEL 	long newqty;
//DEL //	double trmainreq;
//DEL 	ASSERT(strlen(secsym)<sizeof(ppos->msecsym));
//DEL 	ASSERT(share>=0);
//DEL 	ASSERT(price>=0);
//DEL 
//DEL 	ppos=FindPosition(secsym,type);
//DEL 
//DEL 	if (ppos==NULL) { //did not find the same secsym and type in the list
//DEL 		ppos=itemPosition::GetVoidBuff() ;
//DEL 		ppos->Clear();
//DEL 		ppos->maccid=mitemifo.maccid;
//DEL 		ppos->mbrid=mitemifo.mbrid;
//DEL 		strcpy(ppos->msecsym,secsym);
//DEL 		ppos->mptype=type;
//DEL 		time(&ppos->mc_date);
//DEL 		ppos->mm_date=ppos->mc_date;
//DEL 		mLstPositions.AddTail(0,(long)ppos); 
//DEL 	} 
//DEL 	newqty=ppos->mqty +share;
//DEL 	takeDayBP=0;
//DEL 	takeNBP=0;
//DEL 	switch(type){
//DEL 	case 1: //the trade is by cash;
//DEL 		if (ppos->mqty >=ppos->mInitQty) {
//DEL 			takeDayBP=price*share *2;
//DEL 			ppos->mBuyPwer+=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty*2)/(newqty*2);
//DEL 		}else if (newqty>ppos->mInitQty) {
//DEL 			takeNBP=(ppos->mInitQty-ppos->mqty)*ppos->mInitPrice *2;
//DEL 			takeDayBP=(newqty-ppos->mInitQty)* price *2;
//DEL 			ppos->mBuyPwer +=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty*2)/(newqty*2);
//DEL 		}else {
//DEL 			takeNBP=(newqty)*ppos->mInitQty*2;
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 		}
//DEL 		mitemEquity.mCurrBP-=takeDayBP;
//DEL 		mitemEquity.mCurrNBP-=takeNBP;
//DEL 	
//DEL 		break;
//DEL 	case 2: // the trade is margin
//DEL 		if (ppos->mqty >=ppos->mInitQty) {
//DEL 			takeDayBP=price*share;
//DEL 			ppos->mBuyPwer+=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty)/newqty;
//DEL 		}else if (newqty>ppos->mInitQty) {
//DEL 			takeNBP=(ppos->mInitQty-ppos->mqty)*ppos->mInitPrice;
//DEL 			takeDayBP=(newqty-ppos->mInitQty)* price ;
//DEL 			ppos->mBuyPwer +=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty)/newqty;
//DEL 		}else {
//DEL 			takeNBP=(newqty)*ppos->mInitQty;
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 		}
//DEL 		mitemEquity.mCurrBP-=takeDayBP;
//DEL 		mitemEquity.mCurrNBP-=takeNBP;
//DEL 	
//DEL 		break;
//DEL 	case 3: // short sell
//DEL 		if (ppos->mqty >=ppos->mInitQty) {
//DEL 			takeDayBP=price*share;
//DEL 			ppos->mBuyPwer+=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty)/newqty;
//DEL 		}else if (newqty>ppos->mInitQty) {
//DEL 			takeNBP=(ppos->mInitQty-ppos->mqty)*ppos->mInitPrice;
//DEL 			takeDayBP=(newqty-ppos->mInitQty)* price ;
//DEL 			ppos->mBuyPwer +=takeDayBP;
//DEL 			ppos->mavgcost=(ppos->mBuyPwer +ppos->mInitQty*ppos->mInitQty)/newqty;
//DEL 		}else {
//DEL 			takeNBP=(newqty)*ppos->mInitQty;
//DEL 			ppos->mavgcost=ppos->mInitPrice;
//DEL 		}
//DEL 		mitemEquity.mCurrBP-=takeDayBP;
//DEL 		mitemEquity.mCurrNBP-=takeNBP;
//DEL 		break;
//DEL 	}; 
//DEL 
//DEL 	ppos->mqty =newqty;
//DEL 	return TRUE;
//DEL }
/*
BOOL Account::ReCalculatePosition()
{
	//Clear all the caculations before,
//	itemAccEquity oldequity;
//	oldequity.Clear();
//	oldequity.CopyFrom(&mitemEquity); 

	intlst mtmpposlst;
	SavePositionLst(&mtmpposlst); 
	mLstTrades.LockLst();
	mLstPositions.LockLst();
	
	intitem *pitem;
	itemPosition *ppos;
	mLstPositions.EnumFirst(&pitem);
	while(pitem!=NULL) {
		ppos=(itemPosition*)pitem->val;
		ASSERT(ppos!=NULL);
		ppos->SetToInit();
		mLstPositions.EnumNext(&pitem);
	}
	mitemEquity.SetToInit();
	//recalculate all the trades.
	
	itemTrade *ptrade;
	mLstTrades.EnumFirst(&pitem);
	while(pitem!=NULL) {
		ptrade=(itemTrade*)pitem->val;
		ASSERT(ptrade!=NULL);
		TradeAddEffect(ptrade);
		mLstTrades.EnumNext(&pitem);
	}

	mLstPositions.UnlockLst();
	mLstTrades.UnlockLst();
	SendPosLstDiff(&mtmpposlst);
//	WORD eqdiff;
//	eqdiff=oldequity.CompareItem(&mitemEquity);
//	if (eqdiff) {
//		char pkg[200];
//		MakePkgByEquity(pkg,eqdiff);
//		Trader::SPKGAcc(pkg,mitemifo.maccid,mitemifo.mbrid,0);  
//	}

	return TRUE;
}
*/

BOOL Account::MakePkgByEquity(char *pkgbuff, WORD flag)
{
	PkgToken tok;
	tok.InitToSet(pkgbuff);
	tok.setWORD(1001);
	tok.setBYTE(1<<4);
	mitemEquity.SetPkg(&tok,flag|1);
	tok.setBYTE(0);
	tok.finalizeSet();
	return TRUE;
}

double Account::ReCalOrderHoldBP()
{
	if (mitemifo.mstatus&(1<<15)) {// use the extented buing power caculation.
		return ReCalOrderHoldBP_Ext(); 
	}

	double holdbp;
	holdbp=0;
	strmap longposmap;
	strmap shortposmap;
	stritem *pstritem;

	intitem *pposintitem;
	itemPosition *ppos;
	mLstOrders.LockLst();
	mLstPositions.LockLst();
	mLstPositions.EnumFirst(&pposintitem);
	while(pposintitem!=NULL) {
		ppos=(itemPosition*)pposintitem->val; 
		if (ppos->mqty) {
			int qty1;
			qty1=ppos->GetEffectQty();
			switch(ppos->mptype) {
			case 3:
				pstritem=shortposmap.FindItem(ppos->msecsym);
				if (pstritem==NULL) {
					shortposmap.Add(ppos->msecsym,qty1); 
				}else {
					pstritem->val +=qty1 ;				
				}
				break;
			case 1: case 2:
				pstritem=longposmap.FindItem(ppos->msecsym);
				if (pstritem==NULL) {
					longposmap.Add(ppos->msecsym,qty1); 
				}else {
					pstritem->val +=qty1;
				}
			}
		}
		mLstPositions.EnumNext(&pposintitem);
	}
	mLstPositions.UnlockLst();

	intitem *porderintitem;
	itemOrder *porder;
	mLstOrders.EnumFirst(&porderintitem);
	while(porderintitem) {
		porder=(itemOrder*)porderintitem->val; 
		ASSERT(porder!=NULL);
		int qty2;
		qty2=porder->GetOpenBpQty();
		if (qty2>0) {
			if (porder->IsBuyOrder()) {// buy order;
				pstritem=shortposmap.FindItem(porder->msecsym);
			}else {// sell order;
				pstritem=longposmap.FindItem(porder->msecsym);
			}
			if (pstritem!=NULL) {
				if (pstritem->val>=qty2) {
					pstritem->val-=qty2;
					qty2=0;
				}else {
					qty2-=pstritem->val;
					pstritem->val =0;
				}
			}
		}

		if ((qty2>0)&&(porder->msecsym[0]=='+')) {
			if (!porder->IsBuyOrder()){
				itemOptsym *pop;
				pop=itemOptsym::FindInfo(porder->msecsym+1);
				if (pop) {
					if (pop->IsCall()){
						pstritem=shortposmap.FindItem(pop->undsymb);
					}else {// sell order;
						pstritem=longposmap.FindItem(pop->undsymb);
					}
					if (pstritem!=NULL) {
						if (pstritem->val>=qty2) {
							pstritem->val-=qty2;
							qty2=0;
						}else {
							qty2-=pstritem->val;
							pstritem->val =0;
						}
					}


				}
			}
		}
		
		if (qty2>0) {
			holdbp+=porder->GetHoldBPRate(this)*qty2 ; 				
			holdbp+=theCfgFile.mCommBPPreHold;
		}
		mLstOrders.EnumNext(&porderintitem);
	}
	mLstOrders.UnlockLst();
	return holdbp;
}

BOOL Account::SavePositionLst(intlst *pposlst,BOOL binit)
{
	mLstPositions.LockLst();
	intitem *pintitem;
	itemPosition *ppos;
	itemPosition *ppos1;
	mLstPositions.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		ppos=(itemPosition *)pintitem->val;
		ASSERT(ppos!=NULL);
		ppos1=itemPosition::GetVoidBuff();
		ASSERT(ppos1!=NULL);
		ppos1->CopyFrom(ppos);
		if (binit) ppos1->SetToInit();
		pposlst->AddTail(0,(long)ppos1);
		mLstPositions.EnumNext(&pintitem);
	}
	mLstPositions.UnlockLst();
	return TRUE;
}

//DEL BOOL Account::SendPosLstDiff(intlst *pposlst)
//DEL {
//DEL 	mLstPositions.LockLst();
//DEL 	intitem *pintitem;
//DEL 	itemPosition *ppos;
//DEL 	time_t newtime;
//DEL 	time(&newtime);
//DEL 	char msgbuf[300];
//DEL 	mLstPositions.EnumFirst(&pintitem);
//DEL 	while (pintitem!=NULL){
//DEL 		ppos=(itemPosition*)pintitem->val;
//DEL 		//find the position in the pposlst;
//DEL 		intitem *pintitem1;
//DEL 		itemPosition *pitem1; 
//DEL 		itemPosition *pretpos=NULL;
//DEL 		pposlst->EnumFirst(&pintitem1);
//DEL 		while(pintitem1!=NULL) {
//DEL 			pitem1=(itemPosition*)pintitem1->val;
//DEL 			if (pitem1!=NULL){;
//DEL 				if ((pitem1->mptype ==ppos->mptype) &&
//DEL 					(strcmp(pitem1->msecsym,ppos->msecsym)==0)) {
//DEL 					pretpos=pitem1;
//DEL 					break;
//DEL 				}
//DEL 			}
//DEL 			pposlst->EnumNext(&pintitem1); 
//DEL 		}
//DEL 		//find the different;
//DEL 		if (pretpos!=NULL) {
//DEL 			if (pretpos->mqty!=ppos->mqty) {
//DEL 				ppos->mm_date=newtime;
//DEL 				ppos->MakeInfoPkg(msgbuf,0xffff);
//DEL 				Trader::SPKGAcc(msgbuf,mitemifo.maccid,mitemifo.mbrid,0);
//DEL 			}
//DEL 			itemPosition::PutVoidBuff(pretpos);
//DEL 			pintitem1->val=0;
//DEL 		}else {
//DEL 			ppos->mm_date=newtime;
//DEL 			ppos->MakeInfoPkg(msgbuf,0xffff);
//DEL 			Trader::SPKGAcc(msgbuf,mitemifo.maccid,mitemifo.mbrid,0);
//DEL 		}
//DEL 		//end of the different;
//DEL 		mLstPositions.EnumNext(&pintitem);
//DEL 	}
//DEL 	mLstPositions.UnlockLst();
//DEL 	return TRUE;
//DEL }
//this function recaluate all the ticket and position;
BOOL Account::TktRecaluate(BOOL bSendPos, BOOL bSendEquity)
{
	intlst tmpCombinedTicket; // the tempory combined ticket;
	intlst tmpTicket;//tempory ticket map used. 
	intlst tmpposlst;//tempory position list;
	itemAccEquity tmpEquity;// tempory equity of this account;
//-------Combine Trade to tmpTicket--------
	itemTrade *ptrade;
	intitem* PItrade;
	mLstTrades.LockLst();
	mLstTrades.EnumFirst(&PItrade);
	while(PItrade){
		ptrade=(itemTrade*)PItrade->val; 
		TktAddTrade(&tmpCombinedTicket,ptrade);
		mLstTrades.EnumNext(&PItrade); 
	}
	mLstTrades.UnlockLst();
//------Init tempory data to the mornint-----
	SavePositionLst(&tmpposlst,TRUE); 
	tmpEquity.CopyFrom(&mitemEquity);
	tmpEquity.SetToInit();
//-----recalculate the trades	-----	
	itemTicket *pticket;
	int exetime;
	while(tmpCombinedTicket.GetHead(&exetime,(int*)&pticket)) {
		TktTicketAddEffect(pticket,	&tmpTicket,&tmpposlst,&tmpEquity);
	}
	TktPosUpdate(&tmpposlst,bSendPos);
	TktTicketUpdate(&tmpTicket); 
	TktEquityUpdate(&tmpEquity,bSendEquity); 

/*	
	//recalculate all the trades.
	
	itemTrade *ptrade;
	mLstTrades.EnumFirst(&pitem);
	while(pitem!=NULL) {
		ptrade=(itemTrade*)pitem->val;
		ASSERT(ptrade!=NULL);
		TradeAddEffect(ptrade);
		mLstTrades.EnumNext(&pitem);
	}

	mLstPositions.UnlockMap();
	mLstTrades.UnlockMap();
	SendPosLstDiff(&mtmpposlst);
*/
	return TRUE;
}


// this function add a trade to the ticket list.
// if there are ticket need to be combine, combine it , 
//     otherwise generate a new ticket.
BOOL Account::TktAddTrade(intlst *ticklst, itemTrade *ptrade)
{
	if (!ptrade->IsOpen()) return TRUE;
	intitem *PItrade;
	itemTicket *ptmpticket;
	BOOL finded;
	finded=FALSE;
	if (ptrade->morderid>0) {
		ticklst->LockLst();
		ticklst->EnumFirst(&PItrade);
		while(PItrade!=NULL) {
			ptmpticket=(itemTicket*)PItrade->val;
			ASSERT(ptmpticket!=NULL);
			if (ptmpticket->morderid==ptrade->morderid) {
				TktCombineTicket(ptmpticket,ptrade);
				finded=TRUE;
				break;
			}
			ticklst->EnumNext(&PItrade);
		}
		ticklst->UnlockLst();
	}
	if (finded==FALSE) {
		ptmpticket=itemTicket::GetVoidBuff();
		TktCreateTicketFromTrade(ptmpticket,ptrade);
		ticklst->AddAscent(ptmpticket->me_date,(long)ptmpticket); 
	}
	return TRUE;
}
// this function combine a trade to ticket.
// calcuate the new qty and price;
BOOL Account::TktCombineTicket(itemTicket *pticket, itemTrade *ptrade)
{
	ASSERT(pticket->morderid==ptrade->morderid);
	ASSERT(ptrade->morderid!=0);
	

	int newqty;
	double newprice;
	if (!ptrade->IsOpen()) return TRUE;
	newqty=pticket->mqty+ptrade->mqty;
	newprice=(pticket->mqty*pticket->mprice +ptrade->mqty*ptrade->mprice)/newqty;
	pticket->mqty=newqty;
	pticket->mprice=newprice;
	pticket->mexecnum++; 
	if(ptrade->mcommision>=0) {
		if (pticket->IsFixedComm()) {
			if (pticket->mcommision<ptrade->mcommision) {
				pticket->mcommision=ptrade->mcommision;
			}
		}else {
			pticket->mcommision=ptrade->mcommision;
			pticket->SetFixedComm(TRUE);
		}

		
	}else {
		pticket->mstatus &= ~((DWORD)1<<21);
		pticket->mcommision=0;
	}
	if (ptrade->IsOrignalTrade()) {
		pticket->mcomm1+=ptrade->GetEcnFee(NULL); 
	}else {
		pticket->mcomm1=ptrade->mEcnFee;
	}
	return TRUE;
}

BOOL Account::TktCreateTicketFromTrade(itemTicket *pticket, itemTrade *ptrade)
{
	pticket->mtrid=ptrade->mtrid;
	pticket->maccid =ptrade->maccid;
	pticket->mentrid=ptrade->mentrid;
	pticket->mtradeid=ptrade->mtradeid;
	pticket->morderid=ptrade->morderid;
	pticket->mbrid=ptrade->mbrid;
	memccpy(pticket->mroute,ptrade->mroute,0,sizeof(pticket->mroute));
	memccpy(pticket->mbkrsym,ptrade->mbkrsym,0,sizeof(pticket->mbkrsym));
	pticket->mstatus=ptrade->mstatus;
	pticket->mexecnum=1;
	pticket->mqty=ptrade->mqty;
	pticket->mprice=ptrade->mprice;
	memccpy(pticket->msecsym,ptrade->msecsym,0,sizeof(pticket->msecsym));
	pticket->mexchange =ptrade->mexchange;
	memccpy(pticket->mrrno,ptrade->mrrno,0,sizeof(pticket->mrrno));
	pticket->me_date=ptrade->me_date;
	pticket->mm_date=ptrade->mm_date;
	if(ptrade->mcommision!=-1) {
		pticket->mcommision=ptrade->mcommision;
		pticket->SetFixedComm(TRUE);
	}else {
		pticket->mcommision=0;
		pticket->SetFixedComm(FALSE); 
	}
	if (ptrade->IsOrignalTrade()) {
		pticket->mcomm1=ptrade->GetEcnFee(NULL); 
	}else {
		pticket->mcomm1=ptrade->mEcnFee;
	}
	return TRUE;
}

BOOL Account::TktTicketAddEffect(itemTicket *pticket, intlst *pTktlst, intlst *pPoslst, itemAccEquity  *pEquity)
{
	long qty;
	qty=pticket->mqty;
	itemTicket *p1,*p2;
	double tmpcommision;
	pEquity->mtotalshares+=pticket->mqty; 
	if (pticket->IsBuyTrade()) {// buy trade;
		if(pticket->mstatus&(1<<29)) {
			TktAddPos(2,pticket,pTktlst,pPoslst,pEquity);
			tmpcommision=pticket->GetCommision(this,pTktlst);
			pticket->mcommision=tmpcommision;
			pEquity->mComm+=tmpcommision; 
			pEquity->mtktnum++;
			return TRUE;
		}
		p1=TktCoverPos(3,pticket,pTktlst,pPoslst,pEquity);
		if (p1!=pticket) {
			tmpcommision=pticket->GetCommision(this,pTktlst);
			pticket->mcommision=tmpcommision;
			pEquity->mComm+=tmpcommision; 
			pEquity->mtktnum++;
		}
		if (p1!=NULL) {
			if (p1->IsCashTrade()) { 
				TktAddPos(1,p1,pTktlst,pPoslst,pEquity);
				tmpcommision=p1->GetCommision(this,pTktlst);
				p1->mcommision=tmpcommision;
				pEquity->mComm+=tmpcommision; 
				pEquity->mtktnum++;
			}else {
				TktAddPos(2,p1,pTktlst,pPoslst,pEquity);
				tmpcommision=p1->GetCommision(this,pTktlst);
				p1->mcommision=tmpcommision;
				pEquity->mComm+=tmpcommision; 
				pEquity->mtktnum++;
			}
		}
	}else {// sell trade;
		if (pticket->mstatus&(1<<29)){
			TktAddPos(3,pticket,pTktlst,pPoslst,pEquity);
			tmpcommision=pticket->GetCommision(this,pTktlst);
			pticket->mcommision=tmpcommision;
			pEquity->mComm+=tmpcommision; 
			pEquity->mtktnum++;

			return TRUE;
		}
		if (pticket->IsCashTrade()&&(!pticket->IsShortTrade())) {// type 1;
			p1=TktCoverPos(1,pticket,pTktlst,pPoslst,pEquity);
			if (p1!=pticket) {
				tmpcommision=pticket->GetCommision(this,pTktlst);
				pticket->mcommision=tmpcommision;
				pEquity->mComm+=tmpcommision; 
				pEquity->mtktnum++;
			}
			if(p1!=NULL) {
				p2=TktCoverPos(2,p1,pTktlst,pPoslst,pEquity);
				if (p2!=p1) {
					tmpcommision=p1->GetCommision(this,pTktlst);
					p1->mcommision=tmpcommision;
					pEquity->mComm+=tmpcommision; 
					pEquity->mtktnum++;
				}
			}else p2=NULL;

		}else {// type 2 , type 3;
			p1=TktCoverPos(2,pticket,pTktlst,pPoslst,pEquity);
			if (p1!=pticket) {
				tmpcommision=pticket->GetCommision(this,pTktlst);
				pticket->mcommision=tmpcommision;
				pEquity->mComm+=tmpcommision; 
				pEquity->mtktnum++;
			}
			if(p1!=NULL) {
				p2=TktCoverPos(1,p1,pTktlst,pPoslst,pEquity);
				if (p2!=p1) {
					tmpcommision=p1->GetCommision(this,pTktlst);
					p1->mcommision=tmpcommision;
					pEquity->mComm+=tmpcommision; 
					pEquity->mtktnum++;
				}
			}else p2=NULL;
		}
		if (p2!=NULL) {// still can't finish all the trade;
			TktAddPos(3,p2,pTktlst,pPoslst,pEquity);
			tmpcommision=p2->GetCommision(this,pTktlst);
			p2->mcommision=tmpcommision;
			pEquity->mComm+=tmpcommision; 
			pEquity->mtktnum++;
		}
	}
	return TRUE;
}

//  this function use trade to cover the position, 
//	ntype is the type we want to cover, valid value:1,2,3
//  pticket is the ticket we used to cover.
//  ptktlst is the new ticket list that we will put into;
//  pposlst is the new effected position,
//  pequity is the new equity we will effect.

//  return is the lefted ticket that we can't covered.
itemTicket * Account::TktCoverPos(int ntype, itemTicket *pticket, intlst *ptktlst, intlst *pposlst, itemAccEquity *pequity)
{
	if (ntype==3) { //short position can only use buy trade to cover,
		if (!pticket->IsBuyTrade()) return pticket;
	}else {// otherwise we use sell trade to cover type 1 or type 2 position.
		if (pticket->IsBuyTrade()) return pticket;
	}

	int undshr;
	BOOL bOpt;
	BOOL bCASH;

	if (pticket->msecsym[0]=='+') {
		itemOptsym *posm;
		posm=itemOptsym::FindInfo(pticket->msecsym);
		if (posm==NULL) undshr=100;
		else undshr=posm->pmundlyshr;
		bOpt=TRUE;
		bCASH=TRUE; 
	}else if (pticket->msecsym[0]=='#'){
		FutureIfo *pifo=theCfgFile.mpIssueData->GetFutureInfo(pticket->msecsym);
		if (pifo) undshr=(int)pifo->m_multiplier;
		bOpt=FALSE;
		bCASH=TRUE;
	}else {
		undshr=1;
		bOpt=FALSE;
		if (ntype==1) {
			bCASH=TRUE; 
		}else if (ntype==2) {
			if ((pticket->mprice<theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0))  {
				bCASH=TRUE;
			}else {
				bCASH=FALSE;
			}
		}else {
			bCASH=FALSE;
		}
	}

	itemPosition *ppos;
	ppos=FindPosition(pticket->msecsym,ntype,pposlst);
	if (ppos==NULL) return pticket;
	if (ppos->mqty==0) return pticket;

	int coverqty;
	double coverprice;
	double recovDayBP;
	double recovNBP;
	itemTicket *pretticket;
	pretticket=NULL;

	if (pticket->mqty > ppos->mqty ) {
		coverqty= ppos->mqty ;
		pretticket=itemTicket::GetVoidBuff();
		memcpy(pretticket,pticket,sizeof(itemTicket));
		double comm1=pticket->mcomm1;
		int    totalqty=pticket->mqty; 
		pretticket->mqty=pticket->mqty-coverqty;
		pticket->mqty=coverqty;
//		split the ticket, need to split the EcnFee.
		pticket->mcomm1=comm1*pticket->mqty/totalqty;
		pretticket->mcomm1=comm1*pretticket->mqty/totalqty;
	}else {
		coverqty= pticket->mqty;
	};
	coverprice=pticket->mprice;

	if (mitemifo.IsDVPAccount()) {
		pticket->SetShortTrade(FALSE);
		pticket->SetCashTrade(TRUE);
		if (pticket->IsBuyTrade()) {
			pticket->SetCoverTicket(FALSE);
		}else {
			pticket->SetCoverTicket(TRUE);
		}

	}else {
		pticket->SetCoverTicket(TRUE);
		switch(ntype) {
		case 3://cover short
			pticket->SetShortTrade(TRUE);
			pticket->SetCashTrade(FALSE);
			break;
		case 2://cover margin position;
			pticket->SetShortTrade(FALSE);
			pticket->SetCashTrade(FALSE);
			break;
		case 1://cover cash position;
			pticket->SetShortTrade(FALSE);
			pticket->SetCashTrade(TRUE);
			break;
		}
	}

	int newqty;
	double oldcost,newcost,initcost;
	
	oldcost=ppos->mavgcost*ppos->mqty; 
	initcost=ppos->mInitPrice *ppos->mInitQty;
//	newcost=oldcost-(pticket->mqty*pticket->mprice);
	newqty=ppos->mqty -coverqty;


	recovDayBP=0;
	recovNBP=0;
	double recovMktVal=0;
	BOOL bReleaseDay		=mitemifo.mstatus & (1<<27);
	BOOL bReleaseOverNight	=mitemifo.mstatus & (1<<28);
	BOOL bRelease1_2		=mitemifo.mstatus & (1<<29);//Release half buying power.

	if (ppos->mqty>ppos->mHoldqty) {
		if (newqty >ppos->mHoldqty) {
			double bpshare;
			double holdcost;
			holdcost=ppos->mInitPrice*ppos->mHoldqty; 
			ppos->mavgcost=(((oldcost-holdcost)/(ppos->mqty-ppos->mHoldqty))*(newqty-ppos->mHoldqty)+holdcost)/newqty;

			
			bpshare=ppos->mBuyPwer/(ppos->mqty-ppos->mHoldqty);
			recovDayBP=bpshare*coverqty;
			recovNBP=0;//bpshare*coverqty;
			ppos->mBuyPwer-=recovDayBP;
		}else {
			recovDayBP=ppos->mBuyPwer;
			recovMktVal=pticket->mprice*(ppos->mHoldqty-newqty)*undshr;
			if (bCASH) {
				if (bReleaseOverNight)recovNBP=recovMktVal*(bRelease1_2? 0.5:theCfgFile.mNightMargBPRate) ;
				if (bReleaseDay)	recovDayBP+=recovMktVal*(bRelease1_2? 1 : theCfgFile.mDayMargBPRate);   //cash covers 4 times;
			}else {
				if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
				if (bReleaseDay) recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mNightMargBPRate); //margin position recover 2 times;
			}
/*			if ((ntype==1)||(bCASH)) {
				if (bReleaseOverNight)recovNBP=recovMktVal*(bRelease1_2? 0.5:theCfgFile.mNightMargBPRate) ;
				if (bReleaseDay)	recovDayBP+=recovMktVal*(bRelease1_2? 1 : theCfgFile.mDayMargBPRate);   //cash covers 4 times;
			}else if (ntype==2) {
				if ((pticket->mprice<theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0))  {
					if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate) ;
					if (bReleaseDay)	   recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mDayMargBPRate);   //cash covers 4 times;  
				}else {
					if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
					if (bReleaseDay) recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mNightMargBPRate); //margin position recover 2 times;
				};
			}else {
				if (bReleaseOverNight)  recovNBP=recovMktVal*(bRelease1_2? 0.5 :theCfgFile.mNightMargBPRate);
				if (bReleaseDay)        recovDayBP+=recovMktVal*(bRelease1_2? 1: theCfgFile.mNightMargBPRate); //margin position recover 2 times;

			}
*/
			ppos->mavgcost=ppos->mInitPrice;
			ppos->mBuyPwer=0;
			ppos->mHoldqty=newqty;
		}
	}else {
		recovMktVal=pticket->mprice*(coverqty)*undshr;
		if (bCASH) {
			if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
			if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mDayMargBPRate);   //cash covers 4 times;
		}else {
			if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
			if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mNightMargBPRate); //margin position recover 2 times;
		}
/*		if ((ntype==1)||(bCASH)) {
			if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
			if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mDayMargBPRate);   //cash covers 4 times;
		}else if (ntype==2) {
			if ((pticket->mprice<theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0)) {
				if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate) ;
				if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mDayMargBPRate );   //cash covers 4 times;
			}else {
				if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
				if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mNightMargBPRate); //margin position recover 2 times;
			};
		}else {
			if (bReleaseOverNight) recovNBP=recovMktVal*(bRelease1_2?0.5:theCfgFile.mNightMargBPRate);
			if (bReleaseDay)       recovDayBP+=recovMktVal*(bRelease1_2?1:theCfgFile.mNightMargBPRate); //margin position recover 2 times;
		}
*/
		ppos->mHoldqty=newqty; 
	}
	

	//cash position may not recover buying power;
	if ((ntype!=1)||(theCfgFile.mbRecvBPForCash)) {
			pequity->mCurrBP+=recovDayBP;	
			pequity->mCurrNBP+=recovNBP; 
	}

	pequity->mCurrNBP+=0;//recovNBP;

	ppos->mqty = newqty;
	
	newcost=ppos->mavgcost*ppos->mqty; 
	double pl=0;
	switch (ntype) {
	case 1:
	case 2:
		pl=((newcost +coverqty*coverprice)-oldcost)*undshr;
		ppos->mRealize+=    pl;
		pequity->mRealized+=pl;
		break;
	case 3:
		pl=(oldcost-(newcost +coverqty*coverprice))*undshr;
		ppos->mRealize+=    pl;
		pequity->mRealized+=pl;

		/*BULLET
		{
			itemPosition *pbullet; 
			pbullet=FindPosition(pticket->msecsym,4,pposlst);
			if (pbullet) {
				if (pbullet->mqty <pbullet->mInitQty) {
					pbullet->mqty +=pticket->mqty; 
					if (pbullet->mqty >pbullet->mInitQty) pbullet->mqty=pbullet->mInitQty;
					time(&pbullet->mm_date); 
				}
			}
		}*/
		

		break;
	}

	if (mitemifo.mstatus&(1<<30)) {// PLBP change buying power with the P/L
		pequity->mCurrBP+=pl;	
		pequity->mCurrNBP+=pl; 

	}


	time(&ppos->mm_date);

	ptktlst->AddAscent((long)pticket->me_date,(long)pticket);
	if (ptktlst==&mLstTickets) {
		pticket->mticketid=theCfgFile.GetNewTicketID();//InterlockedIncrement((long*)&theCfgFile.mCurrTicketID);  
	}

	return pretticket;
}

//Add a certain position this this ticket.
void Account::TktAddPos(int ntype, itemTicket *pticket, intlst *ptktlst, intlst *pposlst, itemAccEquity *pequity)
{
	itemPosition * ppos;
	double takeDayBP,takeNBP;
//	double trmainreq;
	int undshr;
	BOOL bOpt;
	BOOL bCall;
	BOOL bFutures;
	double dopcprice;
	dopcprice=0;
	ppos=FindPosition(pticket->msecsym,ntype,pposlst);
	FutureIfo *pfuturesifo;

	if (ppos==NULL) { //did not find the same secsym and type in the list
		ppos=itemPosition::GetVoidBuff();
		ppos->Clear();
		ppos->maccid=mitemifo.maccid;
		ppos->mbrid=mitemifo.mbrid;
		strcpy(ppos->msecsym,pticket->msecsym);
		ppos->mptype=ntype;
		time(&ppos->mc_date);
		ppos->mm_date=ppos->mc_date;
		pposlst->AddTail(0,(long)ppos); 
	}
	
	if (pticket->msecsym[0]=='+') {
		itemOptsym *posm;
		posm=itemOptsym::FindInfo(pticket->msecsym);
		if (posm==NULL) {
			undshr=100;
			dopcprice=0;
		} else {
			undshr=posm->pmundlyshr;
			if (posm->mcurrundprice>0) {
				dopcprice=((double)posm->mcurrundprice)/1000;
			}else {
				dopcprice=((double)posm->strikepice)/1000; 
			}
		}

		bOpt=TRUE;
		bCall=posm->IsCall();
		bFutures=FALSE;
	}else if (pticket->msecsym[0]=='#'){
		pfuturesifo=theCfgFile.mpIssueData->GetFutureInfo(pticket->msecsym);
		if (pfuturesifo) undshr=(int)pfuturesifo->m_multiplier;
		bOpt=FALSE;
		bFutures=TRUE;
	}else {
		undshr=1;
		bOpt=FALSE;
		bCall=FALSE;
		bFutures=FALSE;
	}



	double oldcost,initcost;
	long newqty;

	newqty=ppos->mqty +pticket->mqty;
	
	oldcost=ppos->mqty * ppos->mavgcost;
	initcost=ppos->mInitPrice*ppos->mInitPrice;
//	newcost=oldcost+ pticket->mprice *pticket->mqty;
	
	double nDayMarginRatio=theCfgFile.mDayMargBPRate;
	double nNightMarginRatio=theCfgFile.mNightMargBPRate;
	double nShortHoldRatio=1;

	if (mitemifo.mstatus &(1<<16)) { //2x BP account;
		nDayMarginRatio=2;
	}
	if (mitemifo.mstatus & (1<<30)) {// PLBP change buying power with the P/L
		if (ntype==3){
			nDayMarginRatio=0.5;
			nNightMarginRatio=0.5;
			nShortHoldRatio=0.5;
		}else {
			nDayMarginRatio=1;
			nNightMarginRatio=1;
		}
	}

	takeDayBP=0;
	takeNBP=0;
	if (bOpt) {
		switch(ntype){
		case 1: case 2: //option alwarys cash;
			if (ppos->mqty >=ppos->mHoldqty) {
				takeDayBP=pticket->mprice*pticket->mqty*undshr*nDayMarginRatio;
				takeNBP=pticket->mprice*pticket->mqty*undshr*nNightMarginRatio;
				ppos->mBuyPwer+=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;// (ppos->mBuyPwer +ppos->mHoldqty*ppos->mInitPrice*nDayMarginRatio)/(newqty*nDayMarginRatio);
			}else if (newqty>ppos->mHoldqty) {
				takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice*undshr *nDayMarginRatio;
	//			takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose()*nNightMarginRatio;
				takeNBP=pticket->mprice*pticket->mqty*undshr*nNightMarginRatio;
				ppos->mBuyPwer +=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;//(ppos->mBuyPwer +ppos->mHoldqty*ppos->mInitPrice*nDayMarginRatio)/(newqty*nDayMarginRatio);
			}else {
				takeNBP=pticket->mprice*pticket->mqty*undshr*nNightMarginRatio;//(pticket->mqty)*ppos->GetYClose()*nNightMarginRatio;
	//			ppos->mavgcost=newcost/newqty;
			}
			pticket->SetCashTrade(TRUE);
			pticket->SetShortTrade(FALSE);
			break;
		case 3: // short sell
			Account::stPosInfo stposifo;
			FindPosInfo(pticket->msecsym,&stposifo);
			int cvqty,ucvqty;
			cvqty=0;
			if (bCall) {
				cvqty=stposifo.mLong-stposifo.moptCWrite;
				if (cvqty<0) cvqty=0;
			}else {
				cvqty=stposifo.mShort-stposifo.moptPWrite;
				if (cvqty<0) cvqty=0;
			}

			if (ppos->mqty >=ppos->mHoldqty) {
				ucvqty=pticket->mqty*undshr - cvqty;
				if (ucvqty<0) ucvqty=0;
				if(dopcprice>0.001) {
					takeDayBP=dopcprice * ucvqty;
					takeNBP=dopcprice * ucvqty  ;
				}else {
					takeDayBP=pticket->mprice * ucvqty *nDayMarginRatio;
					takeNBP=pticket->mprice * ucvqty * nNightMarginRatio; 
				}
				ppos->mBuyPwer+=takeDayBP;
			}else if (newqty>ppos->mHoldqty) {
				ucvqty=(newqty-ppos->mHoldqty)-cvqty;
				
				if (ucvqty<0) ucvqty=0;
				if(dopcprice>0.001) {
					takeDayBP=dopcprice * ucvqty; 
					takeNBP=dopcprice * ucvqty  ; 
				}else {
					takeDayBP=pticket->mprice * ucvqty *nDayMarginRatio;
					takeNBP=pticket->mprice * ucvqty * nNightMarginRatio; 
				}
				ppos->mBuyPwer+=takeDayBP;
			}else {
				if(dopcprice>0.001) {
					takeNBP=dopcprice * pticket->mqty*undshr;
				}else {
					takeNBP=pticket->mqty *pticket->mprice *undshr;
				}
			}
			pticket->SetShortTrade(TRUE);
			break;
		}; 

	}else if (bFutures) {
		int Dmultipler;
		int Nmultipler;
		if (pfuturesifo==NULL){
			Dmultipler=2000;
			Nmultipler=2000;
		}else {
			Dmultipler=(int)pfuturesifo->m_DayInit; 
			Nmultipler=(int)pfuturesifo->m_Maintain;
		}
		int ratio=theCfgFile.mDayMargBPRate;
		Dmultipler*=ratio;
		ratio=theCfgFile.mNightMargBPRate;
		Nmultipler*=ratio;
		if (ppos->mqty >=ppos->mHoldqty) {
			takeDayBP=pticket->mqty*Dmultipler;
			takeNBP=pticket->mqty*Nmultipler;
			ppos->mBuyPwer+=takeDayBP;
		}else if (newqty>ppos->mHoldqty) {
			takeDayBP=(newqty-ppos->mHoldqty)* Dmultipler;
			takeNBP=pticket->mprice*pticket->mqty*Nmultipler;
			ppos->mBuyPwer +=takeDayBP;
		}else {
			takeNBP=pticket->mqty*Nmultipler ;
		}
		if (mitemifo.IsDVPAccount()) {
			pticket->SetShortTrade(FALSE);
			pticket->SetCashTrade(TRUE);
		}else {
			pticket->SetShortTrade(TRUE);
		}
		pticket->SetCashTrade(TRUE);
		pticket->SetShortTrade(FALSE);

	}else {// the equity
		switch(ntype){
		case 1: //the trade is by cash;
			if (ppos->mqty >=ppos->mHoldqty) {
				takeDayBP=pticket->mprice*pticket->mqty*nDayMarginRatio;
				takeNBP=pticket->mprice*pticket->mqty*nNightMarginRatio;
				ppos->mBuyPwer+=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;// (ppos->mBuyPwer +ppos->mHoldqty*ppos->mInitPrice*nDayMarginRatio)/(newqty*nDayMarginRatio);
			}else if (newqty>ppos->mHoldqty) {
				takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice *nDayMarginRatio;
	//			takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose()*nNightMarginRatio;
				takeNBP=pticket->mprice*pticket->mqty*nNightMarginRatio;
				ppos->mBuyPwer +=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;//(ppos->mBuyPwer +ppos->mHoldqty*ppos->mInitPrice*nDayMarginRatio)/(newqty*nDayMarginRatio);
			}else {
				takeNBP=pticket->mprice*pticket->mqty*nNightMarginRatio;//(pticket->mqty)*ppos->GetYClose()*nNightMarginRatio;
	//			ppos->mavgcost=newcost/newqty;
			}
			pticket->SetCashTrade(TRUE);
			pticket->SetShortTrade(FALSE);
			break;
		case 2: // the trade is margin
			if (ppos->mqty >=ppos->mHoldqty) {
				if ((pticket->mprice<theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0)) {// less than 5$ require full cash
					takeDayBP=pticket->mprice*pticket->mqty *nDayMarginRatio; 
					takeNBP=pticket->mprice*pticket->mqty*nNightMarginRatio;
				}else {
					takeDayBP=pticket->mprice*pticket->mqty;
					takeNBP=pticket->mprice*pticket->mqty;
				}
				ppos->mBuyPwer+=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;//(ppos->mBuyPwer +ppos->mHoldqty*ppos->mInitPrice)/newqty;
			}else if (newqty>ppos->mHoldqty) {
				if ((pticket->mprice <theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0)) {// less than 5$
					takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice *nDayMarginRatio;
	//				takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose() *nNightMarginRatio;
					takeNBP=pticket->mprice*pticket->mqty *nNightMarginRatio;
				}else {
					takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice;
	//				takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose();
					takeNBP=pticket->mprice*pticket->mqty;
				}
				ppos->mBuyPwer +=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;
			}else {
	//			takeNBP=(pticket->mqty)*ppos->GetYClose();
	//			ppos->mavgcost=newcost/newqty;
				takeNBP=pticket->mprice*pticket->mqty;
				
	//			ppos->mavgcost=ppos->mInitPrice;
			}
			if (mitemifo.IsDVPAccount()) {
				pticket->SetCashTrade(TRUE);
			}else {
				pticket->SetCashTrade(FALSE);
			}
			pticket->SetShortTrade(FALSE);
			break;
		case 3: // short sell
			if (ppos->mqty >=ppos->mHoldqty) {
				if ((pticket->mprice <theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0)) {// less than 5$
					takeDayBP=pticket->mprice*pticket->mqty *nDayMarginRatio*nShortHoldRatio ;
					takeNBP=pticket->mprice*pticket->mqty *nNightMarginRatio*nShortHoldRatio ; 
				}else {
					takeDayBP=pticket->mprice*pticket->mqty*nShortHoldRatio ;
					takeNBP=pticket->mprice*pticket->mqty*nShortHoldRatio ; 
				}
				ppos->mBuyPwer+=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;
			}else if (newqty>ppos->mHoldqty) {
				if ((pticket->mprice <theCfgFile.mFullRequireSwitch)&&((mitemifo.mstatus&(1<<26))==0)) {
					takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice *nDayMarginRatio*nShortHoldRatio ;
	//				takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose()*nNightMarginRatio;
					takeNBP=pticket->mprice*pticket->mqty*nNightMarginRatio*nShortHoldRatio ;
				}else {
					takeDayBP=(newqty-ppos->mHoldqty)* pticket->mprice*nShortHoldRatio  ;
	//				takeNBP=(ppos->mHoldqty-ppos->mqty)*ppos->GetYClose();
					takeNBP=pticket->mprice *pticket->mqty*nShortHoldRatio ;
				}
				ppos->mBuyPwer +=takeDayBP;
	//			ppos->mavgcost=newcost/newqty;
			}else {
	//			ppos->mavgcost=newcost/newqty;
	//			ppos->mavgcost=ppos->mInitPrice;
	//			takeNBP=(pticket->mqty)*ppos->GetYClose();
				takeNBP=pticket->mqty *pticket->mprice*nShortHoldRatio ;
			}
			if (mitemifo.IsDVPAccount()) {
				pticket->SetShortTrade(FALSE);
				pticket->SetCashTrade(TRUE);
			}else {
				pticket->SetShortTrade(TRUE);
			}
			/*
			if(pticket->mstatus &(1<<29)) { // short need to reduce bullet.
				itemPosition *pbullet; 
				pbullet=FindPosition(pticket->msecsym,4,pposlst);
				if (pbullet) {
					pbullet->mqty-=pticket->mqty; 
					if ( pbullet->mqty <0) pbullet->mqty=0;
					time(&pbullet->mm_date); 
				}
			
			}
			*/
			break;
		}; 
	}
	
	if (newqty>ppos->mHoldqty) {
		if (ppos->mqty>ppos->mHoldqty) {
			ppos->mavgcost=(oldcost+ pticket->mprice *pticket->mqty)/newqty;
		}else {
			double holdcost;
			holdcost=ppos->mInitPrice *ppos->mHoldqty; 
			ppos->mavgcost=(holdcost+(newqty-ppos->mHoldqty)*pticket->mprice)/newqty;
		}
	}else {
		ppos->mavgcost=ppos->mInitPrice; 
	}


	pequity->mCurrBP-=takeDayBP;

	pequity->mCurrNBP-=0;//takeNBP;

	if (mitemifo.IsDVPAccount()) {
		if (pticket->IsBuyTrade())
			pticket->SetCoverTicket(FALSE);
		else 
			pticket->SetCoverTicket(TRUE);
	}else {
		pticket->SetCoverTicket(FALSE);
	}
	ppos->mqty =newqty;


	double newcost,buycost;
	newcost=ppos->mavgcost*newqty;
	buycost=oldcost+ pticket->mprice *pticket->mqty;
	double pl=0;
	switch (ntype) {
	case 1:
	case 2:
		pl=(newcost-buycost)*undshr;
		ppos->mRealize+=pl;
		pequity->mRealized+=pl;;
		break;
	case 3:
		pl=(buycost-newcost)*undshr;
		ppos->mRealize+=pl;
		pequity->mRealized+=pl;
		break;
	}

	if (mitemifo.mstatus&(1<<30)) {// PLBP change buying power with the P/L
		pequity->mCurrBP+=pl;	
		pequity->mCurrNBP+=pl; 

	}


	time(&ppos->mm_date);
	ptktlst->AddAscent((long)pticket->me_date,(long)pticket);
	if (ptktlst==&mLstTickets) {
		pticket->mticketid=theCfgFile.GetNewTicketID();// InterlockedIncrement((long*)&theCfgFile.mCurrTicketID);  
	}
	return ;
}

void Account::TktPosUpdate(intlst *pposlst, BOOL bSendDiff)
{
	mLstPositions.LockLst();
	intitem *pintitem;
	itemPosition *ppos;
	time_t newtime;
	time(&newtime);
	char msgbuf[300];
	mLstPositions.EnumFirst(&pintitem);
	itemPosition *pnewpos;
	int key;
	
	while (pintitem!=NULL){
		ppos=(itemPosition*)pintitem->val;
		//find the position in the pposlst;
		pposlst->GetHead(&key,(int*)&pnewpos);
		ASSERT(pnewpos!=NULL);
		ASSERT(pnewpos->mptype ==ppos->mptype);
		ASSERT(strcmp(pnewpos->msecsym,ppos->msecsym)==0);
		if ((pnewpos->mqty!=ppos->mqty)||(pnewpos->mavgcost!=ppos->mavgcost))  {
			ppos->mqty=pnewpos->mqty;
			ppos->mavgcost=pnewpos->mavgcost;
			ppos->mBuyPwer=pnewpos->mBuyPwer;
			ppos->mRealize=pnewpos->mRealize;
			ppos->mHoldqty=pnewpos->mHoldqty;
			ppos->mm_date=newtime;
			if (bSendDiff){
				ppos->MakeInfoPkg(msgbuf,0xffff);
				Trader::SPKGAcc(msgbuf,mitemifo.maccid,mitemifo.mbrid,0);
			}
		}
		itemPosition::PutVoidBuff(pnewpos);
		//end of the different;
		mLstPositions.EnumNext(&pintitem);
	}
	while(pposlst->GetHead(&key,(int*)&pnewpos)) {
		pnewpos->mm_date=newtime;
		pnewpos->MakeInfoPkg(msgbuf,0xffff);
		Trader::SPKGAcc(msgbuf,mitemifo.maccid,mitemifo.mbrid,0);
		mLstPositions.AddTail(0,(int)pnewpos);
	}
	mLstPositions.UnlockLst();
	return;

}

void Account::TktEquityUpdate(itemAccEquity *pequity, BOOL bSendDiff)
{
	WORD eqdiff;
	eqdiff=pequity->CompareItem(&mitemEquity);
	if (eqdiff) {
		mitemEquity.CopyFrom(pequity);
		if (bSendDiff) {
			char pkg[200];
			MakePkgByEquity(pkg,eqdiff);
			Trader::SPKGAcc(pkg,mitemifo.maccid,mitemifo.mbrid,0);  
		}
	}
}

void Account::TktTicketUpdate(intlst *pticketlst)
{
	intitem *pintitem;
	itemTicket *pticket;
	time_t newtime;
	itemTicket *pnewticket;
	int key;
	
	time(&newtime);
	mLstTickets.LockLst();
	mLstTickets.EnumFirst(&pintitem);
	while (pintitem!=NULL){
		pticket=(itemTicket*)pintitem->val;
		//find the position in the pposlst;
		if (pticketlst->GetHead(&key,(int*)&pnewticket)) {
			long ticketid;
			ticketid=pticket->mticketid;
			pticket->CopyFrom(pnewticket);
			pticket->mm_date=newtime;
			pticket->mticketid=ticketid;
			pnewticket->PutToBuff(); 
		}else {
			pticket->mqty=0;
		};
		mLstTickets.EnumNext(&pintitem);
	}
	while(pticketlst->GetHead(&key,(int*)&pnewticket)) {
		pnewticket->mm_date=newtime;
		pnewticket->mticketid=theCfgFile.GetNewTicketID();// InterlockedIncrement((long*)&theCfgFile.mCurrTicketID);  
		mLstTickets.AddTail(0,(int)pnewticket);
	}
	mLstTickets.UnlockLst();
	return;
}

//this function is used to Add a trade to account 
// And this trade must not been combine with other tickets
// and will not change the order of execution.
BOOL Account::TktPatchAddTrade(itemTrade *ptrade, BOOL bSendPos, BOOL bSendEquity)
{
	if (!ptrade->IsOpen()) return TRUE;
	itemAccEquity tmpEquity;// tempory equity of this account;
	tmpEquity.CopyFrom(&mitemEquity);
//-----recalculate the trades	-----	
	itemTicket *pticket;
	time_t tm1;
	time(&tm1);
	pticket=itemTicket::GetVoidBuff();
	TktCreateTicketFromTrade(pticket,ptrade);
	TktTicketAddEffect(pticket,	&mLstTickets,&mLstPositions,&tmpEquity);
	if (bSendPos) {
		//send the position which is modify letter than tm1;
		TktPosRefresh(tm1);
	}
	TktEquityUpdate(&tmpEquity,bSendEquity); 
	
	return TRUE;
}

//this function is send the position information 
// to the client. only positions which modify time later than 
// lastmodify will be send to client
//
void Account::TktPosRefresh(time_t lastmidify)
{
	mLstPositions.LockLst();
	intitem *pintitem;
	itemPosition *ppos;
	char msgbuf[300];
	mLstPositions.EnumFirst(&pintitem);
	while (pintitem!=NULL){
		ppos=(itemPosition*)pintitem->val;
		//find the position in the pposlst;
		if (ppos->mm_date>=lastmidify){
			ppos->MakeInfoPkg(msgbuf,0xffff);
			Trader::SPKGAcc(msgbuf,mitemifo.maccid,mitemifo.mbrid,0);
		}
		//end of the different;
		mLstPositions.EnumNext(&pintitem);
	}
	mLstPositions.UnlockLst();
	return;

}

BOOL Account::TktNeedRecalcuate(itemTrade *ptrade)
{
	intitem *PITicket;
	itemTicket *pticket;
	BOOL finded;
	finded=FALSE;
	if (!ptrade->IsOpen()) return FALSE;
	mLstTickets.LockLst();
	mLstTickets.EnumFirst(&PITicket);
	while(PITicket) {
		pticket=(itemTicket*)PITicket->val;
		ASSERT(pticket!=NULL);
		if (strcmp(pticket->msecsym,ptrade->msecsym)==0) {
			if (pticket->me_date>ptrade->me_date) {
				finded=TRUE;
				break;
			}
			if (pticket->morderid==ptrade->morderid){
				if (ptrade->morderid!=0){
					finded=TRUE;
					break;
				}
			}
		}
		mLstTickets.EnumNext(&PITicket);
	}
	mLstTickets.UnlockLst();
	return finded;
}

BOOL Trader::FindTraderAccount(int ntrid, long *pacc, int *num)
{
	ASSERT(pacc!=NULL);
	ASSERT(*num>1);
	Trader *ptr;
	int retnum;
	retnum=0;
	ptr=FindTrader(ntrid);
	if (ptr!=NULL) {
		AC_TR_relation *pAC_TR;
		ptr=Trader::FindTrader(ntrid);
		if (ptr!=NULL) {
			pAC_TR=ptr->pFistTrader;
			while(pAC_TR!=NULL) {
				pacc[retnum]=pAC_TR->maccid;
				retnum++;
				if (retnum>=*num) break;
				pAC_TR=pAC_TR->pnexttr;
			}
		}
	} 
	*num=retnum;
	return TRUE;
}

BOOL Account::SaveAllData()
{
	char stst[1000], sttm[30];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Manually Savedata:%s",sttm);
	theCfgFile.mlog.AddLogStr(stst);
	if (theCfgFile.mRealTimeDBWrite==FALSE) {//write the order and trades to database;
		BOOL  bret;
		bret=itemOrder::SaveAllToDB();
		theCfgFile.mlog.AddLogStrFmt("Save order DB %s",bret?"Successful":"Fail");  
		bret=itemTrade::SaveAllToDB();
		theCfgFile.mlog.AddLogStrFmt("Save trade DB %s",bret?"Successful":"Fail");
	}
	SaveAllTickets();
	SaveAllPosition();
	SaveAllEquityInfo();
	return TRUE;
}

BOOL Account::SaveAllPosition()
{


	char sttm[30];
	char stst[100];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Save Position Info,%s",sttm);
	theCfgFile.mlog.AddLogStr(stst);

	sMapAccid.LockMap();

	srstPosition rstpos;
	theOrderDB.mdb.ExecuteSQL("delete mNewPosition");
	rstpos.init(&theOrderDB.mdb);
	rstpos.OpenToUpdate(TRUE,0,"",0);
	intitem *pintitem;
	intitem *PIpos;
	itemPosition *ppos;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstPositions.LockLst();
		pacc->mLstPositions.EnumFirst(&PIpos);
		while(PIpos!=NULL) {
			ppos=(itemPosition*)PIpos->val;
			rstpos.mitem.CopyFrom(ppos);
			rstpos.AddNew();
			pacc->mLstPositions.EnumNext(&PIpos);
		}
		pacc->mLstPositions.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	rstpos.Close();
	return TRUE;
}

BOOL Account::SaveAllTickets()
{
	char sttm[30];
	char stst[50];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Save Tickets Info,%s",sttm);
	theCfgFile.mlog.AddLogStr(stst);

	sMapAccid.LockMap();
	srstTicket rstticket;
	theOrderDB.mdb.ExecuteSQL("delete mTickets");
	rstticket.init(&theOrderDB.mdb);
	rstticket.Open();
	intitem *pintitem;
	intitem *PIticket;
	itemTicket *pticket;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstTickets.EnumFirst(&PIticket);
		while(PIticket!=NULL) {
			pticket=(itemTicket*)PIticket->val;
			rstticket.mitem.CopyFrom(pticket);
			try{
				rstticket.AddNew();
			}catch (SRSTErr *perr) {
				char str[100];
				sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
				theCfgFile.mlog.AddLogStr(str);
				theCfgFile.mlog.AddLogStr(perr->merrmsg);
				delete perr;
				return FALSE;
			}
			pacc->mLstTickets.EnumNext(&PIticket);
		}
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	rstticket.Close();
	return TRUE;
}

BOOL Trader::DispatchMessage(itemMessage *pitem, itemTrader *ptrader)
{
	itemTrader *psendtrader;
//	itemTrader *precvtrader;
	Trader *ptr;
	if (pitem==NULL) return FALSE;
	if (ptrader) psendtrader=ptrader;
	else {
		ptr=Trader::FindTrader(pitem->mFr); 
		if (ptr==NULL) {
			return FALSE;
		}
		psendtrader=&ptr->minfo;
	}

	CString msgto(pitem->mTo);
	msgto.TrimLeft();
	CString allstr;
	itemBranch *pbr;
	long brid;
	AfxExtractSubString(allstr,msgto,0,':');
	srstMessage srst;

	if (allstr.CompareNoCase("ALL")==0) {
		if (psendtrader->IsTrader()) return FALSE; 
		CString brstr;
		if (AfxExtractSubString(brstr,msgto,1,':')==TRUE) {
			pbr=theCfgFile.mpItemPool->GetBranch(brstr); 
			if (pbr==NULL){
				return NULL;
			}else {
				brid=pbr->mbrid;
			}
			if (psendtrader->mbrid!=0) {
				if (brid!=psendtrader->mbrid) {
					return FALSE;
				}
			}
		}else {
			brid=psendtrader->mbrid;
		}

		intitem *PItr;
		Trader::sMapTrid.EnumFirst(&PItr);
		while(PItr) {
			ptr=(Trader*)PItr->val;
			if (ptr!=NULL){
				if ((ptr->minfo.mbrid==brid)||(brid==0)){
					pitem->mid=ptr->minfo.mtrid; 
					pitem->mflag &= ~1;
					if (ptr->mpClient!=NULL) {
						ptr->mpClient->SendMsg( pitem);
						pitem->mflag|=1;
					}else {
					}
					srst.AddToDb(pitem);
				}
			}
			Trader::sMapTrid.EnumNext(&PItr);
		}
	}else {
		ptr=Trader::FindTrader(pitem->mTo);
		if (ptr==NULL) return FALSE;
		else {
			pitem->mid=ptr->minfo.mtrid; 
			pitem->mflag&= ~1;
			if (ptr->mpClient!=NULL) {
				ptr->mpClient->SendMsg( pitem);
				pitem->mflag|=1;
			}else {
			}
			srst.AddToDb(pitem);
		}
	}
	return TRUE;
}

int Trader::LogItOut(LPCSTR pmsg)
{
	
	if (mpClient!=NULL){
		Trader::sMapTrid.LockMap();
		PkgToken tok; 
		mServerId=0;
		mpClient->mptrifo=NULL;
		char  retbuff[1000];
		tok.InitToSet(retbuff); 
		tok.setWORD(4); //the return package for login
		if (pmsg!=NULL){
			tok.setVarStr(pmsg);
		}else {
			tok.setVarStr("");
		}
		//dabuff *ppkgloginret;
		//ppkgloginret=dabuff::GetVoidDaBuff();
		//ppkgloginret->AddPkgBuff(tok.finalizeSet());
		mpClient->SendPkg(tok.finalizeSet());
		mpClient->mtrid=0;
		mpClient->Shutdown();


		mpClient->AsyncSelect(FD_WRITE);
		int ri;
		ri=0;
		if (IsTrader()) {
			ri=mOnlineTrader.RemoveVal(minfo.mtrid,(long)mpClient); 
			if (ri==0) {
				ri=mOnlineAdmin.RemoveVal(minfo.mtrid,(long)mpClient); 
			}
		}else {
			ri=mOnlineAdmin.RemoveVal(minfo.mtrid,(long)mpClient); 
			if (ri==0){
				ri=mOnlineTrader.RemoveVal(minfo.mtrid,(long)mpClient); 
			}
		}
		if (ri) mpClient->RefDec();
		char str[500];
		CTime t1;
		t1=CTime::GetCurrentTime(); 
		if (pmsg!=NULL)	sprintf(str,"KickOut,%s,%s,%s",minfo.mtracno,t1.Format("%Y/%m/%d %H:%M:%S"),pmsg); 
		else sprintf(str,"KickOut,%s,%s",minfo.mtracno,t1.Format("%Y/%m/%d %H:%M:%S")); 
		theCfgFile.mLoginLog.AddLogStr(str);
		mpClient=NULL;
		
		Trader::sMapTrid.UnlockMap();
	}
	return 0;
}


void Account::UnLock()
{
	InterlockedIncrement(&mLockNum);
}

void Account::Lock()
{
	while (InterlockedDecrement(&mLockNum) < -1) {
		InterlockedIncrement(&mLockNum);
		Sleep(0);
	}
}

BOOL Trader::SPKGTrader(char *pkg, long trid)
{
	BOOL msended;
	msended=FALSE;
	CClient *pclient;
	intitem *pclientitem;
	intlst senlst;

	mOnlineTrader.LockMap();
	pclientitem=mOnlineTrader.FindItem(trid);
	if (pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		senlst.AddTail(0,pclientitem->val);
		pclient->RefInc();
//		pclient->SendPkg(pkg);
//		pclient->AsyncSelect(FD_WRITE); 
		msended=TRUE;
	}
	mOnlineTrader.UnlockMap(); 
	if (msended==TRUE) return TRUE;
	
	mOnlineAdmin.LockMap();
	pclientitem=mOnlineAdmin.FindItem(trid);
	if (pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		senlst.AddTail(0,pclientitem->val);
		pclient->RefInc();
		//pclient->SendPkg(pkg);
		//pclient->AsyncSelect(FD_WRITE); 
		msended=TRUE;
	}
	mOnlineAdmin.UnlockMap(); 


	int nkey,nval;
	while (senlst.GetHead(&nkey,&nval)) {
		CClient *pclient=(CClient *)nval;
		pclient->SendPkg(pkg);
		pclient->AsyncSelect(FD_WRITE);
		pclient->RefDec();
	}


	return TRUE;

}

BOOL Account::CheckOrderBP(itemOrder *porder,char *pmsg)
{
	if (mitemifo.mstatus&(1<<15)){
		return CheckOrderBP_Ext(porder,pmsg);
	}


	Account::stPosInfo stposifo,stordifo;
	double holdbp=0;
	int bpqty;

	int nTotalPos=0;

	if (porder==NULL) return FALSE;
	if (porder->msecsym[0]=='+') {// option
		FindPosInfo(porder->msecsym,&stposifo); 
		FindOrderHoldIfo(porder->msecsym,&stordifo);
		if (porder->IsBuyOrder()) {
			bpqty=porder->mqty*stposifo.m_primshr;
			if (stposifo.m_optShort>0) bpqty-=stposifo.m_optShort;
			if (stordifo.m_optLong>0)  bpqty+=stposifo.m_optLong;
			if (bpqty>(porder->mqty*stposifo.m_primshr)) bpqty=porder->mqty*stposifo.m_primshr;
			if (bpqty<0) bpqty=0;
			holdbp=porder->GetHoldBPRate(this)*bpqty +theCfgFile.mCommBPPreHold;
		}else { //write options
			int covqty=0;
			int  c=0;
			bpqty=porder->mqty*stposifo.m_primshr;
			if (stposifo.m_optLong>0) bpqty-=stposifo.m_optLong;
			if (stordifo.m_optShort>0) bpqty+=stposifo.m_optShort;
			if (bpqty>(porder->mqty*stposifo.m_primshr)) bpqty=porder->mqty*stposifo.m_primshr;
			if (bpqty<0) bpqty=0;

			
			if (stposifo.mbCall) {//call
				if (stposifo.mLong>0) covqty=stposifo.mLong;
				if (stposifo.moptCWrite>0) covqty-=stposifo.moptCWrite; 
				if (stordifo.mShort>0) covqty-=stordifo.mShort;
				if (stordifo.moptCWrite>0)covqty-=stordifo.moptCWrite;

			}else {//puts
				if (stposifo.mShort>0) covqty=stposifo.mShort;
				if (stposifo.moptPWrite>0) covqty-=stposifo.moptPWrite; 
				if (stordifo.mLong>0) covqty-=stordifo.mLong;
				if (stordifo.moptPWrite>0)covqty-=stordifo.moptPWrite;
			}
			if (covqty<0) covqty=0;
			if (covqty>bpqty) bpqty=0;
			else bpqty-=covqty;
			holdbp=porder->GetHoldBPRate(this)*bpqty +theCfgFile.mCommBPPreHold;

		}
		
	}else { // equity  and futures
		

		FindPosInfo(porder->msecsym,&stposifo); 
		FindOrderHoldIfo(porder->msecsym,&stordifo);
		if (porder->IsBuyOrder()) {
			bpqty=porder->mqty;
			if ((porder->mstatus &(1<<29))==0) {
				if (stposifo.mShort>0) bpqty-=stposifo.mShort;
			}
			if (stordifo.mLong>0)  bpqty+=stordifo.mLong;
			if (bpqty>porder->mqty) bpqty=porder->mqty;
			if (bpqty<0) bpqty=0;
		}else {
			bpqty=porder->mqty;
			if ((porder->mstatus &(1<<29))==0) {
				if (stposifo.mLong>0) bpqty-=stposifo.mLong;
			}
			if (stordifo.mShort>0) bpqty+=stordifo.mShort;
			if (bpqty>porder->mqty) bpqty=porder->mqty;
			if (bpqty<0)bpqty=0;		
		}
		if (bpqty>0) {
			holdbp=porder->GetHoldBPRate(this)*bpqty +theCfgFile.mCommBPPreHold;
			//porder->mstatus&=~(1<<22);
		} else {
			//porder->mstatus|=(1<<22);

		}
		if (mitemCtrl.maxsymbshare>0) {
			if (porder->IsBuyOrder()) {
				if (( stposifo.mLong +porder->mqty )>mitemCtrl.maxsymbshare) {
					strcpy(pmsg,"Symb Share Limt!");
					return FALSE;
				}
			}else {
				if ((stposifo.mShort+porder->mqty)>mitemCtrl.maxsymbshare) {
					strcpy(pmsg,"Symb Share Limt!");
					return FALSE;
				}
			}
			
		}
	}



	if (holdbp>0) {
		if (mitemifo.mstatus&(1<<17)) {
			if (mitemEquity.mCurrBP>=holdbp){
				if ((mitemEquity.mOpenBP*theCfgFile.mOrderBpCtrlRatio)>=(mitemEquity.mCurrHoldBP+holdbp)){
					return TRUE;
				}else {
					strcpy(pmsg,"BuyingPower!");
					return FALSE;
				}
			}else {
				strcpy(pmsg,"BuyingPower!");
				return FALSE;
			}
		}else {
			if ((mitemEquity.mCurrBP -mitemEquity.mCurrHoldBP)>=holdbp){
				return TRUE;
			}else {
				strcpy(pmsg,"BuyingPower!");
				return FALSE;
			}
		}
	}
	
	return TRUE;
}

int Account::FindOrderHoldQty(LPCSTR secsym, BOOL bBuy)
{

	int holdqty=0;
	itemOrder * p;
	intitem *PIorder;
	mLstOrders.EnumFirst(&PIorder);
	if (bBuy) {
		while(PIorder) {
			p=(itemOrder *)PIorder->val;
			if (p!=NULL){
				if ((p->IsBuyOrder())&&
					(p->IsOpenOrder()) &&
					(strcmp(secsym,p->msecsym)==0)) {
					holdqty+=p->mlvsqty;
				}
			}
			mLstOrders.EnumNext(&PIorder); 
		}
	}else {
		while(PIorder) {
			p=(itemOrder *)PIorder->val;
			if (p!=NULL){
				if ((!p->IsBuyOrder())&&
					(p->IsOpenOrder()) &&
					(strcmp(secsym,p->msecsym)==0)) {
					holdqty+=p->mlvsqty;
				}
			}
			mLstOrders.EnumNext(&PIorder); 
		}
	}
	return holdqty;
}

BOOL Account::SaveAllPositionToFile(LPCSTR fname)
{

	char sttm[30];
	char stst[50];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Save All Position to file,%s",sttm);
	theCfgFile.mlog.AddLogStr(stst);

	CFile posfile;
	BOOL bret;
	bret=posfile.Open(fname,CFile::modeWrite |CFile::modeCreate |CFile::shareDenyNone);
	if (bret==FALSE) return FALSE;
	char posst[200];
	char accst[100];
	char ctmst[100];
	char mtmst[100];
	sprintf(posst,"Seq,Branch,Account,Symbol,Type,InitQty,"
							  "InitPrice,Qty,AvgCost,Hold,BP,"
							  "Realized,CTime,MTime\xd\xa");
	posfile.Write(posst,strlen(posst)); 

	sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PIpos;
	itemPosition *ppos;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	int counter=0;
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstPositions.LockLst();
		pacc->mLstPositions.EnumFirst(&PIpos);
		while(PIpos!=NULL) {
			ppos=(itemPosition*)PIpos->val;
			//save the position;
			//branch,acc,secsym,type,initqty,
			//initprice,qty,avgcost,holqty,bpr,
			//realize,c_date,m_date;
			Account *pacc;
			pacc=FindAcc(ppos->maccid);
			if (pacc) {
				strcpy(accst,pacc->mitemifo.mname); 
				tm *ptm;
				tm tm1;
				if (ppos->mc_date>0) {
					ptm=localtime(&(ppos->mc_date));
					memcpy(&tm1,ptm,sizeof(tm));
					sprintf(ctmst,"%d/%d/%d %d:%d:%d",
						tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday,    
						tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
				}else {
					ctmst[0]=0;
				}
			
				if (ppos->mm_date>0) {
					ptm=localtime(&(ppos->mm_date));
					memcpy(&tm1,ptm,sizeof(tm));
					sprintf(mtmst,"%d/%d/%d %d:%d:%d",
						tm1.tm_year+1900,tm1.tm_mon+1,tm1.tm_mday,    
						tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
				}else {
					ctmst[0]=0;
				}
				counter++;
				sprintf(posst,"%d,%d,%s,%s,%d,%d,"
							  "%.20g,%d,%.20g,%d,%.20g,"
							  "%.20g,%s,%s\xd\xa",
							  counter,
							  ppos->mbrid,accst,ppos->msecsym,ppos->mptype,ppos->mInitQty,
							  ppos->mInitPrice,ppos->mqty,ppos->mavgcost,ppos->mHoldqty,ppos->mBuyPwer,
							  ppos->mRealize,ctmst,mtmst);
				posfile.Write(posst,strlen(posst)); 
			}
			pacc->mLstPositions.EnumNext(&PIpos);
		}
		pacc->mLstPositions.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}

	sMapAccid.UnlockMap();
	posfile.Close();
	return TRUE;
}

BOOL Account::CountAllTickets(int &numtotal,int &numreal,double &comm)
{
	int ntotal;
	int nreal;
	double dcomm;
	ntotal=0;
	nreal=0;
	dcomm=0;

	sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PIticket;
	itemTicket *pticket;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstTickets.LockLst(); 
		pacc->mLstTickets.EnumFirst(&PIticket);
		while(PIticket!=NULL) {
			pticket=(itemTicket*)PIticket->val;
			if (pticket->IsOpen()) {
				ntotal++;
				if (!pticket->IsTraining()){
					if (!theCfgFile.IsTestSecurity(pticket->msecsym)){
						nreal++;
						dcomm+=pticket->mcommision;
					}
				}
			}
			pacc->mLstTickets.EnumNext(&PIticket);
		}
		pacc->mLstTickets.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	numtotal=ntotal;
	numreal=nreal;
	comm=dcomm;
	return TRUE;
}

BOOL Account::CountAllOrders(int &numtotal, int &numreal, int &numopen, int &numexed, int &numexecs)
{
	int ntotal,nreal,nopen,nexed,nexecs;
	
	ntotal=0;
	nreal=0;
	nopen=0;
	nexed=0;
	nexecs=0;

	sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PIorder;
	itemOrder *porder;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstOrders.LockLst();
		pacc->mLstOrders.EnumFirst(&PIorder);
		while(PIorder!=NULL) {
			porder=(itemOrder*)PIorder->val;
			ntotal++;
			if (!porder->IsTestOrder()){
				if (!theCfgFile.IsTestSecurity(porder->msecsym)){
					nreal++;
					if (porder->IsOpenOrder()) nopen++;
					if (porder->mexecnum>0) {
						nexed++;
						nexecs+=porder->mexecnum;
					}
				}
			}
			pacc->mLstOrders.EnumNext(&PIorder);
		}
		pacc->mLstOrders.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	numtotal=ntotal;
	numreal=nreal;
	numopen=nopen;
	numexed=nexed;
	numexecs=nexecs;
	return TRUE;

}

BOOL Account::CountAllTrades(int &numtotal, int &numreal, int &numadd, int &numcanced)
{
	int ntotal,nreal,nadd,ncxl;
	
	ntotal=0;
	nreal=0;
	nadd=0;
	ncxl=0;

	sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PItrade;
	itemTrade *ptrade;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstTrades.LockLst();
		pacc->mLstTrades.EnumFirst(&PItrade);
		while(PItrade!=NULL) {
			ptrade =(itemTrade *)PItrade->val;
			ntotal++;
			if (!ptrade->IsTraining()){
				if (!theCfgFile.IsTestSecurity(ptrade->msecsym)){
					if (ptrade->IsOpen()) {
						nreal++;
						if (ptrade->morderid==0) nadd++;
					}else {
						ncxl++;
					}
				}
			}
			pacc->mLstTrades.EnumNext(&PItrade);
		}
		pacc->mLstTrades.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	numtotal=ntotal;
	numreal=nreal;
	numadd=nadd;
	numcanced=ncxl;
	return TRUE;

}

BOOL Account::CancelAllOpenOrders(LPCSTR proute)
{

	int ntotal,nopen,nsended;
	
	
	ntotal=0;
	nopen=0;
	nsended=0;

	sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PIorder;
	itemOrder *porder;
	Account *pacc;
	sMapAccid.EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		pacc->mLstOrders.LockLst();
		pacc->mLstOrders.EnumFirst(&PIorder);
		while(PIorder!=NULL) {
			porder=(itemOrder*)PIorder->val;
			ntotal++;
			if (porder->IsOpenOrder()) {
				BOOL bIsGTC=FALSE;
				if (porder->IsGTCOrder()) {
					bIsGTC=TRUE;
				}
				nopen++;
				BOOL bFilter=FALSE;
				if (porder->mlvsqty<=0){
					bFilter=TRUE;
				}else if (proute!=NULL){
					if (strcmp(porder->mroute,proute)!=0) bFilter=TRUE;
				}
				if (!bFilter) {
					OrderRouter *proute;
					proute=theCfgFile.FindRouter(porder);
					if (proute&&proute->IsEnabled()) {
						BOOL ret;
						char msgstr[50];
						memset(msgstr,0,sizeof(msgstr));
						ret=proute->SendCancelOrder(porder,0,msgstr);
						if (bIsGTC) {
							porder->ms1&=~1;
						}
						nsended++;
					}
				}
			}
			pacc->mLstOrders.EnumNext(&PIorder);
		}
		pacc->mLstOrders.UnlockLst();
		sMapAccid.EnumNext(&pintitem);
	}
	sMapAccid.UnlockMap();
	theCfgFile.mlog.AddLogStrFmt("Cancel All order:%d total,%d open, sent %d cancels",
		ntotal,nopen,nsended);
	return TRUE;
}


BOOL Account::RecoverFromLog(char *plogfile)
{
	CFile f1;
	if (f1.Open(plogfile,CFile::modeRead|CFile::shareDenyNone)==FALSE) return TRUE;
	char buff[2000];
	CArchive ar(&f1,CArchive::load,1000,buff);
	char st[2000];
	intmap tmaporder;
	intmap tmaptrade;
	itemOrder od1,*pod;
	itemTrade tr1,*ptr; 
	intitem *PIitem;
	tmaporder.init(1000);
	tmaptrade.init(1000);
	while(ar.ReadString(st,sizeof(st))) {
		if (st[0]=='%') {
			if (st[1]=='O') {
				od1.LoadStr(st);
				PIitem=tmaporder.FindItem(od1.morderid);
				if (PIitem) {
					pod=(itemOrder*)PIitem->val;
					pod->CopyFrom(&od1);
				}else {
					pod=(itemOrder*)itemOrder::sitembuff.GetVoidBuff();
					pod->CopyFrom(&od1);
					tmaporder.Add(od1.morderid,(long)pod);
				}
			}else if (st[1]=='T'){
				tr1.LoadStr(st);
				PIitem=tmaptrade.FindItem(tr1.mtradeid);
				if (PIitem) {
					ptr=(itemTrade*)PIitem->val;
					ptr->CopyFrom(&tr1);
				}else {
					ptr=itemTrade::GetVoidBuff();
					ptr->CopyFrom(&tr1);
					tmaptrade.Add(ptr->mtradeid,(long)ptr);
				}
			}else if (st[1]=='M') {
				itemManualOrder mod,*pmod;
				mod.LoadStr(st);
				PIitem=itemManualOrder::smapOrderid.FindItem(mod.id);
				if (PIitem!=NULL){
					pmod=(itemManualOrder*)PIitem->val;
					memcpy(pmod,&mod,sizeof(itemManualOrder));
				}else {
					pmod=(itemManualOrder*)itemManualOrder::sOrderBuffer.GetVoidBuff();
					memcpy(pmod,&mod,sizeof(itemManualOrder));
					itemManualOrder::smapOrderid.Add(mod.id,(long)pmod);
				}
				theCfgFile.SetMaxOrderID(mod.id);
			}else if (strncmp(st+1,"SCLOSESTAT",10)==0) {
				int status=atoi(st+11);
				theCfgFile.mIsClosed=status;
			}
		}
	}
	ar.Close();
	f1.Close();
//------ compare;
	if ((tmaptrade.GetCounter()!=itemTrade::sMapTradeID.GetCounter())||
		(tmaporder.GetCounter()!=itemOrder::sMapOrderID.GetCounter())) {//Trade and order is not the same;
		CString str;
		char buff[100];
		sprintf(buff,"LogFile has %d orders,Database has %d orders",
				tmaporder.GetCounter(),itemOrder::sMapOrderID.GetCounter());
		theCfgFile.mlog.AddLogStrFmt(buff);
		str=buff;
		str+="\r\n";
		sprintf(buff,"LogFile has %d Trades,Database has %d Trades",
				tmaptrade.GetCounter(),itemTrade::sMapTradeID.GetCounter());
		theCfgFile.mlog.AddLogStrFmt(buff);
		str+=buff;
		str += "\r\n\r\n";
		sprintf(buff,"Use Log files, press Yes\r\n");
		str+=buff;
		sprintf(buff,"Use Database, press No");
		str+=buff;
		int usedb=0;
		if (theCfgFile.mRealTimeDBWrite) { 
			if (AfxMessageBox(str,MB_YESNO)==IDYES) {
				usedb=0;
			}else {
				usedb=1;
			}
		}else {
			//if not realtime write, always use the log rile
			usedb=0;
		}
			
		if (usedb==0){
			theCfgFile.mlog.AddLogStr("Recover from Logfile");

			RemoveAllOrderTrades();
			Account *pacc;
			tmaporder.EnumFirst(&PIitem);
			while(PIitem) {
				pod=(itemOrder*)PIitem->val;
				if (pod) {
					pacc=Account::FindAcc(pod->maccid);
					if (pacc==NULL) {
						theCfgFile.mlog.AddLogStr("ERROR:34876 the Trade can't find account");
					}else {
						PIitem->val=0;
						theCfgFile.SetMaxOrderID(pod->morderid);
						pacc->mLstOrders.AddAscent(pod->mc_date,(long)pod);
						itemOrder::sMapOrderID.Add(pod->morderid,(long)pod); 
					}
				}
				tmaporder.EnumNext(&PIitem); 
			}
			

			tmaptrade.EnumFirst(&PIitem);
			while(PIitem) {
				ptr=(itemTrade*)PIitem->val;
				if (ptr) {
					pacc=Account::FindAcc(ptr->maccid);
					if (pacc==NULL) {
						theCfgFile.mlog.AddLogStr("ERROR:34876 the Trade can't find account");
					}else {
						PIitem->val=0;
						theCfgFile.SetMaxTradeID(ptr->mtradeid);  
						pacc->mLstTrades.AddAscent(ptr->me_date,(long)ptr);
						itemTrade::sMapTradeID.Add(ptr->mtradeid,(long)ptr); 
					}
				}
				tmaptrade.EnumNext(&PIitem); 
			}
			tmaptrade.RemoveAll(); 
			
		};
	}

//------ remove buffer;
	tmaporder.EnumFirst(&PIitem);
	while(PIitem) {
		pod=(itemOrder*)PIitem->val;
		if (pod) itemOrder::sitembuff.PutVoidBuff((char*)pod); 
		tmaporder.EnumNext(&PIitem); 
	}
	tmaporder.RemoveAll(); 
	
	tmaptrade.EnumFirst(&PIitem);
	while(PIitem) {
		ptr=(itemTrade*)PIitem->val;
		if (ptr) ptr->PutToBuff(); 
		tmaptrade.EnumNext(&PIitem); 
	}
	tmaptrade.RemoveAll(); 
	return TRUE;

}

BOOL Account::RemoveAllOrderTrades()
{
	Account::sMapAccid.LockMap();
	intitem *PIitem;
	itemOrder::sMapOrderID.EnumFirst(&PIitem);
	while(PIitem) {
		itemOrder::sitembuff.PutVoidBuff((char*)PIitem->val);
		itemOrder::sMapOrderID.EnumNext(&PIitem); 
	}
	itemOrder::sMapOrderID.RemoveAll();
	itemTrade::sMapTradeID.EnumFirst(&PIitem);
	itemTrade *ptr;
	while(PIitem) {
		ptr=(itemTrade *)PIitem->val;
		ptr->PutToBuff();
		itemTrade::sMapTradeID.EnumNext(&PIitem); 
	}
	itemTrade::sMapTradeID.RemoveAll();
	Account *pacc;
	Account::sMapAccid.EnumFirst(&PIitem);
	while(PIitem) {
		pacc=(Account*)PIitem->val;
		pacc->mLstOrders.RemoveAll();
		pacc->mLstTrades.RemoveAll(); 
		Account::sMapAccid.EnumNext(&PIitem);
	}
	Account::sMapAccid.UnlockMap();
	return TRUE;
}

BOOL Account::FindPosInfo(LPCSTR secsym, Account::stPosInfo *pifo)
{
	memset(pifo,0,sizeof(stPosInfo));
	itemOptsym *poptifo;
	if (secsym[0]=='+') {
		strcpy(pifo->moptSym,secsym); 
		poptifo=itemOptsym::FindInfo(secsym);
		if (poptifo!=NULL){
			strcpy(pifo->msecsym,poptifo->undsymb); 
			pifo->m_primshr=poptifo->pmundlyshr;  
			pifo->mbCall=poptifo->IsCall();
			pifo->mexpday=poptifo->expday;
			pifo->mstrikepice=poptifo->strikepice;  
		}
	} else {
		strcpy(pifo->msecsym,secsym);
	}

	intitem *pintitem;
	itemPosition *pitem; 
	intlst *plst;
	plst=&mLstPositions;
//	if (pPoslst==NULL) plst=&mLstPositions;
//	else plst=pPoslst;
	plst->LockLst();
	plst->EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pitem=(itemPosition*)pintitem->val;
		ASSERT(pitem!=NULL);
		ASSERT(pitem->maccid==mitemifo.maccid); 
		if (pitem->msecsym[0]=='+') {
			if (strcmp(pitem->msecsym,pifo->moptSym)==0) {
				switch(pitem->mptype) {
				case 1: case 2:
					pifo->m_optLong+=pitem->mqty*pifo->m_primshr; 
					break;
				case 3:
					pifo->m_optShort+=pitem->mqty*pifo->m_primshr; 
					break;
				}
			}
			itemOptsym *poptsym;
			poptsym=itemOptsym::FindInfo(pitem->msecsym);
			if (poptsym!=NULL) {
				if (strcmp(poptsym->undsymb,pifo->msecsym)==0) {
					if (poptsym->IsCall()) {
						switch(pitem->mptype) {
						case 1: case 2:
							pifo->moptCLong+=(pitem->mqty*poptsym->pmundlyshr);
							break;
						case 3:
							pifo->moptCWrite+=(pitem->mqty*poptsym->pmundlyshr);
							break;
						}
					}else {
						switch(pitem->mptype) {
						case 1: case 2:
							pifo->moptPLong+=(pitem->mqty*poptsym->pmundlyshr);
							break;
						case 3:
							pifo->moptPWrite+=(pitem->mqty*poptsym->pmundlyshr);
							break;
						}

					}
				}
			}
		}else {
			if ((strcmp(pitem->msecsym,pifo->msecsym)==0)) {
				switch(pitem->mptype) {
				case 1: case 2:
					pifo->mLong+=pitem->mqty;
					break;
				case 3:
					pifo->mShort+=pitem->mqty;
					break;
				}
			}
		}
		plst->EnumNext(&pintitem); 
	}
	plst->UnlockLst();
	return  TRUE;
}

BOOL Account::FindOrderHoldIfo(LPCSTR secsym, Account::stPosInfo *pifo)
{
	memset(pifo,0,sizeof(stPosInfo));
	
	itemOptsym *poptifo;
	if (secsym[0]=='+') {
		strcpy(pifo->moptSym,secsym); 
		poptifo=itemOptsym::FindInfo(secsym);
		if (poptifo!=NULL){
			strcpy(pifo->msecsym,poptifo->undsymb); 
			pifo->m_primshr=poptifo->pmundlyshr;  
			pifo->mbCall=poptifo->IsCall();
			pifo->mexpday=poptifo->expday;
			pifo->mstrikepice=poptifo->strikepice;  
		}
	} else {
		strcpy(pifo->msecsym,secsym);
	}

	itemOrder * p;
	intitem *PIorder;
	mLstOrders.EnumFirst(&PIorder);
	while(PIorder) {
		p=(itemOrder *)PIorder->val;
		if (p!=NULL){
			if (p->IsOpenOrder()){
				if (p->msecsym[0]=='+') {
					if (strcmp(p->msecsym,pifo->moptSym)==0) {
						if (p->IsBuyOrder()) {
							pifo->m_optLong+=p->mlvsqty*pifo->m_primshr; 
						}else {
							pifo->m_optShort+=p->mlvsqty*pifo->m_primshr; 
						}
					}
					itemOptsym *poptsym;
					poptsym=itemOptsym::FindInfo(p->msecsym);
					if (poptsym!=NULL) {
						if (strcmp(poptsym->undsymb,secsym)==0) { 
							if (poptsym->IsCall()) {
								if (p->IsBuyOrder()){
									pifo->moptCLong+=(p->mlvsqty*poptsym->pmundlyshr);
								}else {
									pifo->moptCWrite+=(p->mlvsqty*poptsym->pmundlyshr);
								}
							}else {
								if (p->IsBuyOrder()){
									pifo->moptPLong+=(p->mlvsqty*poptsym->pmundlyshr);
								}else {
									pifo->moptPWrite+=(p->mlvsqty*poptsym->pmundlyshr);
								}
							}
						}
					}
				}else {
					if ((strcmp(pifo->msecsym,p->msecsym)==0)){
						if (p->IsBuyOrder()) {
							pifo->mLong+=p->mlvsqty;
						}else {
							pifo->mShort+=p->mlvsqty; 
						}
					}
					
				}
			}
		}
		mLstOrders.EnumNext(&PIorder); 
	}
	return TRUE;
}

int Account::FindPositionNum(LPCSTR secsym)
{
	intitem *pintitem;
	itemPosition *pitem; 
	itemPosition *pret=NULL;
	int qty;
	qty=0;
	intlst *plst;
	plst=&mLstPositions;
//	if (pPoslst==NULL) plst=&mLstPositions;
//	else plst=pPoslst;
	plst->LockLst();
	plst->EnumFirst(&pintitem);
	while(pintitem!=NULL) {
		pitem=(itemPosition*)pintitem->val;
		ASSERT(pitem!=NULL);
		ASSERT(pitem->maccid==mitemifo.maccid); 
		if (strcmp(pitem->msecsym,secsym)==0){
			switch(pitem->mptype) {
			case 1: case 2:
				qty+=pitem->mqty;
				break;
			case 3:
				qty-=pitem->mqty; 
				break;
			}
		}
		plst->EnumNext(&pintitem); 
	}
	plst->UnlockLst();

	return  qty;
}

BOOL AC_TR_relation::MakeInfoPkgNew(char *buff, BYTE flag)
{
	PkgToken tokret;
	tokret.InitToSet(buff);
	tokret.setWORD(1003);
	tokret.setBYTE(flag);
	tokret.setlong(mtrid);
	tokret.setlong(maccid);
	tokret.setlong(mstatus);
	tokret.setlong(mdealid);
	tokret.setlong(mm_date);
	tokret.setlong(mOpdealid);
	tokret.setlong(mFtdealid);
	tokret.setBYTE(0);
	tokret.finalizeSet();
	return TRUE;
	

}

BOOL AC_TR_relation::GetPkgNew(PkgToken *ptok)
{
	mtrid=ptok->getlong();
	maccid=ptok->getlong();
	mstatus=ptok->getlong();
	mdealid=ptok->getlong();
	mm_date=ptok->getlong();
	mOpdealid=ptok->getlong(); 
	mFtdealid=ptok->getlong();
	return TRUE;

}

BOOL Trader::SPKGTraderAll(char *pkg)
{
	BOOL msended;
	msended=FALSE;
	CClient *pclient;
	intitem *pclientitem;
	intlst senlst;
	mOnlineTrader.LockMap();
	mOnlineTrader.EnumFirst(&pclientitem);
	while(pclientitem) {
		pclient=(CClient*)pclientitem->val;
		senlst.AddTail(0,pclientitem->val);
		pclient->RefInc();
//		pclient->SendPkg(pkg);
//		pclient->AsyncSelect(FD_WRITE); 
//		msended=TRUE;
		mOnlineTrader.EnumNext(&pclientitem);

	}
	mOnlineTrader.UnlockMap(); 
//	if (msended==TRUE) return TRUE;
	
	mOnlineAdmin.LockMap();
	mOnlineAdmin.EnumFirst(&pclientitem);
	while (pclientitem!=NULL) {
		pclient=(CClient*)pclientitem->val;
		senlst.AddTail(0,pclientitem->val);
		pclient->RefInc();
//		pclient->SendPkg(pkg);
//		pclient->AsyncSelect(FD_WRITE); 
//		msended=TRUE;
		mOnlineAdmin.EnumNext(&pclientitem); 
	}
	mOnlineAdmin.UnlockMap(); 

	int nkey,nval;
	while (senlst.GetHead(&nkey,&nval)) {
		CClient *pclient=(CClient *)nval;
		pclient->SendPkg(pkg);
		pclient->AsyncSelect(FD_WRITE);
		pclient->RefDec();
	}


	return TRUE;

}


double Account::ReCalOrderHoldBP_Ext()
{
	
	FixLenMem f1;
	f1.init(sizeof(st_bpifo),2040);
	strmap bpmap;
	bpmap.init(10);
	stritem *bpitem;
	st_bpifo *bpifo;

	intitem *pposintitem;
	itemPosition *ppos;
	mLstPositions.LockLst();
	mLstPositions.EnumFirst(&pposintitem);
	while(pposintitem!=NULL) {
		ppos=(itemPosition*)pposintitem->val; 
		if (ppos->mqty) {
			bpitem=bpmap.FindItem(ppos->msecsym);
			if (bpitem==NULL) {
				bpifo=(st_bpifo*)f1.GetVoidBuff();
				memset(bpifo,0,sizeof(st_bpifo));
				bpitem=bpmap.Add(ppos->msecsym,(long)bpifo);  
			}else {
				bpifo=(st_bpifo*)(bpitem->val);
			}

			int qty1;
			qty1=ppos->GetEffectQty();
			switch(ppos->mptype) {
			case 1:case 2:
				bpifo->longpos+=qty1;
				break;
			case 3:
				bpifo->shrtpos+=qty1;
				break;
			}
		}
		mLstPositions.EnumNext(&pposintitem);
	}
	mLstPositions.UnlockLst();



//	double holdbp;
//	holdbp=0;
///	strmap longposmap;
//	strmap shortposmap;
//	stritem *pstritem;

	intitem *porderintitem;
	itemOrder *porder;

	mLstOrders.LockLst();
	mLstOrders.EnumFirst(&porderintitem);
	while(porderintitem) {
		porder=(itemOrder*)porderintitem->val; 
		ASSERT(porder!=NULL);
		int qty2;
		qty2=porder->GetOpenBpQty();
		if (qty2>0) {

			bpitem=bpmap.FindItem(porder->msecsym);
			if (bpitem==NULL) {
				bpifo=(st_bpifo*)f1.GetVoidBuff();
				memset(bpifo,0,sizeof(st_bpifo));
				bpitem=bpmap.Add(porder->msecsym,(long)bpifo);  
			}else {
				bpifo=(st_bpifo*)(bpitem->val);
			}
			if (porder->IsBuyOrder()) {// buy order;
				if (bpifo->shrtpos>qty2) {
					bpifo->shrtpos-=qty2;
					qty2=0;
				}else {
					qty2-=bpifo->shrtpos;
					bpifo->shrtpos=0;
				}
			}else {// sell order;
				if (bpifo->longpos>qty2) {
					bpifo->longpos-=qty2;
					qty2=0;
				}else {
					qty2-=bpifo->longpos;
					bpifo->longpos=0;
				}
			}
		}

		if ((qty2>0)&&(porder->msecsym[0]=='+')) {
			if (!porder->IsBuyOrder()){ //write option.
				itemOptsym *pop;
				pop=itemOptsym::FindInfo(porder->msecsym+1);
				if (pop) {
					stritem *bpudlyitem;
					st_bpifo *bpudlyifo;

					bpudlyitem=bpmap.FindItem(pop->undsymb);
					if (bpudlyitem!=NULL) {
						bpudlyifo=(st_bpifo*)(bpitem->val);
						if (pop->IsCall()){
							if(bpudlyifo->shrtpos>qty2) {
								bpudlyifo->shrtpos-=qty2;
								qty2=0;
							}else {
								qty2-=bpudlyifo->shrtpos;
								bpudlyifo->shrtpos=0;
							}

						}else {// puts;
							if(bpudlyifo->longpos>qty2) {
								bpudlyifo->longpos-=qty2;
								qty2=0;
							}else {
								qty2-=bpudlyifo->longpos;
								bpudlyifo->longpos=0;
							}
						}
					}
				}
			}
		}
		
		if (qty2>0) {
			double holdbp=porder->GetHoldBPRate(this)*qty2;;
			if (porder->IsBuyOrder()) {
				bpifo->buybp+=holdbp;
			}else {
				bpifo->sellbp+=holdbp;
			}
			bpifo->commionbp+=theCfgFile.mCommBPPreHold;
		}
		mLstOrders.EnumNext(&porderintitem);
	}
	mLstOrders.UnlockLst();

	double holdbp;
	holdbp=0;
	bpmap.EnumFirst(&bpitem);
	while(bpitem){
		bpifo=(st_bpifo*)bpitem->val;
		holdbp+= bpifo->maxbp();
		holdbp+=bpifo->commionbp;
		bpmap.EnumNext(&bpitem);
	}
	return holdbp;

}

BOOL Account::CheckOrderBP_Ext(itemOrder *pneworder,char *pmsg)
{
	
	st_bpifo stbpifo;
	memset(&stbpifo,0,sizeof(st_bpifo));
	BOOL bIsCover;

	intitem *pposintitem;
	itemPosition *ppos;
	mLstPositions.LockLst();
	mLstPositions.EnumFirst(&pposintitem);
	while(pposintitem!=NULL) {
		ppos=(itemPosition*)pposintitem->val; 
		if ((ppos->mqty)&&(strcmp(ppos->msecsym,pneworder->msecsym)==0)) { 
			int qty1;
			qty1=ppos->GetEffectQty();
			switch(ppos->mptype) {
			case 1:case 2:
				stbpifo.longpos+=qty1;
				break;
			case 3:
				stbpifo.shrtpos+=qty1;
				break;
			}
		}
		mLstPositions.EnumNext(&pposintitem);
	}
	mLstPositions.UnlockLst();

	if (mitemCtrl.maxsymbshare>0) {
		if (pneworder->IsBuyOrder()) {
			if (( stbpifo.longpos+pneworder->mqty )>mitemCtrl.maxsymbshare) {
				strcpy(pmsg,"Symb Share Limt!");
				return FALSE;
			}
		}else {
			if ((stbpifo.shrtpos+pneworder->mqty)>mitemCtrl.maxsymbshare) {
				strcpy(pmsg,"Symb Share Limt!");
				return FALSE;
			}
		}
		
	}



	if (mitemifo.mstatus&(1<<17)) {//position bp only;
		bIsCover=FALSE;
		if (pneworder->IsBuyOrder()) {
			if (stbpifo.shrtpos>pneworder->mqty){
				bIsCover=TRUE;
			}
		}else {
			if (stbpifo.longpos>pneworder->mqty){
				bIsCover=TRUE;
			}
		}
	}



	intitem *porderintitem;
	itemOrder *porder;

	mLstOrders.LockLst();
	mLstOrders.EnumFirst(&porderintitem);
	while(porderintitem) {
		porder=(itemOrder*)porderintitem->val; 
		ASSERT(porder!=NULL);
		int qty2;
		qty2=0;
		if (strcmp(porder->msecsym,pneworder->msecsym)==0) {
			qty2=porder->GetOpenBpQty();
			if (qty2>0) {
				if (porder->IsBuyOrder()) {// buy order;
					if (stbpifo.shrtpos>qty2) {
						stbpifo.shrtpos-=qty2;
						qty2=0;
					}else {
						qty2-=stbpifo.shrtpos;
						stbpifo.shrtpos=0;
					}
				}else {// sell order;
					if (stbpifo.longpos>qty2) {
						stbpifo.longpos-=qty2;
						qty2=0;
					}else {
						qty2-=stbpifo.longpos;
						stbpifo.longpos=0;
					}
				}
			}
		}else if ((pneworder->msecsym[0]!='+')&&(porder->msecsym[0]=='+')) { //option order;
			if (!porder->IsBuyOrder()){ //write option.
				itemOptsym *pop;
				pop=itemOptsym::FindInfo(porder->msecsym+1);
				if ((pop)&&(strcmp(pop->undsymb,pneworder->msecsym)==0))  {
					qty2=porder->GetOpenBpQty();
					if (pop->IsCall()){
						if(stbpifo.shrtpos>qty2) {
							stbpifo.shrtpos-=qty2;
							qty2=0;
						}else {
							qty2-=stbpifo.shrtpos;
							stbpifo.shrtpos=0;
						}
					}else {// puts;
						if(stbpifo.longpos>qty2) {
							stbpifo.longpos-=qty2;
							qty2=0;
						}else {
							qty2-=stbpifo.longpos;
							stbpifo.longpos=0;
						}
					}
					
				}
			}
		}

		if (qty2>0) {
			double holdbp=porder->GetHoldBPRate(this)*qty2;;
			if (porder->IsBuyOrder()) {
				stbpifo.buybp+=holdbp;
			}else {
				stbpifo.sellbp+=holdbp;
			}
			stbpifo.commionbp+=theCfgFile.mCommBPPreHold;
		}
		mLstOrders.EnumNext(&porderintitem);
	}
	mLstOrders.UnlockLst();

	double oldbp;
	oldbp=stbpifo.maxbp();

	int bpqty;
	bpqty=pneworder->GetOpenBpQty();
	if (pneworder->IsBuyOrder()) {// buy order;
		if (stbpifo.shrtpos>bpqty) {
			stbpifo.shrtpos-=bpqty;
			bpqty=0;
		}else {
			bpqty-=stbpifo.shrtpos;
			stbpifo.shrtpos=0;
		}
		if (bpqty>0) {
			stbpifo.buybp +=pneworder->GetHoldBPRate(this)*bpqty;
		}
	}else {// sell order;
		if (stbpifo.longpos>bpqty) {
			stbpifo.longpos-=bpqty;
			bpqty=0;
		}else {
			bpqty-=stbpifo.longpos;
			stbpifo.longpos=0;
		}
		if (bpqty>0) {
			stbpifo.sellbp+=pneworder->GetHoldBPRate(this)*bpqty;
		}
	}

	double holdbp;
	
	holdbp=stbpifo.maxbp()-oldbp;

	if (holdbp>0.001) {//avoid the double  zero problem; and if no bp need , don't even caculate the commisstion buypower.
		if (mitemifo.mstatus&(1<<17)) {
			if ((mitemEquity.mCurrBP>=holdbp)||bIsCover){
				if ((mitemEquity.mOpenBP*theCfgFile.mOrderBpCtrlRatio)>=(mitemEquity.mCurrHoldBP+holdbp)){
					return TRUE;
				}else {
					strcpy(pmsg,"BuyingPower!");
					return FALSE;
				}
			}else {
				strcpy(pmsg,"BuyingPower!");
				return FALSE;
			}
		}else {
			if ((mitemEquity.mCurrBP -mitemEquity.mCurrHoldBP)>=holdbp){
				return TRUE;
			}else {
				strcpy(pmsg,"BuyingPower!");
				return FALSE;
			}
		}
	}
	
	return TRUE;


}

//find the account and add a refrence counter
CClient * Trader::FindRefClient(int trid)
{
	intitem *pintitem;
	CClient *pclient=NULL;

	sMapTrid.LockMap();
	pintitem=sMapTrid.FindItem(trid);
	if (pintitem!=NULL){
		Trader *t1;
		t1=(Trader*)pintitem->val;
		if (t1->mpClient!=NULL) {
			pclient=t1->mpClient;
			pclient->RefInc();
		}
	}
	sMapTrid.UnlockMap();
	return pclient;

}


BOOL Account::LoadSODQuest(LPCSTR filename)

{
	CFile fp;
	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmt("Can't open file :%s",filename); 
		return FALSE;
	}; 
	CArchive ar(&fp,CArchive::load);
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		if (st[0]=='#') continue;
		int i;
		i=0;
		starry[0]=strtok(st,",");
		for (i=1;i<6;i++){
			starry[i]=strtok(NULL,",");
			if (starry[i]==NULL) {
				break;
			}
		};
		if (i<5) {
			theCfgFile.mlog.AddLogStr("LoadSODFileError");
			break;
			
		}
		if (strcmp(starry[2],"$BP")!=0) {
			continue;
		}
/*		double kkk;
		sscanf(starry[3],"%lf",&kkk);
		char str[100];
		sprintf(str,"%g",kkk);
		theCfgFile.mlog.AddLogStr(str);
*/		
		pacc=FindAccByName(starry[1] );
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 
			itemAccEquity eq;
			eq.CopyFrom(&pacc->mitemEquity);
			sscanf(starry[3],"%lf",&eq.mOpenBP);
			eq.mOpenNBP=eq.mOpenBP;
			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  

			//memcpy(&pacc->mitemEquity,&rstAccEquiy.mitem,sizeof(itemAccEquity));
		};
		
	}
	ar.Close();
	fp.Close();
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	return TRUE;

}

//this file is load the start day file, which is the internal format.
//the clearnance file need to translated to this internal file format.
//file format:
//Account,totalequity,marginequity,daybp,nightbp
BOOL Account::LoadSODFile(LPCSTR filename)
{
	CFile fp;
	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		return FALSE;
	}; 
	CArchive ar(&fp,CArchive::load);
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		if (st[0]=='#') continue;
		int i;
/*		i=0;
		starry[0]=strtok(st,",");
		for (i=1;i<6;i++){
			starry[i]=strtok(NULL,",");
			if (starry[i]==NULL) {
				break;
			}
		};
*/
		i=30;
		CStrFunc::BreakStr(st,',',starry,&i);
		if (i!=5) {
			theCfgFile.mlog.AddLogStr("LoadSODFileError");
			break;
			
		}
		pacc=FindAccByName(starry[0]);
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 
			itemAccEquity eq;
			eq.CopyFrom(&pacc->mitemEquity);
			if (strlen(starry[1])){
				sscanf(starry[1],"%lf",&eq.mOpenEQ); 
			}
			if (strlen(starry[2])){
				sscanf(starry[2],"%lf",&eq.mOpenMGEQ); 
			}
			if (strlen(starry[3])) {
				sscanf(starry[3],"%lf",&eq.mOpenBP);
			}
			if (strlen(starry[4])) {
				sscanf(starry[4],"%lf",&eq.mOpenNBP); 
			}
			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		};
		
	}
	ar.Close();
	fp.Close();
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	theCfgFile.mlog.AddLogStrFmt("Load Equity and BP file: %d items changed!",changed);
	return TRUE;
}


//This function load penson ext044 file.which has buying power and equity information inside.
//
//AccountNumber		char(12)	
//OfficeCode		char(3)	
//TotalMarketValueFirmCurrency		decimal(28,10)	
//EquityMarketValueFirmCurrency		decimal(28,10)	
//MaintenanceRequirementFirmCurrency		decimal(28,10)	
//MaintenanceExcess		decimal(28,10)	
//MarginEquityFirmCurrency		decimal(28,10)	
//TotalEquityFirmCurrency		decimal(28,10)	
//SMAFirmCurrency		decimal(28,10)	
//DTBPFirmCurrency		decimal(28,10)	
//RTBPFirmCurrency		decimal(28,10)	
//ONBPFirmCurrency		decimal(28,10)	
//ProcessDate		datetime	


BOOL Account::LoadPenson044(LPCSTR filename)
{
	CStdioFile fp;
	theCfgFile.mlog.AddLogStrFmt("Loading Penson Ext044 Equity and BP file: %s",filename);

	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmt("Can't open file %s",filename);
		return FALSE;
	}; 
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (fp.ReadString(st,sizeof(st))){
		if (st[0]=='#') continue;
		int i;
		i=30;
		CStrFunc::BreakStr(st,',',starry,&i);
		if (i<13) {
			theCfgFile.mlog.AddLogStr("File format error:Ext044");
			break;
			
		}
		pacc=FindAccByName(starry[0]);
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 

			itemAccEquity eq;
			double bp,nbp;
			eq.CopyFrom(&pacc->mitemEquity);
			if (strlen(starry[7])){
				sscanf(starry[7],"%lf",&eq.mOpenEQ); 
			}
			if (strlen(starry[6])){
				sscanf(starry[6],"%lf",&eq.mOpenMGEQ); 
			}
			if (strlen(starry[9])) {
				sscanf(starry[9],"%lf",&bp);
			}
			if (strlen(starry[11])) {
				sscanf(starry[11],"%lf",&nbp); 
			}

			if (pacc->mitemifo.IsMargin()) { 
				eq.mOpenBP=bp;
				eq.mOpenNBP=nbp;
			}else {// cash account:
				eq.mOpenBP=bp*theCfgFile.mDayMargBPRate;
				eq.mOpenNBP=nbp*theCfgFile.mDayMargBPRate;
			}


			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		};
		
	}
	fp.Close();
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	theCfgFile.mlog.AddLogStrFmt("Load Equity and BP file: %d items changed!",changed);
	return TRUE;


}

//Account No.,TotalMarketValue,MarginMarketValue,MaintenanceRequired,MarginEquity,TotalEquity,SMA,DayTradingBuyingPower,RegtBuyingPower,OverNightBuyingPower
//133584,,,,,50000.00,,50000.00,,

BOOL Account::LoadNorthAmerica002(LPCSTR filename)
{
	CStdioFile fp;
	theCfgFile.mlog.AddLogStrFmt("Loading NorthAmerica Ext002 Equity and BP file: %s",filename);

	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmt("Can't open file %s",filename);
		return FALSE;
	}; 
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (fp.ReadString(st,sizeof(st))){
		if (st[0]=='#') continue;
		int i;
		i=30;
		CStrFunc::BreakStr(st,',',starry,&i);
		if (i<9) {
			theCfgFile.mlog.AddLogStr("File format error:Ext002");
			break;
			
		}
		pacc=FindAccByName(starry[0]);
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 

			itemAccEquity eq;
			double bp,nbp;
			bp=nbp=0;
			eq.CopyFrom(&pacc->mitemEquity);
			if (strlen(starry[5])){
				sscanf(starry[5],"%lf",&eq.mOpenEQ); 
			}
			if (strlen(starry[4])){
				sscanf(starry[4],"%lf",&eq.mOpenMGEQ); 
			}
			if (strlen(starry[7])) {
				sscanf(starry[7],"%lf",&bp);
				if (bp>100000000) {
					theCfgFile.mlog.AddLogStrFmt("BuyingPower too big! %s,%g",starry[7],bp); 
				}
			}
			if (strlen(starry[9])) {
				sscanf(starry[9],"%lf",&nbp); 
			}
			
			if (nbp>bp) {//for TDAM, they want overnight buying power to be buying power.
				bp=nbp;
			}


			if (pacc->mitemifo.IsMargin()) { 
				eq.mOpenBP=bp;
				eq.mOpenNBP=nbp;
			}else {// cash account:
				eq.mOpenBP=bp*theCfgFile.mDayMargBPRate;
				eq.mOpenNBP=nbp*theCfgFile.mDayMargBPRate;
			}


			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		};
		
	}
	fp.Close();
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	theCfgFile.mlog.AddLogStrFmt("Load Equity and BP file: %d items changed!",changed);
	return TRUE;


}


BOOL Account::LoadCCSBPFile(LPCSTR filename)
{
	CStdioFile fp;
	theCfgFile.mlog.AddLogStrFmt("Loading CCS Equity and BP file: %s",filename);

	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmt("Can't open file %s",filename);
		return FALSE;
	}; 
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (fp.ReadString(st,sizeof(st))){
		if (st[0]=='#') continue;
		int i;
		i=30;
		CStrFunc::BreakStr(st,',',starry,&i);
		if (i<26) {
			theCfgFile.mlog.AddLogStr("File format error:CCS BP");
			break;
			
		}
		pacc=FindAccByName(starry[0]);
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 
			itemAccEquity eq;
			eq.CopyFrom(&pacc->mitemEquity);
			double bp,nbp;
			if (strlen(starry[25])){
				sscanf(starry[25],"%lf",&eq.mOpenEQ); 
			}
			if (strlen(starry[14])){
				sscanf(starry[14],"%lf",&eq.mOpenMGEQ); 
			}
			if (strlen(starry[18])) {
				sscanf(starry[18],"%lf",&bp);
			}
			if (strlen(starry[19])) {
				sscanf(starry[19],"%lf",&nbp); 
			}

			if (pacc->mitemifo.IsMargin()) { 
				eq.mOpenBP=bp;
				eq.mOpenNBP=nbp;
			}else {// cash account:
				eq.mOpenBP=bp*theCfgFile.mDayMargBPRate;
				eq.mOpenNBP=nbp*theCfgFile.mDayMargBPRate;
			}

			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		};
		
	}
	fp.Close();
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	theCfgFile.mlog.AddLogStrFmt("Load Equity and BP file: %d items changed!",changed);
	return TRUE;
}


//this function mostly used to save most of the information to a file to generate 
//upload file, reports etc.
BOOL SaveReportFile(LPCSTR fname)
{


	char sttm[30];
	char stst[50];
	char buf[1000];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	sprintf(stst,"Save the report file %s >%s",sttm,fname);
	theCfgFile.mlog.AddLogStr(stst);

	CStdioFile rptfile;
	BOOL bret;
	bret=rptfile.Open(fname,CFile::modeWrite |CFile::modeCreate |CFile::shareDenyNone);
	if (bret==FALSE) return FALSE;

//-------branch information-----------------------

	intitem *PIBr;
	PIBr=theCfgFile.mpItemPool->MAP_Branch.EnumFirst();
	while(PIBr!=NULL){
		itemBranch * pbitem=NULL;
		pbitem=(itemBranch*)PIBr->val;
		pbitem->SaveStr(buf,sizeof(buf),0xffffffff);
		rptfile.WriteString(buf);
		rptfile.WriteString("\n");
		PIBr=theCfgFile.mpItemPool->MAP_Branch.EnumNext(PIBr);
	}

//--------trader informatin------------------------

	intitem *PItrader;
	Trader::sMapTrid.LockMap();
	Trader::sMapTrid.EnumFirst(&PItrader);
	while(PItrader!=NULL){
		Trader *tritem;
		itemTrader *ptrader;
		tritem=(Trader*)PItrader->val;
		ptrader=&tritem->minfo;
		ptrader->SaveStr(buf,sizeof(buf),0xffffffff);
		rptfile.WriteString(buf);
		rptfile.WriteString("\n");
		Trader::sMapTrid.EnumNext(&PItrader);
	}
	Trader::sMapTrid.UnlockMap();

//---------account information and the ticket information;

	Account::sMapAccid.LockMap();
	intitem *pintitem;
	intitem *PIticket;
//	itemPosition *ppos;
//	intitem *PIpos;
	itemTicket   *pticket;
	Account *pacc;
	Account::sMapAccid.EnumFirst(&pintitem);
	int counter=0;
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
		//save account informmation;
		pacc->mitemifo.SaveStr(buf,sizeof(buf),0xffffffff);
		rptfile.WriteString(buf);
		rptfile.WriteString("\n");


//		//save the position;
//		pacc->mLstPositions.LockLst();
//		pacc->mLstPositions.EnumFirst(&PIpos);
//		while(PIpos!=NULL) {
//			ppos=(itemPosition*)PIpos->val;
//			////write
//			
//			pacc->mLstPositions.EnumNext(&PIpos);
//		}
//		pacc->mLstPositions.UnlockLst();


		//save the tickets;
		pacc->TktRecaluate(FALSE,FALSE); 
		pacc->mLstTickets.LockLst();
		pacc->mLstTickets.EnumFirst(&PIticket);
		while(PIticket!=NULL) {
			pticket=(itemTicket*)PIticket->val;
			pticket->SaveStr(buf,sizeof(buf),0xffffffff);
			rptfile.WriteString(buf);
			rptfile.WriteString("\n");
			////write;
			pacc->mLstTickets.EnumNext(&PIticket);
		}
		pacc->mLstTickets.UnlockLst();


		Account::sMapAccid.EnumNext(&pintitem);
	}
	Account::sMapAccid.UnlockMap();


	rptfile.Close();

	return TRUE;
}

int Account::AutoCoverPosition(LPCSTR symbol,int trid,double dprice,BYTE primexch)
{
	if ((mitemifo.mstatus&(1<<21))==0) return 0;

	Account::stPosInfo stposifo;
	Account::stPosInfo stordifo;
	FindPosInfo(symbol,&stposifo);
	FindOrderHoldIfo(symbol,&stordifo);


	if ((stposifo.mLong==0)&&(stposifo.mShort==0)) return 0;
	if ((stordifo.mLong!=0)||(stordifo.mShort!=0)) return 0;

	itemOrder ord;
	BOOL ret;
	ord.Clear();
	ord.mtrid=trid;
	ord.maccid=mitemifo.maccid;
	ord.maskprice=ord.mbidprice=dprice;
	strncpy(ord.msecsym,symbol,sizeof(ord.msecsym)-1);
	strncpy(ord.mroute,"SBSH",sizeof(ord.mroute)-1);
	ord.mprice=dprice;
	if (stposifo.mLong>0) {
		ord.mqty=stposifo.mLong;
	}else if (stposifo.mShort>0) {
		ord.mqty=stposifo.mShort;
		ord.mstatus|=(1<<6);
	}else {
		return 0;
	}
	ord.mentrid=trid;
	ord.mbrid=mitemifo.mbrid;
	ord.mc_date=time(NULL);
	ord.mm_date=ord.mc_date;
	ord.mexchange=primexch; 
	ord.mtmforce=0xffff; //send as day order;
	

	if(mitemifo.IsTraining()) ord.mstatus|=(1<<24);
	ord.mstatus|=(1<<9); //set as market order;
	
	char msg[200];
	ret=ord.NewOrder(0xffffffff,NULL,msg);
	if (ret) {
		theCfgFile.mlog.AddLogStr("Send Market auto cover order Successed!");
		theCfgFile.mlog.AddLogStrFmt("%%E,R,%d,%d",ord.morderid,ord.mc_date); 
	}else{
		theCfgFile.mlog.AddLogStrFmt("Send Market auto cover order Fail:%s!",msg);
	}
	return 0;
}


BOOL Account::LoadBP_NBLT_GAON(LPCSTR filename)
{
	FILE *fp;
	theCfgFile.mlog.AddLogStrFmt("Loading NBLT_GAON BP file: %s",filename);

	fp=fopen(filename,"rb");
	//if (fp.Open(filename,CFile::modeRead)==FALSE) {
	if (fp==NULL) {
		theCfgFile.mlog.AddLogStrFmt("Can't open file %s",filename);
		return FALSE;
	}; 
	char st[1000];
	char *starry[30];
	Account *pacc;
	int changed;
	changed=0;
	while (fgets(st,sizeof(st),fp)) { //fp.ReadString(st,sizeof(st))){
		if (st[0]=='#') continue;
		int i;
		i=30;
		CStrFunc::BreakStr(st,'@',starry,&i);
		if (i<9) {
			theCfgFile.mlog.AddLogStr("File format error:NBLT_GAON");
			break;
			
		}
		pacc=FindAccByName(starry[4]);
		if (pacc!=NULL) {
			if (pacc->mitemifo.mstatus & (1<<25) ) continue; 

			itemAccEquity eq;
			double bp1,bp2,bp3;
			eq.CopyFrom(&pacc->mitemEquity);
			bp1=bp2=bp3=0;
			if (strlen(starry[5])) {
				sscanf(starry[5],"%lf",&bp1);
			}
			if (strlen(starry[6])) {
				sscanf(starry[6],"%lf",&bp2);
			}
			if (strlen(starry[7])) {
				sscanf(starry[7],"%lf",&bp3);
			}

			double bp=bp1+bp2+bp3;
			if (pacc->mitemifo.IsMargin()) { 
				eq.mOpenBP=bp;
				eq.mOpenNBP=bp;
			}else {// cash account:
				eq.mOpenBP=bp*theCfgFile.mDayMargBPRate;
				eq.mOpenNBP=bp*theCfgFile.mDayMargBPRate;
			}


			changed++;
			time(&(eq.mm_time));
			pacc->mitemEquity.CopyFrom(&eq);
			pacc->TktRecaluate(FALSE,FALSE); 
			char pkg[200];
			pacc->MakePkgByEquity(pkg,0xffff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		};
		
	}
//	fp.Close();
	fclose(fp);
	if (changed>0) {
		SaveAllEquityInfo(FALSE); 
	}
	theCfgFile.mlog.AddLogStrFmt("Load GAON BP file: %d items changed!",changed);
	return TRUE;
}

int Account::GetRoundTripTrades()
{
	mLstTickets.LockLst();
	strmap posmap;
	posmap.init(20);
	intitem *PI=NULL;
	int roundtrips=0;
	while(mLstTickets.Enum(PI)) {
		itemTicket *pticket=(itemTicket*)PI->val;
		stritem *Pitem=posmap.FindItem(pticket->msecsym);
		if (Pitem==NULL) {
			Pitem=posmap.Add(pticket->msecsym,0);
		}
		if (pticket->IsBuyTrade()){
			if (Pitem->val<0) {
				roundtrips++;
			}
			Pitem->val+=pticket->mqty; 
		}else{
			if (Pitem->val>0) {
				roundtrips++;
			}
			Pitem->val-=pticket->mqty;
		}
	}

	mLstTickets.UnlockLst();
	return roundtrips;
}

void Account::CancelAllUnwindOrder(itemOrder *pord)
{
	int currpos=0;
	intitem *PI=NULL; 
	mLstPositions.LockLst();
	while(mLstPositions.Enum(PI)) {
		itemPosition *pitem=(itemPosition*)PI->val;
		if (strcmp(pitem->msecsym,pord->msecsym)==0) {  
			switch(pitem->mptype) {
			case 1:case 2: 
				currpos+=pitem->mqty;
				break;
			case 3: 
				currpos-=pitem->mqty;
				break;
			}
		}
	}
	mLstPositions.UnlockLst();

	BOOL bIsBuy=pord->IsBuyOrder();
	if (bIsBuy) {
		if (currpos<0) return;
	}else {
		if (currpos>0) return;
	}
	//cancel all order with same side and symbol and the flag is on;
	mLstOrders.LockLst();
	PI=NULL;
	while(mLstOrders.Enum(PI)){
		itemOrder* porder=(itemOrder*)PI->val;
		if (!porder->IsOpenOrder()) continue;
		if (porder->IsBuyOrder()!=bIsBuy) continue;
		if ((porder->ms1 & (1<<11))==0) continue;
		if (strcmp(porder->msecsym,pord->msecsym)!=0) continue;
		if (porder->ms1 & 3) continue; //cancel rejected and cancelling;
		OrderRouter *proute;
		proute=theCfgFile.FindRouter(porder);
		if (proute&&proute->IsEnabled()) {
			BOOL ret;
			char msgstr[50];
			memset(msgstr,0,sizeof(msgstr));
			ret=proute->SendCancelOrder(porder,0,msgstr);
			if (!ret) {
				theCfgFile.mlog.AddLogStrFmtTm(0,"Auto Cancel order error:%d, %s",porder->morderid,porder->msecsym);
			}else {
				time_t t1=time(NULL);
				theCfgFile.mlog.AddLogStrFmt("%%E,CR,%d,%d",porder->morderid,t1); 

			}


		}
		
/*			BOOL bFilter=FALSE;
			if (porder->mlvsqty<=0){
				bFilter=TRUE;
			}else if (proute!=NULL){
				if (strcmp(porder->mroute,proute)!=0) bFilter=TRUE;
			}
			if (!bFilter) {
				OrderRouter *proute;
				proute=theCfgFile.FindRouter(porder);
				if (proute&&proute->IsEnabled()) {
					BOOL ret;
					char msgstr[50];
					memset(msgstr,0,sizeof(msgstr));
					ret=proute->SendCancelOrder(porder,0,msgstr);
					if (bIsGTC) {
						porder->ms1&=~1;
					}
					nsended++;
				}
			}

		}
*/
	}
	mLstOrders.UnlockLst();

}



BOOL Account::IsUnwindOrder(itemOrder *porder)
{
	int pos=FindPositionNum(porder->msecsym);
	if (porder->IsBuyOrder()) {
		if (-pos>=porder->mqty) return TRUE;
	}else{
		if ( pos>=porder->mqty)	return TRUE;
	}
	return FALSE;

}

BOOL Trader::UpdateExBranchLst()
{
	char sqlstr[1000];
	TRY{
		sprintf(sqlstr,"delete mAdmBranchList where trid=%d",minfo.mtrid);
		theOrderDB.mdb.ExecuteSQL(sqlstr);
		for (int i=0;i<ADM_MAX_BRANCH;i++) {
			if (mBranchList[i]<=0) break;
			sprintf(sqlstr,"insert mAdmBranchList (trid,brid,stat1) values (%d,%d,%d)",
				minfo.mtrid,mBranchList[i],mBranchPerm[i]); 
			theOrderDB.mdb.ExecuteSQL(sqlstr);
		}

	}CATCH(CDBException, e) {
		char msg[1000];
		e->GetErrorMessage(msg,sizeof(msg));
		theCfgFile.mlog.AddLogStr(msg); 
//		e.Delete() ;
		return FALSE;
	}END_CATCH

	return TRUE;
}
