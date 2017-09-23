// itemsDefcpp.cpp: implementation of the itemBranch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../orderserv.h"
#include "itemsDef.h"
//#include "SRecSet\SRecSet.h"
#include "itemsSRst.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "Functions\strfuncs.h"
#include "..\OrderDB.h"
extern OrderDB theOrderDB;

#include "..\OrderConfig.h"
extern OrderConfig theCfgFile;

#include "Functions\encrypt.h"

FixLenMem itemPosition::mitembuff; 
FixLenMem itemTrade::sitembuff;
FixLenMem itemTicket::sitembuff;
FixLenMem itemOrder::sitembuff;
FixLenMem itemDeal::sitemDealbuff;
FixLenMem itemPassthrufee::sitemPassTFbuff;
 
strmap    itemOptsym::sMapSymb;
FixLenMem itemOptsym::sitembuff;

intmap itemTrade::sMapTradeID;
intmap itemOrder::sMapOrderID;
intmap itemDeal::sMapDealID;

intmap itemManualOrder::smapOrderid;
FixLenMem itemManualOrder::sOrderBuffer; 


//itemPosition * itemPosition::phashtab[HASHSIZE];
//simplelock itemPosition::mlock;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////



void itemBranch::CopyItem(itemBranch *pitem,WORD flag)
{
	
	if (flag& 0x8000) modifyflag=pitem->modifyflag;
	if (flag&1)		mbrid=pitem->mbrid;
	if (flag&(1<<1)) memccpy(mshort,pitem->mshort,0,sizeof(mshort));
	if (flag&(1<<2)) memccpy(mname,pitem->mname,0,sizeof(mname));
	if (flag&(1<<3)) mstatus=pitem->mstatus;
	if (flag&(1<<4)) mper_id=pitem->mper_id;
	if (flag&(1<<5)) madd_id=pitem->madd_id;
	if (flag&(1<<6)) mc_date=pitem->mc_date;
	if (flag&(1<<7)) mm_date=pitem->mm_date;
	if (flag&(1<<8)) memccpy(mnote,pitem->mnote,0,sizeof(mnote));
}

int itemBranch::GetPkgInfo(PkgToken *ptok, itemPerson*pperson, itemAddress *padd)
{
	WORD flag;
	flag=ptok->getWORD();
	if (flag&1) mbrid=ptok->getlong();
	if (flag & (1<<1)) ptok->getString(mshort,sizeof(mshort));
	if (flag & (1<<2)) ptok->getString(mname,sizeof(mname));
	if (flag & (1<<3)) mstatus=ptok->getDWORD();
	if (flag & (1<<4)) mper_id=ptok->getDWORD();
	if (flag & (1<<5)) madd_id=ptok->getDWORD();
	if (flag & (1<<6)) mc_date=ptok->getlong();
	if (flag & (1<<7)) mm_date=ptok->getlong();
	if (flag & (1<<8)) ptok->getString(mnote,sizeof(mnote));
	if (flag & (1<<9))	pperson->GetPkgInfo(ptok);
	if (flag & (1<<10))	padd->GetPkgInfo(ptok);
	modifyflag|=flag;
	return 0;
}

//////////////////////////////////////////////////////////////////////
// itemAddress Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void itemAddress::CopyItem(itemAddress *pitem,WORD flag)
{
	if (flag& 0x8000) modifyflag=pitem->modifyflag;
	if (flag& 1	)	maddid=pitem->maddid ;
	if (flag& 1<<1) mbrid=pitem->mbrid;
	if (flag& 1<<4) memccpy(maddress,pitem->maddress,0,sizeof(maddress));
	if (flag& 1<<5) memccpy(mcity,pitem->mcity,0,sizeof(mcity));
	if (flag& 1<<6) memccpy(mstate,pitem->mstate,0,sizeof(mstate));
	if (flag& 1<<7) memccpy(mzip,pitem->mzip,0,sizeof(mzip));
	if (flag& 1<<8) memccpy(mcountry,pitem->mcountry,0,sizeof(mcountry));
	if (flag& 1<<2) memccpy(mphone,pitem->mphone,0,sizeof(mphone));
	if (flag& 1<<3) memccpy(mfax,pitem->mfax,0,sizeof(mfax));
	if (flag& 1<<10) mc_date=pitem->mc_date;
	if (flag& 1<<11) mm_date=pitem->mm_date;
	if (flag& 1<<12) memccpy(mnote,pitem->mnote,0,sizeof(mnote));
}

int itemAddress::GetPkgInfo(PkgToken *ptok)
{
	WORD flag;
	flag=ptok->getWORD(); 
	if (flag==0) {
		modifyflag=0;
		return 0;
	}

	if (flag &1 ) maddid=ptok->getDWORD();
	if (flag &(1<<1)) mbrid=ptok->getDWORD();
	if (flag&(1<<2)) ptok->getString(mphone,sizeof(mphone)); 
	if (flag&(1<<3)) ptok->getString(mfax,sizeof(mfax)); 
	if (flag&(1<<4)) ptok->getString(maddress,sizeof(maddress));  
	if (flag&(1<<5)) ptok->getString(mcity,sizeof(mcity));  
	if (flag&(1<<6)) ptok->getString(mstate,sizeof(mstate));
	if (flag&(1<<7)) ptok->getString(mzip,sizeof(mzip));
	if (flag&(1<<8))  ptok->getString(mcountry,sizeof(mcountry));
	if (flag&(1<<10)) mc_date=ptok->getlong();
	if (flag&(1<<11)) mm_date=ptok->getlong();
	if (flag&(1<<12))ptok->getString(mnote,sizeof(mnote));
	modifyflag =flag;
	return flag;

/*	BYTE fieldtype;
	BOOL Loopflag;
	Loopflag=TRUE;
	while(Loopflag){
		fieldtype=ptok->getBYTE();
		switch(fieldtype) {
		case 0:
		case 1: 
			if(fieldtype==1) {
				maddid=ptok->getlong(); 
			}
			if (fieldtype==0) {
				Loopflag=FALSE;
			}
			break;
		case 2:// flag
			mflag=ptok->getDWORD();
			modifyflag|=1<<1;
			break;
		case 3:// phone
			ptok->getString(mphone,sizeof(mphone));
			modifyflag|=1<<2;
			break;
		case 4:// branch flag
			ptok->getString(mfax,sizeof(mfax));
			modifyflag|=1<<3;
			break;
		case 5://address 
			ptok->getString(maddress,sizeof(maddress));
			modifyflag|=1<<4;
			break;
		case 6://city
			ptok->getString(mcity,sizeof(mcity));
			modifyflag|=1<<5;
			break;

		case 7://state
			ptok->getString(mstate,sizeof(mstate));
			modifyflag|=1<<6;
			break;

		case 8://zip
			ptok->getString(mzip,sizeof(mzip));
			modifyflag|=1<<7;
			break;

		case 9://country
			ptok->getString(mcountry,sizeof(mcountry));
			modifyflag|=1<<8;
			break;

		case 11://Create Time;
			mc_date=ptok->getlong();
			modifyflag|=1<<10;
			break;
		case 12://modify time;
			mm_date=ptok->getlong();
			modifyflag|=1<<11;
			break;
		case 13://note
			ptok->getString(mnote,sizeof(mnote));
			modifyflag|=1<<12;
			break;
		}
	} 
	return 0;
*/
}

int itemAddress::SetPkgInfo(PkgToken *ptok,WORD flag)
{
	flag|=1;
	ptok->setWORD(flag);
	if (flag &1 ) ptok->setDWORD(maddid);
	if (flag &(1<<1)) ptok->setDWORD(mbrid);
	if (flag&(1<<2)) ptok->setVarStr(mphone); 
	if (flag&(1<<3)) ptok->setVarStr(mfax); 
	if (flag&(1<<4)) ptok->setVarStr(maddress );  
	if (flag&(1<<5)) ptok->setVarStr(mcity);  
	if (flag&(1<<6)) ptok->setVarStr(mstate);
	if (flag&(1<<7)) ptok->setVarStr(mzip);
	if (flag&(1<<8))  ptok->setVarStr(mcountry);
	if (flag&(1<<10)) ptok->setlong(mc_date);
	if (flag&(1<<11)) ptok->setlong(mm_date);
	if (flag&(1<<12))ptok->setVarStr(mnote);
	return 0;
	/*
	ptok->setBYTE(1);
	ptok->setDWORD(maddid);
	if (modifyflag&(1<<1)) { // flag;
		ptok->setBYTE(2);
		ptok->setDWORD(mflag);
	}
	
	if (modifyflag&(1<<2)) { // phone
		ptok->setBYTE(3);
		ptok->setVarStr(mphone); 
	}

	if (modifyflag&(1<<3)) { // fax
		ptok->setBYTE(4);
		ptok->setVarStr(mfax); 
	}

	if (modifyflag&(1<<4)) { // address
		ptok->setBYTE(5);
		ptok->setVarStr(maddress );  
	}

	if (modifyflag&(1<<5)) { // city
		ptok->setBYTE(6);
		ptok->setVarStr(mcity);  
	}
	
	if(modifyflag&(1<<6)) { ///state
		ptok->setBYTE(7);
		ptok->setVarStr(mstate);
	}
	
	if(modifyflag&(1<<7)) { ///zip
		ptok->setBYTE(8);
		ptok->setVarStr(mzip);
	}

	if(modifyflag&(1<<8)) { ///country
		ptok->setBYTE(9);
		ptok->setVarStr(mcountry);
	}
	
	if (modifyflag&(1<<12)) { // note 
		ptok->setBYTE (13);
		ptok->setVarStr(mnote);
		
	}
	return 0;
	*/
}

int itemBranch::SetPkgInfo(PkgToken *ptok,WORD flag)
{
	flag |=1;
	ptok->setWORD(flag);
	if (flag & 1) ptok->setDWORD(mbrid);
	if (flag&(1<<1)) ptok->setVarStr(mshort);
	if (flag&(1<<2)) ptok->setVarStr(mname); 
	if (flag&(1<<3)) ptok->setDWORD(mstatus);  
	if(flag&(1<<4)) ptok->setlong(mper_id);
	if(flag&(1<<5)) ptok->setlong(madd_id );
	if (flag&(1<<6)) ptok->setlong(mc_date);
	if (flag&(1<<7)) ptok->setlong(mm_date);
	if (flag&(1<<8)) ptok->setVarStr(mnote);
	return 0;
}

void itemPerson::CopyItem(itemPerson *pitem,WORD flag)
{
	if (flag& 0x8000) modifyflag=pitem->modifyflag;
	if (flag& 1) mperid=pitem->mperid;

	if (flag& 1<<1) mbrid=pitem->mbrid;
	if (flag& 1<<2) mentity_type=pitem->mentity_type;
	if (flag& 1<<3) memcpy(mtitle,pitem->mtitle,sizeof(mtitle));
	if (flag& 1<<4) memcpy(mentity_name,pitem->mentity_name,sizeof(mentity_name));
	if (flag& 1<<5) memcpy(mfname,pitem->mfname ,sizeof(mfname));
	if (flag& 1<<6) memcpy(mmname,pitem->mmname,sizeof(mmname));
	if (flag& 1<<7) memcpy(mlname,pitem->mlname ,sizeof(mlname));
	if (flag& 1<<8) memcpy(mSSN,pitem->mSSN,sizeof(mSSN));
	if (flag& 1<<9) memcpy(mphone,pitem->mphone,sizeof(mphone));
	if (flag& 1<<10) memcpy(memail,pitem->memail ,sizeof(memail));
	if (flag& 1<<11) mc_date=pitem->mc_date;
	if (flag& 1<<12) mm_date=pitem->mm_date;
	if (flag& 1<<13) memcpy(mnote,pitem->mnote,sizeof(mnote));
}

int itemPerson::GetPkgInfo(PkgToken *ptok)
{
	WORD flag;
	flag=ptok->getWORD(); 
	if (flag &1)mperid=ptok->getlong();  
	if (flag & (1<<1)) mbrid=ptok->getDWORD();
	if (flag & (1<<2)) mentity_type=ptok->getBYTE();
	if (flag & (1<<3)) ptok->getString(mtitle,sizeof(mtitle));
	if (flag & (1<<4)) ptok->getString(mentity_name ,sizeof(mentity_name));
	if (flag & (1<<5)) ptok->getString(mfname ,sizeof(mfname));
	if (flag & (1<<6)) ptok->getString(mmname ,sizeof(mmname));
	if (flag & (1<<7)) ptok->getString(mlname,sizeof(mlname));
	if (flag & (1<<8)) ptok->getString(mSSN,sizeof(mSSN ));
	if (flag & (1<<9))	ptok->getString(mphone,sizeof(mphone));
	if (flag & (1<<10))	ptok->getString(memail,sizeof(memail));
	if (flag & (1<<11)) mc_date=ptok->getlong();
	if (flag & (1<<12))	mm_date=ptok->getlong();
	if (flag & (1<<13))	ptok->getString(mnote,sizeof(mnote));
	modifyflag=flag;
	return flag;
}

int itemPerson::SetPkgInfo(PkgToken *ptok,WORD flag)
{
	flag |=1;
	ptok->setWORD(flag);
	if (flag & 1) 	ptok->setDWORD(mperid);
	if (flag&(1<<1)) ptok->setDWORD(mbrid);
	if (flag&(1<<2)) ptok->setBYTE(mentity_type); 
	if (flag&(1<<3)) ptok->setVarStr(mtitle); 
	if (flag&(1<<4)) ptok->setVarStr(mentity_name);  
	if (flag&(1<<5)) ptok->setVarStr(mfname );  
	if (flag&(1<<6)) ptok->setVarStr(mmname);
	if (flag&(1<<7)) ptok->setVarStr(mlname);
	if (flag&(1<<8)) ptok->setVarStr(mSSN );
	if (flag&(1<<9)) ptok->setVarStr(mphone);
	if (flag&(1<<10)) ptok->setVarStr(memail);
	if (flag&(1<<11)) ptok->setlong(mc_date);
	if (flag&(1<<12)) ptok->setlong(mm_date);
	if (flag&(1<<13)) ptok->setVarStr(mnote);
	return 0;

}

BOOL itemBranch::MakeInfoPkg(char *pbuff,WORD flag)
{
	PkgToken tokret;
	tokret.InitToSet(pbuff);
	tokret.setWORD(1004);
	SetPkgInfo(&tokret,flag);
	tokret.finalizeSet();
	return TRUE;
}

BOOL itemAddress::MakeInfoPkg(char *pbuff,WORD flag)
{
	PkgToken tokret;
	tokret.InitToSet(pbuff);
	tokret.setWORD(1011);
	SetPkgInfo(&tokret,flag);
	tokret.setWORD(0);
	tokret.finalizeSet();
	return TRUE;

}

BOOL itemPerson::MakeInfoPkg(char *pbuff,WORD flag)
{
	PkgToken tokret;
	tokret.InitToSet(pbuff);
	tokret.setWORD(1010);
	SetPkgInfo(&tokret,flag);
	tokret.finalizeSet();
	return TRUE;
}

void itemPerson::Clear()
{
	modifyflag=0;
	mperid=0;
	mbrid=0;
	mentity_type=0;
	memset(mtitle,0,sizeof(mtitle));
	memset(mentity_name,0,sizeof(mentity_name));
	memset (mfname,0,sizeof(mfname));
	memset(mmname,0,sizeof(mmname));
	memset(mlname,0,sizeof(mlname));
	memset(mSSN,0,sizeof(mSSN));
	memset(mphone,0,sizeof(mphone));
	memset(memail,0,sizeof(memail));
	mc_date=0;
	mm_date=0;
	memset(mnote,0,sizeof(mnote));
}

void itemBranch::Clear()
{
	modifyflag=0;
	mbrid=0;
	memset(mshort,0,sizeof(mshort));
	mstatus=0;
	memset(mname,0,sizeof(mname));
	mper_id=0;
	madd_id=0;
	mc_date=0;
	mm_date=0;
	memset(mnote,0,sizeof(mnote));
}

void itemAddress::Clear()
{
	modifyflag=0;
	maddid=0;
	mbrid=0;
	memset(maddress,0,sizeof(maddress));
	memset(mcity,0,sizeof(mcity));
	memset(mstate,0,sizeof(mstate));
	memset(mzip,0,sizeof(mzip));
	memset(mcountry,0,sizeof(mcountry));
	memset(mphone,0,sizeof(mphone));
	memset(mfax,0,sizeof(mfax));
	mc_date=0;
	mm_date=0;
	memset (mnote,0,sizeof(mnote));
}

void itemTrader::Clear()
{
	modifyflag=0;
	mtrid=0;
	memset(mtracno,0,sizeof(mtracno));
	memset(mPWD,0,sizeof(mPWD));
	mbrid=0;
	mstatus=0;
	mtype=0;
	mPerid=0;
	mAdd1ID=0;
	mAdd2ID=0;
	mc_date=0;
	mm_date=0;
	memset (mnote,0,sizeof(mtracno));
	mstatus1=0;
	mstatus2=0;
	mstatus3=0;
}

void itemTrader::CopyItem(itemTrader *pitem, long flag)
{
	if (flag& 0x70000000) modifyflag=pitem->modifyflag;
	if (flag& 1) mtrid=pitem->mtrid;
	if (flag& 1<<1) memcpy(mtracno,pitem->mtracno,sizeof(mtracno));
	if (flag& 1<<2) memcpy(mPWD,pitem->mPWD ,sizeof(mPWD));
	if (flag& 1<<3) mbrid=pitem->mbrid;
	if (flag& 1<<4) mstatus=pitem->mstatus ;
	if (flag& 1<<5) mPerid=pitem->mPerid;
	if (flag& 1<<6) mAdd1ID =pitem->mAdd1ID ;
	if (flag& 1<<7) mAdd2ID=pitem->mAdd2ID ;
	if (flag& 1<<8) mc_date=pitem->mc_date;
	if (flag& 1<<9) mm_date=pitem->mm_date;
	if (flag& 1<<10) memcpy(mnote,pitem->mnote,sizeof(mnote));
	if (flag& 1<<14){
			mstatus1=pitem->mstatus1;
			mstatus2=pitem->mstatus2;
			mstatus3=pitem->mstatus3;
	}
	if (flag & (1<<15)) m_master_trader=pitem->m_master_trader;
}

int itemTrader::GetPkgInfo(PkgToken *ptok,itemPerson *pper1,itemAddress *padd1,itemAddress *padd2)
{
	//	BYTE fieldtype;
	//BOOL Loopflag;
	//Loopflag=TRUE;
	WORD flag;
	flag=ptok->getWORD();
	if (flag &1) mtrid=ptok->getlong(); 	
	if (flag &(1<<1)) ptok->getString(mtracno,sizeof(mtracno));
	if (flag &(1<<2)) ptok->getString(mPWD,sizeof(mPWD));
	if (flag &(1<<3)) mbrid=ptok->getlong();  // brid
	if (flag &(1<<4)) mstatus=ptok->getDWORD();// status;
	if (flag &(1<<5)) mPerid=ptok->getlong();
	if (flag &(1<<6)) mAdd1ID=ptok->getlong( );
	if (flag &(1<<7)) mAdd2ID=ptok->getlong();
	if (flag &(1<<8)) mc_date=ptok->getlong();
	if (flag &(1<<9)) mm_date=ptok->getlong();
	if (flag &(1<<10)) ptok->getString(mnote,sizeof(mnote));
	if (flag &(1<<11)) pper1->GetPkgInfo(ptok);
	if (flag &(1<<12)) padd1->GetPkgInfo(ptok);
	if (flag &(1<<13)) padd2->GetPkgInfo(ptok);
	if (ptok->GetLeftLen()>4){ //new package, 
		if (flag &(1<<14)){
			mstatus1=ptok->getDWORD();
			mstatus2=ptok->getDWORD();
			mstatus3=ptok->getDWORD();
		}
	}
	if (ptok->GetLeftLen()>=4){
		if (flag&(1<<15)) {
			m_master_trader=ptok->getlong();
		}
	}
	modifyflag |=flag;
	return 0;
}

