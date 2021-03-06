// PkgBuff.cpp: implementation of the CPkgBuff class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "PkgBuff.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma init_seg (lib)
//CPkgBuff thePkgbuf;
FixLenMem dabuff::mPkgBuff;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// this function is just used to append a udp package to 
// the UDP MTU set as 1460, for ethernet.
// this len is just only package len; don't include the type and msglen tag.
// we set it default to 1440; can't be bigger than 1470
int dabuff::m_mtusize=1440;


dabuff * dabfchain::GetDabuff()
{
	dabuff * retbuf;
	if (phead==NULL) {
		return NULL;
	}
	mCritLock.Lock(); 
	retbuf=phead;
	if (phead !=NULL) {
		if (phead==pend) {
			phead=pend=NULL;
		}else {
			phead=phead->pnext;
		}
		mbuffnum--;
	}
	mCritLock.Unlock(); 
	return retbuf;
}

int dabfchain::PutDabuff(dabuff *pbf, BOOL bTail)
{
	ASSERT(pbf!=NULL);
	mCritLock.Lock(); 
	if (bTail==TRUE) {
		if (pend==NULL) {
			phead=pend=pbf;
		}else{
			pend->pnext=pbf;
			pend=pbf;
		}
		pbf->pnext =NULL;
	}else {
		pbf->pnext =phead;
		if(phead ==NULL){
			phead=pend=pbf;
		}else{
			phead=pbf;
		};
	}
	mbuffnum++;
	mCritLock.Unlock();
	return 0;	
}

/*BOOL CPkgBuff::Init()
{
	if (mbuff != NULL ) {
		return FALSE;
	}
	mbuff=(char *)malloc(BF_NU * sizeof(dabuff));
	if(mbuff==NULL){
		return FALSE;
	}
	int i;
	dabuff *ptmpbuff;
//	mfreechain.dabfchain();
	for (i=0;i<BF_NU;i++){
		ptmpbuff=(dabuff * )(mbuff+i*sizeof(dabuff));
		PutVoidDaBuff(ptmpbuff);
	}
	return TRUE;
}

dabuff* CPkgBuff::GetVoidDaBuff()
{
	if (InterlockedDecrement(&mbfnu) < 0) {
		AfxMessageBox(" NO void DataBuff!");
		WaitForSingleObject(hEvent, INFINITE);
		AfxMessageBox(" Get void DataBuff!");
	};
	dabuff* ptmpbuff;
	ptmpbuff=mfreechain.GetDabuff();
	ASSERT(ptmpbuff!=NULL);
	ptmpbuff->bufend =ptmpbuff->bufbegin =0;
	ptmpbuff->pnext =NULL;
	return ptmpbuff;
}

int CPkgBuff::PutVoidDaBuff(dabuff *pdabuff)
{
	ASSERT(pdabuff!=NULL);
	mfreechain.PutDabuff(pdabuff,FALSE);
	if (InterlockedIncrement(&mbfnu) <= 0) {
		ReleaseSemaphore(hEvent, 1, NULL);
	}

	return 0;
}
*/
int dabfchain::PeekFirstPkgLen()
{
	if (phead ==NULL) {
		return -1;
	} 
	//return phead->bufend - phead->bufbegin;
	int ret;
	mCritLock.Lock();
	ret=-1;
	if (phead) {
		ret=phead->GetPkgLen(); 
	}
	mCritLock.Unlock();
	return ret;

}

BOOL dabuff::AddPkgBuff(LPCSTR pkgbuf)
{
	ASSERT(pkgbuf!=NULL);
	int pkglen;
	pkglen= *((WORD *)(pkgbuf));
//	if (pkglen<=2) return FALSE;
	ASSERT(pkglen>2);
	if (pkglen+ bufend >BF_SZ) {
		return FALSE;
	} else {
		memmove(DataBuff+bufend,pkgbuf,pkglen);
		bufend+=pkglen; 
		return TRUE;
	}
}


PkgToken::PkgToken()
{
	pkgbuff=NULL;
	pkglen=0;
	currpos=0;
}

PkgToken::~PkgToken()
{

}

