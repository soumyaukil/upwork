// Property.cpp: implementation of the Property class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "autotrade.h"
#include "Property.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

FixLenMem Property::mdatabuff(sizeof(prop_float),2044);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Property::Property()
{
	phead =NULL;
	pCurr=NULL;
}



Property::~Property()
{

}

double Property::GetDBLProperty(LPCSTR prName)
{
	prop_float * ptmp;
	mlock.Lock();
	ptmp=phead;
	while(ptmp!=NULL) {
		if (strcmp(prName,ptmp->name)==0) {
			mlock.Unlock();
			return ptmp->val; 
		}
		ptmp=ptmp->pnext;
	}
	mlock.Unlock();
	return 0;
}

BOOL Property::SetDBLProperty(LPCSTR prname, double prval)
{	prop_float * ptmp;
	prop_float * ptmp1;
	mlock.Lock();
	if (prval==0){ //delete property
		ptmp=ptmp1=phead;
		while(ptmp!=NULL) {
			if (strcmp(prname,ptmp->name)==0) {
				if (ptmp==phead) {
					phead=ptmp->pnext;
				} else {
					ptmp1->pnext =ptmp->pnext;
				}
				if (pCurr==ptmp) {
					pCurr=ptmp->pnext; 
				}
				mdatabuff.PutVoidBuff((char*)ptmp);
				break;
			}
			ptmp1=ptmp;
			ptmp=ptmp->pnext;
		}
	} else { //add property
		ptmp=phead;
		while(ptmp!=NULL) {
			if (strcmp(prname,ptmp->name)==0){
				ptmp->val= prval;
				mlock.Unlock();
				return TRUE;
			}
			ptmp=ptmp->pnext;
		}	
		ptmp=(prop_float *)mdatabuff.GetVoidBuff();
		ptmp->pnext	=phead;
		strcpy(ptmp->name,prname);
		ptmp->val=prval; 
		phead =ptmp;
	}
	mlock.Unlock();
	return TRUE;
}

prop_float * Property::GetFistPosition()
{
	pCurr=phead;
	return pCurr;
}

prop_float * Property::GetNext()
{
	prop_float *tmpcurr;
	tmpcurr=pCurr;
	if (pCurr!=NULL) {
		pCurr=pCurr->pnext; 
	}
	return (tmpcurr);
}

BOOL Property::ClearAll()
{
	prop_float * tmpprop;
	mlock.Lock();
	while(phead!=NULL) {
		tmpprop=phead;
		phead=phead->pnext;
		Property::mdatabuff.PutVoidBuff((char*)tmpprop);
	}
	pCurr=NULL;
	mlock.Unlock();
	return TRUE;
}

BOOL Property::SetPropertyBag(Property *pprobag)
{
	ClearAll();
	mlock.Lock();
	pCurr=NULL;
	phead=pprobag->phead;
	mlock.Unlock();
	pprobag->phead=NULL;
	pprobag->pCurr=NULL;
	return TRUE;
}
