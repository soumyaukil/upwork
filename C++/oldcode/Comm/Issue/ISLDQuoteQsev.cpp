// ISLDQuote.cpp: implementation of the ISLDQLst class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "client.h"
#include "ISLDQuote.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
FixLenMem ISLDQLst::sItemBuff(sizeof(ISLDQitem),4090);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ISLDQLst::ISLDQLst()
{
	mphead=NULL;
}

ISLDQLst::~ISLDQLst()
{
	RemoveAll();
}


ISLDQitem * ISLDQLst::GetItemBuff()
{
	return (ISLDQitem*)sItemBuff.GetVoidBuff();
}

void ISLDQLst::ReleaseItemBuff(ISLDQitem *pitem)
{	
	sItemBuff.PutVoidBuff((char*)pitem); 
}


ISLDQitem *ISLDQLst::AddItem(DWORD nrefnum, BYTE nflag, double dprice, DWORD nshare, time_t ntime)
{
	mLock.Lock();
	ISLDQitem *p1,*p2,*p;
	p1=mphead;
	p2=NULL;
	if (nflag=='B') {//Buy order
		while(p1) {
			if (dprice>=p1->price) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
/*		{   
			ISLDQitem *pp;
			pp=mphead;
			char st[100];
			sprintf(st,"add new price%g,share %d;",dprice,nshare);
			TRACE(st);
			while(pp) {
				sprintf(st,"add%d,price%g,share %d;",pp,pp->price,pp->shares );
				TRACE(st);
				pp=pp->pnext;
			}
			TRACE("\n");			
		}
*/
	}else {
		while(p1) {
			if (dprice<=p1->price) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
	}
	p=GetItemBuff();
	p->flag =nflag;
	p->nrefnum=nrefnum;
	p->price=dprice;
	p->shares=nshare;
	p->modifytime =ntime;
	p->pnext=p1;
	if (p2==NULL) mphead=p;
	else p2->pnext =p;
/*f (nflag=='B')
	{   
			ISLDQitem *pp;
			pp=mphead;
			char st[100];
			sprintf(st,"Result:",dprice,nshare);
			TRACE(st);
			while(pp) {
				sprintf(st,"add%d,price%g,share %d;",pp,pp->price,pp->shares );
				TRACE(st);
				pp=pp->pnext;
			}
			TRACE("\n");			
	}
*/
	mLock.Unlock();
	return p;
}

ISLDQitem * ISLDQLst::FindItem(DWORD nrefnum)
{
	mLock.Lock();
	ISLDQitem *pret;
	pret=mphead;
	while(pret){
		if (nrefnum==pret->nrefnum){
			break;
		}
		pret=pret->pnext;
	}
	mLock.Unlock();
	return pret;
}

ISLDQitem * ISLDQLst::ModifyItem(DWORD nrefnum, int ndecress,int ntime)
{
	mLock.Lock();
	ISLDQitem *pret,*p1,*p2;
	p1=mphead;
	p2=NULL;
	pret=NULL;
	while(p1){
		if (nrefnum==p1->nrefnum){
			break;
		}
		p2=p1;
		p1=p1->pnext;
	}

	if (p1!=NULL) {
		p1->shares-=ndecress;
		p1->modifytime =ntime;
		if (p1->shares<=0){
			if (p2!=NULL){
				p2->pnext =p1->pnext;
			}else {
				mphead=p1->pnext;
			}
			ReleaseItemBuff(p1); 
		}else{
			pret=p1;
		}
	}
	mLock.Unlock();
	return pret;
}	

void ISLDQLst::RemoveAll()
{
	mLock.Lock();
	ISLDQitem *p,*p1;
	p=mphead;
	while(p) {
		p1=p;
		ReleaseItemBuff(p);
		p=p1->pnext;
	}
	mphead=NULL;
	mLock.Unlock();
}