BYTE PkgToken::getBYTE()
{
	BYTE ret;
	ret=*(BYTE*)(pkgbuff+currpos);
	currpos=currpos+1;
	return ret;
}

DWORD PkgToken::getDWORD()
{
	DWORD ret;
	ret=*(DWORD *)(pkgbuff+currpos);
	currpos=currpos+sizeof ret;
	return ret;
}

WORD PkgToken::getWORD()
{
	WORD ret;
	ret=*(WORD*)(pkgbuff+currpos);
	currpos=currpos+sizeof ret;
	return ret;
}

CString PkgToken::getCString()
{	
	WORD strlen;
//	char * pRawStr;
	BYTE len1,len2;
	len1=*(BYTE *)(pkgbuff+currpos);
	if (len1==255) {
		strlen=255;
		currpos++;
		len2=*(BYTE *)(pkgbuff+currpos);
		if (len2==255) {
			AfxMessageBox("RawStr is too long!");
		} else {
			strlen +=len2;
		}
	}else {
		strlen =len1;
	}
	currpos++;
	if (strlen==0) {
		CString retst;
		retst.Empty();
		return retst;
	}else {
		CString retst(pkgbuff+currpos,strlen);
		currpos+=strlen;
		return retst;
	}
}

int PkgToken::InitToGet(char *ppkg,WORD beginpos)
{
	pkglen=*(WORD *)ppkg;
	ASSERT(pkglen>2);
	pkgbuff=ppkg;
	currpos=beginpos;
	return 0;
}

int PkgToken::InitToSet(char * ppkg,WORD len)
{
	pkglen=2;
	pkgbuff=ppkg;
	currpos=2;
	if (len==0) mbufflen=BF_SZ;
	else mbufflen=len;
	return 0;
}

void PkgToken::setBYTE(BYTE val)
{
	*(BYTE*)(pkgbuff+currpos)=val;
	currpos=currpos+1;
}

void PkgToken::setWORD(WORD val)
{
	*(WORD *)(pkgbuff+currpos)=val;
	currpos=currpos+sizeof(WORD);
}

void PkgToken::setDWORD(DWORD val)
{
	*(DWORD *)(pkgbuff+currpos)=val;
	currpos=currpos+sizeof(DWORD);
}


void PkgToken::setFixStr(LPCSTR pstr, WORD len)
{

//	char * pRawStr;
	if (len==0) {
		*(BYTE *)(pkgbuff+currpos)=0;
		currpos++;
	}else {
		WORD actlen=0;
		if (len<255) {
			*(BYTE *)(pkgbuff+currpos)=len &0xff;
			actlen+=len;
		}else {
			*(BYTE *)(pkgbuff+currpos)=255;
			currpos+=1;
			len-=255;
			actlen=255;
			if (len<255) {
				*(BYTE *)(pkgbuff+currpos)=len & 0xff;
				actlen+=len;
			}else {
				actlen+=255;
				*(BYTE *)(pkgbuff+currpos)=255;
			}
		}
		currpos++;
		memcpy(pkgbuff+currpos,pstr,actlen);
		currpos+=actlen;
	}
	return;
}

void PkgToken::setVarStr(LPCTSTR pstr)
{
	int len;
	len=strlen(pstr);
	setFixStr(pstr,len);
}

char * PkgToken::finalizeSet()
{
	*(WORD*)pkgbuff=currpos;
	return pkgbuff;
}

void PkgToken::setFixData(LPCSTR pstr , WORD len)
{
	memcpy(pkgbuff+currpos,pstr,len);
	currpos+=len;
	return ;
}

void PkgToken::getFixData(char *pstr, WORD len)
{
	memcpy(pstr,pkgbuff+currpos,len);
	currpos+=len;
	return ;

}

void PkgToken::setDouble(double val)
{
	*(double*)(pkgbuff+currpos)=val;
	currpos=currpos+sizeof(double);
}

double PkgToken::getDouble()
{
	double ret;
	ret=*(double*)(pkgbuff+currpos);
	currpos=currpos+sizeof ret;
	return ret;
}


long PkgToken::getlong()
{
	long ret;
	ret=*(long*)(pkgbuff+currpos);
	currpos=currpos+sizeof (long);
	return ret;
}