int itemTrader::SetPkgInfo(PkgToken *ptok,WORD flag)
{
	flag|=1;
	ptok->setWORD(flag); 
	if (flag &1) ptok->setDWORD(mtrid);
	if (flag&(1<<1))ptok->setVarStr(mtracno);  // Tracno ;
	if (flag&(1<<2)) ptok->setVarStr(mPWD); //pwd
	if (flag&(1<<3)) ptok->setlong(mbrid);  // brid
	if (flag&(1<<4)) ptok->setDWORD(mstatus);// status;
	if (flag&(1<<5)) ptok->setlong(mPerid);
	if (flag&(1<<6)) ptok->setlong(mAdd1ID );
	if (flag&(1<<7)) ptok->setlong(mAdd2ID);
	if (flag&(1<<8)) ptok->setlong(mc_date);
	if (flag&(1<<9)) ptok->setlong(mm_date);
	if (flag&(1<<10)) ptok->setVarStr(mnote);
	if (flag&(1<<14)) {
		ptok->setDWORD(mstatus1);
		ptok->setDWORD(mstatus2);
		ptok->setDWORD(mstatus3);
	}
	if (flag&(1<<15)) {
		ptok->setlong(m_master_trader); 
	}
	return 0;
}

BOOL itemTrader::MakeInfoPkg(char *pbuff,WORD flag)
{
	PkgToken tokret;
	tokret.InitToSet(pbuff);
	tokret.setWORD(1000);
	SetPkgInfo(&tokret,flag);
	tokret.finalizeSet();
	return TRUE;
}

