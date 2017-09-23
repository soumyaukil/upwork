#include "stdafx.h"

#include "itemsSet.h"



void itemsSet::Init()
{
	m_ordermap.init(4000);
	m_tradesmap.init(4000);
	m_positionmap.init(4000);
	m_accountmap.init(1000);
	m_tradermap.init(1000);
	m_equitymap.init(1000);
	m_accountlink.init(1000);
	m_symbmap.init(2000);// this is only api, not quoteserver, 2000 should be enought;
}

int itemsSet::GetAccid(char *acc)
{
	intitem *PIitem;
	int ret=0;
	m_accountmap.LockMap();
	m_accountmap.EnumFirst(&PIitem);
	while(PIitem) {
		itemAccIfo *pacc;
		pacc=(itemAccIfo *)PIitem->val;
		if (strcmp(pacc->mname,acc)==0) {
			ret=pacc->maccid;
			break;
		}
		m_accountmap.EnumNext(&PIitem);
	}
	m_accountmap.UnlockMap(); 
	return ret;
}

const char * itemsSet::GetAccName(int accid)
{
//	itemAccIfo *pacc;
	intitem *PIitem;
	PIitem=m_accountmap.FindItem(accid);
	if (PIitem==NULL) {
		return NULL;
	}else {
		itemAccIfo *pacc;
		pacc=(itemAccIfo *)PIitem->val;
		return pacc->mname; 
	}

}

itemAcct_Link * itemsSet::FindAcc_Link(int trid, int accid)
{
	if ((trid ==0)||(accid==0)) return NULL;
	itemAcct_Link *pitem,*pret;
	
	intitem *PIitem;
	m_accountlink.EnumFirstKey(accid,&PIitem);
	pret=NULL;
	while(PIitem){
		pitem=(itemAcct_Link*)PIitem->val;
		if (pitem->mtrid==trid) {
			pret=pitem;
			break;
		}
		m_accountlink.EnumNextKey(accid,&PIitem); 
	}
	return pret;
}

itemsyminfo * itemsSet::GetSymbol(LPCSTR symbol, BOOL baddnew)
{
	stritem *PIitem;
	itemsyminfo *psym=NULL;
	PIitem=m_symbmap.FindItem(symbol);
	if(PIitem){ //found;
		psym=(itemsyminfo*)PIitem->val;
	}else {
		if (baddnew==TRUE){
			psym=new itemsyminfo;
			strcpy(psym->msecsym,symbol);
			m_symbmap.Add(psym->msecsym,(long)psym);
		}
	};
	return psym;
}