void PkgToken::setlong(long val)
{
	*(long *)(pkgbuff+currpos)=val;
	currpos=currpos+sizeof(long);
}

/***********************************
if this package has some incompleted 
data, move it to the leftmost.
return is the number of byte for this 
package.
************************************/
int dabuff::TrimPkgBuff()
{ 
	if (bufbegin==0) return bufend;
	int buflen;
	buflen=bufend-bufbegin;
	if (buflen>0) memmove(DataBuff,DataBuff+bufbegin,buflen); 
	bufbegin=0;
	bufend=buflen;
	return buflen;
}

char * PkgToken::setmsgpkg(char *pkgbuff, WORD pkgtype, WORD pkgsubtype, WORD pkgval, LPCSTR pkgmsg)
{
	ASSERT( pkgbuff !=NULL);
	InitToSet(pkgbuff);
	setWORD(pkgtype);
	setWORD(pkgsubtype);
	setWORD(pkgval);
	setVarStr(pkgmsg);
	return (finalizeSet());
}

int dabuff::InitBuff(int blockbufnum)
{
	return (mPkgBuff.init(sizeof(dabuff),sizeof(dabuff)*blockbufnum));

}

dabuff* dabuff::GetVoidDaBuff()
{
	dabuff* ptmpbuff;
	ptmpbuff=(dabuff*)mPkgBuff.GetVoidBuff();
	ASSERT(ptmpbuff!=NULL);
	ptmpbuff->bufend =ptmpbuff->bufbegin =0;
	ptmpbuff->pnext =NULL;
	return ptmpbuff;
}

int dabuff::PutVoidDaBuff(dabuff *pdabuff)
{
	mPkgBuff.PutVoidBuff((char *)pdabuff);
	return 0;
}



char* PkgToken::getString(char *ptr, int len)
{
	WORD strlen;
//	char * pRawStr;
	BYTE len1,len2;
	len1=*(BYTE *)(pkgbuff+currpos);
	if (len1==255) {
		strlen=255;
		currpos++;
		len2=*(BYTE *)(pkgbuff+currpos);
		if (len2==255) {
			AfxMessageBox("RawStr is too long!");
		} else {
			strlen +=len2;
		}
	}else {
		strlen =len1;
	}
	currpos++;
	if (len>0){
		if (strlen>=len){
			memcpy(ptr,pkgbuff+currpos,len);
			ptr[len-1]=0;
		}else {
			if (strlen>0) {
				memcpy(ptr,pkgbuff+currpos,strlen);
				ptr[strlen]=0;
			}else {
				ptr[0]=0;
			}
		}
	}
	currpos+=strlen;
	return ptr;
}


void PkgToken::setPrice(double *lastprice, double newprice)
{
	long diff;
	double ddiff;
	if ((newprice<0.000001)&&(newprice>-0.000001)){
		*(BYTE*)(pkgbuff+currpos)=7;
		currpos=currpos+1;
		return;
	}
	ddiff=(newprice-*lastprice) *100;
	diff=(int)ddiff;
	if (((ddiff-diff)<0.0000001)&&((ddiff-diff)>-0.0000001)){
		diff+=64;
		if ((diff<=127)&&(diff>=0)){
			*(BYTE*)(pkgbuff+currpos)=(diff&0xff)<<1;
			*lastprice=newprice;
			currpos=currpos+1;
			return;
		}
	}
	double price1;
	price1=newprice*100;
	int intprice;
	intprice=(int)price1;
	if (((price1-intprice)<0.00001)&&((price1-intprice)>-0.00001)) {
		if (intprice<0x3fff){
			*(WORD*)(pkgbuff+currpos)=(intprice<<2)+1;
			*lastprice=newprice;
			currpos+=2;
			return;
		}
	}
	*(BYTE*)(pkgbuff+currpos)=3;
	*lastprice=newprice;
	*(double*)(pkgbuff+currpos+1)=newprice;
	currpos=currpos+sizeof(double)+1;
}