/*
upload it in database,*/
BOOL itemTrader::UpdateData()
{
//	char sqlstr[1000];
//	CTime currtime;
//	char datestr[20];
	if (modifyflag==0) return TRUE;
	if (mtrid==0) {//Add new trader;
		mtrid=theOrderDB.ChangeSysID("CurrTrid",1); 
//		currtime=CTime::GetCurrentTime();
		time(&mm_date);
		mc_date=mm_date;
		modifyflag|=((1<<9)|(1<<8));
	}else {
		time(&mm_date);
		modifyflag|=1<<9;
	}

/*	{
		theCfgFile.mLoginLog.AddLogStrFmt("ch epwd:>%s<",mPWD); 
		encrypt  en;
		char dpw[100];
		en.DecryptPWD((unsigned char*)mPWD,(unsigned char*)dpw);
		theCfgFile.mLoginLog.AddLogStrFmt("ch dpwd:>%s<",dpw); 
	}
*/

	srstTrader rsttr;
	try{
		rsttr.OpenToUpdate(mtrid);
		if(rsttr.IsEOF()){
			rsttr.mitem.CopyItem(this);
			rsttr.AddNew();
		}else {
			rsttr.mitem.CopyItem(this);
			rsttr.mDirtyFlag=modifyflag;
			rsttr.Update();
		}
		rsttr.Close();
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
/*Update the content to database.*/
BOOL itemPerson::UpdateData()
{

	if (modifyflag==0) return TRUE;
	srstPerson srst;
	try{
		srst.OpenToUpdate(mperid);
		if (srst.IsEOF()){
			time(&mc_date);
			mm_date=mc_date;
			modifyflag=0x7fff; 
			mperid =theOrderDB.ChangeSysID("CurrPerid",1); 
			srst.mitem.CopyItem(this,0xffff); 
//			memcpy(&srst.mitem,this,sizeof(itemAddress));
			srst.AddNew();
		}else {
			time(&mm_date);
			modifyflag|=1<<12;
			srst.mitem.CopyItem(this,0xffff); 
//			memcpy(&srst.mitem,this,sizeof(itemAddress));
			srst.Update();
		}
		srst.Close();
	}catch(SRSTErr *perr){
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		
}

BOOL itemAddress::UpdateData()
{
	if (modifyflag==0) return TRUE;
	srstAddress srst;
	try{
		srst.OpenToUpdate(maddid);
		if (srst.IsEOF()){
			time(&mc_date);
			mm_date=mc_date;
			modifyflag=0x7fff; 
			maddid =theOrderDB.ChangeSysID("CurrAddid",1); 
			memcpy(&srst.mitem,this,sizeof(itemAddress));
			srst.AddNew();
		}else {
			time(&mm_date);
			modifyflag|=1<<11;// change modify date
			memcpy(&srst.mitem,this,sizeof(itemAddress));
			srst.Update();
		}
		srst.Close();
	}catch(SRSTErr *perr){
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		

}

BOOL itemBranch::UpdateData()
{
//	char sqlstr[1000];
	CTime currtime;
//	char datestr[20];
	if (modifyflag==0) return TRUE;
	if (mbrid==0) {//add new;
		mbrid=theOrderDB.ChangeSysID("CurrBrid",1); 
	}
	srstBranch  srst;
	srst.OpenToUpdate(mbrid);
	if (srst.IsEOF()) {
		currtime=CTime::GetCurrentTime();
		mm_date=mc_date=currtime.GetTime();
		srst.mDirtyFlag=0xffff;
		srst.mitem.CopyItem(this,0xffff);
		srst.AddNew();
	}else {
		currtime=CTime::GetCurrentTime();
		mm_date=currtime.GetTime();
		srst.mDirtyFlag=modifyflag|7;
		srst.mitem.CopyItem(this,modifyflag);
		srst.Update();
	}
	srst.Close();
	
	return TRUE;		
}

void itemAccIfo::Clear()
{
	modifyflag=0;
	maccid=0;
	memset(mname,0,sizeof(mname));
	mbrid=0;
	mstatus=0;
	memset(mrrno,0,sizeof(mrrno));
	mlinkacc=0;
	mper_id=0;
	maddr1_id =0;
	maddr2_id=0;
	mc_date=0;
	mm_date=0;
	memset(mnote,0,sizeof(mnote));
}



void itemAccIfo::CopyItem(itemAccIfo *pitem, long flag)
{
	//this function just copy the content, don't copy the pointer.
	if (flag & 0x70000000) modifyflag=pitem->modifyflag;
	if (flag & 1 ) maccid=pitem->maccid;
	if (flag & (1<<1)) memcpy(mname,pitem->mname,sizeof(mname));
	if (flag & (1<<2)) mbrid=pitem->mbrid ;
	if (flag & (1<<3)) mstatus=pitem->mstatus;
	if (flag & (1<<4)) memccpy(mrrno,pitem->mrrno,0,sizeof(mrrno));
	if (flag & (1<<5)) mlinkacc=pitem->mlinkacc;
	if (flag & (1<<7)) mper_id=pitem->mper_id;
	if (flag & (1<<8)) maddr1_id =pitem->maddr1_id;
	if (flag & (1<<9)) maddr2_id=pitem->maddr2_id ;
	if (flag & (1<<10)) mc_date =pitem->mc_date;
	if (flag & (1<<11)) mm_date=pitem->mm_date ;
	if (flag & (1<<12)) memcpy(mnote,pitem->mnote,sizeof(mnote));


}


BOOL itemAccIfo::UpdateData()
{
	char sqlstr[1000];
	CTime currtime;
	char datestr[20];
	if (modifyflag==0) return TRUE;
	if (maccid==0) {//add new;
		maccid=theOrderDB.ChangeSysID("CurrAccid",1); 
		currtime=CTime::GetCurrentTime();
		mm_date=mc_date=currtime.GetTime();
		strcpy(datestr,currtime.Format("%m/%d/%y %H:%M:%S"));
		sprintf(sqlstr,"insert maccountifo "
			"(accid,name,brid,status,rrno,linkacc,per_id,add1_id,add2_id,c_date,m_date,note)"
			"values(%d,'%s',%d,%d,'%s',%d,%d,%d,%d,'%s','%s','%s')",
			maccid,mname,mbrid,mstatus,mrrno,mlinkacc,mper_id,maddr1_id ,maddr2_id ,datestr,datestr,mnote);
		modifyflag=0x3ffff; 
	} else {
		currtime=CTime::GetCurrentTime();
		mm_date=currtime.GetTime();
		modifyflag|=1<<11;
		CStrFunc tok;
		tok.InitToSet(sqlstr);
		tok.AppendStr("update maccountifo set ");
		if (modifyflag& 1<<1) 
			sprintf(tok.SeekToEnd(),"name='%s',",mname);
		if (modifyflag& 1<<2) 
			sprintf(tok.SeekToEnd(),"brid=%d,", mbrid);
		if (modifyflag& 1<<3) 
			sprintf(tok.SeekToEnd(),"status=%d,",mstatus);
		if (modifyflag& 1<<4) 
			sprintf(tok.SeekToEnd(),"rrno='%s',",mrrno);
		if (modifyflag& 1<<5) 
			sprintf(tok.SeekToEnd(),"linkacc=%d,",mlinkacc);
		if (modifyflag& 1<<7) 
			sprintf(tok.SeekToEnd(),"per_id=%d,",mper_id);
		if (modifyflag& 1<<8) 
			sprintf(tok.SeekToEnd(),"add1_id=%d,",maddr1_id );
		if (modifyflag& 1<<9)
			sprintf(tok.SeekToEnd(),"add2_id=%d,",maddr2_id ) ;
		if (modifyflag& 1<<12) 
			sprintf(tok.SeekToEnd(),"note='%s',",mnote);
		sprintf(tok.SeekToEnd(),"m_date='%s',",currtime.Format("%m/%d/%y %H:%M:%S"));
		tok.SeekToEnd();
		tok.CutLastChar();
		sprintf(tok.SeekToEnd()," where accid=%d ", maccid);
	}
	theCfgFile.mlog.AddLogStr(sqlstr); 
	TRY{
		theOrderDB.mdb.ExecuteSQL(sqlstr);
	}CATCH(CDBException, e) {
		char msg[1000];
		e->GetErrorMessage(msg,sizeof(msg));
		theCfgFile.mlog.AddLogStr(msg); 
//		e.Delete() ;
		return FALSE;
	}END_CATCH
	
	return TRUE;		

}

void itemPosition::CopyFrom(itemPosition *pitem)
{
	maccid=pitem->maccid;
	strcpy(msecsym,pitem->msecsym);
	mptype=pitem->mptype;
	mbrid=pitem->mbrid;
	mInitQty=pitem->mInitQty;
	mInitPrice=pitem->mInitPrice;
	mqty=pitem->mqty;
	mHoldqty=pitem->mHoldqty;  
	mavgcost=pitem->mavgcost;
	mBuyPwer=pitem->mBuyPwer;
	mRealize=pitem->mRealize;
	mc_date =pitem->mc_date;
	mm_date=pitem->mm_date;
}


BOOL itemPosition::MakeInfoPkg(char *pbuff, WORD flag)
{
	PkgToken tokret;
	tokret.InitToSet(pbuff);
	tokret.setWORD(1002);
	tokret.setWORD(flag);

	if (flag & iPosAcc) tokret.setlong(maccid);
	if (flag & iPosSec) tokret.setVarStr(msecsym);
	if (flag & iPosType) tokret.setBYTE(mptype);
	if (flag & iPosBrid) tokret.setlong(mbrid);
	if (flag & iPosInitQty) tokret.setlong(mInitQty);
	if (flag & iPosInitPric) tokret.setDouble(mInitPrice); 
	if (flag & iPosQty) tokret.setlong(mqty);
	if (flag & iPosAvgPric) tokret.setDouble(mavgcost);
	if (flag & iPosBP) tokret.setDouble(mBuyPwer);
	if (flag & iPosReal) tokret.setDouble(mRealize);
	if (flag & iPosCdat) tokret.setlong(mc_date);
	if (flag & iPosMdat) tokret.setlong(mm_date);
	tokret.finalizeSet();
	return TRUE;
}

BOOL itemPosition::GetPkg(PkgToken *ptok, WORD *pflag)
{
	WORD flag;
	flag=ptok->getWORD();

	if (flag & iPosAcc) maccid=ptok->getlong();
	if (flag & iPosSec) ptok->getString(msecsym,sizeof(msecsym));
	if (flag & iPosType) mptype=ptok->getBYTE();
	if (flag & iPosBrid) mbrid=ptok->getlong();
	if (flag & iPosInitQty) {
		int oldqty=mInitQty;
		mInitQty=ptok->getlong();
//		mHoldqty+=mInitQty-oldqty;
//		if (mHoldqty<0) mHoldqty=0; 
	}
	if (flag & iPosInitPric) mInitPrice=ptok->getDouble();
	if (flag & iPosQty) mqty=ptok->getlong();
	if (flag & iPosAvgPric) mavgcost=ptok->getDouble();
	if (flag & iPosBP) mBuyPwer=ptok->getDouble();
	if (flag & iPosReal) mRealize=ptok->getDouble();
	if (flag & iPosCdat) mc_date=ptok->getlong();
	if (flag & iPosMdat) mm_date=ptok->getlong();
	*pflag=flag;
	return TRUE;
}

// action:
// 0, add;1 modify, 2 delete
BOOL itemPosition::ModifyDB(WORD flag)
{
	ASSERT((flag & 0xE)==0xE);
	srstPosition rstpos;
//	rstpos.init(&theOrderDB.mdb);  
	try{
		rstpos.OpenToUpdate(FALSE,maccid,msecsym,mptype);
		if (rstpos.IsEOF()) {
			rstpos.mitem.CopyFrom(this);
			rstpos.AddNew(); 
		}else {
			rstpos.mDirtyFlag=flag;
			rstpos.mitem.CopyFrom(this);
			rstpos.Update();
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		
	

}

void itemOrder::CopyFrom(itemOrder *pitem)
{
	ASSERT(pitem!=NULL);
	memmove(this,pitem,sizeof(itemOrder));
}



BOOL itemOrder::GetPkg(PkgToken *ptok, DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getDWORD();
	if (flag&deforderid) morderid=ptok->getlong();
	if (flag&deftrid) mtrid=ptok->getlong();
	if (flag&defaccid) maccid=ptok->getlong();
	if (flag&defentrid) mentrid=ptok->getlong();
	if (flag&deforigoid) morigoid=ptok->getlong();
	if (flag&deforigtkn) morigtkn=ptok->getlong();
	if (flag&defbrid) mbrid=ptok->getlong();
	if (flag&defroute) ptok->getString(mroute,sizeof(mroute));
	if (flag&defbkrsym) ptok->getString(mbkrsym,sizeof(mbkrsym));
	if (flag&defstatus) mstatus=ptok->getlong();
	if (flag&defexecnum) mexecnum=ptok->getBYTE();
	if (flag&defconftxt) ptok->getString(mconftxt,sizeof(mconftxt));
	if (flag&deftoken) ptok->getString(mtoken,sizeof(mtoken)); 
	if (flag&defqty) mqty=ptok->getlong();
	if (flag&deflvsqty) mlvsqty=ptok->getlong();
	if (flag&defcxlqty) mcxlqty=ptok->getlong();
	if (flag&defsecsym) ptok->getString(msecsym,sizeof(msecsym));
	if (flag&defexchange) mexchange=ptok->getBYTE();
	if (flag&defprice) mprice=ptok->getDouble();
	if (flag&defstopprice) mstopprice=ptok->getDouble();
	if (flag&deftrailprice) mtrailprice=ptok->getDouble();
	if (flag&defaskprice) maskprice=ptok->getDouble();
	if (flag&defbidprice) mbidprice=ptok->getDouble();
	if (flag&defrrno) ptok->getString(mrrno,sizeof(mrrno));
	if (flag&deftmforce) mtmforce=ptok->getWORD();
	if (flag&defc_date) mc_date=ptok->getlong();
	if (flag&defm_date) mm_date=ptok->getlong();
	if (flag&defmexts1) ms1=ptok->getlong();
	if (flag&defmextf1) mf1=ptok->getlong();
	if (flag&defmextf2) mf2=ptok->getlong();
	if (flag&defmextf3) mf3=ptok->getlong();

	*pflag=flag;
	return TRUE;
}

BOOL itemOrder::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setDWORD(flag);
	if (flag&deforderid) ptok->setlong(morderid);
	if (flag&deftrid) ptok->setlong(mtrid);
	if (flag&defaccid) ptok->setlong(maccid);
	if (flag&defentrid) ptok->setlong(mentrid);
	if (flag&deforigoid) ptok->setlong(morigoid);
	if (flag&deforigtkn) ptok->setlong(morigtkn);
	if (flag&defbrid)	ptok->setlong(mbrid);
	if (flag&defroute) ptok->setVarStr(mroute);
	if (flag&defbkrsym) ptok->setVarStr(mbkrsym);
	if (flag&defstatus) ptok->setlong(mstatus);
	if (flag&defexecnum) ptok->setBYTE(mexecnum);
	if (flag&defconftxt) ptok->setVarStr(mconftxt);
	if (flag&deftoken) ptok->setVarStr(mtoken);
	if (flag&defqty) ptok->setlong(mqty);
	if (flag&deflvsqty) ptok->setlong(mlvsqty);
	if (flag&defcxlqty) ptok->setlong(mcxlqty);
	if (flag&defsecsym) ptok->setVarStr(msecsym);
	if (flag&defexchange) ptok->setBYTE(mexchange);
	if (flag&defprice) ptok->setDouble(mprice);
	if (flag&defstopprice) ptok->setDouble(mstopprice);
	if (flag&deftrailprice) ptok->setDouble(mtrailprice);
	if (flag&defaskprice) ptok->setDouble(maskprice);
	if (flag&defbidprice) ptok->setDouble(mbidprice);
	if (flag&defrrno) ptok->setVarStr(mrrno);
	if (flag&deftmforce) ptok->setWORD(mtmforce);
	if (flag&defc_date) ptok->setlong(mc_date);
	if (flag&defm_date) ptok->setlong(mm_date);
	if (flag&defmexts1) ptok->setlong(ms1);
	if (flag&defmextf1) ptok->setlong(mf1);
	if (flag&defmextf2) ptok->setlong(mf2);
	if (flag&defmextf3) ptok->setlong(mf3);


	return TRUE;
}


// check the new order's buypower and shortlist and etc'
// the pmsg is a string buff must be 20 character;

BOOL itemOrder::CheckNewOrder(DWORD flag, char *pmsg)
{
	if (msecsym[0]=='+') {
		if (theCfgFile.mbEnableOption==FALSE) {
			strncpy(pmsg,"Option disabled",20);
			return FALSE;
		}
	}

	Trader *ptr;
	ptr=Trader::FindTrader(mtrid);
	if (ptr ==NULL) {
		strncpy(pmsg,"Wrong Trader",20);
		return FALSE;
	}

	Account *pac;
	pac=Account::FindAcc(maccid);
	if (pac==NULL) {
		strncpy(pmsg,"Unexist Account!",20);
		return FALSE;
	}
	if (!pac->mitemifo.IsOpen()) {
		strncpy(pmsg,"Invalid Account!",20);
		return FALSE;
	}

	if (pac->mitemifo.mstatus&(1<<20)) {
		if (!theCfgFile.IsMktHours(0)){
			strncpy(pmsg,"Mkt Closed!",20);
			return FALSE;
		}

	}

	if (pac->mitemifo.mstatus &(1<<23)) {//the employee account;
		ms1|=(1<<9);
	}
	if (pac->mitemEquity.mOpenEQ<25000) {
		if ((pac->mitemifo.mstatus & (1<<31))==0){ 
			if (pac->GetRoundTripTrades()>3) {
				strncpy(pmsg,"3 roundtrips limit!",20);
				return FALSE;
			}
		}
	}

	if (mqty<=0) {
		strncpy(pmsg,"Invalid share!",20);
		return FALSE;
	}
	
	if (theCfgFile.mpIssueData!=NULL) {
		if (ptr->minfo.mstatus1&(1<<1)) {//check untradeable list;
			stritem *PIitem;
			PIitem=theCfgFile.mpIssueData->mSymbLst[0].FindItem(msecsym);
			if ((PIitem))  {
				strncpy(pmsg,"Sec isn't tradable!",20);
				return FALSE;
			}
		}
		if (ptr->minfo.mstatus1&(1<<2)){//check tradable list;
			stritem *PIitem;
			PIitem=theCfgFile.mpIssueData->mSymbLst[1].FindItem(msecsym);
			if (PIitem==NULL)  {
				strncpy(pmsg," ",20);
				return FALSE;
			}
		}
	}


	
	mstatus&=~((1<<31)|(1<<22)); // clearn the cover and naked flag;
	if (msecsym[0]=='+'){// options;
		if ((pac->mitemifo.mstatus&(1<<11))==0) {
			strncpy(pmsg,"Not Permitted!",20);
			return FALSE;
		}
			
		if (IsBuyOrder()){
			int pos, poshold;
			pos=pac->FindPositionNum(msecsym);
			poshold=pac->FindOrderHoldQty(msecsym,TRUE);// find the buy ordres;
			if ((-pos-mqty-poshold)<0) {//buy to open;
			}else {//has short position, buy cover;
				mstatus|=(1<<22);
			}
		}else { //sell order;
			int pos, poshold;
			pos=pac->FindPositionNum(msecsym);
			poshold=pac->FindOrderHoldQty(msecsym,FALSE);
			if ((pos-mqty-poshold)<0) {// sell short, or sell open;
				if ((pac->mitemifo.mstatus&(1<<12))==0) { // no write option
					strncpy(pmsg,"Not Permitted!",20);
					return FALSE;
				}
			}else {// sell long position, sell cover;
				mstatus|=(1<<22);
			}
	
			itemOptsym *poptsym;
			poptsym=itemOptsym::FindInfo(msecsym);
			if (poptsym==NULL) {
				strncpy(pmsg,"No OptInfo!",20);
				return FALSE;
				
			}
			
			Account::stPosInfo stposifo,stordifo;
			pac->FindPosInfo(poptsym->undsymb,&stposifo); 
			pac->FindOrderHoldIfo(poptsym->undsymb,&stordifo);
			if (poptsym->IsCall()) {
				int qty;
				qty=stposifo.mLong-stposifo.moptCWrite+stposifo.moptCLong;
				qty-=(stordifo.mShort+stordifo.moptCWrite);
				if((qty-(mqty*poptsym->pmundlyshr))<0) {
					mstatus|=(1<<31); //naked;
				}
			}else {
				int qty;
				qty=stposifo.mShort-stposifo.moptPWrite+stposifo.moptPLong;
				qty-=(stordifo.mLong+stordifo.moptPWrite);
				if((qty-(mqty*poptsym->pmundlyshr))<0) {
					mstatus|=(1<<31); //naked;
				}
			}
			if ((mstatus&(1<<31))&&((pac->mitemifo.mstatus&(1<<13))==0)) {// no naked option
				strncpy(pmsg,"Not Permitted!",20);
				return FALSE;
			}
		}
	
	}else if (IsFuture()) {// check if this is futures;
		if ((pac->mitemifo.mstatus&(1<<18))==0) {
			strncpy(pmsg,"Not Permitted!",20);
			return FALSE;
		}
		FutureIfo *pifo=theCfgFile.mpIssueData->GetFutureInfo(msecsym);
		if (pifo==NULL) {
			strncpy(pmsg,"Invalid Future!",20);
			return FALSE;
		}

		
	}else {
		//the equity don't need to the flag for cover or not, so we don't caculate.
		//


		//if trader has to check shortlist is short orde
		//check the trade is in shortlist;
		if (mstatus &(1<<29)) {
			if ((mstatus & (1<<6))==0) {
				mstatus|=(1<<7);
			}
		}
		if (IsShortOrder()&& (ptr->IsCheckShrtLst())) {
			int shortsz;
			int nlist=0;
			double shortpriceswitch=0;
			if (ptr->minfo.mstatus&(1<<31)){
				nlist=1;
				shortpriceswitch=theCfgFile.mShortPriceSwitch1;
			}else {
				nlist=0;
				shortpriceswitch=theCfgFile.mShortPriceSwitch;
			}
			shortsz=theCfgFile.GetShortAbleSize(msecsym,mexchange,nlist);

			if (shortsz<mqty) {
				strncpy(pmsg,"Sec isn't shortable!",20);
				return FALSE;
			}
			if ((mprice>0)&&(mprice<shortpriceswitch)) {
				strncpy(pmsg,"Sec isn't shortable!",20);
				return FALSE;
			}
		}
		if (!IsCashOrder()) {
/*			if (theCfgFile.GetMarginAble(msecsym,mexchange)==FALSE){
				strncpy(pmsg,"Sec isn't Marginable",20);
				return FALSE;
			}
*/
			if (!pac->mitemifo.IsMargin()) {
				strncpy(pmsg,"Not margin account",20);
				return FALSE;
			}
		}

		if ((pac->mitemifo.mstatus&(1<<13))==0) {// no naked option
			Account::stPosInfo stposifo,stordifo;
			pac->FindPosInfo(msecsym,&stposifo); 
			pac->FindOrderHoldIfo(msecsym,&stordifo);
			int oqty,cqty,lqty;
			if (IsBuyOrder()) {
				oqty=stposifo.moptPWrite+stordifo.moptPWrite;
				if (oqty>stposifo.mShort) {
					cqty=stposifo.mShort; 
					lqty=0;
				}else{
					cqty=oqty;
					lqty=stposifo.mShort-oqty;
				}
				if ((cqty>0)&&(mqty>lqty)){
					strncpy(pmsg,"Opt Will go Naked",20);
					return FALSE;
				}

			}else {
				oqty=stposifo.moptCWrite+stordifo.moptCWrite;
				if (oqty>stposifo.mLong) {
					cqty=stposifo.mLong; 
					lqty=0;
				}else{
					cqty=oqty;
					lqty=stposifo.mLong-oqty;
				}
				if ((cqty>0)&&(mqty>lqty)){
					strncpy(pmsg,"Opt Will go Naked",20);
					return FALSE;
				};

			}
		}

	}
	
	if (ptr->IsCheckBP()&&(!pac->mitemifo.IsDVPAccount())) {
		if (pac->CheckOrderBP(this,pmsg)==FALSE){
//			strncpy(pmsg,"BuyingPower!",20);
			return FALSE;
		}
/*
		if (pac->mitemEquity.mOpenEQ<=pac->mitemEquity.mRealized) {
			strncpy(pmsg,"Low Equity!",20);
			return FALSE;
		}
*/
	}

	if (pac->mitemCtrl.mMaxLoss>0.01) {
		if ((-(pac->mitemEquity.mRealized+pac->mitemEquity.mComm))>=pac->mitemCtrl.mMaxLoss){
			if (!pac->IsUnwindOrder(this)) {
				strncpy(pmsg,"Exceed MaxLoss!",20);
				return FALSE;
			}
		}
	}
	if (pac->mitemCtrl.maxshare>0) {
		if ((pac->mitemEquity.mtotalshares+mqty)>pac->mitemCtrl.maxshare) {
			if (!pac->IsUnwindOrder(this)) {
				strncpy(pmsg,"Exceed MaxShare!",20);
				return FALSE;
			}
		}
	}
	
//	if (pac->mitemCtrl.maxsymbshare>0) {}// in CheckOrderBP.

	
	// for trader, must check if they has the ablitity to trade.
	AC_TR_relation *prealtion;
	prealtion=AC_TR_relation::Finditem(mtrid,maccid);
	if (prealtion==NULL) {
		strncpy(pmsg,"Wrong Account",20);
		return FALSE;
	}
	if ((prealtion->mstatus&9)!=9) {
		strncpy(pmsg,"No Authority",20);
		return FALSE;
	}
	itemDeal *pdeal;
	int ndealid;
	switch(msecsym[0]) {
	case '+':
		ndealid=prealtion->mOpdealid;
		break;
	case '#':
		ndealid=prealtion->mFtdealid;
		break;
	default:
		ndealid=prealtion->mdealid;
	}
	if (ndealid==0) ndealid=prealtion->mdealid;
	pdeal=itemDeal::FindDeal(ndealid);
	if (pdeal==NULL) {
		pdeal=itemDeal::FindDeal(0);
	}
	if (pdeal==NULL) {
		strncpy(pmsg,"No deal related!",20);
		return FALSE;
	}

	itemDeal tmpdeal;
	tmpdeal.Clear();
	pdeal->GetDealValue(0x1,&tmpdeal);
	if (tmpdeal.msharelmt<mqty) {
		strncpy(pmsg,"Excess Share limit!",20);
		return FALSE;
	}
	return TRUE;

}

//cumstor send a new order to this Server
BOOL itemOrder::NewOrder(DWORD flag,char *pclient,char* errmsg,int flagex)
{
	OrderRouter *proute;
	itemOrderActive oac;
/*	itemOptsym *ios; 
	if (msecsym[0]=='+') {
		ios=itemOptsym::FindInfo(msecsym); 
	
	} else {
		ios=NULL;
	}
*/
	
	oac.Clear(); 
	if (CheckNewOrder(0,oac.mnote)==FALSE) {
		if (pclient) {
			oac.FillInfo(this);
			oac.SetType(LOASendRej); 
			oac.SendActive(pclient);
		}
		if (errmsg) {
			strcpy(errmsg,oac.mnote);
		}
		return FALSE;
	}
	
	
	proute=theCfgFile.FindRouter(this);//mroute);
	if (proute==NULL) {
		if (pclient) {
			oac.FillInfo(this);
			oac.SetType(LOASendRej); 
			strcpy(oac.mnote,"Wrong Route");
			oac.SendActive(pclient);
		}
		if (errmsg) {
			strcpy(errmsg,"Wrong Route");
		}
		return FALSE;
	}
	
	if (proute->IsExchDisable(msecsym,mexchange)){
		if (pclient) {
			oac.FillInfo(this);
			oac.SetType(LOASendRej); 
			strcpy(oac.mnote,"Route NoPermission!");
			oac.SendActive(pclient);

		}
		if (errmsg) {
			strcpy(errmsg,"Route NoPermission!");
		}
	
		return FALSE;
	}
	
	if (!proute->IsEnabled()) {
		if (!IsGTCOrder()){
			if (pclient) {
				oac.FillInfo(this);
				oac.SetType(LOASendRej); 
				strcpy(oac.mnote,"Route disable!");
				oac.SendActive(pclient);
			}
			if (errmsg) {
				strcpy(errmsg,"Route disable!");
			}
			return FALSE;
		}
	}
	
	if (proute->mNewOrderEnable==FALSE) {
		if (!IsGTCOrder()){
			if (pclient) {
				oac.FillInfo(this);
				oac.SetType(LOASendRej); 
				strcpy(oac.mnote,"Route no neworder!");
				oac.SendActive(pclient);
			}
			if (errmsg) {
				strcpy(errmsg,"Route no neworder!");
			}
			return FALSE;
		}
	}

	if (proute->m_bDisableCustomerOrder) {
		if (flagex & (1<<0)) {
			if (pclient) {
				oac.FillInfo(this);
				oac.SetType(LOASendRej); 
				strcpy(oac.mnote,"Route no neworder!");
				oac.SendActive(pclient);
			}
			if (errmsg) {
				strcpy(errmsg,"Route no neworder!");
			}
			return FALSE;
		}
	}

	{
		intitem *PI=proute->mAuthLst.FindItem(maccid);
		if (PI) {
			int val=PI->val;
			if (val&2) {//not allow;
				if (pclient) {
					oac.FillInfo(this);
					oac.SetType(LOASendRej); 
					strcpy(oac.mnote,"Route Acc Limited!");
					oac.SendActive(pclient);

				}
				if (errmsg) {
					strcpy(errmsg,"Route Acc Limited!");
				}
			
				return FALSE;
	
			}
		}
	}


	DWORD newflag;
	morderid=theCfgFile.GetNewOrderID();//(DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrOrderID);  
	newflag=flag;
	// do the Order process..............
	mlvsqty=mqty;
	newflag|=deflvsqty;
	mstatus&=(~0x3FL);//clean all other status
	mstatus|=1; //open order 
	ms1&=(~0x3FL);//clean all other status
	if (proute->IsAutoRoute()) mstatus|=(1<<23);
	mm_date=time(NULL);
	if(mc_date==0){
		mc_date=mm_date;
	}
	newflag|=(defm_date|defc_date);
	//----send order;
	BOOL bsending;
	itemOrder *porder;

	Account *pacc;
	pacc=Account::FindAcc(maccid);
	if (pacc==0) {
		if (errmsg) {
			strcpy(errmsg,"Wrong Account");
		}
		return FALSE;
	}
	pacc->Lock(); 
	char orderpkg[500];
	bsending=proute->SendNewOrder(this,orderpkg); 
	if (bsending) {
		porder=Account::AddNewOrder(this);
		if (pclient) {
			((CClient*)pclient)->SendOrderTknRply(1,morigtkn,morderid);
		}

		oac.FillInfo(this);
		oac.SetType(LOASending); 
	//	strcpy(oac.mnote,"Route disable!");
		oac.SendActive(mbrid);
		oac.SaveToDB();
	//After save to db, then send the order to order daemon;
		if (proute->IsEnabled()) {
			proute->SendPkg(orderpkg);  
			proute->AsyncSelect(FD_WRITE);
		}

		if (strlen(mconftxt)>0){
			newflag|=defconftxt; //mconftxt is valid.
		}
		//----------------------------
	//	AddToList();
		porder->ModifyDB(newflag);
		porder->SendChange(newflag);
		double hldpwr=pacc->ReCalOrderHoldBP();

		if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
			pacc->mitemEquity.mCurrHoldBP=hldpwr;
			char pkg[200];
			pacc->MakePkgByEquity(pkg,1<<8);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}

	}
	pacc->UnLock();
	return bsending;
}

//This function only send order, don't do anything else.
// used for the GTC order resending when the route is connected.
BOOL itemOrder::SendOrder() 
{
		DWORD newflag=0;
		OrderRouter *proute;

/* don't check the order, since this order is already been checked. 
	
		char message[200];
		if (CheckNewOrder(0,message)==FALSE) {
			theCfgFile.mLoginLog.AddLogStrFmtTm(0,"SendOrder Warn:oid:%d Sym:%s,%s", 
				morderid,msecsym,message); 
//			return FALSE;
		}
*/
		
		proute=theCfgFile.FindRouter(this);//mroute);
			
		if ((proute!=NULL)&&!proute->IsEnabled()) {
			theCfgFile.mlog.AddLogStrFmtTm(0,"SendOrder Err:oid:%d Sym:%s,%s", 
				morderid,msecsym,"Route disable!"); 
			return FALSE;
		}

		char orderpkg[500];
		BOOL bsending=proute->SendNewOrder(this,orderpkg); 
		if (bsending==FALSE) {
			theCfgFile.mlog.AddLogStrFmtTm(0,"SendOrder Err:oid:%d Sym:%s,%s", 
				morderid,msecsym,mconftxt); 
			return FALSE;
		}
		itemOrderActive oac;
		oac.Clear(); 

		oac.FillInfo(this);
		oac.SetType(LOASending); 
		oac.SendActive(mbrid);
		oac.SaveToDB();

		//After save to db, then send the order to order daemon;
		proute->SendPkg(orderpkg);  
		proute->AsyncSelect(FD_WRITE);

		newflag|=defstatus;
		newflag|=defmexts1;
		if (strlen(mconftxt)>0){
			newflag|=defconftxt; //mconftxt is valid.
		}
			//----------------------------
		//	AddToList();
		ModifyDB(newflag);
		SendChange(newflag);
/*		double hldpwr=pacc->ReCalOrderHoldBP();

		if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
			pacc->mitemEquity.mCurrHoldBP=hldpwr;
			char pkg[200];
			pacc->MakePkgByEquity(pkg,1<<8);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
*/		return TRUE;

}


BOOL itemOrder::ModifyDB(DWORD flag)
{

	char buff[2000];
	SaveStr(buff,sizeof(buff),flag);
	theCfgFile.mlog.AddLogStr(buff);
	if (theCfgFile.mRealTimeDBWrite==FALSE) return TRUE;
	srstOrders rstorder;
//	rstorder.init(&theOrderDB.mdb);  
	try {
		rstorder.OpenToUpdate(morderid);
		if (rstorder.IsEOF()) {
			rstorder.mitem.CopyFrom(this);
			rstorder.AddNew(); 
		}else {
			rstorder.mDirtyFlag=flag;
			rstorder.mitem.CopyFrom(this);
			rstorder.Update();
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		

}

BOOL itemPosition::SendChange(int change)
{
	char pkg[100];
	BOOL ret;
	ret=MakeInfoPkg(pkg,change);
	ASSERT(ret !=NULL);
	return Trader::SPKGAcc(pkg,maccid,mbrid,0); 
}

BOOL itemOrder::SendChange(DWORD change)
{
	char pkg[200];
	PkgToken tok;
	tok.InitToSet(pkg);
	tok.setWORD(1020);
	tok.setBYTE(1);
	SetPkg(&tok,change|deforderid);
	return Trader::SPKGAcc(tok.finalizeSet(),maccid,mbrid,0); 
}

//this function is used to make the package for query,
//by it status, maybe it can save some communition bytes.
//if no stope price or trailer price.
BOOL itemOrder::SetInfoPkg(char *pbuff,DWORD flag)
{
	PkgToken tok;
	tok.InitToSet(pbuff);
	tok.setWORD(1020);
	tok.setBYTE(1);
	SetPkg(&tok,flag);
	tok.finalizeSet();
	return TRUE;
}

BOOL itemOrder::CancelOrder(int norderid, int nlvqty, char *msg)
{
	itemOrder *porder;
	intitem *pintitem;
	ASSERT(msg!=NULL);
	*((WORD*)msg)=0;
	PkgToken tok;
	tok.InitToSet(msg); 
	tok.setWORD(1020);
	pintitem=itemOrder::sMapOrderID.FindItem(norderid);
	if (pintitem==NULL) {
		tok.setBYTE(7);// reject cancel;
		tok.setlong(norderid);
		tok.setVarStr("order doesn't exist");
		tok.finalizeSet();
		return TRUE;
	}
	porder=(itemOrder*)pintitem->val;
	return TRUE;
}



itemOrder * itemOrder::FindActiveOrder(int orderid)
{
	itemOrder *porder;
	intitem *pintitem;
	pintitem=itemOrder::sMapOrderID.FindItem(orderid);
	if (pintitem==NULL) {
		return NULL;
	}
	porder=(itemOrder*)pintitem->val;
	return porder;
}

void itemTrade::Clear()
{
	memset(this,0,sizeof(itemTrade));
	mcommision=-1;
}


BOOL itemTrade::ModifyDB(DWORD flag)
{
	char buff[2000];
	SaveStr(buff,sizeof(buff),flag);
	theCfgFile.mlog.AddLogStr(buff);
	if (theCfgFile.mRealTimeDBWrite==FALSE) return TRUE;
	srstTrade rsttrade;
//	rstorder.init(&theOrderDB.mdb);  
	try{
		rsttrade.OpenToUpdate(mtradeid);
		if (rsttrade.IsEOF()) {
			rsttrade.mitem.CopyFrom(this);
			rsttrade.AddNew(); 
		}else {
			rsttrade.mDirtyFlag=flag;
			rsttrade.mitem.CopyFrom(this);
			rsttrade.Update();
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		

}


itemTrade * itemTrade::FindTrade(long ntradeid)
{
	itemTrade *ptrade;
	intitem *pintitem;
	pintitem=sMapTradeID.FindItem(ntradeid);
	if (pintitem==NULL) {
		return NULL;
	}
	ptrade=(itemTrade*)pintitem->val;
	return ptrade;
}

BOOL itemTrade::GetPkg(PkgToken *ptok, DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getDWORD();
	if (flag&deftradeid	) mtradeid=ptok->getlong();
	if (flag&deftrid	) mtrid=ptok->getlong();
	if (flag&defaccid	) maccid=ptok->getlong();
	if (flag&defentrid	) mentrid=ptok->getlong();
	if (flag&deforigacc	) morigacc=ptok->getlong();
	if (flag&deforigtrid) morigtrid=ptok->getlong();
	if (flag&deforigoid	) morigoid=ptok->getlong(); 
	if (flag&deforderid	) morderid=ptok->getlong();
	if (flag&defbrid	) mbrid=ptok->getlong();
	if (flag&defroute	) ptok->getString(mroute,sizeof(mroute));
	if (flag&defbkrsym	) ptok->getString(mbkrsym,sizeof(mbkrsym));
	if (flag&deftrtype	) mtrtype=ptok->getBYTE();
	if (flag&defstatus	) mstatus=ptok->getlong();
	if (flag&defexecnum	) mexecnum=ptok->getBYTE();
	if (flag&defconftxt	) ptok->getString(mconftxt,sizeof(mconftxt));
	if (flag&deftoken	) ptok->getString(mtoken,sizeof(mtoken));
	if (flag&defqty		) mqty=ptok->getlong();
	if (flag&defprice	) mprice=ptok->getDouble();
	if (flag&defsecsym	) ptok->getString(msecsym,sizeof(msecsym));
	if (flag&defexchange) mexchange=ptok->getBYTE();
	if (flag&defecnfee)   mEcnFee=ptok->getDouble();
	if (flag&deftradepl)  mTradePL=ptok->getDouble();
	if (flag&defrrno	) ptok->getString(mrrno,sizeof(mrrno));
	if (flag&defcommision) mcommision=ptok->getDouble();
	if (flag&defe_date	) me_date=ptok->getlong();
	if (flag&defc_date	) mc_date=ptok->getlong();
	if (flag&defm_date	) mm_date=ptok->getlong();
	*pflag=flag;
	return TRUE;

}

BOOL itemTrade::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setDWORD(flag);
	if (flag&deftradeid	) ptok->setlong(mtradeid);
	if (flag&deftrid	) ptok->setlong(mtrid);
	if (flag&defaccid	) ptok->setlong(maccid);
	if (flag&defentrid	) ptok->setlong(mentrid);
	if (flag&deforigacc	) ptok->setlong(morigacc);
	if (flag&deforigtrid) ptok->setlong(morigtrid);
	if (flag&deforigoid	) ptok->setlong(morigoid);
	if (flag&deforderid	) ptok->setlong(morderid);
	if (flag&defbrid	) ptok->setlong(mbrid);
	if (flag&defroute	) ptok->setVarStr(mroute);
	if (flag&defbkrsym	) ptok->setVarStr(mbkrsym);
	if (flag&deftrtype	) ptok->setBYTE(mtrtype);
	if (flag&defstatus	) ptok->setlong(mstatus);
	if (flag&defexecnum	) ptok->setBYTE(mexecnum);
	if (flag&defconftxt	) ptok->setVarStr(mconftxt);
	if (flag&deftoken	) ptok->setVarStr(mtoken);
	if (flag&defqty		) ptok->setlong(mqty);
	if (flag&defprice	) ptok->setDouble(mprice);
	if (flag&defsecsym	) ptok->setVarStr(msecsym);
	if (flag&defexchange) ptok->setBYTE(mexchange);
	if (flag&defecnfee)   ptok->setDouble(mEcnFee);
	if (flag&deftradepl)  ptok->setDouble(mTradePL);
	if (flag&defrrno	) ptok->setVarStr(mrrno);
	if (flag&defcommision)ptok->setDouble(mcommision);
	if (flag&defe_date	) ptok->setlong(me_date);
	if (flag&defc_date	) ptok->setlong(mc_date);
	if (flag&defm_date	) ptok->setlong(mm_date);
	return TRUE;

}



BOOL itemTrade::SetInfoPkg(char *pbuff)
{
	PkgToken tok;
	tok.InitToSet(pbuff);
	tok.setWORD(1021);
	tok.setBYTE(1);
	SetPkg(&tok,0xffffffff);
	tok.finalizeSet();
	return TRUE;
}

void itemTrade::CopyFrom(itemTrade *ptrade)
{
	ASSERT(ptrade!=NULL);
	memmove(this,ptrade,sizeof(itemTrade));
}

BOOL itemTrade::SendChange(DWORD change)
{
	char pkg[200];
	PkgToken tok;
	tok.InitToSet(pkg);
	tok.setWORD(1021);
	tok.setBYTE(1);
	change |= 1;//make sure that the Tradeid will be send.
	SetPkg(&tok,change);
	return Trader::SPKGAcc(tok.finalizeSet(),maccid,mbrid,0); 

}

void itemTrade::CopyFrom(itemTrade *pitem, DWORD flag)
{

	if (flag&deftradeid		) mtradeid=pitem->mtradeid;
	if (flag&deftrid		) mtrid=pitem->mtrid;
	if (flag&defaccid		) maccid=pitem->maccid;
	if (flag&defentrid		) mentrid=pitem->mentrid;
	if (flag&deforigacc		) morigacc=pitem->morigacc;
	if (flag&deforigtrid	) morigtrid=pitem->morigtrid;
	if (flag&deforigoid		) morigoid =pitem->morigoid;
	if (flag&deforderid		) morderid=pitem->morderid;
	if (flag&defbrid		) mbrid=pitem->mbrid;
	if (flag&defroute		) strcpy(mroute,pitem->mroute);
	if (flag&defbkrsym		) strcpy(mbkrsym,pitem->mbkrsym);
	if (flag&deftrtype		) mtrtype=pitem->mtrtype;
	if (flag&defstatus		) mstatus=pitem->mstatus;
	if (flag&defexecnum		) mexecnum=pitem->mexecnum;
	if (flag&defconftxt		) strcpy(mconftxt,pitem->mconftxt);
	if (flag&deftoken		) strcpy(mtoken,pitem->mtoken);
	if (flag&defqty			) mqty=pitem->mqty;
	if (flag&defprice		) mprice=pitem->mprice;
	if (flag&defsecsym		) strcpy(msecsym,pitem->msecsym);
	if (flag&defexchange	) mexchange=pitem->mexchange;
	if (flag&defecnfee  	) mEcnFee=pitem->mEcnFee;
	if (flag&deftradepl	)     mTradePL=pitem->mTradePL;
	if (flag&defrrno		) strcpy(mrrno,pitem->mrrno);
	if (flag&defcommision	) mcommision=pitem->mcommision;
	if (flag&defe_date		) me_date=pitem->me_date;
	if (flag&defc_date		) mc_date=pitem->mc_date;
	if (flag&defm_date		) mm_date=pitem->mm_date;
}




itemTrade * itemTrade::FindTradeByExe(long accid, long orderid, BYTE exeseq)
{
	itemTrade *ptrade;
	itemTrade *ret;
	intitem *pitem;
	itemTrade::sMapTradeID.LockMap();
	itemTrade::sMapTradeID.EnumFirst(&pitem);
	ret=NULL;
	while(pitem!=NULL) {
		ptrade=(itemTrade*)pitem->val;
		ASSERT(ptrade !=NULL);
		//ASSERT(ptrade->maccid==accid);
		if ((ptrade->morderid==orderid)&&(ptrade->mexecnum==exeseq)) {
			ret=ptrade;
			break;// find the execute;
		}
		itemTrade::sMapTradeID.EnumNext(&pitem);  
	}
	itemTrade::sMapTradeID.UnlockMap(); 
	return ret;

}

BOOL itemTrade::CopyFromOrder(itemOrder *porder)
{	
	ASSERT(porder!=NULL);
	mtrid=porder->mtrid; 
	maccid=porder->maccid;
	mentrid=porder->mentrid;
	morigacc=porder->maccid;
	morigtrid=porder->mtrid;
	morderid=porder->morderid; 
	morigoid=porder->morigoid;
	mbrid=porder->mbrid;
	strcpy(mroute,porder->mroute);
	strcpy(mbkrsym,porder->mbkrsym);
	mtrtype='O';
	mstatus=(porder->mstatus & ~0x3f) | 1;
	strcpy(mconftxt,porder->mconftxt);
	strcpy(msecsym,porder->msecsym);
	mexchange=porder->mexchange; 
//	maskprice=porder->maskprice;
//	mbidprice=porder->mbidprice;
	strcpy(mrrno,porder->mrrno);
	strcpy(mtoken,porder->mtoken);
	return TRUE;
}

BOOL itemOrder::SetOrderOpenQty(int Qty)
{
	mlvsqty=Qty;
	//change buying power;
	return TRUE;
}

void itemPosition::Clear()
{
	memset(this,0,sizeof(itemPosition));

}

void itemAccEquity::Clear()
{
	memset(this,0,sizeof(itemAccEquity));

/*
	mOpenBP=0;		// open buying powering
	mOpenNBP=0;	// open Night buying powing
	mOpenEQ=0;		// open equity
	mOpenMGEP=0;	// open margin equity
	mOpenEQRQ=0;	// open Equity require;
	mCurrBP=0;		// current buying poweing 
	mCurrNBP=0;	// current overnight buying power,
	mCurrEQ=0;		// current EQutiy
	mCurrMGEQ=0;	// current Margin EQuity;
	mCurrEQRQ=0;	// current EQurity Require;
	mEQm_date=0;	// equity modify flag;
*/
}

WORD itemAccEquity::GetPkg(PkgToken *ptok)
{
	WORD flag=0;
	flag=ptok->getWORD();
	if (flag&defaccid		)	maccid=ptok->getlong();
	if (flag&defOpenBP		)	mOpenBP=ptok->getDouble();		// open buying powering
	if (flag&defOpenNBP		)	mOpenNBP=ptok->getDouble() ;	// open Night buying powing
	if (flag&defOpenEQ		)	mOpenEQ=ptok->getDouble() ;		// open equity
	if (flag&defOpenMGEQ	)	mOpenMGEQ=ptok->getDouble() ;	// open margin equity
	if (flag&defOpenEQRQ	)	mOpenEQRQ=ptok->getDouble() ;	// open Equity require;
	if (flag&defCurrBP		)	mCurrBP=ptok->getDouble() ;		// current buying poweing 
	if (flag&defCurrNBP		)	mCurrNBP=ptok->getDouble() ;	// current overnight buying power,
	if (flag&defCurrHoldBP	)	mCurrHoldBP=ptok->getDouble() ;	// current Order Hold Buying power;
	if (flag&defCurrEQ		)	mCurrEQ=ptok->getDouble() ;		// current EQutiy
	if (flag&defCurrMGEQ	)	mCurrMGEQ=ptok->getDouble() ;	// current Margin EQuity;
	if (flag&defCurrEQRQ	)	mCurrEQRQ=ptok->getDouble() ;	// current EQurity Require;
	if (flag&defComm		)   mComm=ptok->getDouble();		// current commision if this account
	if (flag&deftktnum		)	mtktnum=ptok->getlong();		// the last modify time;
	if (flag&defRealized	)   mRealized=ptok->getDouble(); 
	if (flag&defm_time		)   mm_time=ptok->getlong(); 
	return flag;
}

BOOL itemAccEquity::SetPkg(PkgToken *ptok, WORD flag)
{
	ptok->setWORD(flag);
	if (flag & defaccid		)	ptok->setlong(maccid);
	if (flag & defOpenBP	)	ptok->setDouble(mOpenBP);		// open buying powering
	if (flag & defOpenNBP	)	ptok->setDouble(mOpenNBP) ;	// open Night buying powing
	if (flag & defOpenEQ	)	ptok->setDouble(mOpenEQ) ;		// open equity
	if (flag & defOpenMGEQ	)	ptok->setDouble(mOpenMGEQ) ;	// open margin equity
	if (flag & defOpenEQRQ	)	ptok->setDouble(mOpenEQRQ) ;	// open Equity require;
	if (flag & defCurrBP	)	ptok->setDouble(mCurrBP) ;		// current buying poweing 
	if (flag & defCurrNBP	)	ptok->setDouble(mCurrNBP) ;	// current overnight buying power,
	if (flag & defCurrHoldBP)	ptok->setDouble(mCurrHoldBP) ;	// current Order Hold Buying power;
	if (flag & defCurrEQ	)	ptok->setDouble(mCurrEQ) ;		// current EQutiy
	if (flag & defCurrMGEQ	)	ptok->setDouble(mCurrMGEQ) ;	// current Margin EQuity;
	if (flag & defCurrEQRQ	)	ptok->setDouble(mCurrEQRQ) ;	// current EQurity Require;
	if (flag & defComm		)	ptok->setDouble(mComm);			//current commision;
	if (flag & deftktnum	)	ptok->setlong(mtktnum);		// the last modify time;
	if (flag & defRealized	)	ptok->setDouble(mRealized);
	if (flag & defm_time	)	ptok->setlong(mm_time);		//=ptok->getlong(); 
	return TRUE;

}

WORD itemAccIfo::GetPkg(PkgToken *ptok)
{
	WORD flag;
	flag=ptok->getWORD();
	if (flag & 1)		maccid=ptok->getlong(); 
	if (flag & (1<<1))	ptok->getString(mname,sizeof(mname));
	if (flag & (1<<2))	mbrid=ptok->getDWORD();
	if (flag & (1<<3))  mstatus=ptok->getDWORD();
	if (flag & (1<<4))  ptok->getString(mrrno,sizeof(mrrno));
	if (flag & (1<<5))	mlinkacc =ptok->getDWORD();
	if (flag & (1<<7)) 	mper_id =ptok->getDWORD();
	if (flag & (1<<8))	maddr1_id=ptok->getDWORD();
	if (flag & (1<<9))	maddr2_id =ptok->getDWORD();
	if (flag & (1<<10)) mc_date =ptok->getlong();
	if (flag & (1<<11)) mm_date =ptok->getlong();
	if (flag & (1<<12)) ptok->getString(mnote,sizeof(mnote));
	return flag;
}

BOOL itemAccIfo::SetPkg(PkgToken *ptok, WORD flag)
{
	ptok->setWORD(flag);
	if (flag &1)		ptok->setDWORD(maccid); // the accid;
	if (flag&(1<<1))	ptok->setVarStr(mname); 
	if (flag & (1<<2))	ptok->setlong(mbrid); 
	if (flag& (1<<3))	ptok->setDWORD(mstatus);  
	if (flag& (1<<4))	ptok->setVarStr(mrrno);
	if (flag & (1<<5))	ptok->setDWORD(mlinkacc);
	if (flag & (1<<7))	ptok->setDWORD(mper_id);
	if (flag &(1<<8))	ptok->setDWORD(maddr1_id);
	if (flag & (1<<9))	ptok->setDWORD(maddr2_id);
	if (flag & (1<<10)) ptok->setlong(mc_date );
	if (flag & (1<<11)) ptok->setlong(mm_date );
	if (flag&(1<<12))	ptok->setVarStr(mnote);
	return TRUE;
}

itemPosition * itemPosition::GetVoidBuff()
{
	return (itemPosition*)mitembuff.GetVoidBuff();
}

void itemPosition::PutVoidBuff(itemPosition *pitem)
{
	ASSERT(pitem!=NULL);
	mitembuff.PutVoidBuff((char*)pitem);
}

void itemPosition::InitBuff()
{
	mitembuff.init(sizeof(itemPosition),102400); 
}

void itemTrade::InitBuff()
{
 	sitembuff.init(sizeof(itemTrade),102400);  
	sMapTradeID.init(3000); 
}

void itemOrder::InitBuff()
{
	sitembuff.init(sizeof(itemOrder),102400);  
	sMapOrderID.init(3000);
}


void itemPosition::SetToInit()
{
	mRealize=0;
	mqty=mInitQty;
	mHoldqty=mInitQty; 
	mavgcost=mInitPrice;
	mBuyPwer=0;
}

void itemAccEquity::SetToInit()
{
	mCurrBP=mOpenBP;
	mCurrEQ=mOpenEQ;
	mCurrEQRQ=mOpenEQRQ;
//	mCurrHoldBP=0; 
	mCurrMGEQ=mOpenMGEQ;
	mCurrNBP=mOpenNBP;
	mComm=0;
	mtktnum=0; 
	mRealized=0;
	mtotalshares=0; 
}

void itemAccEquity::CopyFrom(itemAccEquity *pitem)
{
	memcpy(this,pitem,sizeof(itemAccEquity));
}

WORD itemAccEquity::CompareItem(itemAccEquity *pitem)
{
	WORD flag=0;

	if (maccid!=pitem->maccid)			(flag|=defaccid		);
	if (mOpenBP!=pitem->mOpenBP)		(flag|=defOpenBP	);		// open buying powering
	if (mOpenNBP!=pitem->mOpenNBP)		(flag|=defOpenNBP	);	// open Night buying powing
	if (mOpenEQ!=pitem->mOpenEQ)		(flag|=defOpenEQ	);		// open equity
	if (mOpenMGEQ!=pitem->mOpenMGEQ)	(flag|=defOpenMGEQ	);	// open margin equity
	if (mOpenEQRQ!=pitem->mOpenEQRQ)	(flag|=defOpenEQRQ	);	// open Equity require;
	if (mCurrBP!=pitem->mCurrBP)		(flag|=defCurrBP	);		// current buying poweing 
	if (mCurrNBP!=pitem->mCurrNBP)		(flag|=defCurrNBP	);	// current overnight buying power,
	if (mCurrHoldBP!=pitem->mCurrHoldBP)(flag|=defCurrHoldBP);	// current Order Hold Buying power;
	if (mCurrEQ!=pitem->mCurrEQ)		(flag|=defCurrEQ	);		// current EQutiy
	if (mCurrMGEQ!=pitem->mCurrMGEQ)	(flag|=defCurrMGEQ	);	// current Margin EQuity;
	if (mCurrEQRQ!=pitem->mCurrEQRQ)	(flag|=defCurrEQRQ	);	// current EQurity Require;
	if (mComm !=pitem->mComm)			(flag|=defComm		);
	if (mtktnum!=pitem->mtktnum)		(flag|=deftktnum	); 
	if (mRealized!=pitem->mRealized)	(flag|=defRealized	);
	if (mm_time!=pitem->mm_time)		(flag|=defm_time	);		// the last modify time;

	return flag;
}


void itemAccCtrl::CopyFrom(itemAccCtrl *pitem)
{
	memcpy(this,pitem,sizeof(itemAccCtrl));
}




//this function caculate the hold buying power per share
//
double itemOrder::GetHoldBPRate(Account *pacc)
{
	double retval;
	double nDayMarginRatio=theCfgFile.mDayMargBPRate;
	double nShortHoldRatio=1;
	if (pacc->mitemifo.mstatus & (1<<30)) {// PLBP change buying power with the P/L
		if (IsShortOrder()) {
			nDayMarginRatio=0.5;
			nShortHoldRatio=0.5;
		}else {
			nDayMarginRatio=1;
		}
	}


	if (msecsym[0]=='+') {
		if (IsBuyOrder()) {
			retval=nDayMarginRatio*mprice;
		}else {
			itemOptsym *pops;
			pops=itemOptsym::FindInfo(msecsym);
			if (pops) {
				if (pops->mcurrundprice>0) {
					retval=((double)pops->mcurrundprice)/1000;
				}else {
					retval=((double)pops->strikepice)/1000; 
				}
			}else {
				retval=nDayMarginRatio*mprice;
			}
		}
	}else if (IsFuture()) {
		FutureIfo *pifo=theCfgFile.mpIssueData->GetFutureInfo(msecsym);
		if (pifo==NULL){
			retval=2000;
		}else {
			retval=pifo->m_DayInit; 
		}
		if (ms1&(1<<7)) nDayMarginRatio=2;
		return retval*nDayMarginRatio;
	}else{
		double effprice;
		if (IsStopOrder()) {
			if (IsBasicStopOrder() ) {
				if (IsMarketOrder()) {
					effprice=mstopprice;
				}else{
					if (IsBuyOrder()) {
						effprice=mprice;
					}else {
						effprice=(mprice>mbidprice)?mprice:mbidprice;
					}
				}
			}else if(IsTrailerOrder()) {
				effprice=maskprice*1.1; // hold mMKTOrderBPHoldRate more;
			}else if (IsRangeOrder()) {
				effprice=(mprice>mstopprice)?mprice:mstopprice;
			}else {
				if (IsBuyOrder()) {
					effprice=mprice;
				}else {
					effprice=(mprice>mbidprice)?mprice:mbidprice;
				}
			}
		}else {
			if (IsMarketOrder()) {
				if (IsBuyOrder()) {
					effprice=maskprice;
				}else {
					effprice=mbidprice; 
				}
			}else {
				if (IsBuyOrder()) {
					effprice=mprice;
				}else {
					effprice=(mprice>mbidprice)?mprice:mbidprice;
				}
			}
		}
		if (effprice<=0.000000001) {
			effprice=500;
		}

		if (IsCashOrder()) {//CASH
			if (ms1&(1<<7)) nDayMarginRatio=2;
			if (IsMarketOrder()) {
				retval=nDayMarginRatio*effprice*theCfgFile.mMKTOrderBPHoldRate ;
			}else {
				retval=nDayMarginRatio*effprice;
			}
		}else {//
			BOOL bFullRequire=FALSE;
			if (!theCfgFile.GetMarginAble(msecsym,mexchange)){
				theCfgFile.mlog.AddLogStrFmt("Debug:%s is not margainable",msecsym); 
				bFullRequire=TRUE;
			}else {
				if (mprice<theCfgFile.mMKTHoldSWCHPrice){
					if (pacc->mitemifo.mstatus&(1<<26)) {//don't need full requirement for <3 dollar.
					}else {
						bFullRequire=TRUE;
					}
				}
			}
			if (bFullRequire) {//(mprice<theCfgFile.mMKTHoldSWCHPrice)||(!theCfgFile.GetMarginAble(msecsym,mexchange))){	//less
				if (ms1&(1<<7)) nDayMarginRatio=2;
				if (IsMarketOrder()) {
					retval=nDayMarginRatio*effprice*theCfgFile.mMKTOrderBPHoldRate;
				}else {
					retval=nDayMarginRatio*effprice;
				}
			}else {		//more than 6$ should be margin able
				if (IsMarketOrder()) {
					retval=effprice*theCfgFile.mMKTOrderBPHoldRate;
				}else {
					retval=effprice;
				}
			}
			if (IsShortOrder()) {
				retval*=nShortHoldRatio; 
			}
		}
		
	}
	return retval;
}

void itemDeal::Init()
{
	itemPassthrufee::sitemPassTFbuff.init(sizeof(itemPassthrufee), sizeof(itemPassthrufee)*1000);
	sitemDealbuff.init(sizeof(itemDeal),sizeof(itemDeal)*1000);
	sMapDealID.init(1000);
	srstDeal srst;
	srst.mitem.Clear();
	srst.OpenToRead();
	while(!srst.IsEOF()) {
		
		srst.mitem.FixFlag(); //fix the flag;
		
		srst.mitem.Add_Modify_Map();		
		srst.mitem.Clear();
		srst.MoveNext();
	}
	srst.Close();

	srstPassthruFee srstpass;
	srstpass.OpenToRead();
	itemDeal *pitem;
	itemPassthrufee *pitempath; 
	while(!srstpass.IsEOF()) {
		
		srstpass.mitem.FixFlag(); //fix the flag;

		pitem=FindDeal(srstpass.mdealid);
		if (pitem!=NULL) {
			pitempath=pitem->FindPathFee(srstpass.mitem.mpath);
			if (pitempath!=NULL) {
				pitempath->CopyFrom(&srstpass.mitem);
			}else {
				pitempath=(itemPassthrufee*)itemPassthrufee::sitemPassTFbuff.GetVoidBuff();
				pitempath->CopyFrom(&srstpass.mitem);
				pitempath->pnext=pitem->ppathfee;
				pitem->ppathfee=pitempath;
			}
		}
		srstpass.MoveNext();
	}
	srstpass.Close();
}

BOOL itemDeal::Add_Modify_Map()
{
	itemDeal *plst;
	intitem *pintitem;
	pintitem=sMapDealID.FindItem(mdealid);
	if (pintitem==NULL) {
		plst=(itemDeal*)sitemDealbuff.GetVoidBuff();
		plst->Clear(); 
		plst->CopyFrom(this);
		sMapDealID.Add(mdealid,(long)plst);
		return TRUE;
	}
	plst=(itemDeal*)pintitem->val;
	plst->CopyFrom(this);
	return TRUE;
}

void itemDeal::CopyFrom(itemDeal *pdeal)
{
	mdealid=pdeal->mdealid;
	strcpy(mname,pdeal->mname);
	mdefaultdeal=pdeal->mdefaultdeal;
	mshareadj=pdeal->mshareadj;
	msharelmt=pdeal->msharelmt;
	mlimitadj=pdeal->mlimitadj;
	mmktadj=pdeal->mmktadj;
	mvaladj=pdeal->mvaladj; 
	mbaseshare=pdeal->mbaseshare;
	mbasecharge=pdeal->mbasecharge;
	msharecap=pdeal->msharecap;
	mcommcap=pdeal->mcommcap;
	mmincomm=pdeal->mmincomm; 
	msharecap1=pdeal->msharecap1;
	mshareadj1=pdeal->mshareadj1; 
	strcpy(mnote,pdeal->mnote);
	mm_date=pdeal->mm_date;
	mbrid=pdeal->mbrid;
	mflag=pdeal->mflag;
	msymbolfee=pdeal->msymbolfee;
}

BOOL itemDeal::Clear()
{
	memset(this,0,sizeof(itemDeal));
	return TRUE;
}

itemDeal * itemDeal::FindDeal(long dealid)
{
	intitem *pintitem;
	pintitem=sMapDealID.FindItem(dealid);
	if (pintitem==NULL) {
		return NULL;
	}
	return ((itemDeal*)pintitem->val);
}

itemPassthrufee * itemDeal::FindPathFee(LPCSTR ppath)
{
	itemPassthrufee *pret;
	pret=ppathfee;
	while(pret!=NULL) {
		if (strcmp(ppath,pret->mpath)==0) return pret;
		pret=pret->pnext;
	}
	return NULL;
}

void itemPassthrufee::CopyFrom(itemPassthrufee *pitem)
{
	strcpy(mpath,pitem->mpath);
	mflag=pitem->mflag;
	costpershrA=pitem->costpershrA;
	costperexe=pitem->costperexe;
	costpershr=pitem->costpershr;
	costpertkt=pitem->costpertkt;
}

void itemTicket::InitBuff()
{
 	sitembuff.init(sizeof(itemTicket),102400);  
}

itemTicket * itemTicket::GetVoidBuff()
{
	itemTicket *pret;
	pret=(itemTicket*)sitembuff.GetVoidBuff();
	if (pret==NULL) return NULL;
	memset(pret,0,sizeof(itemTicket));
	return pret;
}

void itemTicket::PutToBuff()
{
	ASSERT(this!=NULL);
	sitembuff.PutVoidBuff((char*)this); 
}

itemTrade * itemTrade::GetVoidBuff()
{
	itemTrade *pret;
	pret=(itemTrade*) sitembuff.GetVoidBuff();
	if (pret==NULL) return NULL;
	memset(pret,0,sizeof(itemTrade));
	return pret;
}

void itemTrade::PutToBuff()
{
	ASSERT(this!=NULL);
	sitembuff.PutVoidBuff((char*)this); 
}

//This is the commision calauate 
//
double itemTicket::GetCommision(Account *pacc,intlst *tktlst)
{
	if (IsFixedComm()) return mcommision;
	int ndealid;
	AC_TR_relation *prelation;
	prelation=AC_TR_relation::Finditem(mtrid,maccid);
	if (prelation==NULL) ndealid=0;
	else {
		switch(msecsym[0]) {
		case '+':
			ndealid=prelation->mOpdealid;
			break;
		case '#':
			ndealid=prelation->mFtdealid;
			break;
		default:
			ndealid=prelation->mdealid;
		}
		if (ndealid==0) ndealid=prelation->mdealid;
	}
	itemDeal *pdeal;
	pdeal=itemDeal::FindDeal(ndealid);
	if (pdeal==NULL) {
		pdeal=itemDeal::FindDeal(0);
	}
	if (pdeal==NULL) return 0;

	itemDeal tmpdeal;
	tmpdeal.Clear();
	pdeal->GetDealValue(0xffff,&tmpdeal);


	double retcomm;
	retcomm=tmpdeal.mbasecharge;
	if (pdeal->mflag & (1<<28)) {
		int qty=mqty;
		while(qty>tmpdeal.mbaseshare) {
			retcomm+=tmpdeal.mbasecharge;
			qty-=tmpdeal.mbaseshare;
		}
	}else 	if (mqty>tmpdeal.mbaseshare) {
		if (mqty<tmpdeal.msharecap) { 
			retcomm+=(mqty-tmpdeal.mbaseshare)*tmpdeal.mshareadj;
		}else {
			retcomm+=(tmpdeal.msharecap-tmpdeal.mbaseshare)*tmpdeal.mshareadj;
			if (tmpdeal.msharecap1>tmpdeal.msharecap) {
				if (mqty<tmpdeal.msharecap1){
					retcomm+=(mqty-tmpdeal.msharecap)*tmpdeal.mshareadj1;
				}else {
					retcomm+=(tmpdeal.msharecap1-tmpdeal.msharecap)*tmpdeal.mshareadj1;
				}
			}
		}
	}


	if (IsMarketOrder()) {
		retcomm+=tmpdeal.mmktadj;
	}else {
		retcomm+=tmpdeal.mlimitadj;
	}
	
	if (tmpdeal.mvaladj>0){
		retcomm+=tmpdeal.mvaladj * mqty * mprice;
	}

	//	check the cost pathree;
	itemPassthrufee tmppathfee; 
	if (IsPhoneTrade()){ 
		memset(&tmppathfee,0,sizeof(itemPassthrufee)); 
		pdeal->GetPathFeeValue(0xf,"PHONE",&tmppathfee);
		
		retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe*mexecnum;
		retcomm+=tmppathfee.costpershr*mqty;
	}

	double persymbolfee=0;
	if (pdeal->msymbolfee>0.000001) {
		BOOL isFirstTicket=TRUE;
		itemTicket *pticket;
		intitem *PIitem; 
		tktlst->EnumFirst(&PIitem);
		while(PIitem) {
			pticket=(itemTicket *)PIitem->val;
			if (pticket!=this){
				if (strcmp(pticket->msecsym,msecsym)==0) {
					isFirstTicket=FALSE;
					break;
				}
			}

			PIitem=PIitem->pnext;
		}
		if (isFirstTicket) persymbolfee=pdeal->msymbolfee;
	}



	if (theCfgFile.mbEcnFeeInCap){
		retcomm+=mcomm1; // don't caculate ecnfee again;
		if (retcomm>tmpdeal.mcommcap) {
			retcomm=tmpdeal.mcommcap;
		}
		if (retcomm<tmpdeal.mmincomm) {
			retcomm=tmpdeal.mmincomm;
		}
		retcomm+=persymbolfee;
	}else {
		if (retcomm>tmpdeal.mcommcap) {
			retcomm=tmpdeal.mcommcap;
		}
		if (retcomm<tmpdeal.mmincomm) {
			retcomm=tmpdeal.mmincomm;
		}
		retcomm+=mcomm1; // don't caculate ecnfee again;
		retcomm+=persymbolfee;
	}

	return retcomm;
}
/*
This function find the values of this deal, if some values
is default, go to find the default value.
flag is defined as bitwise combine of values
	0x1	: sharelimit;
	0x2	: shareadj;
	0x4	: limitadj;
	0x8	: marketadj;
	0x10: baseshare;
	0x20: basecharge;
	0x40: sharecap;
	0x80: commcap;
	0x100: valueadj;
	0x200: minumn commision;
	0x400: sharecap1;
	0x800: shareadj1;
*/
BOOL itemDeal::GetDealValue(int flag, itemDeal *pdeal)
{
	int tmpflag;
	tmpflag=flag&0xfff;
	itemDeal *pdefdel;
	pdefdel=this;
	int level=50;
	while(pdefdel){
		if (level==0) return FALSE;
		level--;
		if (tmpflag&0x1) {
			if (pdefdel->mflag&0x1) {
				pdeal->msharelmt=pdefdel->msharelmt;
				tmpflag &= ~0x1;
			}
		}
		if (tmpflag&0x2) {
			if (pdefdel->mflag&0x2) {
				pdeal->mshareadj=pdefdel->mshareadj;
				tmpflag &= ~0x2;
			}
		}
		if (tmpflag&0x4) {
			if (pdefdel->mflag &0x04) {
				pdeal->mlimitadj=pdefdel->mlimitadj;
				tmpflag&= ~0x4;
			}
		}
		if (tmpflag&0x8) {
			if (pdefdel->mflag&0x08) {
				pdeal->mmktadj=pdefdel->mmktadj;
				tmpflag&= ~0x8;
			}
		}
		if (tmpflag &0x10) {
			if (pdefdel->mflag&0x10) {
				pdeal->mbaseshare=pdefdel->mbaseshare;
				tmpflag&= ~0x10;
			}
		}
		if (tmpflag & 0x20) {
			if (pdefdel->mflag&0x20) {
				pdeal->mbasecharge=pdefdel->mbasecharge;
				tmpflag&=~0x20;
			}
		}
		if(tmpflag & 0x40) {
			if (pdefdel->mflag&(0x40)) {
				pdeal->msharecap=pdefdel->msharecap;
				tmpflag&=~0x40;
			}
		}

		if (tmpflag&0x80) {
			if (pdefdel->mflag&0x80) {
				pdeal->mcommcap=pdefdel->mcommcap;
				tmpflag&=~0x80;
			}
		}
		if (tmpflag&0x100) {
			if (pdefdel->mflag& 0x100){
				pdeal->mvaladj=pdefdel->mvaladj;
				tmpflag&=~0x100;
			}
		}
		
		if (tmpflag&0x200) {
			if (pdefdel->mflag& 0x200){
				pdeal->mmincomm=pdefdel->mmincomm;
				tmpflag&=~0x200;
			}
		}
		if (tmpflag & 0x400) {
			if (pdefdel->mflag& 0x400){
				pdeal->msharecap1=pdefdel->msharecap1;
				tmpflag&=~0x400;
			}
		}

		if (tmpflag & 0x800) {
			if (pdefdel->mflag& 0x800){
				pdeal->mshareadj1=pdefdel->mshareadj1;
				tmpflag&=~0x800;
			}
		}

		
		if (tmpflag==0) return TRUE;
		if (pdefdel->mdealid==0) return TRUE;
		pdefdel=FindDeal(pdefdel->mdefaultdeal);
		if (pdefdel==NULL){
			pdefdel=FindDeal(0);
		}
	}
	return FALSE;
}

/*
This function find the values of this deal, if some values
is default, go to find the default value.
flag is defined as bitwise combine of values
	0x1: costpertxt;
	0x2: costperexe;
	0x4: costpershr;
	0x8: costpercancel;
*/

BOOL itemDeal::GetPathFeeValue(int flag,LPCSTR ppath,itemPassthrufee *ppathfee)
{
	int tmpflag;
	tmpflag=flag&0x0f;

	itemDeal *pdefdel;
	pdefdel=this;
	itemPassthrufee *pfee;
	int level=50;
	while(pdefdel){
		if (level==0) return FALSE; //avoid unlimit recurse;
		level--;

		pfee=pdefdel->FindPathFee(ppath);
		if (pfee!=NULL) {
			if (tmpflag&0x1) {
				if (pfee->mflag&0x1) {
					ppathfee->costpertkt=pfee->costpertkt;
					tmpflag &=~0x1;
				}
			}
			if (tmpflag&0x2) {
				if (pfee->mflag&0x2) {
					ppathfee->costperexe=pfee->costperexe;
					tmpflag &=~0x2;
				}
			}
			if (tmpflag&0x4) {
				if (pfee->mflag&0x4) {
					ppathfee->costpershr=pfee->costpershr;
					tmpflag &=~0x4;
				}
			}
			if (tmpflag&0x8) {
				if (pfee->mflag&0x8) {
					ppathfee->costpershrA=pfee->costpershrA;
					tmpflag &=~0x8;
				}
			}
			if (tmpflag==0) return TRUE;
			if (pdefdel->mdealid==0) return TRUE;
		}
		pdefdel=FindDeal(pdefdel->mdefaultdeal);
		if (pdefdel==NULL){
			pdefdel=FindDeal(0);
		}
	}
	return FALSE;
}

BOOL itemTicket::SetInfoPkg(char *pbuff)
{
	PkgToken tok;
	tok.InitToSet(pbuff);
	tok.setWORD(1022);
	tok.setBYTE(1);
	SetPkg(&tok,0x1ffffff);
	tok.finalizeSet();
	return TRUE;
}

BOOL itemTicket::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setDWORD(flag);
	if (flag&(1<<0)) ptok->setlong(mticketid);
	if (flag&(1<<1)) ptok->setlong(mtrid);
	if (flag&(1<<2)) ptok->setlong(maccid);
	if (flag&(1<<3)) ptok->setlong(mentrid);
	if (flag&(1<<4)) ptok->setlong(mtradeid);
	if (flag&(1<<5)) ptok->setlong(morderid);
	if (flag&(1<<6)) ptok->setlong(mbrid);
	if (flag&(1<<7)) ptok->setVarStr(mroute);
	if (flag&(1<<8)) ptok->setVarStr(mbkrsym);
	if (flag&(1<<9)) ptok->setlong(mstatus);
	if (flag&(1<<10)) ptok->setBYTE(mexecnum);
	if (flag&(1<<11)) ptok->setlong(mqty);
	if (flag&(1<<12)) ptok->setDouble(mprice);
	if (flag&(1<<13)) ptok->setVarStr(msecsym);
	if (flag&(1<<14)) ptok->setBYTE(mexchange);
	if (flag&(1<<15)) ptok->setVarStr(mrrno);
	if (flag&(1<<16)) ptok->setDouble(mcommision);
	if (flag&(1<<17)) ptok->setDouble(mcomm1);
	if (flag&(1<<18)) ptok->setlong(me_date);
	if (flag&(1<<19)) ptok->setlong(mm_date);
	return TRUE;
}

void itemTicket::CopyFrom(itemTicket *pticket)
{
	memcpy(this,pticket,sizeof(itemTicket));
}

BOOL itemAccEquity::UpdateDB()
{

	if (maccid<=0) return FALSE;
	srstAccEquity rstaccequity;
	try {
		rstaccequity.OpenToUpdate(FALSE,maccid);
		memcpy(&rstaccequity.mitem,this,sizeof(itemAccEquity));
		if (rstaccequity.IsEOF()) { 
			rstaccequity.AddNew();
		}else {
			rstaccequity.Update();
		}
		rstaccequity.Close();
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;

}

int itemMessage::GetPkgInfo(PkgToken *ptok)
{
	ptok->getString(mFr,sizeof(mFr));
	ptok->getString(mTo,sizeof(mTo));
	mid=ptok->getlong();
	ptok->getString(mTitle,sizeof(mTitle));
	mflag=ptok->getlong();
	mPrior=ptok->getBYTE();
	ptok->getString(mtxt1,sizeof(mtxt1));
	ptok->getString(mtxt2,sizeof(mtxt2));
	ptok->getString(mtxt3,sizeof(mtxt3));
	ptok->getString(mtxt4,sizeof(mtxt4));
	ms_date=ptok->getlong();
	return 0;
}

int itemMessage::SetPkgInfo(PkgToken *ptok)
{
	ptok->setVarStr(mFr);
	ptok->setVarStr(mTo);
	ptok->setlong(mid);
	ptok->setVarStr(mTitle);
	ptok->setlong(mflag);
	ptok->setBYTE(mPrior);
	ptok->setVarStr(mtxt1);
	ptok->setVarStr(mtxt2);
	ptok->setVarStr(mtxt3);
	ptok->setVarStr(mtxt4);
	ptok->setlong(ms_date);
	return 0;
}

void itemMessage::Clear()
{
	memset(mFr,0,sizeof(mFr));
	memset(mTo,0,sizeof(mTo));
	mid=0;
	memset(mTitle,0,sizeof(mTitle));
	mflag=0;
	mPrior=0;
	memset(mtxt1,0,sizeof(mtxt1));
	memset(mtxt2,0,sizeof(mtxt2));
	memset(mtxt3,0,sizeof(mtxt3));
	memset(mtxt4,0,sizeof(mtxt4));
	ms_date=0;
}

void itemOrderActive::Clear()
{
	memset(this,0,sizeof(itemOrderActive));
}

BOOL itemOrderActive::GetPkg(PkgToken *ptok)
{
	mtrid=ptok->getlong();
	maccid=ptok->getlong();
	mflag=ptok->getlong();
	morderid=ptok->getlong();
	ptok->getString(msecsym,sizeof(msecsym));
	ptok->getString(mroute,sizeof(mroute));
	mprice=ptok->getDouble();
	mqty=ptok->getlong();
	mlogtime=ptok->getlong();
	ptok->getString(mnote,sizeof(mnote));
	return TRUE;
}

BOOL itemOrderActive::SetPkg(PkgToken *ptok)
{
	ptok->setlong(mtrid);
	ptok->setlong(maccid);
	ptok->setlong(mflag);
	ptok->setlong(morderid);
	ptok->setVarStr(msecsym);
	ptok->setVarStr(mroute);
	ptok->setDouble(mprice);
	ptok->setlong(mqty);
	ptok->setlong(mlogtime);
	ptok->setVarStr(mnote); 
	return TRUE;
}

BOOL itemOrderActive::SendActive(int brid)
{
	char pkg[200];
	if (mlogtime==0) mlogtime=time(NULL);
	PkgToken tok;
	tok.InitToSet(pkg);
	tok.setWORD(1023);
	tok.setBYTE(1);
	tok.setBYTE(1);
	SetPkg(&tok);
	BOOL bret=Trader::SPKGAcc(tok.finalizeSet(),maccid,brid,0); 
	if (theCfgFile.mbRecOrderActiveToLog) {
		char str[1000];
		itemOrderActive::SaveStr(str,sizeof(str),0xffffffff);
		theCfgFile.mlog.AddLogStr(str);
	}
	return bret;
}

BOOL itemOrderActive::FillInfo(itemOrder *porder)
{
	mtrid=porder->mtrid;
	maccid=porder->maccid;
	if (porder->morderid!=0) {
		morderid=porder->morderid;
	}else if (porder->morigoid!=0) {
		morderid=porder->morigoid;
	}
//	time(&mlogtime);
	mprice=porder->mprice;
	mqty=porder->mqty; 
	strcpy(msecsym,porder->msecsym);
	strcpy(mroute,porder->mroute);
	if (porder->IsBuyOrder()) {
		mflag |=0x100;
	}
	if (porder->IsShortOrder()) {
		mflag |=0x200;
	}
	if (porder->IsStopOrder()) {
		mflag|=0x400;
		if (porder->IsTrailerOrder()) {
			mflag|=0x800;
			mprice=porder->mtrailprice; 
		}else {
			mprice=porder->mstopprice; 
		}
	}
	return TRUE;
}


//#define LOASending		1
//#define LOASendRej		3
//#define LOAAccept		2

//#define LOACanceling	4	
//#define LOACanceled		5
//#define LOACancelRej	6

//#define LOATimeout		7

//#define LOAExecuted		8
BOOL itemOrderActive::SetType(BYTE btype)
{
	mflag&=~0xff;
	mflag|=btype;
	return TRUE;
}

BOOL itemOrderActive::SaveToDB()
{
/* don't save order active in database.
	srstOrderActive srst;
	try{
		if (srst.OpenToAdd()) {
			memcpy(&srst.mitem,this,sizeof(itemOrderActive));
			srst.AddNew();
			srst.Close();
			return TRUE;
		}
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return FALSE;
*/
	return TRUE;
}

BOOL itemOrderActive::SendActive(char *pclient)
{
	char pkg[200];
	if (mlogtime==0) mlogtime=time(NULL);
	PkgToken tok;
	tok.InitToSet(pkg);
	tok.setWORD(1023);
	tok.setBYTE(1);
	tok.setBYTE(1);
	SetPkg(&tok);
	CClient* p;
	p=(CClient*) pclient;
	p->SendPkg(tok.finalizeSet());
	p->AsyncSelect(FD_WRITE); 

	if (theCfgFile.mbRecOrderActiveToLog) {
		char str[1000];
		itemOrderActive::SaveStr(str,sizeof(str),0xffffffff);
		theCfgFile.mlog.AddLogStr(str);
	}
	return TRUE;
	//Trader::SPKGAcc(tok.finalizeSet(),maccid,brid,0); 
}

void itemAccEquity::SetOpenDayBP(double bp)
{
	double incr;
	incr=bp-mOpenBP;
	mOpenBP=bp;
	mCurrBP+=incr; 
}

void itemAccEquity::SetOpenNightBP(double bp)
{
	double incr;
	incr=bp-mOpenNBP;
	mOpenNBP=bp;
	mCurrNBP=bp;//+=incr; 
}

void itemAccEquity::SetOpenTotalEQ(double totaleq)
{
	double incr;
	incr=totaleq-mOpenEQ;
	mOpenEQ=totaleq;
	mCurrEQ+=incr;
	mOpenMGEQ+=incr;
	mCurrMGEQ+=incr;
	mOpenEQRQ-=incr;
	mCurrEQRQ-=incr; 
}

void itemAccEquity::SetOpenMarginEQ(double margeq)
{
	double incr;
	incr=margeq-mOpenMGEQ;
	mOpenMGEQ=margeq;
	mCurrMGEQ+=incr;
}

double itemPosition::GetYClose()
{
	double ret;
	ret=theCfgFile.mpIssueData->GetYestDayClose(msecsym);
	if (ret!=0) return ret;
	return mInitPrice;

}


BOOL itemOrder::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%O,");
	sprintf(tok.SeekToEnd(),"%d,", morderid);
	sprintf(tok.SeekToEnd(),"%d,", mtrid);
	sprintf(tok.SeekToEnd(),"%d,", maccid);
	sprintf(tok.SeekToEnd(),"%d,", mentrid);
	sprintf(tok.SeekToEnd(),"%d,", morigoid);
	sprintf(tok.SeekToEnd(),"%d,", morigtkn);
	sprintf(tok.SeekToEnd(),"%d,", mbrid);
	sprintf(tok.SeekToEnd(),"%s,", mroute);
	sprintf(tok.SeekToEnd(),"%s,", mbkrsym);
	sprintf(tok.SeekToEnd(),"%d,", mstatus);
	sprintf(tok.SeekToEnd(),"%d,", mexecnum);
	sprintf(tok.SeekToEnd(),"%s,", mconftxt);
	sprintf(tok.SeekToEnd(),"%s,", mtoken);
	sprintf(tok.SeekToEnd(),"%d,", mqty);
	sprintf(tok.SeekToEnd(),"%d,", mlvsqty);
	sprintf(tok.SeekToEnd(),"%d,", mcxlqty);
	sprintf(tok.SeekToEnd(),"%s,", msecsym);
	sprintf(tok.SeekToEnd(),"%d,", mexchange);
	sprintf(tok.SeekToEnd(),"%g,", mprice);
	sprintf(tok.SeekToEnd(),"%g,", mstopprice);
	sprintf(tok.SeekToEnd(),"%g,", mtrailprice);
	sprintf(tok.SeekToEnd(),"%g,", maskprice);
	sprintf(tok.SeekToEnd(),"%g,", mbidprice);
	sprintf(tok.SeekToEnd(),"%s,", mrrno);
	sprintf(tok.SeekToEnd(),"%d,", mtmforce);
	sprintf(tok.SeekToEnd(),"%d,", mc_date);
	sprintf(tok.SeekToEnd(),"%d,", mm_date);
	sprintf(tok.SeekToEnd(),"%d,",ms1); 
	sprintf(tok.SeekToEnd(),"%d,",mf1); 
	sprintf(tok.SeekToEnd(),"%d,",mf2); 
	sprintf(tok.SeekToEnd(),"%d,",mf3); 
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

BOOL itemTrade::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%T,");
	sprintf(tok.SeekToEnd(),"%d,", mtradeid);
	sprintf(tok.SeekToEnd(),"%d,", mtrid);
	sprintf(tok.SeekToEnd(),"%d,", maccid);
	sprintf(tok.SeekToEnd(),"%d,", mentrid);
	sprintf(tok.SeekToEnd(),"%d,", morigacc);
	sprintf(tok.SeekToEnd(),"%d,", morigtrid);
	sprintf(tok.SeekToEnd(),"%d,", morigoid);
	sprintf(tok.SeekToEnd(),"%d,", morderid);
	sprintf(tok.SeekToEnd(),"%d,", mbrid);
	sprintf(tok.SeekToEnd(),"%s,", mroute);
	sprintf(tok.SeekToEnd(),"%s,", mbkrsym);
	sprintf(tok.SeekToEnd(),"%d,", mtrtype);
	sprintf(tok.SeekToEnd(),"%d,", mstatus);
	sprintf(tok.SeekToEnd(),"%d,", mexecnum);
	sprintf(tok.SeekToEnd(),"%s,", mconftxt);
	sprintf(tok.SeekToEnd(),"%s,", mtoken);
	sprintf(tok.SeekToEnd(),"%d,", mqty);
	sprintf(tok.SeekToEnd(),"%g,", mprice);
	sprintf(tok.SeekToEnd(),"%s,", msecsym);
	sprintf(tok.SeekToEnd(),"%d,", mexchange);
	sprintf(tok.SeekToEnd(),"%g,", mEcnFee);
	sprintf(tok.SeekToEnd(),"%g,", mTradePL);
	sprintf(tok.SeekToEnd(),"%s,", mrrno);
	sprintf(tok.SeekToEnd(),"%g,", mcommision);
	sprintf(tok.SeekToEnd(),"%d,", me_date);
	sprintf(tok.SeekToEnd(),"%d,", mc_date);
	sprintf(tok.SeekToEnd(),"%d,", mm_date);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;

}

DWORD itemOrder::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigoid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigtkn=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mconftxt,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtoken,ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mlvsqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mcxlqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mstopprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mtrailprice=atof(ptmp);
	ptmp=tok.GetStr(',');	maskprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mbidprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mtmforce=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)	ms1=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)	mf1=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)   mf2=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)   mf3=atoi(ptmp);

	return 0xffffffff;
}

DWORD itemTrade::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	mtradeid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigacc=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigoid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mtrtype=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mconftxt,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtoken,ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	mEcnFee=atof(ptmp);
	ptmp=tok.GetStr(',');	mTradePL=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mcommision=atof(ptmp);
	ptmp=tok.GetStr(',');	me_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	return 0xffffffff;
}


