// QuoteLst.cpp: implementation of the QuoteLst class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QuoteLst.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FixLenMem QuoteLst::sQuoteInfoBuff;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

QuoteLst::QuoteLst()
{
	mphead=NULL;
}

QuoteLst::~QuoteLst()
{
	RemoveAll();
}

void QuoteLst::Init()
{
	sQuoteInfoBuff.init(sizeof(QuoteIfo),10230); 
}

QuoteIfo * QuoteLst::GetItemBuff()
{
	return (QuoteIfo*)sQuoteInfoBuff.GetVoidBuff();
}

void QuoteLst::ReleaseItemBuff(QuoteIfo *pitem)
{
	sQuoteInfoBuff.PutVoidBuff((char*)pitem); 
}



//this is for Feed Center, need to caculate the up down.
//don't need to be sorted!
//flag: 0x1:price, 0x2:size, 0x4 QuoteCondition,0x8,time;
QuoteIfo * QuoteLst::AddQuote(BOOL bAscend, DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime,int flag)
{
	mLock.Lock();
	QuoteIfo *p;
	QuoteIfo *pitem;
	p=mphead;
	while(p!=NULL) {
		if (p->MMID==nMMID) {
			break;
		}
		p=p->pnext;
	}
	if (p==NULL) {
		pitem=GetItemBuff();
		
		if (dprice>0) pitem->Price=dprice;
		else pitem->Price=0;		
		
		pitem->Size=nSize;
		
		pitem->MMID=nMMID;
		pitem->QuoteTime=ntime;  
		nQuoteCondition|=0xFE00;
		pitem->QuoteCondition=nQuoteCondition;

		pitem->pprev=NULL;
		pitem->pnext=mphead;
		if (mphead)mphead->pprev=pitem;
		mphead=pitem;

	}else {
		int newcondition;
		pitem=p;
		if (flag & 0x4) {	
			newcondition=(nQuoteCondition & 0x1ff)|0x8000; 
			if ((pitem->QuoteCondition&0xff)!=(nQuoteCondition&0xff)) {
				newcondition|=0x2000;
			}
		}else {
			newcondition=(pitem->QuoteCondition & 0x1ff)|0x8000; 
		}

		if ((flag &0x8) && (pitem->QuoteTime!=ntime)) {
			newcondition|=0x4000;
			pitem->QuoteTime=ntime;
		}

		if ((flag& 0x1)&&(dprice>=0)) {
			if (pitem->Price==dprice) {
			}else {
				if (dprice>pitem->Price) {
					newcondition|= 0x800+0x200;
				}else {
					newcondition|= 0x200;
				}
				pitem->Price=dprice;
			}
		}
		if ((flag&0x2)&&(nSize>=0)) {
			if (nSize>pitem->Size) {
				newcondition|= 0x1400;
				pitem->Size=nSize;
			}else if (nSize<pitem->Size) {
				newcondition|=0x400;
				pitem->Size=nSize;
			}
		}
		
		pitem->QuoteCondition=newcondition; 
	}
	mLock.Unlock();
	return pitem;
}

void QuoteLst::RemoveAll()
{
	QuoteIfo *p,*p1;
	mLock.Lock();
	p=mphead;
	while(p!=NULL){
		p1=p->pnext;  
		ReleaseItemBuff(p);
		p=p1;
	}
	mphead =NULL;
	mLock.Unlock();
}

