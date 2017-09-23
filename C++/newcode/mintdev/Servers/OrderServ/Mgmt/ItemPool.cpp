// ItemPool.cpp: implementation of the ItemPool class.
//
//////////////////////////////////////////////////////////////////////
#include "../orderserv.h"
#include "ItemPool.h"
#include "itemsSrst.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ItemPool::ItemPool()
{
	Initualize();
}

ItemPool::~ItemPool()
{
	Finalize();
}


bool ItemPool::Initualize()
{
	FMB_Branch.init(sizeof(itemBranch),2048); 
	MAP_Branch.init(30); 
	return TRUE;
}

bool ItemPool::Finalize()
{
	//Release branch information
	intitem *PIBranch;
	PIBranch=MAP_Branch.EnumFirst();
	while(PIBranch) {
		FM_RealeaseBranchItem((itemBranch*)PIBranch->val);
		PIBranch=MAP_Branch.EnumNext(PIBranch); 
	}
	MAP_Branch.RemoveAll();  
	//---------------
	return TRUE;
}

itemBranch * ItemPool::GetBranch(long brid)
{
	intitem *PIBranch;
	PIBranch=MAP_Branch.FindItem(brid);
	if (PIBranch){
		return (itemBranch *)PIBranch->val;
	}else {
		return NULL;
	}
}


bool ItemPool::DBLoadBranchItem()
{
	srstBranch rst;
	rst.OpenToRead();
	itemBranch *pbr; 
	while(!rst.IsEOF()) {
		if (!rst.mitem.IsDeleted()) {
			pbr=FM_GetBranchItem();
			ASSERT(pbr!=NULL);
			pbr->CopyItem(&rst.mitem); 
			MAP_Branch.Add(pbr->mbrid,(long)pbr);
		}
		rst.MoveNext(); 
	}
	rst.Close();
	return TRUE;
}

bool ItemPool::AddBranchToMap(itemBranch *pitem)
{
	ASSERT(pitem!=NULL);
	MAP_Branch.Add(pitem->mbrid,(long)pitem);
	return TRUE;
}

itemBranch * ItemPool::GetBranch(LPCSTR pshort)
{
	intitem *PIbr;
	itemBranch *pbr;
	if (pshort==NULL) return NULL;
	MAP_Branch.EnumFirst(&PIbr);
	while (PIbr){
		pbr=(itemBranch*)PIbr->val;
		if (strcmp(pshort,pbr->mshort)==0) {
			return pbr;
		}
		MAP_Branch.EnumNext(&PIbr);
	}
	return NULL;
}