BOOL itemTrade::SaveAllToDB()
{
	char sqlstr[500];
	sprintf(sqlstr,"delete mtrades");
	theOrderDB.mdb.ExecuteSQL(sqlstr);
	srstTrade rsttrade;

	try{
		rsttrade.OpenToUpdate(0);
		intitem *PIitem;
		itemTrade *ptr;
		sMapTradeID.EnumFirst(&PIitem);
		while(PIitem){
			ptr=(itemTrade *)PIitem->val;
			if (ptr) {
				rsttrade.mitem.CopyFrom(ptr);
				rsttrade.AddNew(); 
			}
			sMapTradeID.EnumNext(&PIitem);
		}

	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		

}

BOOL itemOrder::SaveAllToDB()
{
	char sqlstr[500];
	sprintf(sqlstr,"delete morders");
	theOrderDB.mdb.ExecuteSQL(sqlstr);

	srstOrders rstorder;

	rstorder.OpenToUpdate(0);
	intitem *PIitem;
	itemOrder *pod;
	try{
		sMapOrderID.EnumFirst(&PIitem);
		while(PIitem){
			pod=(itemOrder*)PIitem->val;
			if (pod) {
				rstorder.mitem.CopyFrom(pod);
				rstorder.AddNew(); 
			}
			sMapOrderID.EnumNext(&PIitem);
		}

	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	return TRUE;		

}

void itemOrder::Clear()
{
	memset(this,0,sizeof(itemOrder));
}

void itemOptsym::InitBuff()
{
	sitembuff.init(sizeof(itemOptsym),102400);  
	sMapSymb.init(500);
}

itemOptsym * itemOptsym::FindAddInfo(itemOptsym *psym, BOOL bAdd,BOOL bAddDB)
{
	if (psym==NULL) return NULL;
	stritem *PIitem;
	itemOptsym *pitem;
	PIitem=sMapSymb.FindItem(psym->optsymb);
	pitem=NULL;
	if (PIitem==NULL) {
		if (bAdd==TRUE) {
			pitem=(itemOptsym*)sitembuff.GetVoidBuff();
			pitem->CopyFrom(psym); 
			sMapSymb.Add(pitem->optsymb,(long)pitem); 
			if (bAddDB) {
				srstOptsym rst;
				try{
					rst.OpenToUpdate(psym->optsymb);
					rst.mitem.CopyFrom(psym);
					if (rst.IsEOF()) {
						rst.AddNew();
					}else{
						rst.Update();
					}
					rst.Close();
				}catch(SRSTErr *perr) {
					char str[100];
					sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
					theCfgFile.mlog.AddLogStr(str);
					theCfgFile.mlog.AddLogStr(perr->merrmsg);
					delete perr;
				}
			}
		}
	}else {
		pitem=(itemOptsym*)PIitem->val; 
		if ((psym->strikepice>0)&&(psym->strikepice!=pitem->strikepice)) {//strike price changed;
			pitem->strikepice=psym->strikepice;
			pitem->pmundlyshr=psym->pmundlyshr;
			if ( bAddDB) {//update the database also.
				srstOptsym rst;
				try{
					rst.OpenToUpdate(psym->optsymb);
					rst.mitem.CopyFrom(psym);
					if (rst.IsEOF()) {
						rst.AddNew();
					}else{
						rst.Update();
					}
					rst.Close();
				}catch(SRSTErr *perr) {
					char str[100];
					sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
					theCfgFile.mlog.AddLogStr(str);
					theCfgFile.mlog.AddLogStr(perr->merrmsg);
					delete perr;
				}
			}
		}
	}
	return pitem;
}

void itemOptsym::CopyFrom(itemOptsym *pitem)
{
	ASSERT(pitem);
	memcpy(this,pitem,sizeof(itemOptsym));
}

itemOptsym * itemOptsym::FindInfo(LPCSTR psymb)
{
	if (psymb==NULL) return NULL;
	stritem *PIitem;
	itemOptsym *pitem;
	PIitem=sMapSymb.FindItem(psymb);
	pitem=NULL;
	if (PIitem) pitem=(itemOptsym*)PIitem->val;
	return pitem;
}

BOOL itemOptsym::IsCall()
{
	int len;
	len=strlen(optsymb);
	if (len<=3) return FALSE;
	if (optsymb[len-2]<='L') return TRUE;
	else return FALSE;
}

int itemOrder::GetOpenBpQty()
{
	if (IsOpenOrder()==FALSE) return 0;
	if (msecsym[0]=='+') {
		itemOptsym *popf;
		popf=itemOptsym::FindInfo(msecsym);
		if (popf==NULL) {
			return 100*mlvsqty;
		}else {
			return mlvsqty*popf->pmundlyshr;
		}
	}else {
		return mlvsqty;
	}
}

int itemPosition::GetEffectQty()
{
	if (msecsym[0]=='+') {
		itemOptsym *popf;
		popf=itemOptsym::FindInfo(msecsym);
		if (popf==NULL) {
			return 100*mqty;
		}else {
			return mqty*popf->pmundlyshr;
		}
	}else {
		return mqty;
	}
}

void itemDeal::FixFlag()
{
	if ((mflag==0)) {
		if (msharelmt!=-1) {
			mflag |= 0x1;
		}
		if (mshareadj!=-1){
			mflag |= 0x2;
		}
		if (mlimitadj!=-1) {
			mflag |= 0x4;
		}
		if (mmktadj!=-1) {
			mflag |= 0x8;
		}
		if (mbaseshare!=-1) {
			mflag |= 0x10;
		}
		if (mbasecharge!=-1) {
			mflag |=0x20;
		}
		if (msharecap!=-1) {
			mflag |=0x40;
		}
		if (mcommcap!=-1) {
			mflag |=0x80;
		}
		if (mvaladj!=-1) {
			mflag |=0x100;
		}
	}
}

void itemPassthrufee::FixFlag()
{
	if (mflag==0){
		if (costpertkt!=-1) {
			mflag |= 0x1;
		}
		if (costperexe!=-1) {
			mflag |= 0x2;
		}
		if (costpershr!=-1) {
			mflag |= 0x4;
		}
		if (costpershrA!=-1) {
			mflag |= 0x8;
		}
	}
}

itemDeal * itemDeal::GetVoidDeal()
{
	itemDeal *pdeal;
	pdeal=(itemDeal*)sitemDealbuff.GetVoidBuff();
	pdeal->Clear(); 
	return pdeal;
}

BOOL itemDeal::PutVoidDeal(itemDeal *pdeal)
{
	itemPassthrufee  *p1,*p2;
	p1=p2=pdeal->ppathfee;
	while(p1!=NULL) {
		p2=p1->pnext;
		itemPassthrufee::sitemPassTFbuff.PutVoidBuff((char *)p1);
		p1=p2;
	}
	pdeal->ppathfee=NULL;
	sitemDealbuff.PutVoidBuff((char*) pdeal);
	return TRUE;
}

void itemDeal::CopyDeal(itemDeal *pdeal)
{
	CopyFrom(pdeal);

	itemPassthrufee  *p1,*p2;
	p1=p2=ppathfee;
	while(p1!=NULL) {
		p2=p1->pnext;
		itemPassthrufee::sitemPassTFbuff.PutVoidBuff((char *)p1);
		p1=p2;
	}
	ppathfee=NULL;
	
	p1=pdeal->ppathfee;
	while(p1) {
		p2=(itemPassthrufee *)itemPassthrufee::sitemPassTFbuff.GetVoidBuff();
		p2->CopyFrom(p1); 
		p2->pnext=ppathfee;
		ppathfee=p2;
		p1=p1->pnext;
	}
}

BOOL itemDeal::GetPkgInfoNew(PkgToken *ptok,int ver)
{
	int pathnu;
	mdealid=ptok->getlong();  
	mflag=ptok->getlong();
	mbrid=ptok->getlong();
	ptok->getString(mname,sizeof(mname));
	mdefaultdeal=ptok->getlong();
	if (mflag&0x1) msharelmt=ptok->getlong();
	else msharelmt=-1;
	if (mflag&0x2) mshareadj=ptok->getCpDouble();
	else mshareadj=-1;
	if (mflag& 0x4) mlimitadj=ptok->getCpDouble();
	else mlimitadj=-1;
	if (mflag & 0x8) mmktadj=ptok->getCpDouble();
	else mmktadj=-1;
	if (mflag & 0x10) mbaseshare=ptok->getlong();
	else mbaseshare=-1;
	if (mflag & 0x20) mbasecharge=ptok->getCpDouble();
	else mbasecharge=-1;
	if (mflag & 0x40) msharecap=ptok->getlong();
	else msharecap=-1;
	if (mflag & 0x80) mcommcap=ptok->getCpDouble();
	else mcommcap=-1;

	if (ver>1) {
		if (mflag & 0x100) mvaladj=ptok->getCpDouble();
		else mvaladj=-1;
		if (ver>2) {
			if (mflag& 0x200) mmincomm=ptok->getCpDouble(); 
		}
		if (ver>3) {
			if (mflag &0x400) msharecap1=ptok->getlong();
			if (mflag &0x800) mshareadj1=ptok->getCpDouble();  
		}
		if (ver>4) {
			msymbolfee=ptok->getCpDouble();  
		}
	}
	
	ptok->getString(mnote,sizeof(mnote));
	mm_date=ptok->getlong(); //time(&pdeal->mm_date);
	pathnu=ptok->getBYTE();
	
	itemPassthrufee  *p1,*p2;
	p1=p2=ppathfee;
	while(p1!=NULL) {
		p2=p1->pnext;
		itemPassthrufee::sitemPassTFbuff.PutVoidBuff((char *)p1);
		p1=p2;
	}
	ppathfee=NULL;

	int i;
	BYTE pathflag;
	for (i=0;i<pathnu;i++) {
		p1=(itemPassthrufee*)itemPassthrufee::sitemPassTFbuff.GetVoidBuff();
		pathflag=ptok->getBYTE();
		p1->mflag=pathflag;
		ptok->getString(p1->mpath,sizeof(p1->mpath));
		if (pathflag&0x1) p1->costpertkt=ptok->getCpDouble();
		else p1->costpertkt=-1;
		if (pathflag&0x2) p1->costperexe=ptok->getCpDouble();
		else p1->costperexe=-1; 
		if (pathflag&0x4) p1->costpershr=ptok->getCpDouble();
		else p1->costpershr=-1;
		if (pathflag&0x8) p1->costpershrA=ptok->getCpDouble();
		else p1->costpershrA=-1;
		p1->pnext =ppathfee;
		ppathfee =p1;
	}
	return TRUE;

}

BOOL itemDeal::SaveDealToDB()
{
	try{
		srstDeal rstdeal;
		rstdeal.OpenToUpdate(mdealid); 
		if (rstdeal.IsEOF()) {
			rstdeal.mitem.CopyFrom(this);
			rstdeal.AddNew();
		}else {
			rstdeal.mitem.CopyFrom(this);
			rstdeal.Update();
		}
		rstdeal.Close();
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
	}

	// delete the old pathfee information;
	itemPassthrufee *p1;
	try{
		srstPassthruFee rstpfee;
		rstpfee.OpenToUpdate(mdealid);
		while(!rstpfee.IsEOF())
		{
			rstpfee.Delete();
			rstpfee.MoveNext();
		}
		p1=ppathfee;
		while(p1!=NULL) {
			rstpfee.mitem.CopyFrom(p1);
			rstpfee.AddNew();
			p1=p1->pnext;
		}
		rstpfee.Close();
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;

	}
	return TRUE;
}

BOOL itemDeal::SetPkgInfoNew(PkgToken *ptok,int ver)
{
	if (ver>=2)
		ptok->setWORD(1033); // the package id is 1031 
	else 
		ptok->setWORD(1032); // the package id is 1031 
	ptok->setBYTE(1);	// information package;
	ptok->setlong(mdealid);
	ptok->setlong(mflag);
	ptok->setlong(mbrid);
	ptok->setVarStr(mname);
	ptok->setlong(mdefaultdeal);
	if (mflag& 0x1) ptok->setlong(msharelmt);
	if (mflag& 0x2) ptok->setCpDouble(mshareadj);
	if (mflag& 0x4) ptok->setCpDouble(mlimitadj);
	if (mflag& 0x8) ptok->setCpDouble(mmktadj);
	if (mflag & 0x10) ptok->setlong(mbaseshare);
	if (mflag & 0x20) ptok->setCpDouble(mbasecharge);
	if (mflag & 0x40) ptok->setlong(msharecap);
	if (mflag & 0x80) ptok->setCpDouble(mcommcap);
	if (ver>1) {
		if (mflag& 0x100) ptok->setCpDouble(mvaladj); 
		if (ver>2) {
			if (mflag &0x200) ptok->setCpDouble(mmincomm); 
		}
		if (ver>3) {
			if (mflag &0x400) ptok->setlong(msharecap1);
			if (mflag & 0x800) ptok->setCpDouble(mshareadj1); 
		}
		if (ver>4) {
			ptok->setCpDouble(msymbolfee); 
		}
	}
	ptok->setVarStr(mnote);
	ptok->setlong(mm_date);

	int pathnu=0;
	itemPassthrufee *ppath;
	ppath=ppathfee;
	while (ppath) {
		pathnu++;
		ppath =ppath->pnext;
	}
	if(pathnu>50) pathnu=50;
	
	ptok->setBYTE(pathnu);

	ppath=ppathfee;
	BYTE pathmask;
	for (int i=0;i<pathnu;i++) {
		ASSERT(ppath !=NULL);
		pathmask=(BYTE)ppath->mflag;
		ptok->setBYTE(pathmask);
		ptok->setVarStr(ppath->mpath);
		if (pathmask&0x1) ptok->setCpDouble(ppath->costpertkt);
		if (pathmask&0x2) ptok->setCpDouble(ppath->costperexe);
		if (pathmask&0x4) ptok->setCpDouble(ppath->costpershr);
		if (pathmask&0x8) ptok->setCpDouble(ppath->costpershrA);
		ppath =ppath->pnext;
	}
	ptok->finalizeSet();
	return TRUE;
}

double itemTrade::GetEcnFee(itemDeal *pdealcatch)
{
	itemDeal *pdeal;
	if (pdealcatch==NULL){
		int ndealid;
		AC_TR_relation *prelation;
		prelation=AC_TR_relation::Finditem(mtrid,maccid);
		if (prelation==NULL) ndealid=0;
		else {
			switch(msecsym[0]) {
			case '+':
				ndealid=prelation->mOpdealid;
				break;
			case '#':
				ndealid=prelation->mFtdealid;
				break;
			default:
				ndealid=prelation->mdealid;
			}
			if (ndealid==0) ndealid=prelation->mdealid;
		}
		pdeal=itemDeal::FindDeal(ndealid);
		if (pdeal==NULL) {
			pdeal=itemDeal::FindDeal(0);
		}
		if (pdeal==NULL) return 0;
	}else {
		pdeal=pdealcatch; 
	}

	itemDeal tmpdeal;
	tmpdeal.Clear();
	pdeal->GetDealValue(0xffff,&tmpdeal);

	double retcomm;
	retcomm=0;
//	BOOL bRouteToECN;
//	bRouteToECN=FALSE;
	BOOL bfinded=FALSE;
	//	check the cost pathree;
	itemPassthrufee tmppathfee; 
	memset(&tmppathfee,0,sizeof(itemPassthrufee)); 
	if (strcmp(mroute,"INET")==0) {
		if (mtrtype=='D') {//DOT order;
			bfinded=pdeal->GetPathFeeValue(0xf,"INETD",&tmppathfee);
		}
		if (!bfinded) {
			if (mstatus &(1<<27)){
				bfinded=FALSE;
				switch(theCfgFile.GetSecType(msecsym,mexchange)) {
				case 'L':
					bfinded=pdeal->GetPathFeeValue(0xf,"INETLR",&tmppathfee);
					break;
				}
				if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"INETR",&tmppathfee);
			}else {
				switch(theCfgFile.GetSecType(msecsym,mexchange)) {
				case 'L':
					bfinded=pdeal->GetPathFeeValue(0xf,"INETL",&tmppathfee);
					break;
				case 'E':
					bfinded=pdeal->GetPathFeeValue(0xf,"INETE",&tmppathfee);
					break;
				default :
					bfinded=pdeal->GetPathFeeValue(0xf,"INETQ",&tmppathfee);
				}
			}
		}
		if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"INETQ",&tmppathfee);

		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			if (tmppathfee.costpershrA>0){
				retcomm+=tmppathfee.costpershrA*mqty;
			}else if(mprice>1) { //for the add liquity, less than 1 dollar don't have add liquidity.
				retcomm+=tmppathfee.costpershrA*mqty;
			}
		}else {
			if ((mprice>1)||(mstatus &(1<<27))) { //routing out or big than 1 dollar.
				retcomm+=tmppathfee.costpershr*mqty;
			}else {
				retcomm+=mqty*mprice*0.001;
			}
		}
	}else if (strcmp(mroute,"ARCA")==0) {
		if (mstatus &(1<<27)){
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'U':
				bfinded=pdeal->GetPathFeeValue(0xf,"ARCAU",&tmppathfee);
				if (bfinded) {
					if (theCfgFile.mbARCAUNoSpecialCharge==FALSE){
						if (mprice<1) {
							tmppathfee.costpershrA=0;
							tmppathfee.costpershr=mprice*0.003;
						}
					}
				}
				break;
			case 'L':
				bfinded=pdeal->GetPathFeeValue(0xf,"ARCALR",&tmppathfee);
				break;
			}
			if (!bfinded)	bfinded=pdeal->GetPathFeeValue(0xf,"ARCAR",&tmppathfee);
		}else {
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				bfinded=pdeal->GetPathFeeValue(0xf,"ARCAL",&tmppathfee);
				break;
			case 'E':
				bfinded=pdeal->GetPathFeeValue(0xf,"ARCAE",&tmppathfee);
				break;
			case 'U':
				if (pdeal->GetPathFeeValue(0xf,"ARCAU",&tmppathfee)==FALSE){
					bfinded=pdeal->GetPathFeeValue(0xf,"ARCAQ",&tmppathfee);
				};
				break;
			default :
				bfinded=pdeal->GetPathFeeValue(0xf,"ARCAQ",&tmppathfee);
			}
		}

		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			retcomm+=tmppathfee.costpershrA*mqty;
		}else {
			retcomm+=tmppathfee.costpershr*mqty;
		}
	}else if (strcmp(mroute,"BATS")==0) {
		char exchtype=theCfgFile.GetSecType(msecsym,mexchange);
		if (IsRouteOut()) {
			if (mtrtype=='D') {//route to NYSE;
				bfinded=pdeal->GetPathFeeValue(0xf,"BATSD",&tmppathfee);
			}
			if (!bfinded) {
				if (exchtype=='L') {
					bfinded=pdeal->GetPathFeeValue(0xf,"BATSLR",&tmppathfee);
				}
				if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"BATSR",&tmppathfee);
			}
		}
		
		if (!bfinded){
			if (exchtype=='L') {
				bfinded=pdeal->GetPathFeeValue(0xf,"BATSL",&tmppathfee);
			}
			if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"BATS",&tmppathfee);
		}
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		
		if (IsRouteOut()) {
			if (mprice<1.0) {
				retcomm+=0.0026*mqty*mprice; 
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}else {
			if (mprice>=1.0) {
				if (IsAddLiquity()) {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}
		}
	}else if (strcmp(mroute,"EDGA")==0) {
		if (IsRouteOut()) {
			bfinded=pdeal->GetPathFeeValue(0xf,"EDGAR",&tmppathfee);
			if (bfinded) {
				if (mtrtype=='D') {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
				if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
				retcomm+=tmppathfee.costperexe;
			}
		}
		if (!bfinded) {
			bfinded=pdeal->GetPathFeeValue(0xf,"EDGA",&tmppathfee);
			if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"ATTN",&tmppathfee);
			if (bfinded) {
				if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
				retcomm+=tmppathfee.costperexe;
				if (IsAddLiquity()) {
					if (mtrtype=='H'){
						if (tmppathfee.costpershrA<0.003) tmppathfee.costpershrA=0.003;
					}
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					if (mtrtype=='H'){
						if (tmppathfee.costpershr<0.003) tmppathfee.costpershr=0.003;
					}
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}else { //not find, hiden order.
				if (mtrtype=='H'){
					retcomm+=0.003*mqty;
				}
			}
		}
	}else if (strcmp(mroute,"EDGX")==0) {
		switch(theCfgFile.GetSecType(msecsym,mexchange)) {
		case 'U': //OTCBB
			bfinded=pdeal->GetPathFeeValue(0xf,"EDGXU",&tmppathfee);
			if (mprice>=1.0) {
				if (IsAddLiquity()) {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}else {
				if (IsAddLiquity()) {
					retcomm+=mqty*mprice*0.002;//0.001*mqty;
				}
			}
			break;
		case 'L': //Listed;
			if (IsRouteOut()) {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXLR",&tmppathfee);
				if (mtrtype=='D') {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}

			}else {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXL",&tmppathfee);
				if (IsAddLiquity()) {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}
			break;// a bug which cause the double charg for the EDGX.
		case 'E': //ETF;
			if (IsRouteOut()) {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXER",&tmppathfee);
				if (mtrtype=='D') {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}else {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXE",&tmppathfee);
				if (IsAddLiquity()) {
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}
	
			break;
		default:  //NASDAQ
			if (IsRouteOut()) {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXQR",&tmppathfee);
				if (mtrtype=='Z') {
					retcomm+=tmppathfee.costpershr*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}else {
				bfinded=pdeal->GetPathFeeValue(0xf,"EDGXQ",&tmppathfee);
				if (mprice>=1.0) {
					if (IsAddLiquity()) {
						retcomm+=tmppathfee.costpershrA*mqty;
					}else {
						retcomm+=tmppathfee.costpershr*mqty;
					}
				}else {
					if (!IsAddLiquity()) {
						retcomm+=0.001*mqty;
					}
				}



			}
			break;
		}

		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
	}else if (strcmp(mroute,"TRAC")==0) {
		switch(theCfgFile.GetSecType(msecsym,mexchange)) {
		case 'U': //OTCBB
			double ret;
			ret=mqty*mprice*0.005;
			retcomm+=ret;
			tmppathfee.costpertkt=2;
			break;
		case 'L': //Listed;
		case 'E': //ETF;
			break;
		default:  //NASDAQ
			if (mprice<1.00) {	
				if (IsRouteOut()) {
					retcomm+=0.0026*mqty;
				}else {
					if (IsAddLiquity()) {
						retcomm+=-0.0025*mqty*mprice;
					}else {
						retcomm+=0.003*mqty*mprice;
					}
				}
			}else {
				if (IsRouteOut()) {
					bfinded=pdeal->GetPathFeeValue(0xf,"TRACQR",&tmppathfee);
					retcomm+=tmppathfee.costpershr*mqty;
				}else {
					bfinded=pdeal->GetPathFeeValue(0xf,"TRACQ",&tmppathfee);
					if (IsAddLiquity()) {
						retcomm+=tmppathfee.costpershrA*mqty;
					}else {
						retcomm+=tmppathfee.costpershr*mqty;
					}
				}
			}
			break;
		}

		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;

	}else if (strcmp(mroute,"SBSH")==0) {
		switch(theCfgFile.GetSecType(msecsym,mexchange)) {
		case 'L':
			bfinded=pdeal->GetPathFeeValue(0xf,"SBSHL",&tmppathfee);
			break;
		}

		if (!bfinded) pdeal->GetPathFeeValue(0xf,"SBSH",&tmppathfee);
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			retcomm+=tmppathfee.costpershrA*mqty;
		}else {
			retcomm+=tmppathfee.costpershr*mqty;
		}

/*	}else  if (strcmp(mroute,"SIZE")==0) {
		if (mstatus &(1<<27)){
			bfinded=FALSE;
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				bfinded=pdeal->GetPathFeeValue(0xf,"SIZELR",&tmppathfee);
				break;
			}
			if (!bfinded) pdeal->GetPathFeeValue(0xf,"SIZER",&tmppathfee);
		}else {
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				pdeal->GetPathFeeValue(0xf,"SIZEL",&tmppathfee);
				break;
			case 'E':
				pdeal->GetPathFeeValue(0xf,"SIZEE",&tmppathfee);
				break;
			default :
				pdeal->GetPathFeeValue(0xf,"SIZEQ",&tmppathfee);
			}
		}
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			retcomm+=tmppathfee.costpershrA*mqty;
		}else {
			retcomm+=tmppathfee.costpershr*mqty;
		}
	}else if (strcmp(mroute,"ISLD")==0) {
		BOOL bfind=FALSE;

		if (mstatus &(1<<27)){
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				bfind=pdeal->GetPathFeeValue(0xf,"ISLDLR",&tmppathfee);
				break;
			}
			if (!bfind) pdeal->GetPathFeeValue(0xf,"ISLDR",&tmppathfee);
		}
		if (!bfind) {

			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				pdeal->GetPathFeeValue(0xf,"ISLDL",&tmppathfee);
				break;
			case 'E':
				pdeal->GetPathFeeValue(0xf,"ISLDE",&tmppathfee);
				break;
			default :
				pdeal->GetPathFeeValue(0xf,"ISLDQ",&tmppathfee);
			}
		}
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			if (tmppathfee.costpershrA>0){
				retcomm+=tmppathfee.costpershrA*mqty;
			}else if(mprice>1) { //for the add liquity, less than 1 dollar don't have add liquidity.
				retcomm+=tmppathfee.costpershrA*mqty;
			}
		}else {
			if ((mprice>1)||(mstatus &(1<<27))) { //routing out or big than 1 dollar.
				retcomm+=tmppathfee.costpershr*mqty;
			}else {
				retcomm+=mqty*mprice*0.001;
			}
		}
	}else if (strcmp(mroute,"INCA")==0) {
		if (mstatus &(1<<27)){
			pdeal->GetPathFeeValue(0xf,"INCAR",&tmppathfee);
		}else {
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				pdeal->GetPathFeeValue(0xf,"INCAL",&tmppathfee);
				break;
			case 'E':
				pdeal->GetPathFeeValue(0xf,"INCAE",&tmppathfee);
				break;
			default :
				pdeal->GetPathFeeValue(0xf,"INCAQ",&tmppathfee);
			}
		}
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			retcomm+=tmppathfee.costpershrA*mqty;
		}else {
			retcomm+=tmppathfee.costpershr*mqty;
		}

	}else if (strcmp(mroute,"BRUT")==0) {
		if (mstatus &(1<<27)){
			bfinded=FALSE;
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				bfinded=pdeal->GetPathFeeValue(0xf,"BRUTLR",&tmppathfee);
				break;
			}
			if (!bfinded) pdeal->GetPathFeeValue(0xf,"BRUTR",&tmppathfee);
		}else {
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				pdeal->GetPathFeeValue(0xf,"BRUTL",&tmppathfee);
				break;
			case 'E':
				pdeal->GetPathFeeValue(0xf,"BRUTE",&tmppathfee);
				break;
			default :
				pdeal->GetPathFeeValue(0xf,"BRUTQ",&tmppathfee);
			}
		}
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			if (tmppathfee.costpershrA>0){
				retcomm+=tmppathfee.costpershrA*mqty;
			}else if(mprice>1) { //for the add liquity, less than 1 dollar don't have add liquidity.
				retcomm+=tmppathfee.costpershrA*mqty;
			}
		}else {
			if ((mprice>1)||(mstatus &(1<<27))) { //routing out or big than 1 dollar.
				retcomm+=tmppathfee.costpershr*mqty;
			}else {
				retcomm+=mqty*mprice*0.001;
			}
		}
	}else if (strcmp(mroute,"SNET")==0) {
		if (mstatus&(1<<25)) {//broadcast;
			pdeal->GetPathFeeValue(0xf,"SNETB",&tmppathfee);
			if (mexecnum==1) {
				retcomm+=tmppathfee.costpertkt;
			}
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}else {
			if (theCfgFile.IsECN(mbkrsym)){
				pdeal->GetPathFeeValue(0xf,"SNETE",&tmppathfee);
				if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
				retcomm+=tmppathfee.costperexe;
				if (IsAddLiquity()){ 
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
				bRouteToECN=TRUE;
			}else {// is market maker
				pdeal->GetPathFeeValue(0xf,"SNETM",&tmppathfee);
				if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
				retcomm+=tmppathfee.costperexe;
				if (IsAddLiquity()) { 
					retcomm+=tmppathfee.costpershrA*mqty;
				}else {
					retcomm+=tmppathfee.costpershr*mqty;
				}
			}
		}
	}else if (strcmp(mroute,"SMTG")==0){
		if (mqty<2000) {
			pdeal->GetPathFeeValue(0xf,"SMTGA",&tmppathfee);
			if (mexecnum==1)retcomm+=tmppathfee.costpertkt;
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}else {
			pdeal->GetPathFeeValue(0xf,"SMTGB",&tmppathfee);
			if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) { 
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}
		if (theCfgFile.IsECN(mbkrsym))	bRouteToECN=TRUE;
*/
	}else if (strcmp(mroute,"BELZ")==0) {//BLZ berg;
		if (msecsym[0]=='+' ){
			bfinded=pdeal->GetPathFeeValue(0xf,"BELZO",&tmppathfee);
		}
		if ((!bfinded)&&(mstatus &(1<<27))){
			bfinded=pdeal->GetPathFeeValue(0xf,"BELZR",&tmppathfee);
		}
		if (!bfinded){
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				if (!bfinded) bfinded= pdeal->GetPathFeeValue(0xf,"BELZL",&tmppathfee);
				break;
			}
			
			if (!bfinded) bfinded= pdeal->GetPathFeeValue(0xf,"BELZ",&tmppathfee);
		}
		if (bfinded) {
			if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}
	}else if (strcmp(mroute,"BLZ")==0) {//BLZ berg;
		if (msecsym[0]=='+' ){
			bfinded=pdeal->GetPathFeeValue(0xf,"BLZO",&tmppathfee);
		}
		if ((!bfinded)&&(mstatus &(1<<27))){
			bfinded=pdeal->GetPathFeeValue(0xf,"BLZR",&tmppathfee);
		}
		if (!bfinded){
			switch(theCfgFile.GetSecType(msecsym,mexchange)) {
			case 'L':
				if (!bfinded) bfinded= pdeal->GetPathFeeValue(0xf,"BLZL",&tmppathfee);
				break;
			}
			
			if (!bfinded) bfinded= pdeal->GetPathFeeValue(0xf,"BLZ",&tmppathfee);
		}
		if (bfinded) {
			if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}
	}else if (strcmp(mroute,"NYFX")==0) {

		if ((mstatus &(1<<27))){//milliumn
			bfinded=pdeal->GetPathFeeValue(0xf,"NYFXR",&tmppathfee);
		}
		if (!bfinded) bfinded=pdeal->GetPathFeeValue(0xf,"NYFX",&tmppathfee);
		
		if (bfinded) {
			if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
			retcomm+=tmppathfee.costperexe;
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}
	}else if (strcmp(mroute,"TSX")==0) {
		bfinded=pdeal->GetPathFeeValue(0xf,mroute,&tmppathfee);
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (theCfgFile.mTSXByBasisPoint>0) {
			retcomm+= mqty*mprice* theCfgFile.mTSXByBasisPoint*0.001*0.001;
		}else {
			if (IsAddLiquity()) {
				retcomm+=tmppathfee.costpershrA*mqty;
			}else {
				retcomm+=tmppathfee.costpershr*mqty;
			}
		}

	}else {
		bfinded=pdeal->GetPathFeeValue(0xf,mroute,&tmppathfee);
		if (mexecnum==1) retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		if (IsAddLiquity()) {
			retcomm+=tmppathfee.costpershrA*mqty;
		}else {
			retcomm+=tmppathfee.costpershr*mqty;
		}
	}
/*
	if (bRouteToECN) {
		memset(&tmppathfee,0,sizeof(itemPassthrufee)); 
		if (strcmp(mbkrsym,"ISLD")==0) {
			pdeal->GetPathFeeValue(0xf,"ISLDQ",&tmppathfee);
		}else if (strcmp(mbkrsym,"ARCA")==0) {
			pdeal->GetPathFeeValue(0xf,"ARCAQ",&tmppathfee);
		}else if (strcmp(mbkrsym,"INCA")==0) {
			pdeal->GetPathFeeValue(0xf,"INCAQ",&tmppathfee);
		}else if (strcmp(mbkrsym,"INET")==0) {
			pdeal->GetPathFeeValue(0xf,"INETQ",&tmppathfee);
		}else if (strcmp(mbkrsym,"BRUT")==0) {
			pdeal->GetPathFeeValue(0xf,"BRUTQ",&tmppathfee);
		}else {
			pdeal->GetPathFeeValue(0xf,mbkrsym,&tmppathfee);
		}
		
//		retcomm+=tmppathfee.costpertkt;
		retcomm+=tmppathfee.costperexe;
		retcomm+=tmppathfee.costpershr*mqty;
	}
*/

	return retcomm;
}

BOOL itemTicket::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%t,");
	sprintf(tok.SeekToEnd(),"%d,", 	mticketid	);
	sprintf(tok.SeekToEnd(),"%d,", 	mtrid		);
	sprintf(tok.SeekToEnd(),"%d,", 	maccid		);
	sprintf(tok.SeekToEnd(),"%d,", 	mentrid		);
	sprintf(tok.SeekToEnd(),"%d,", 	mtradeid	);
	sprintf(tok.SeekToEnd(),"%d,", 	morderid	);
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid		);
	sprintf(tok.SeekToEnd(),"%s,",	mroute		);
	sprintf(tok.SeekToEnd(),"%s,", 	mbkrsym		);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus		);
	sprintf(tok.SeekToEnd(),"%d,", 	mexecnum	);
	sprintf(tok.SeekToEnd(),"%d,", 	mqty		);
	sprintf(tok.SeekToEnd(),"%g,", 	mprice		);
	sprintf(tok.SeekToEnd(),"%s,", 	msecsym		);
	sprintf(tok.SeekToEnd(),"%d,", 	mexchange	);
	sprintf(tok.SeekToEnd(),"%s,", 	mrrno		);
	sprintf(tok.SeekToEnd(),"%g,", 	mcommision	);
	sprintf(tok.SeekToEnd(),"%d,", 	me_date		);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date		);
	sprintf(tok.SeekToEnd(),"%g,", 	mcomm1		);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

DWORD itemTicket::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mticketid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtradeid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mcommision=atof(ptmp);
	ptmp=tok.GetStr(',');	me_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mcomm1=atof(ptmp);
	return 0xffffffff;

}