void PkgToken::getPrice(double *lastprice, double *newprice)
{
	BYTE flag;
	flag=*(BYTE*)(pkgbuff+currpos);
	if (flag==7) {
		*newprice=0;
		currpos++;
		return;
	}
	if ((flag &1)==0){
		*newprice=*lastprice+((double)(flag>>1)-64)/100;
		currpos++;
	}else {
		if((flag &2)==0) {
			*newprice=((double)((*(WORD*)(pkgbuff+currpos))>>2))/100;
			currpos+=2;
		}else {
			*newprice=*(double*)(pkgbuff+currpos+1);
			currpos+=sizeof(double)+1;
		}
	}
	*lastprice=*newprice;
}

void PkgToken::setShareInt(unsigned long share)
{
	unsigned long val;
	int b100;
	if (share %100){
		val=share;
		b100=0;
	}else {
		val=share/100;
		b100=1;
	}

	if (val<0x40) {
		*(BYTE*)(pkgbuff+currpos)=(BYTE)((val<<2)+b100);
		currpos++;
		return;
	}else if (val<0x2000){
		*(WORD*)(pkgbuff+currpos)=(WORD)((val<<3)+2+b100);
		currpos+=2;
		return;
	}else if (val<0x10000000) {
		*(DWORD*)(pkgbuff+currpos)=(DWORD)((val<<4)+6+b100);
		currpos+=4;
		return;
	}else {
		*(BYTE*)(pkgbuff+currpos)=(6+8+b100);
		currpos++;
		*(DWORD*)(pkgbuff+currpos)=val;
		currpos+=4;
		return;
	}
}

unsigned long PkgToken::getShareInt()
{
	unsigned char flag;
	unsigned ret;
	flag=*(BYTE*)(pkgbuff+currpos);
	int b100;
	b100=flag & 1;
	if ((flag &2)==0){
		ret=(flag>>2);
		currpos+=1;
	}else if ((flag&4)==0) {
		ret=(*(WORD*)(pkgbuff+currpos))>>3;
		currpos+=2;
	}else if ((flag & 8)==0){
		ret=(*(DWORD*)(pkgbuff+currpos))>>4;
		currpos+=4;
	}else {
		ret=(*(DWORD*)(pkgbuff+currpos+1));
		currpos+=5;
	}
	if (b100) {
		ret*=100;
	}
	return ret;
}

void PkgToken::setLongDiff(long nbase, long nval)
{
	long diff;
	long ndiff;
	diff=nval-nbase;
	ndiff=diff+64;
	if ((ndiff<128)&&(ndiff>0)) {
		*(BYTE*)(pkgbuff+currpos)=(BYTE)((ndiff<<1));
		currpos++;
		return;
	}
	ndiff=diff+0x1ff;
	if ((ndiff<0x3ff)&&(ndiff>0)){
		*(WORD*)(pkgbuff+currpos)=(WORD)((ndiff<<2)+1);
		currpos+=2;
		return;
	}
	*(BYTE*)(pkgbuff+currpos)=3;
	*(long*)(pkgbuff+currpos+1)=nval;
	currpos+=5;
}

long PkgToken::getLongDiff(long nbase)
{
	BYTE flag;
	long ret;
	flag=*(BYTE*)(pkgbuff+currpos);
	if ((flag &1)==0){
		ret=nbase+(long)(flag>>1)-64;
		currpos++;
	}else {
		if((flag &2)==0) {
			ret=nbase+((long)((*(WORD*)(pkgbuff+currpos))>>2))-0x1ff;
			currpos+=2;
		}else {
			ret=*(long*)(pkgbuff+currpos+1);
			currpos+=5;
		}
	}
	return ret;
}

/*int dabfchain::GetBuffCount()
{
	int ret=0;
	dabuff * pbuf;
	mCritLock.Lock(); 
	pbuf=phead;
	while(pbuf){
		ret++;
		pbuf=pbuf->pnext; 
	}
	mCritLock.Unlock(); 
	return ret;
}
*/

void dabuff::GetMemUsage(int &ntotal, int &nvoid)
{
	ntotal=mPkgBuff.CountTotalSize();
	nvoid=mPkgBuff.CountVoidSize(); 
}


