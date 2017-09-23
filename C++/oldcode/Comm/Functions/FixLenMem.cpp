/*盛京印鉴：模块定义：FixLenMem.cpp********************
模块名称：内存管理模块
模块功能：快速内存分配和释放.补充windows内存管理功能
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月3日
调试:李旭功
调试日期:1998年7月15日
**************************************************/

#include "FixLenMem.h"
#include <iostream>
#include <cstdlib>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// 构造函数/析构函数
//////////////////////////////////////////////////////////////////////

FixLenMem::FixLenMem()
{
//	mThreadLock=1;
	mBlockSize=0;
	mMemLen=0;
	mpbuff=NULL;
	mpVoidMem=NULL;
}
FixLenMem::FixLenMem(int MemLen, int BlockSize)
{
//	mThreadLock=1;
	mBlockSize=0;
	mMemLen=0;
	mpbuff=NULL;
	mpVoidMem=NULL;
	init(MemLen,BlockSize);
}

FixLenMem::~FixLenMem()
{
	FreeAllMem(); 
}


int FixLenMem::init(int MemLen, int BlockSize)
{
//	ASSERT(mpbuff==NULL);
//	ASSERT(MemLen>4);
//	ASSERT(BlockSize>1020);
	//按照4个字节对齐,内存存取更快;
	int sfix;
	sfix=MemLen % 4;
	mMemLen=MemLen;
	if (sfix!=0) {
		mMemLen+=(4-sfix);
	}
	mBlockSize=BlockSize;
	mThreadLock.Lock();
	AddBuff();
	mThreadLock.Unlock();
	return 0;
}

int FixLenMem::AddBuff()
{
	char * pbuff;
	int i;
	pbuff=(char*)malloc(mBlockSize);
	if (pbuff==NULL){
//		AfxMessageBox("Out of Memory in Get FixLenMem");
		return -1;
	}
	*((char **)pbuff)=mpbuff;
	mpbuff=pbuff;
	char * ptmp;
	for (i=sizeof(char*);i<=(mBlockSize-mMemLen-4);i+=mMemLen) {
		ptmp=pbuff+i;
		*((char**)ptmp)=mpVoidMem;
		mpVoidMem=ptmp;
	}
	return 0;
}

char * FixLenMem::GetVoidBuff()
{
//	ASSERT (mpbuff!=NULL);
	mThreadLock.Lock();
	char* ptmpbloc = NULL ;
	if (mpVoidMem==NULL){
		if (AddBuff()!=0) {
			mThreadLock.Unlock();
			return NULL; 
		}
	}
	if (mpVoidMem)
	{
		ptmpbloc = mpVoidMem;
		mpVoidMem = *((char**)(ptmpbloc));
	}
	mThreadLock.Unlock();
	return (ptmpbloc);
}



void FixLenMem::PutVoidBuff(char *pbuff)
{
//	ASSERT (mpbuff!=NULL);
	mThreadLock.Lock();
	*((char**)pbuff)=mpVoidMem;
	mpVoidMem=pbuff;
	mThreadLock.Unlock();
}


void simplelock::Lock()
{
	while (InterlockedDecrement(&mlockval) < 0) {
		InterlockedIncrement(&mlockval);
		Sleep(0);
	}
}

void simplelock::Unlock()
{
	InterlockedIncrement(&mlockval);
}

simplelock::simplelock()
{
	mlockval =1;
}


int FixLenMem::CountTotalSize()
{
	int blocknu=0;
	mThreadLock.Lock();
	char *pbuff;
	pbuff=mpbuff;
	while(pbuff!=NULL){
		blocknu++;
		pbuff=*(char**)(pbuff);
	}
	mThreadLock.Unlock();
	return blocknu*mBlockSize;

}

int FixLenMem::CountVoidSize()
{
	int voidnu=0;
	mThreadLock.Lock();

	char* ptmpbloc;
	ptmpbloc=mpVoidMem;
	while(ptmpbloc!=NULL) {
		voidnu++;
		ptmpbloc=*((char**)(ptmpbloc));
	}
	
	mThreadLock.Unlock();
	return voidnu*mMemLen;
}

BOOL FixLenMem::Combine(FixLenMem *pfmem)
{

	if ((pfmem->mBlockSize!=mBlockSize)||
		(pfmem->mMemLen!=mMemLen)) return FALSE;
	mThreadLock.Lock();
	if (mpbuff==NULL) {
		mpbuff=pfmem->mpbuff;
	}else {
		char *pbuff;
		pbuff=mpbuff;
		while(*(char**)(pbuff))pbuff=*((char**)pbuff);
		*(char**)(pbuff)=pfmem->mpbuff;
	}
	pfmem->mpbuff=NULL;

	if (mpVoidMem==NULL) {
		mpVoidMem=pfmem->mpVoidMem; 
	}else {
		char* ptmpbloc;
		ptmpbloc=mpVoidMem;
		while(*((char**)(ptmpbloc))) ptmpbloc=*((char**)(ptmpbloc));
		*((char**)(ptmpbloc))=pfmem->mpVoidMem;
	}
	pfmem->mpVoidMem=NULL;

	mThreadLock.Unlock();
	return TRUE;
}

void FixLenMem::FreeAllMem()
{
	mThreadLock.Lock(); 
	char *pbuff;
	while(mpbuff!=NULL){
		pbuff=mpbuff;
		mpbuff=*(char**)(pbuff);
		free(pbuff);
	}
	mpVoidMem=NULL;
	mThreadLock.Unlock();
}