BOOL itemAccIfo::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%A,");
	sprintf(tok.SeekToEnd(),"%d,", 	maccid);
	sprintf(tok.SeekToEnd(),"%s,", 	mname);
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus);
	sprintf(tok.SeekToEnd(),"%s,", 	mrrno);
	sprintf(tok.SeekToEnd(),"%d,", 	mlinkacc);
	sprintf(tok.SeekToEnd(),"%d,", 	mper_id);
	sprintf(tok.SeekToEnd(),"%d,", 	maddr1_id);
	sprintf(tok.SeekToEnd(),"%d,", 	maddr2_id);
	sprintf(tok.SeekToEnd(),"%d,", 	mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 	mnote);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;

}

DWORD itemAccIfo::LoadStr(char *buff)
{

	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mname,ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mlinkacc=atoi(ptmp);
	ptmp=tok.GetStr(',');	mper_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	maddr1_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	maddr2_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	return 0xffffffff;
}

BOOL itemTrader::SaveStr(char *buff, int bufflen, DWORD flag)
{

	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%L,");
	sprintf(tok.SeekToEnd(),"%d,", 		mtrid);
	sprintf(tok.SeekToEnd(),"%s,", 		mtracno);

	int i;
	for (i=0;mPWD[i];i++){
		sprintf(tok.SeekToEnd(),"%2.2x",((unsigned char)mPWD[i]));
	}
	sprintf(tok.SeekToEnd(),",");

	sprintf(tok.SeekToEnd(),"%d,", 		mbrid);
	sprintf(tok.SeekToEnd(),"%d,", 		mstatus);
	sprintf(tok.SeekToEnd(),"%d,", 		mtype);
	sprintf(tok.SeekToEnd(),"%d,", 		mPerid);
	sprintf(tok.SeekToEnd(),"%d,", 		mAdd1ID);
	sprintf(tok.SeekToEnd(),"%d,", 		mAdd2ID);
	sprintf(tok.SeekToEnd(),"%d,", 		mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 		mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 		mnote);
	sprintf(tok.SeekToEnd(),"%d,%d,%d,",mstatus1,mstatus2,mstatus3); 
	sprintf(tok.SeekToEnd(),"%d,",		m_master_trader); 

	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

DWORD itemTrader::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtracno,ptmp);

	ptmp=tok.GetStr(',');	
	int i;
	for (i=0;ptmp[i];i+=2){
		sscanf(ptmp+i,"%2.2x",&mPWD[i/2]);
	}
	mPWD[i/2]=0;

	strcpy(mPWD,ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtype=atoi(ptmp);
	ptmp=tok.GetStr(',');	mPerid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mAdd1ID=atoi(ptmp);
	ptmp=tok.GetStr(',');	mAdd2ID=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	ptmp=tok.GetStr(',');   mstatus1=atoi(ptmp);
	ptmp=tok.GetStr(',');   mstatus2=atoi(ptmp);
	ptmp=tok.GetStr(',');   mstatus3=atoi(ptmp);
	ptmp=tok.GetStr(',');   if (ptmp) m_master_trader=atoi(ptmp);
	return 0xffffffff;

}