//this function is for Client to insert a quote information to 
//Quote list
QuoteIfo * QuoteLst::InsertQuote(BOOL bAscend, DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime)
{
	QuoteIfo *pitem=NULL;
	BOOL bRemove=FALSE;
	mLock.Lock();
	pitem=mphead;
	while(pitem) {
		if (pitem->MMID==nMMID) break;
		pitem=pitem->pnext;
	}
	
	if (pitem==NULL) {
		if ((nQuoteCondition&0x8000)==0) {
			mLock.Unlock();
			return NULL;
		}
		if ((dprice==0)||(nSize==0)) {
			mLock.Unlock();
			return NULL;
		}

		pitem=GetItemBuff();
		pitem->Price=dprice;
		pitem->Size=nSize;
		pitem->MMID=nMMID;
		pitem->QuoteTime=ntime;  
		pitem->QuoteCondition=nQuoteCondition;
	}else {
		if (pitem->pprev!=NULL) {
			pitem->pprev->pnext=pitem->pnext;
		}else {
			mphead=pitem->pnext; 
		}
		if (pitem->pnext!=NULL) {
			pitem->pnext->pprev=pitem->pprev;
		}

		BYTE flag=(BYTE)(nQuoteCondition>>8);
 		if (flag&0x80){
			pitem->Size=nSize;
 			pitem->QuoteTime=ntime;
 			pitem->QuoteCondition=nQuoteCondition;
			pitem->Price=dprice ;
 		}else {
			if (flag&2){
				pitem->Price=dprice; 
				if (dprice==0) bRemove=TRUE; 
			}
			if (flag&4) {
				pitem->Size=nSize;
				if (nSize==0) bRemove=TRUE; 
			}
 			if (flag&0x20) pitem->QuoteCondition=nQuoteCondition;
			else {
				pitem->QuoteCondition&=0xFF;
				pitem->QuoteCondition|=(nQuoteCondition&0xFF00);
			}
 			if (flag&0x40) pitem->QuoteTime=ntime;
		} 

	}

	if (bRemove) {
		ReleaseItemBuff(pitem);
		mLock.Unlock();
		return NULL;
	}


	QuoteIfo *p=mphead;
	QuoteIfo *plast=NULL;
	if (bAscend) {
		while(p!=NULL) {
			if (p->Price<pitem->Price-0.00000001){
				plast=p;
				p=p->pnext;
				continue;
			}
			if (p->Price<pitem->Price+0.00000001) {
				if (p->Size>pitem->Size) {
					plast=p;
					p=p->pnext;
					continue;
				} 
			}			
			break;
		}
	} else {
		while(p!=NULL) {
			if (p->Price>pitem->Price+0.00000001){
				plast=p;
				p=p->pnext;
				continue;
			}
			if (p->Price>=pitem->Price-0.00000001) {
				if (p->Size>pitem->Size) {
					plast=p;
					p=p->pnext;
					continue;
				} 
			}			
			break;
		}
	}

	if (plast==NULL) {
		pitem->pnext=mphead;
		if (mphead!=NULL){
			mphead->pprev=pitem;
		}
		pitem->pprev=NULL;
		mphead=pitem;
	}else {
		pitem->pnext=plast->pnext;
		if (plast->pnext) plast->pnext->pprev=pitem;
		pitem->pprev=plast;
		plast->pnext=pitem;
	}

	mLock.Unlock();
	return pitem;

//	mLock.Lock();
//	QuoteIfo *p;
//	QuoteIfo *pnew;
//	QuoteIfo *pold;
//	QuoteIfo *pitem;
//	p=mphead;
//	pnew=pold=NULL;
//	if ((nQuoteCondition&(0x8200))==0) {//price doesn't change
//		while(p!=NULL) {
//			if (p->MMID==nMMID){ 
//				pnew=pold=p;
//				break;
//			}
//			p=p->pnext;
//		}
//	}else {
//		if (bAscend) {
//			while(p!=NULL) {
//				if (p->MMID==nMMID) pold=p;
//				if (p->Price>=dprice) {
//					pnew=p; 
//					break;
//				}			
//				p=p->pnext;
//			}
//		} else {
//			while(p!=NULL) {
//				if (p->MMID==nMMID) pold=p;
//				if (p->Price<=dprice) {
//					pnew=p; 
//					break;
//				}			
//				p=p->pnext;
//			}
//		}
//		if ((pold==NULL)&&(p!=NULL)) {
//			p=p->pnext;
//			while(p!=NULL) {
//				if (p->MMID==nMMID){ 
//					pold=p;
//					break;
//				}
//				p=p->pnext;
//			}
//		}
//
//	}	
//
//
//	if (pold==NULL) {
//		if ((nQuoteCondition&0x8000)==0) {
//			mLock.Unlock();
//			return NULL;
//		}
//		if ((dprice==0)||(nSize==0)) {
//			mLock.Unlock();
//			return NULL;
//		}
//
//		pitem=GetItemBuff();
//		pitem->Price=dprice;
//		pitem->Size=nSize;
//		pitem->MMID=nMMID;
//		pitem->QuoteTime=ntime;  
////		nQuoteCondition|=0xff;
//		pitem->QuoteCondition=nQuoteCondition;
//	}else {
//		pitem=pold;
//		BYTE flag=(BYTE)(nQuoteCondition>>8);
// 		if (flag&0x80){
//// 			pitem->QuoteCondition&=0x1ff; 
//			pitem->Size=nSize;
// 			pitem->QuoteTime=ntime;
// 			pitem->QuoteCondition=nQuoteCondition;
//			pitem->Price=dprice ;
// 		}else {
//			if (flag&2) pitem->Price=dprice; 
// 			if (flag&4) pitem->Size=nSize;
// 			if (flag&0x20) pitem->QuoteCondition=nQuoteCondition;
//			else {
//				pitem->QuoteCondition&=0xFF;
//				pitem->QuoteCondition|=(nQuoteCondition&0xFF00);
//			}
// 			if (flag&0x40) pitem->QuoteTime=ntime;
//		} 
//
//		if ((pitem->Price==0)||(pitem->Size==0)) {
//			if (pold->pprev!=NULL) {
//				pold->pprev->pnext=pold->pnext;
//			}else {
//				mphead=pold->pnext; 
//			}
//			if (pold->pnext!=NULL) {
//				pold->pnext->pprev=pold->pprev;
//			}
//			ReleaseItemBuff(pold);
//			mLock.Unlock();
//			return NULL;
//
//		}
//		if ((flag & 0x2)==0) {//price does't changed;
//			mLock.Unlock();
//			return pitem;
//		}else {
//			if (pold!=pnew){
//				if (pold->pprev!=NULL) {
//					pold->pprev->pnext=pold->pnext;
//				}else {
//					mphead=pold->pnext; 
//				}
//				if (pold->pnext!=NULL) {
//					pold->pnext->pprev=pold->pprev;
//				}
//			}else {
//				mLock.Unlock();
//				return pitem;
//			}
//		}
//	}
//	if (pnew==NULL) {
//		if (mphead==NULL) {
//			pitem->pnext=NULL;
//			pitem->pprev=NULL;
//			mphead=pitem;
//		}else {
//			p=mphead;
//			while(p->pnext!=NULL) p=p->pnext;
//			p->pnext =pitem;
//			pitem->pprev=p;
//			pitem->pnext=NULL;
//
//		}
//	}else {
//		pitem->pnext=pnew;
//		pitem->pprev=pnew->pprev; 
//		if (pnew->pprev!=NULL) {
//			pnew->pprev->pnext=pitem;
//		}else {
//			mphead=pitem;
//		};
//		pnew->pprev=pitem; 
//	}
//	mLock.Unlock();
//	return pitem;
}

