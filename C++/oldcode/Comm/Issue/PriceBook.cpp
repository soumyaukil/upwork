// PriceBook.cpp: implementation of the PriceBook class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PriceBook.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FixLenMem PriceBook::sItemBuff(sizeof(st_BookItem),4080);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


st_BookItem* PriceBook::AddOrder(int flag, int nrefnu, int nshares, double nprice)
{

	st_BookItem *phead;
	st_BookItem *p1,*p2;
	p2=NULL;
	if (flag &1) { //ask;
		phead=mpAsk;
		p1=phead;
		while(p1){
			if (nprice<(p1->mprice-0.000001)) break;
			p2=p1;p1=p1->pnext;
		}
	}else {
		phead=mpBid;
		p1=phead;
		while(p1){
			if (nprice>(p1->mprice+0.000001)) break;
			p2=p1;p1=p1->pnext;
		}
	}

	p1=(st_BookItem *)sItemBuff.GetVoidBuff();
	p1->Clear();
	p1->mrefnum=nrefnu; 
	p1->mprice=nprice;
	p1->mshares=nshares;

	if (p2==NULL) {
		p1->pnext=phead;
		phead=p1;
	}else {
		p1->pnext=p2->pnext;
		p2->pnext=p1;

	}
	
	if (flag&1) {
		mpAsk=phead;
	}else {
		mpBid=phead;
	}

	return p1;
}

BOOL PriceBook::DelOrder(int flag, int nrefnu)
{

	st_BookItem *phead;
	phead=(flag &1)?mpAsk:mpBid;
	if (phead==NULL) return FALSE;

	st_BookItem *p1;
	st_BookItem *plast;
	p1=phead;
	plast=NULL;
	while(p1) {
		if (p1->mrefnum==nrefnu) break; 
		plast=p1;p1=p1->pnext;
	}
	if (p1==NULL) return FALSE;

	if (plast==NULL) {
		phead=p1->pnext;
		if (flag&1) {
			mpAsk=phead;
		}else {
			mpBid=phead;
		}
	}else {
		plast->pnext=p1->pnext;
	}
	sItemBuff.PutVoidBuff((char *)p1);
	return TRUE;
}

void st_BookItem::Clear()
{
	memset(this,0,sizeof(st_BookItem));
}


//this function is used by the ECNBOOK Server only!!!

st_BookItem * PriceBook::InsertLayer(int &nAddflag, int flag,int share,double nprice)
{
	nAddflag=0;
	int retlayer=0;
	st_BookItem *phead;
	st_BookItem *p1;
	st_BookItem	*p2;
	p2=NULL;
	if (flag &1) {
		phead=p1=mpAsk;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice <p1->mprice) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
	}else {
		phead=p1=mpBid;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice >p1->mprice) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
	}

	nAddflag|=1;
	p1=(st_BookItem *)sItemBuff.GetVoidBuff();
	p1->Clear();

	if (p2==NULL) {
		p1->pnext=phead;
		phead=p1;
		nAddflag|=2;
	}else {
		p1->pnext=p2->pnext;
		p2->pnext=p1;

	}
	if (flag&1) {
		mpAsk=phead;
	}else {
		mpBid=phead;
	}
	return p1;
}

//flag define is looking for the header;
//	flag: bit 0: 0=bid, 1=ask;
//		  bit 1: 1= share incress, 2=set shares
//		  bit 2: 1= set price;
st_BookItem * PriceBook::ModifyOrder(int flag, int nrefnum, int nshares, double nprice)
{

	st_BookItem *phead;
	phead=(flag &1)?mpAsk:mpBid;
	st_BookItem *p1;
	p1=phead;
	while(p1) {
		if (p1->mrefnum==nrefnum) break;
		p1=p1->pnext;
	}
	if (p1==NULL) {
		p1=AddOrder(flag, nrefnum,nshares, nprice);
		return p1;
	}
	if (flag &(1<<1)) {
		p1->mshares+=nshares; 
	}else {
		p1->mshares=nshares;
	}
	if (flag & (1<<2)) {
		p1->mprice=nprice;
	}

	return p1;

}

LPCSTR st_BookItem::GetBookName(enEBookType bktype)
{
	switch(bktype) {
	case en_EBT_Isld:
		return "ISB";
	case en_EBT_Inca:
		return "INB";
	case en_EBT_Arca:
		return "ACB";
	case en_EBT_Brut:
		return "BUB";
	case en_EBT_OpenBook:
		return "OPB";
	default :
		return "";
	};
}

PriceBook::PriceBook()
{
	mpAsk=NULL;
	mpBid=NULL;
}

PriceBook::~PriceBook()
{
/*	st_BookItem *p;
	while (mpAsk) {
		p=mpAsk;
		mpAsk=mpAsk->pnext;
		sItemBuff.PutVoidBuff((char*)p);
	}
	while(mpBid) {
		p=mpBid;
		mpBid=mpBid->pnext;
		sItemBuff.PutVoidBuff((char*)p);
	};
*/
	PurgeBook();
}


void PriceBook::PurgeBook()
{
	st_BookItem *p;
	while (mpAsk) {
		p=mpAsk;
		mpAsk=mpAsk->pnext;
		sItemBuff.PutVoidBuff((char*)p);
	}
	while(mpBid) {
		p=mpBid;
		mpBid=mpBid->pnext;
		sItemBuff.PutVoidBuff((char*)p);
	};
}

st_BookItem * PriceBook::FindOrderByPrice(int flag, double nprice)
{

	int retlayer=0;
	st_BookItem *p1;
	if (flag &1) {
		p1=mpAsk;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice <p1->mprice) {
				break;
			}
			p1=p1->pnext;
		}
	}else {
		p1=mpBid;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice >p1->mprice) {
				break;
			}
			p1=p1->pnext;
		}
	}
	return NULL;
}

st_BookItem * PriceBook::FindOrderByPrice(int flag, double nprice, BOOL bAdd)
{
	int retlayer=0;
	st_BookItem *phead;
	st_BookItem *p1;
	st_BookItem	*p2;
	p2=NULL;
	if (flag &1) {
		phead=p1=mpAsk;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice <p1->mprice) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
	}else {
		phead=p1=mpBid;
		while(p1) {
			if (nprice==p1->mprice) {
				return p1;
			}
			if (nprice >p1->mprice) {
				break;
			}
			p2=p1;
			p1=p1->pnext;
		}
	}

	if (bAdd) {
		p1=(st_BookItem *)sItemBuff.GetVoidBuff();
		p1->Clear();

		if (p2==NULL) {
			p1->pnext=phead;
			phead=p1;
		}else {
			p1->pnext=p2->pnext;
			p2->pnext=p1;

		}
		if (flag&1) {
			mpAsk=phead;
		}else {
			mpBid=phead;
		}
		return p1;
	}else {
		return NULL;
	}
}



//flag: bit 0:  1=ask;
int PriceBook::SetTop(int flag, double dprice)
{
	int ret=0;
	st_BookItem *p1;
	if (flag&1) {//ask;
		while(mpAsk) {
			if (mpAsk->mprice<dprice-0.00001) {
				p1=mpAsk;
				mpAsk=mpAsk->pnext;
				sItemBuff.PutVoidBuff((char *)p1);
				ret++;
			}else {
				break;
			}
		}
	}else {		 //bid;
		while(mpBid) {
			if (mpBid->mprice>dprice+0.00001) {
				p1=mpBid;
				mpBid=mpBid->pnext;
				sItemBuff.PutVoidBuff((char *)p1);
				ret++;
			}else {
				break;
			}
		}
	}
	return ret;	
}