BOOL itemBranch::SaveStr(char *buff, int bufflen, DWORD flag)
{

	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%B,");
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid);
	sprintf(tok.SeekToEnd(),"%s,", 	mshort);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus);
	sprintf(tok.SeekToEnd(),"%s,", 	mname);
	sprintf(tok.SeekToEnd(),"%d,",	mper_id);
	sprintf(tok.SeekToEnd(),"%d,", 	madd_id);
	sprintf(tok.SeekToEnd(),"%d,", 	mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 	mnote);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

DWORD itemBranch::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mshort,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mname,ptmp);
	ptmp=tok.GetStr(',');	mper_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	madd_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	return 0xffffffff;
}

DWORD itemOptsym::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	strcpy(optsymb,ptmp);
	ptmp=tok.GetStr(',');	expday=atoi(ptmp);
	ptmp=tok.GetStr(',');	strikepice=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(undsymb,ptmp);
	ptmp=tok.GetStr(',');	pmundlyshr=atoi(ptmp);
	ptmp=tok.GetStr(',');	mcurrundprice=atoi(ptmp);
	ptmp=tok.GetStr(',');	m_date=atoi(ptmp);
	return 0xffffffff;
}

BOOL itemOptsym::SaveStr(char *buff, int bufflen)
{
	sprintf(buff,"%P,%s,%d,%d,%s,%d,%d,%d",
		optsymb,expday,strikepice,undsymb,pmundlyshr,mcurrundprice,m_date);
	return TRUE;
}

