// TopLst.cpp: implementation of the TopLst class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "FeedCenter.h"
#include "TopLst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma init_seg(user)

FixLenMem TopLst::mitembuff(sizeof(toplstitem),2048);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TopLst::TopLst()
{
	mphead=NULL;
	mptail=NULL;
	mitemnu=0;
	mTopSize=0; 
	mbEnable=TRUE;
}

TopLst::~TopLst()
{
	RemoveAll();
}


BOOL TopLst::RemoveAll()
{
	mlock.Lock();
	toplstitem *p1,*p2;
	p1=mphead;
	while(p1!=NULL) {
		p2=p1->pnext;
		mitembuff.PutVoidBuff((char*)p1);
		p1=p2;
	}
	mphead=mptail=NULL;
	mitemnu=0; 
	mFastMap.RemoveAll();
	mlock.Unlock();
	return TRUE;
}


BOOL TopLst::init(int nTopSize, int nHashSize)
{
	mFastMap.init(nHashSize); 
	mTopSize=nTopSize;
	return TRUE;
}


//this function always Decend sort.

BOOL TopLst::UpdateData(LPCSTR secsym, double dval)
{
	mlock.Lock();  
	stritem *pitem;
	pitem=mFastMap.FindItem(secsym);
	if (pitem==NULL) {// can't find the item; check from the last;
		toplstitem *pnewitem;
		toplstitem *pprevitem;
		ASSERT(mTopSize>1);
		if (mTopSize>mitemnu){
			pnewitem=(toplstitem*)mitembuff.GetVoidBuff(); 
			ASSERT(pnewitem!=NULL);
			pnewitem->mvalue=dval;
			memccpy (pnewitem->secsym,secsym,0,sizeof(pnewitem->secsym));
			pprevitem=FindPrev(mptail,dval);
			InsertAfter(pnewitem,pprevitem);
			mFastMap.Add(secsym,(long)pnewitem);
			mitemnu++; 
		}else {
#ifdef _DEBUG
			if (mptail==NULL){
				char str[100];
				sprintf(str,"Errro:%d,%d,%s,%g",mTopSize,mitemnu,secsym,dval); 
				ASSERT(FALSE);
			};
#endif
			if (mptail->mvalue<dval){// the value doesn't change.
				pnewitem=mptail;
				mptail=mptail->pprev; 
				mptail->pnext=NULL;
				mFastMap.Remove(pnewitem->secsym);

				pnewitem->mvalue=dval;
				memccpy (pnewitem->secsym,secsym,0,sizeof(pnewitem->secsym));
				pprevitem=FindPrev(mptail,dval);
				InsertAfter(pnewitem,pprevitem);
				mFastMap.Add(secsym,(long)pnewitem);
			}
		}
	}else {//find the item; check near items;
		toplstitem *pnewitem;
		toplstitem *pprevitem;
		pnewitem=(toplstitem *)pitem->val;
		pprevitem=FindPrev(pnewitem,dval);
		if ((pprevitem==pnewitem)||(pprevitem==pnewitem->pprev)) {
			pnewitem->mvalue=dval; 
		}else {
			RemoveItem(pnewitem);
			pnewitem->mvalue=dval;
			InsertAfter(pnewitem,pprevitem); 
		}
	}
	mlock.Unlock(); 
	return TRUE;
}

toplstitem * TopLst::FindPrev(toplstitem *pSeed, double dval)
{
/*	toplstitem *p;
	if (pSeed==NULL) return NULL;
	if (pSeed->mvalue>dval) {
		p=pSeed;
		while(p->pnext) {
			if (p->pnext->mvalue>dval) break;  			
			p=p->pnext;
		}
		return p;
	}else {
		p=pSeed->pprev;
		while(p) {
			if (p->mvalue>dval)  break;
			p=p->pprev;
		}
		return p;
	}
*/
	toplstitem *p;
	p=mptail;
	while(p) {
		if (p->mvalue>dval) break;
		p=p->pprev; 
	}
	return p;
}

void TopLst::InsertAfter(toplstitem *pitem, toplstitem *pafter)
{
	ASSERT(pitem!=NULL);

	if (pafter==NULL) {
		pitem->pnext=mphead;
		mphead=pitem;
		pitem->pprev=NULL;
		if (pitem->pnext==NULL) {
			mptail=pitem;
		}else {
			pitem->pnext->pprev=pitem;
		}
	}else {
		pitem->pnext=pafter->pnext;
		pitem->pprev=pafter ;
		pafter->pnext =pitem;
		if (pitem->pnext ==NULL) {
			mptail=pitem;
		}else {
			pitem->pnext->pprev=pitem;
		}
	}
}

BOOL TopLst::RemoveItem(toplstitem *pitem)
{
	ASSERT(pitem!=NULL);
	if (pitem->pnext==NULL) {
		mptail=pitem->pprev;
	}else {
		pitem->pnext->pprev =pitem->pprev;
	}
	if (pitem->pprev==NULL) {
		mphead=pitem->pnext;
	}else {
		pitem->pprev->pnext=pitem->pnext;   
	}
	return TRUE;
}

toplstitem * TopLst::GetHead()
{
	return mphead;
}

void TopLst::Lock()
{
	mlock.Lock(); 
}

void TopLst::Unlock()
{
	mlock.Unlock(); 
}


BOOL toplstFilter::InTimePeriod(int ntm)
{
	if ((mSecondFrom<=ntm)&&(ntm<mSecondTo)){
		return TRUE;
	};
	return FALSE;
}

void toplstFilter::CopyFrom(toplstFilter *pitem)
{
	mid=pitem->mid;
	mprice=pitem->mprice;
	mSecondFrom=pitem->mSecondFrom;
	mSecondTo=pitem->mSecondTo;
	mshares=pitem->mshares; 
}

void toplstFilter::Clear()
{
	mid=0;
	mprice=0;
	mSecondFrom=0;
	mSecondTo=0;
	mshares=0; 
}