int QuoteLst::GetCount()
{
	int itemnum=0;
	QuoteIfo *pitem;
	pitem=mphead;
	while(pitem!=NULL){
		itemnum++;
		pitem=pitem->pnext;
	}
	return itemnum;
}

QuoteIfo * QuoteLst::RmoveQuote(DWORD nMMID)
{
	QuoteIfo *p;
	p=mphead;
	while(p!=NULL) {
		if (p->MMID==nMMID) break;
		p=p->pnext;
	}
	if (p!=NULL) {
		if (p->pprev!=NULL) {
			p->pprev->pnext=p->pnext;
		}else {
			mphead=p->pnext;
		}

		if (p->pnext!=NULL) {
			p->pnext->pprev=p->pprev;
		}
	}
	return p;
}

//QuoteServer will not use this function.
BOOL QuoteLst::InsertNewQuote(BOOL bAscend, QuoteIfo *pifo)
{
	QuoteIfo *p,*p1;
	if (mphead==NULL){
		pifo->pnext=NULL;
		pifo->pprev=NULL;
		mphead=pifo;
		return TRUE;
	}

	p1=NULL;
	p=mphead;
	if (bAscend) {
		while(p!=NULL) {
			if (p->Price>=pifo->Price) {
				break;
			}			
			p1=p;
			p=p->pnext;
		}
	} else {
		while(p!=NULL) {
			if (p->Price<=pifo->Price) {
				break;
			}
			p1=p;
			p=p->pnext;
		}
	}
	pifo->pprev =p1;
	pifo->pnext =p;
	if (p!=NULL) {
		p->pprev =pifo;
	}
	if (p1!=NULL) {
		p1->pnext=pifo;
	}else {
		mphead=pifo;
	}

	return TRUE;
}
//this function is used as add a quote to client program. but 
//current is doesn't used.
/*
QuoteIfo *QuoteLst::SortInsertQuote(BOOL bAscend, DWORD nMMID, int nQuoteCondition, double dprice, int nSize, time_t ntime)
{
	mLock.Lock();
	QuoteIfo *pold;
	QuoteIfo *pitem;
	pitem=NULL;
	pold=RmoveQuote(nMMID);
	if (pold==NULL) {
		pitem=GetItemBuff();
		memset(pitem,0,sizeof(QuoteIfo));
		pitem->MMID=nMMID;
	}else {
		pitem=pold;
	}
	BYTE flag =(nQuoteCondition>>8) |0xFF;
 	if (flag&0x80){
		pitem->Price=dprice ;
		pitem->Size=nSize;
 		pitem->QuoteTime=ntime;
 		pitem->QuoteCondition=nQuoteCondition;
 	}else {
		if (flag&2) pitem->Price=dprice; 
 		if (flag&4) pitem->Size=nSize;
 		if (flag&0x20) pitem->QuoteCondition=nQuoteCondition;
		else {
			pitem->QuoteCondition&=~0xFF;
			pitem->QuoteCondition|=(nQuoteCondition&0xFF00);
		}
 		if (flag&0x40) pitem->QuoteTime=ntime;
	} 

	if ((pitem->Price==0)||(pitem->Size==0)) {
		ReleaseItemBuff(pitem);
		pitem=NULL;
	}else {
		InsertNewQuote(bAscend,pitem);
	}

	mLock.Unlock();
	return pitem;
}

*/
//this function only for QuoteServer to maintaince the quote list;
QuoteIfo * QuoteLst::QSevInsertQuote(BOOL bAscend, DWORD nMMID, DWORD &nQuoteCondition, double dprice, int nSize, time_t ntime)
{
	mLock.Lock();
	QuoteIfo *p;
	QuoteIfo *pnew;
	QuoteIfo *pold;
	QuoteIfo *pitem;
	p=mphead;
	pnew=pold=NULL;
//	ASSERT(nQuoteCondition&0x8000);
	if ((nQuoteCondition&(0x8200))==0) {//price doesn't change
		while(p!=NULL) {
			if (p->MMID==nMMID){ 
				pnew=pold=p;
				break;
			}
			p=p->pnext;
		}
	}else {
		if (bAscend) {
			while(p!=NULL) {
				if (p->MMID==nMMID) pold=p;
				if (p->Price>=dprice) {
					pnew=p; 
					break;
				}			
				p=p->pnext;
			}
		} else {
			while(p!=NULL) {
				if (p->MMID==nMMID) pold=p;
				if (p->Price<=dprice) {
					pnew=p; 
					break;
				}			
				p=p->pnext;
			}
		}
		if ((pold==NULL)&&(p!=NULL)) {
			p=p->pnext;
			while(p!=NULL) {
				if (p->MMID==nMMID){ 
					pold=p;
					break;
				}
				p=p->pnext;
			}
		}

	}	


	if (pold==NULL) {
		if ((dprice==0)||(nSize==0)) {
			mLock.Unlock();
			return NULL;
		}

		pitem=GetItemBuff();
		pitem->Price=dprice;
		pitem->Size=nSize;
		pitem->MMID=nMMID;
		pitem->QuoteTime=ntime;  
//		nQuoteCondition|=0xff;
		pitem->QuoteCondition=nQuoteCondition;
		
	}else {
		pitem=pold;
		BYTE flag=(BYTE)(nQuoteCondition>>8);
 		if (flag&0x80){
// 			pitem->QuoteCondition&=0x1ff; 
			pitem->Size=nSize;
 			pitem->QuoteTime=ntime;
 			pitem->QuoteCondition=nQuoteCondition;
			pitem->Price=dprice ;
 		}else {
			if (flag&2) pitem->Price=dprice; 
 			if (flag&4) pitem->Size=nSize;
 			if (flag&0x20) pitem->QuoteCondition=nQuoteCondition;
			else {
				pitem->QuoteCondition&=0xFF;
				pitem->QuoteCondition|=(nQuoteCondition&0xFF00);
			}
 			if (flag&0x40) pitem->QuoteTime=ntime;
		} 
		nQuoteCondition&=~0x8000;

		if ((pitem->Price==0)||(pitem->Size==0)) {
			if (pold->pprev!=NULL) {
				pold->pprev->pnext=pold->pnext;
			}else {
				mphead=pold->pnext; 
			}
			if (pold->pnext!=NULL) {
				pold->pnext->pprev=pold->pprev;
			}
			ReleaseItemBuff(pold);
			mLock.Unlock();
			return NULL;
		}

		if ((flag & 0x2)==0) {//price does't changed;
			mLock.Unlock();
			return pitem;
		}else {
			if (pold!=pnew){
				if (pold->pprev!=NULL) {
					pold->pprev->pnext=pold->pnext;
				}else {
					mphead=pold->pnext; 
				}
				if (pold->pnext!=NULL) {
					pold->pnext->pprev=pold->pprev;
				}
			}else {
				mLock.Unlock();
				return pitem;
			}
		}
	}
	if (pnew==NULL) {
		if (mphead==NULL) {
			pitem->pnext=NULL;
			pitem->pprev=NULL;
			mphead=pitem;
		}else {
			p=mphead;
			while(p->pnext!=NULL) p=p->pnext;
			p->pnext =pitem;
			pitem->pprev=p;
			pitem->pnext=NULL;

		}
	}else {
		pitem->pnext=pnew;
		pitem->pprev=pnew->pprev; 
		if (pnew->pprev!=NULL) {
			pnew->pprev->pnext=pitem;
		}else {
			mphead=pitem;
		};
		pnew->pprev=pitem; 
	}
	mLock.Unlock();
	return pitem;

}

