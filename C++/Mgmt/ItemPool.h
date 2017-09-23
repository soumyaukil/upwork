// ItemPool.h: interface for the ItemPool class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMPOOL_H__6EB37601_1F97_11D5_BD2B_0050041B8FEC__INCLUDED_)
#define AFX_ITEMPOOL_H__6EB37601_1F97_11D5_BD2B_0050041B8FEC__INCLUDED_

#include "FUNCTIONS\intmap.h"	// Added by ClassView
#include "FUNCTIONS\FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "itemsDef.h"
class ItemPool  
{
private:
	FixLenMem FMB_Branch;// the branch items
public:
	itemBranch * GetBranch(LPCSTR pshort);
	intmap MAP_Branch;
	BOOL AddBranchToMap(itemBranch *pitem);
	BOOL DBLoadBranchItem();
	itemBranch * GetBranch(long brid);
	inline	itemBranch * FM_GetBranchItem(){
		return (itemBranch *) FMB_Branch.GetVoidBuff();
	};
	inline void FM_RealeaseBranchItem(itemBranch *pitem){
		FMB_Branch.PutVoidBuff((char *)pitem);
	};

	
	BOOL Initualize();
	BOOL Finalize();
	ItemPool();
	virtual ~ItemPool();

};

#endif // !defined(AFX_ITEMPOOL_H__6EB37601_1F97_11D5_BD2B_0050041B8FEC__INCLUDED_)
