// MsgBuff.cpp: implementation of the MsgBuff class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "Client.h"
#include "MsgBuff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MsgBuff::MsgBuff()
{
	phead=NULL;
//	pend=NULL;
	pkgnu=0;
	dealmsgs=0;
}

MsgBuff::~MsgBuff()
{

}

int MsgBuff::PutDabuff(dabuff *pbf)
{
	ASSERT(pbf!=NULL);
	mCriticalLock.Lock(); 
	pbf->pnext =phead;
	if(phead ==NULL){
		phead=pbf;
	};
	InterlockedIncrement(&pkgnu); 
	dealmsgs++;
	mCriticalLock.Unlock();
	return 0;	
}

dabuff * MsgBuff::GetPkg(WORD type, WORD subtype, BOOL bremove)
{
	dabuff * retbuf;
	dabuff * lastbuf;
	if (phead ==NULL) {
		return NULL;
	}

	mCriticalLock.Lock(); 
	retbuf=phead;
	lastbuf=retbuf;
	while(retbuf!=NULL){
		ASSERT(retbuf->bufend>retbuf->bufbegin+4); 
		if (*(WORD*)(retbuf->DataBuff+retbuf->bufbegin+2)==type) {
			if (*(WORD*)(retbuf->DataBuff+retbuf->bufbegin+4)==subtype){
				if (bremove==TRUE) {
					if (retbuf==phead){
						phead=retbuf->pnext;
					}else{
						lastbuf->pnext =retbuf->pnext;
					}
					InterlockedDecrement(&pkgnu); 
				}
				break;
			}
		}  
		lastbuf=retbuf;
		retbuf=retbuf->pnext; 
	}
	mCriticalLock.Unlock(); 
	return retbuf;
}