//this function is for FeedCenter to get hi and low price  and size of OTCBB
BOOL QuoteLst::FindBestPrice(double *hi, int *hisize, double *lo, int *losize)
{
	mLock.Lock();
	QuoteIfo *p;

	p=mphead;
	double mhi,mlo;
	int mhisz,mlosz;

	mhi=mlo=0;
	mhisz=mlosz=0;
	
	while(p!=NULL) {
		if (p->Price>mhi) {
			mhi=p->Price;
			mhisz=p->Size; 
		}else if(p->Price==mhi) {
			mhisz+=p->Size; 
		}
		if ((p->Price<mlo)||(mlo==0)){
			mlo=p->Price;
			mlosz=p->Size; 
		}else if(p->Price==mlo) {
			mlosz+=p->Size; 
		}
		p=p->pnext;
	}
	
	mLock.Unlock();
	
	*hi=mhi;
	*lo=mlo;
	*hisize=mhisz;
	*losize=mlosz;
	return TRUE;
}

void QuoteLst::GetMemUsage(int &ntotal, int &nvoid)
{
	ntotal=sQuoteInfoBuff.CountTotalSize();
	nvoid=sQuoteInfoBuff.CountVoidSize(); 
}

//this function remove from the list which price less than dprice;
//return is quot link,
//caller should free the quoteifo back to the buff!! so no memory leaking.
QuoteIfo * QuoteLst::RmAllPrice(BOOL bless, double dprice,int currtime,int seconds)
{
	QuoteIfo *pret,*p1,*p2;
	mLock.Lock(); 
	p1=mphead;
	pret=NULL;
	p2=NULL;
	if (bless) {
		while(p1) {
			if(p1->Price<dprice) {
				if ((currtime-p1->QuoteTime)>seconds) {//remove;
					p2=p1;
					p1=p1->pnext;
					if (p1) {
						p1->pprev=p2->pprev;
					}
					if (p2->pprev) {
						p2->pprev->pnext=p1;
					}else {
						mphead=p1;
					}
					p2->pnext=pret;
					pret=p2;
				}else {
					p1=p1->pnext;
				}
			}else break;
		}
	}else {
		while(p1) {
			if(p1->Price>dprice) {
				if ((currtime-p1->QuoteTime)>seconds) { 
					p2=p1;
					p1=p1->pnext;
					if (p1) {
						p1->pprev=p2->pprev;
					}
					if (p2->pprev) {
						p2->pprev->pnext=p1;
					}else {
						mphead=p1;
					}
					p2->pnext=pret;
					pret=p2;
				}else {
					p1=p1->pnext;
				}
			}else break;
		}
	}
	mLock.Unlock();
	return pret;
}

