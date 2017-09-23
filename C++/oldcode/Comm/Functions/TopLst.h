// TopLst.h: interface for the TopLst class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TOPLST_H__8837E0A0_17C7_11D5_BD2B_0050041B8FEC__INCLUDED_)
#define AFX_TOPLST_H__8837E0A0_17C7_11D5_BD2B_0050041B8FEC__INCLUDED_

#include "intmap.h"	// Added by ClassView
#include "FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class toplstitem 
{
public:
	char secsym[12];
	int	 mseq;
	double mvalue;
	toplstitem *pprev;
	toplstitem *pnext;
};
class toplstFilter
{
public:
	char	mid;//case 1:mTLSTQActive;
					//case 2:mTLSTQGain;
					//case 3:mTLSTQLost;
					//case 4:mTLSTNActive;
					//case 5:mTLSTNGain;
					//case 6:mTLSTNLost;
	int	mSecondFrom;//the (time*60+Min)*60
	int	mSecondTo;  //the (time*60+Min)*60
	double  mprice;
	int		mshares;
public:
	void Clear();
	void CopyFrom(toplstFilter *pitem);
	BOOL InTimePeriod(int ntm);
	inline BOOL PriceInFilter(double dprice){
		if (dprice>mprice) return TRUE;
		return FALSE;
	};
	inline BOOL VolumeInFilter(int nshares) {
		if (nshares>mshares) return TRUE;
		return FALSE;
	}

};

class TopLst  
{
	static FixLenMem mitembuff;
	strmap mFastMap;
	toplstitem *mphead;
	toplstitem *mptail;
	int mitemnu;//how many items we already keeped in the list.
	int mTopSize;//how many top item we want to retrive;
	simplelock mlock;
public:
	BOOL mbEnable;
private:
	toplstitem * FindPrev(toplstitem *pSeed, double dval);
	void InsertAfter(toplstitem *pitem, toplstitem *pafter);
	BOOL RemoveItem(toplstitem *pitem);

public:
	void Unlock();
	void Lock();
	toplstitem * GetHead();
	BOOL UpdateData(LPCSTR secsym,double dval);
	
	BOOL init(int nTopSize,int nHashSize);
	BOOL RemoveAll();
	TopLst();

	virtual ~TopLst();

};

#endif // !defined(AFX_TOPLST_H__8837E0A0_17C7_11D5_BD2B_0050041B8FEC__INCLUDED_)
