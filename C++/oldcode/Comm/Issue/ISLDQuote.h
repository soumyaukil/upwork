// ISLDQuote.h: interface for the ISLDQLst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISLDQUOTE_H__CBB2A1D1_0CF6_11D5_BD29_0050041B8FEC__INCLUDED_)
#define AFX_ISLDQUOTE_H__CBB2A1D1_0CF6_11D5_BD29_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "..\FUNCTIONS\FixLenMem.h"	// Added by ClassView

struct ISLDQitem {
	DWORD  nrefnum;
	double price;
	DWORD  shares;
	BYTE   flag; // "B" buy, "S" sell;
	time_t modifytime;
	ISLDQitem *pnext; 
};

class ISLDQLst  
{
	simplelock mLock;
	ISLDQitem *mphead;

public:
	ISLDQLst();
	virtual ~ISLDQLst();
public:
	void RemoveAll();
	ISLDQitem * ModifyItem(DWORD nrefnum, int ndecress,int nntime);
	ISLDQitem * FindItem(DWORD nrefnum);
	ISLDQitem * AddItem(DWORD nrefnum,BYTE nflag, double dprice, DWORD nshare,time_t ntime);
	inline ISLDQitem *GetHead(){ return mphead;};
	static void ReleaseItemBuff(ISLDQitem *pitem);
	static ISLDQitem * GetItemBuff();
private:
	static FixLenMem sItemBuff;
	

};

#endif // !defined(AFX_ISLDQUOTE_H__CBB2A1D1_0CF6_11D5_BD29_0050041B8FEC__INCLUDED_)