//this function relsease items back to buffer;
void QuoteLst::ReleaseItemsLst(QuoteIfo *phead)
{
	QuoteIfo *p1,*p2;
	p1=phead;
	while (p1) {
		p2=p1;
		p1=p1->pnext;
		sQuoteInfoBuff.PutVoidBuff((char*)p2); 
	}
}

//this function mark a quotes from remove, don't remove the quote itself.
int QuoteLst::MarkRemove(BOOL bless, double dprice, int currtime, int seconds)
{
	QuoteIfo *p1;
	mLock.Lock(); 
	p1=mphead;
	if (bless) {
		while(p1) {
			if(p1->Price<dprice) {
				if ((currtime-p1->QuoteTime)>seconds) {//remove;
					p1->QuoteCondition|=0x10000;
				}
				p1=p1->pnext;

			}else break;
		}
	}else {
		while(p1) {
			if(p1->Price>dprice) {
				if ((currtime-p1->QuoteTime)>seconds) { 
					p1->QuoteCondition|=0x10000;
				}
				p1=p1->pnext;
			}else break;
		}
	}
	mLock.Unlock();
	return 0;

}

QuoteIfo * QuoteLst::FindQuoteByMMID(DWORD MMID)
{
	QuoteIfo *pitem=NULL;
	pitem=mphead;
	while(pitem) {
		if (pitem->MMID==MMID) break;
		pitem=pitem->pnext;
	}
	return pitem;
}