BOOL dabfchain::AddPkgBuff(LPCSTR pkgbuff)
{
	mCritLock.Lock();
	if (pend!=NULL) {
		if (pend->AddPkgBuff(pkgbuff)==TRUE) {
		}else {
			dabuff *pdabuff;
			pdabuff=dabuff::GetVoidDaBuff();
			VERIFY(pdabuff->AddPkgBuff(pkgbuff));
			pend->pnext=pdabuff;
			pend=pdabuff;
			mbuffnum++; 
		}
	}else {
		dabuff *pdabuff;
		pdabuff=dabuff::GetVoidDaBuff();
		VERIFY(pdabuff->AddPkgBuff(pkgbuff));
		phead=pend=pdabuff;
		mbuffnum++; 
	}
	mCritLock.Unlock();
	return TRUE;
}

BOOL dabfchain::FillPkg(dabuff *pkg)
{
	ASSERT (pkg!=NULL);
	mCritLock.Lock();
	dabuff * ptmpdabuff;
	int waitbflen;
	while (phead) {// if phead==null don't do anything;
		waitbflen=phead->GetPkgLen();
		if ((pkg->GetPkgLen()+waitbflen)<=BF_SZ) {
			pkg->TrimPkgBuff();

			ptmpdabuff=phead;
			if (phead==pend) {
				phead=pend=NULL;
			}else {
				phead=phead->pnext;
			}
			mbuffnum--;

			ASSERT(ptmpdabuff);
			memcpy (pkg->DataBuff + pkg->bufend,ptmpdabuff->DataBuff + ptmpdabuff->bufbegin, waitbflen);
			pkg->bufend+=(WORD)waitbflen;
			dabuff::PutVoidDaBuff(ptmpdabuff); 
		}else {
			break;
		}
	}
	mCritLock.Unlock();
	return TRUE;
	
}
//this function fill the pkg as much as we can, don't check the BF_SZ;
BOOL dabfchain::FillPkgMax(dabuff *pkg)
{
	ASSERT (pkg!=NULL);
	if (pkg->bufbegin!=0){ //Trim package;	pkg->TrimPkgBuff(), imbed to improve speed;
		int buflen=pkg->bufend-pkg->bufbegin;
		if (buflen>0) memmove(pkg->DataBuff,pkg->DataBuff+pkg->bufbegin,buflen); 
		pkg->bufbegin=0;
		pkg->bufend=buflen;
	}

	mCritLock.Lock();
	dabuff * ptmpdabuff;
	while (phead) {// if phead==null don't do anything;
		int left=pkg->GetLeftBFLen();
		int waitbflen=phead->GetPkgLen();
		ptmpdabuff=phead;
		if (left>=waitbflen) {
			if (phead==pend) {
				phead=pend=NULL;
			}else {
				phead=phead->pnext;
			}
			mbuffnum--;
			memcpy (pkg->DataBuff + pkg->bufend,ptmpdabuff->DataBuff + ptmpdabuff->bufbegin, waitbflen);
			pkg->bufend+=(WORD)waitbflen;
			dabuff::PutVoidDaBuff(ptmpdabuff); 
			if (left==waitbflen) break;
		}else {
			memcpy (pkg->DataBuff + pkg->bufend,ptmpdabuff->DataBuff + ptmpdabuff->bufbegin, left);
			pkg->bufend+=left;
			ptmpdabuff->bufbegin+=left;
			break;
		}
	}
	mCritLock.Unlock();
	return TRUE;

}


void PkgToken::setLongDiff1(long nbase, long nval)
{
	long diff;
	long ndiff;
	diff=nval-nbase;
	ndiff=diff+0x40;
	if ((ndiff<0x80)&&(ndiff>0)) {
		*(BYTE*)(pkgbuff+currpos)=(BYTE)((ndiff<<1));
		currpos++;
		return;
	}
	ndiff=diff+0x2000;
	if ((ndiff<0x4000)&&(ndiff>0)){
		*(WORD*)(pkgbuff+currpos)=(WORD)((ndiff<<2)+1);
		currpos+=2;
		return;
	}
	ndiff=diff+0x10000000;
	if ((ndiff<0x20000000)&&(ndiff>0)) {
		*(DWORD*)(pkgbuff+currpos)=(DWORD)((ndiff<<3)+3);
		currpos+=4;
		return;
	}

	*(BYTE*)(pkgbuff+currpos)=7;
	*(long*)(pkgbuff+currpos+1)=nval;
	currpos+=5;

}