BOOL itemOrderActive::SaveStr(char *buff, int bufflen, DWORD flag)
{
	int len=0;
	len+=sprintf(buff+len,"%%a,%d,%d,%d,%d,%s,%s,%f,%d,%d,%s",
		 mtrid,maccid,mflag,morderid,msecsym,mroute,mprice,mqty,mlogtime,mnote);
	ASSERT(len<bufflen);
	return TRUE;

}

DWORD itemOrderActive::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);
	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mflag=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mlogtime=atoi(ptmp);
	ptmp=tok.GetStr(0);		strcpy(mnote,ptmp);
	return 0xffffffff;

}

BOOL itemAccCtrl::MakePkg(char *buffer)
{
	PkgToken tokret;
	tokret.InitToSet(buffer);
	tokret.setWORD(1040);
	tokret.setBYTE(4);//compressed version;
	tokret.setlong(maccid);
	int flag=0;
	
	if (mflag!=0)				flag |=(1<<0);
	if (mflagex!=0)				flag |=(1<<1);
	if (mMaxLoss>0.0001)		flag |=(1<<2);
	if (maxshare!=0)			flag |=(1<<3);
	if (maxtotalshare!=0)		flag |=(1<<4);
	if (maxsymbshare!=0)		flag |=(1<<5); 
	if (maxsymbs!=0)			flag |=(1<<6);
	if (maxUnrealizedLoss!=0)	flag |=(1<<7);
	if (mstarttradetime!=0)		flag |=(1<<8);
	if (mstoptradetime!=0)		flag |=(1<<9);

	tokret.setlong(flag);
	if (flag&(1<<0))	tokret.setlong(mflag);
	if (flag&(1<<1)) tokret.setlong(mflagex);
	if (flag&(1<<2))	tokret.setDouble(mMaxLoss);
	if (flag&(1<<3)) tokret.setlong(maxshare);
	if (flag&(1<<4))	tokret.setlong(maxtotalshare);
	if (flag&(1<<5)) tokret.setlong(maxsymbshare);
	if (flag&(1<<6))	tokret.setlong(maxsymbs);
	if (flag&(1<<7)) tokret.setDouble(maxUnrealizedLoss);
	if (flag&(1<<8))	tokret.setlong(mstarttradetime);
	if (flag&(1<<9)) tokret.setlong(mstoptradetime);
	
	tokret.finalizeSet(); 
	return TRUE;
}

