// PriceBook.h: interface for the PriceBook class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRICEBOOK_H__2D3BABAF_3FF9_4209_BF2B_BAE07795CAC0__INCLUDED_)
#define AFX_PRICEBOOK_H__2D3BABAF_3FF9_4209_BF2B_BAE07795CAC0__INCLUDED_

#include "..\FUNCTIONS\FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//mTYPE		 0:Level2;
//			 1:ARCA;
//		 2:INCA;
//		 3:BRUT;
//		 4:ISLD;

enum enEBookType {
	en_EBT_Lv2=0,	//level2 book;
	en_EBT_Isld=1,	//island book;
	en_EBT_Inca=2,	//inca book
	en_EBT_Brut=3,  //brut book
	en_EBT_Arca=4,  //arca book;
	en_EBT_OpenBook=5,
	
	en_EBT_OldIsld=100,  //arca book;

//	en_EBT_Lv2c=100
};


struct st_BookItem
{
	int mrefnum;
	int mshares; 
	double mprice;
	st_BookItem *pnext;
public:
	static LPCSTR GetBookName(enEBookType bktype);
	void Clear();
};


//this class is for the price book of same security.
// the order have bid and ask size, each as a layer.
// usually one layer is the aggrigated order of same price.
class PriceBook  
{
public:
	st_BookItem *mpBid;
	st_BookItem *mpAsk;
public:
	//this function is used by server 
	st_BookItem * InsertLayer(int &nflag, int flag,int share,double nprice);
	st_BookItem * FindOrderByPrice(int flag, double nprice);
public:
	//set the top price, delete the layers for those layer;
	//return how many layser been deleted.
	int SetTop(int flag, double dprice);

	st_BookItem * FindOrderByPrice(int flag,double nprice,BOOL bAdd);
	void PurgeBook();
	PriceBook();
	~PriceBook();
	
	st_BookItem * ModifyOrder(int flag,int nrefnum, int nshares, double nprice);
	//flag bit 1: 0=bid,1=ask;
	st_BookItem* AddOrder(int flag,int nrefnum, int nshares,double nprice);
	BOOL DelOrder(int flag, int nrefnum);

	static FixLenMem sItemBuff;
};

#endif // !defined(AFX_PRICEBOOK_H__2D3BABAF_3FF9_4209_BF2B_BAE07795CAC0__INCLUDED_)
