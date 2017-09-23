// TradeEngine.cpp: implementation of the TradeEngine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "orderserv.h"
#include "TradeEngine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//TradeEngine theEngine; 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TradeEngine::TradeEngine()
{

}

TradeEngine::~TradeEngine()
{

}

BOOL TradeEngine::InitBuffs()
{
	mBufPosition.init(sizeof(itemPosition),102400);
	return TRUE;
}

BOOL TradeEngine::InitData()
{
/*	BOOL ret;
	sMapAccid.init(accountno); 
	srstAccIfo rstaccifo;
	ret=rstaccifo.Open();
	ASSERT(ret==TRUE);
	Account *pacc;
	while(!rstaccifo.IsEOF()) {
		pacc=new Account;
		pacc->mitemifo.CopyItem (&rstaccifo.mitem);
		sMapAccid.AddNoDup(pacc->mitemifo.maccid,(long)pacc);
		rstaccifo.MoveNext(); 
	}
	rstaccifo.Close();

	srstAccEquity rstAccEquiy;
	ret=rstAccEquiy.Open();
	ASSERT(ret==TRUE);
	while(!rstAccEquiy.IsEOF()) {
		pacc=FindAcc(rstAccEquiy.mitem.maccid);
		if (pacc!=NULL) {
			memcpy(&pacc->mitemEquity,&rstAccEquiy.mitem,sizeof(itemAccEquity));
		};
		rstAccEquiy.MoveNext();
	}
	rstAccEquiy.Close();

//init position:
	srstPosition rstposition;
	rstposition.OpenToRead();
	itemPosition *pitem;
//	Account *pacc;
	while(rstposition.IsEOF()==FALSE) {
		pitem=(itemPosition*)theCfgFile.mBufPosition.GetVoidBuff();
		pitem->CopyFrom(rstposition.mitem);
		pacc=Account::FindAcc(pitem->maccid);
		if (pacc==NULL){
			theCfgFile.mlog.AddLogStr("Error Position");
			theCfgFile.mBufPosition.PutVoidBuff((char*)pitem);
		}else {
			pacc->mLstPositions.Add(0,(long)pitem); 
		}
		rstposition.MoveNext(); 
	}
	rstposition.Close();
*/	

	return 0;

}