BOOL itemAccCtrl_Symb::MakePkg_Sym(int accid,itemAccCtrl_Symb *psym, char *buffer)
{
	
	PkgToken tokret;
	tokret.InitToSet(buffer);

	tokret.setWORD(1040);
	tokret.setBYTE(3); //compressed version;
	tokret.setlong(accid);
	
	while (psym) {
		tokret.setVarStr(psym->msymbol);
		char flag=0;
		if (psym->maxpos>0) flag|=(1<<0);
		if (psym->maxloss>0.0001) flag|=(1<<1);
		if (psym->maxorder>0) flag|=(1<<2);
		
		tokret.setBYTE(flag); 
		if (flag &(1<<0)) tokret.setlong(psym->maxpos);
		if (flag &(1<<1)) tokret.setDouble(psym->maxloss);
		if (flag &(1<<2)) tokret.setlong(psym->maxorder); 
		psym=psym->pnext;
	}

	tokret.finalizeSet(); 
	return TRUE;
}

BOOL itemAccCtrl_Route::MakePkg_Route (int accid,itemAccCtrl_Route *proute, char *buffer)
{
	
	PkgToken tokret;
	tokret.InitToSet(buffer);

	tokret.setWORD(1040);
	tokret.setBYTE(5); //compressed version;
	tokret.setlong(accid);
	
	while (proute) {
		tokret.setVarStr(proute->mroute);
		tokret.setlong(proute->mflag); 
		proute=proute->pnext;
	}

	tokret.finalizeSet(); 
	return TRUE;
}


BOOL itemManualOrder::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setDWORD(flag);
	ptok->setlong(id);
	ptok->setlong(userid);
	ptok->setlong(accid); 
	ptok->setlong(ntype); 
	ptok->setlong(origoid);	
	ptok->setlong(status);
	ptok->setVarStr(equitytype);
	ptok->setVarStr(exchange); 
	ptok->setlong(share);
	ptok->setlong(left_share);
	ptok->setVarStr(price);
	ptok->setVarStr(symbol);
	ptok->setVarStr(currency);
	ptok->setVarStr(note); 
	ptok->setlong(acceptby);
	ptok->setlong(c_time);
	ptok->setlong(m_time);
	return TRUE;
}

BOOL itemManualOrder::GetPkg(PkgToken *ptok, DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getDWORD();
	id=ptok->getlong();
	userid=ptok->getlong();
	accid=ptok->getlong(); 
	ntype=ptok->getlong(); 
	origoid=ptok->getlong();	
	status=ptok->getlong();
	ptok->getString(equitytype,sizeof(equitytype));
	ptok->getString(exchange,sizeof(exchange)); 
	share=ptok->getlong();
	left_share=ptok->getlong();
	ptok->getString(price,sizeof(price));
	ptok->getString(symbol,sizeof(symbol));
	ptok->getString(currency,sizeof(currency));
	ptok->getString(note,sizeof(note)); 
	acceptby=ptok->getlong();
	c_time=ptok->getlong();
	m_time=ptok->getlong();
	return TRUE;
}

itemManualOrder * itemManualOrder::Find(int orderid)
{
	intitem *PI;
	PI=smapOrderid.FindItem(orderid);
	if (PI==NULL) return NULL;

	return (itemManualOrder*)PI->val; 

}

BOOL itemManualOrder::SaveStr(char *buff, int bufflen, DWORD flag)
{
	int len=0;
	len+=sprintf(buff+len,"%%M,%d,%d,%d,%d,%d,%d,",id,userid,accid,ntype,origoid,status);
	len+=sprintf(buff+len,"%s,%s,%d,%d,",equitytype,exchange,share,left_share);
	len+=sprintf(buff+len,"%s,%s,%s,%s,",price,symbol,currency,note);
	len+=sprintf(buff+len,"%d,%d,%d,",acceptby,c_time,m_time);
	return 0;
		
}

DWORD itemManualOrder::LoadStr(char *buff)
{
	char *parr[50];
	int   num=50;
	CStrFunc::BreakStr(buff,',',parr,&num);
	if (num<18) {
		return 0;
	}
	ASSERT(parr[0][0]=='%');
	id=atoi(parr[1]);
	userid=atoi(parr[2]);
	accid=atoi(parr[3]);
	ntype=atoi(parr[4]);
	origoid=atoi(parr[5]);
	status=atoi(parr[6]);
	strncpy(equitytype,parr[7],sizeof(equitytype)-1);
	strncpy(exchange ,parr[8],sizeof(exchange)-1);
	share=atoi(parr[9]);
	left_share=atoi(parr[10]);
	strncpy(price ,parr[11],sizeof(price)-1);
	strncpy(symbol,parr[12],sizeof(symbol)-1);
	strncpy(currency,parr[13],sizeof(currency)-1);
	strncpy(note,parr[14],sizeof(note)-1);
	acceptby=atoi(parr[15]);
	c_time=atoi(parr[16]);
	m_time=atoi(parr[17]);

	return 0;
}

BOOL itemManualOrder::Save()
{
	char buff[2000];
	SaveStr(buff,sizeof(buff),0xffff);
	theCfgFile.mlog.AddLogStr(buff);
	return TRUE;
}

BOOL itemManualOrder::SetInfoPkg(char *pbuff, DWORD flag)
{
	PkgToken tok;
	tok.InitToSet(pbuff);
	tok.setWORD(1029);
	tok.setBYTE(1);
	SetPkg(&tok,flag);
	tok.finalizeSet();
	return TRUE;
}