long PkgToken::getLongDiff1(long nbase)
{
	BYTE flag;
	long ret;
	flag=*(BYTE*)(pkgbuff+currpos);
	if ((flag &1)==0){
		ret=nbase+(long)(flag>>1)-64;
		currpos++;
	}else if((flag &2)==0) {
		ret=nbase+((long)((*(WORD*)(pkgbuff+currpos))>>2))-0x2000;
		currpos+=2;
	}else if ((flag&4)==0){
		ret=nbase+((long)((*(DWORD*)(pkgbuff+currpos))>>3))-0x10000000;
		currpos+=4;
	}else {
		ret=*(long*)(pkgbuff+currpos+1);
		currpos+=5;
	}
	return ret;
}

char * dabuff::GetPkgBuff()
{ 
	int buflen;
	buflen=bufend-bufbegin;
	if (buflen<2) {
		return NULL;
	}else{
		int pkglen;
		char * retbuf;
		retbuf=DataBuff+bufbegin;
		pkglen=*((WORD*)retbuf);
		if (pkglen<2) pkglen=2;
		if (pkglen>BF_SZ) {// this should not happen;
			bufbegin=bufend;
			return NULL;
		}
		if (pkglen>buflen) return NULL;
		ASSERT(pkglen+bufbegin<=BF_SZ);
		bufbegin+=pkglen;
		return retbuf;
	}
}

BOOL dabuff::GetPkgBuff(char **pbuff)
{
	int buflen;
	buflen=bufend-bufbegin;
	if (buflen<2) {
		*pbuff=NULL;
		return TRUE;
	}else{
		int pkglen;
		char * retbuf;
		retbuf=DataBuff+bufbegin;
		pkglen=*((WORD*)retbuf);
		if (pkglen<=2) {
			*pbuff=NULL;
			return FALSE;
		}
		if (pkglen>BF_SZ) {// this should not happen;
			bufbegin=bufend;
			*pbuff=NULL;
			return FALSE;
		}
		if (pkglen>buflen) {
			*pbuff=NULL;
			return TRUE;
		}
		ASSERT(pkglen+bufbegin<=BF_SZ);
		bufbegin+=pkglen;
		*pbuff=retbuf;
		return TRUE;
	}
}

BOOL dabuff::CheckPkgIntegrate()
{
	int pkglen;
	pkglen=*(WORD*)DataBuff;
	if (bufbegin!=0) return FALSE;
	if (pkglen!=bufend) return FALSE;
	if (pkglen>BF_SZ) return FALSE;
	return TRUE;
}

void PkgToken::setCpDouble(double val)
{
	BYTE flag;
	float f1;
	f1=(float)val;
	double diff;
	diff=(f1-val);
//	if ((diff<0.00001)&&(diff>-0.00001)){
	if (diff==0){
		flag=2;
		*(BYTE*)(pkgbuff+currpos)=flag;
		*(float*)(pkgbuff+currpos+1)=f1;
		currpos+=5;
	}else {
		flag=3;
		*(BYTE*)(pkgbuff+currpos)=flag;
		*(double*)(pkgbuff+currpos+1)=f1;
		currpos+=9;
	}
}

double PkgToken::getCpDouble()
{
	double ret;
	BYTE flag;
	flag=*(BYTE*)(pkgbuff+currpos);
	switch(flag&0x3) {
	case 0:
	case 1:
		ret=0;
		break;
	case 2:
		ret=*(float*)(pkgbuff+currpos+1);
		currpos=currpos+sizeof(float) +1;
		break;
	case 3:
		ret=*(double*)(pkgbuff+currpos+1);
		currpos=currpos+sizeof(double) +1;
		break;
	}
	return ret;
}

