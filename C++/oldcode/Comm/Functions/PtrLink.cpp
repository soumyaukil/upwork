// PtrLink.cpp: implementation of the CPtrLink class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "QuoteServer.h"
#include "PtrLink.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FixLenMem CPtrLink::mWatchlstBuff(sizeof(lnkPtr),2044);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPtrLink::CPtrLink()
{
	phead=NULL;
}

CPtrLink::~CPtrLink()
{
	
}

BOOL CPtrLink::AddPtr(void *ptr,int mask)
{
	BOOL ret;
	mlock.Lock();

	lnkPtr *p1;
	BOOL bFinded;
	p1=phead;
	if (phead==NULL) ret=TRUE;
	else ret=FALSE;
	bFinded=FALSE;
	while(p1!=NULL){
		if (p1->mptr==ptr) {
			p1->mflag|=mask;
			bFinded=TRUE;
			break;
		}
		p1=p1->mpNext;
	};

	if (bFinded==FALSE) {
		p1=(lnkPtr*)mWatchlstBuff.GetVoidBuff();
		p1->mptr=ptr;
		p1->mflag=mask;
		p1->mpNext=phead;
		phead=p1;
	}
	mlock.Unlock();
	return ret;
}

BOOL CPtrLink::RemovePtr(void *ptr,int mask)
{
	lnkPtr *p1,*p2;
	BOOL ret;
	mlock.Lock();
	p1=p2=phead;
	while(p1!=NULL){
		if (p1->mptr==ptr) {
			p1->mflag&=~mask;
			if (p1->mflag==0) {
				if(p1==phead) {
					phead=p1->mpNext;
				}else {
					p2->mpNext =p1->mpNext;
				}
				mWatchlstBuff.PutVoidBuff((char*)p1);
			}
			break;
		}
		p2=p1;
		p1=p1->mpNext;
	};
	if (phead==NULL) ret=TRUE;
	else ret=FALSE;
	mlock.Unlock();
	return ret;
}

BOOL CPtrLink::IsExist(void *ptr,int mask)
{
	lnkPtr *p1;
	BOOL ret;
	mlock.Lock();
	p1=phead;
	ret=FALSE;
	while(p1!=NULL){
		if (p1->mptr==ptr) {
			if ((p1->mflag&mask)!=0)  {
				ret=TRUE;
			}
			break;
		}
		p1=p1->mpNext;
	};
	mlock.Unlock();
	return ret;
}

void CPtrLink::GetMemUsage(int &ntotal, int &nvoid)
{
	ntotal=mWatchlstBuff.CountTotalSize();
	nvoid=mWatchlstBuff.CountVoidSize(); 
}

//DEL int lnkPtr::GetCounter()
//DEL {
//DEL 	
//DEL }

int CPtrLink::GetCounter()
{
	lnkPtr *p1;
	int num;
	mlock.Lock();
	p1=phead;
	num=0;
	while(p1!=NULL){
		num++;
		p1=p1->mpNext;
	};
	mlock.Unlock();
	return num;

}