BOOL dabuff::AddUdpPkg(WORD type, LPCSTR pbuf, int len)
{
//	ASSERT(pkgbuf!=NULL);
//	int pkglen;
//	pkglen= *((WORD *)(pkgbuf));
//	if (pkglen<=2) return FALSE;
//	ASSERT(pkglen>2);

	if ((len+bufend+4) >dabuff::m_mtusize) {
		return FALSE;
	} else {
		WORD head[2];
		head[0]=len+4;
		head[1]=type;
		memcpy(DataBuff+bufend,&head,4);	
		memmove(DataBuff+bufend+4,pbuf,len);
		bufend+=(len+4); 
		return TRUE;
	}
}

BOOL dabfchain::AddUdpPkg(WORD type, LPCSTR pbuf, int len)
{
	mCritLock.Lock();
	if (pend!=NULL) {
		if (pend->AddUdpPkg(type,pbuf,len)==TRUE) {
		}else {
			dabuff *pdabuff;
			pdabuff=dabuff::GetVoidDaBuff();
			VERIFY(pdabuff->AddUdpPkg(type,pbuf,len));
			pend->pnext=pdabuff;
			pend=pdabuff;
			mbuffnum++; 
		}
	}else {
		dabuff *pdabuff;
		pdabuff=dabuff::GetVoidDaBuff();
		VERIFY(pdabuff->AddUdpPkg(type,pbuf,len));
		phead=pend=pdabuff;
		mbuffnum++; 
	}
	mCritLock.Unlock();
	return TRUE;

}

int dabuff::AddRawPkg(const char *pkg, DWORD len)
{
	ASSERT(pkg!=NULL);
	int pkglen;
	pkglen= len;
	if (pkglen+ bufend >BF_SZ) {
		return FALSE;
	} else {
		memmove(DataBuff+bufend,pkg,pkglen);
		bufend+=pkglen; 
		return TRUE;
	}	
}

BOOL dabfchain::AddRawPkg(const char *pkg, DWORD len)
{
	mCritLock.Lock();
	if (pend!=NULL) {
		if (pend->AddRawPkg(pkg,len)==TRUE) {
		}else {
			dabuff *pdabuff;
			pdabuff=dabuff::GetVoidDaBuff();
			VERIFY(pdabuff->AddRawPkg(pkg,len));
			pend->pnext=pdabuff;
			pend=pdabuff;
			mbuffnum++; 
		}
	}else {
		dabuff *pdabuff;
		pdabuff=dabuff::GetVoidDaBuff();
		VERIFY(pdabuff->AddRawPkg(pkg,len));
		phead=pend=pdabuff;
		mbuffnum++; 
	}
	mCritLock.Unlock();
	return TRUE;
}


int dabfchain::ConsolidatePackage()
{
	//we need to finish this code.
	return 0;
}

int  PkgToken::IncrCounter(int pos)
{
	int val=*(int*)(pkgbuff+pos);
	val++;
	*(int*)(pkgbuff+pos)=val;
	return val;
}

BOOL dabfchain::FillPkgByIP(dabuff *pkg)
{
	ASSERT (pkg!=NULL);
	mCritLock.Lock();
	dabuff * ptmpdabuff;
	int waitbflen;
	dabuff *p1,*p2;
	p1=phead;
	p2=NULL;

	while (p1) {// if phead==null don't do anything;
		if ((p1->mdestIp==pkg->mdestIp)&&(p1->mdestPort==pkg->mdestPort)) {
			waitbflen=p1->GetPkgLen();
			if ((pkg->GetPkgLen()+waitbflen)<=pkg->m_mtusize) {
				
				ptmpdabuff=p1;
				if (p2==NULL) {
					if (phead==pend) {
						phead=pend=NULL;
					}else {
						phead=phead->pnext;
					}
					p1=phead;
				}else {
					p1=p1->pnext;
					p2->pnext=p1; 
					if (p1==NULL) pend=p2;
				}
				mbuffnum--;

				ASSERT(ptmpdabuff);
				pkg->TrimPkgBuff();
				memcpy (pkg->DataBuff + pkg->bufend,ptmpdabuff->DataBuff + ptmpdabuff->bufbegin, waitbflen);
				pkg->bufend+=(WORD)waitbflen;
				dabuff::PutVoidDaBuff(ptmpdabuff); 
			}else {
				break;
			}
		}else {
			p2=p1;
			p1=p1->pnext;
		}
		
	}
	mCritLock.Unlock();
	return TRUE;
	

}

