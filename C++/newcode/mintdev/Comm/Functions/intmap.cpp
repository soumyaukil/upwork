//--NOTICE: NOTICE: NOTICE: NOTICE:
//
//This modual is Copying from Jun Liu's previous work of Shengjing Tek Inc.
//This modual is Written by Jun Liu and coworkers at Shengjing Tek Inc.
//This modual can't be copied or viewed or modify other than Jun Liu.
//

/*盛京印鉴：模块定义：intmap.cpp********************
模块名称：哈希表查询模块
模块功能：快速表查询.利用哈希算法.实现整数和字符串的
			快速查询
设计:刘军
作者:刘军,李旭功,
编写日期:1998年7月20日
调试:刘军,焦钟鸣,郑联合
调试日期:1998年7月25日
**************************************************/

#include "intmap.h"
#include <cassert>
#include <cstring>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#pragma init_seg(lib)

FixLenMem intitem::itembuff(sizeof(intitem),102300);
FixLenMem stritem::itembuff(sizeof(stritem),102300);

//////////////////////////////////////////////////////////////////////
// 构造/析构函数
//////////////////////////////////////////////////////////////////////
intitem *intitem::GetVoidBuff()
{
	return (intitem*)itembuff.GetVoidBuff();
}

void intitem::PutVoidItem(intitem *pitem)
{
	itembuff.PutVoidBuff((char*)pitem);
}


intmap::intmap()
{
	hashsize=0;
	phead=NULL;
	phashtable=NULL;
}

intmap::~intmap()
{
	RemoveAll();
	mlock.lock();
	if (hashsize!=0){
		if(phashtable!=NULL)
			free(phashtable);
		hashsize=0;
		phashtable=NULL;
	}
	mlock.unlock(); 
}

void intmap::init(int size)
{
	mlock.lock();
	assert(size>=1);
	assert(phashtable==NULL);
	phashtable=(intitem**)malloc(size*(sizeof(intmap*)));
	hashsize=size;
	assert(phashtable!=NULL);
	unsigned int i;
	for(i=0;i<hashsize;i++) {
		phashtable[i]=NULL;
	}
	mlock.unlock();
}

intitem *intmap::Add(long key, long val)
{
	mlock.lock();
	intitem *pitem;
	pitem=intitem::GetVoidBuff();
	assert(pitem!=NULL);
	pitem->key=key;
	pitem->val=val;
	if (hashsize==0){
		pitem->pnext=phead;
		phead=pitem;
	} else {
		unsigned int offset;
		offset=(unsigned long)key%hashsize;
		pitem->pnext=phashtable[offset];
		phashtable[offset]=pitem;
	};
	mlock.unlock();
	return pitem;
}


int intmap::Remove(long key)
{
	int ret=0;
	mlock.lock();
	intitem *pitem;
	intitem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (pitem->key==key) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				intitem::PutVoidItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}

	}else {
		unsigned int offset;
		offset=(unsigned long)key%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (pitem->key==key) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				intitem::PutVoidItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	return ret;
}



intitem * intmap::EnumFirst()
{
	if (hashsize==0) {
		return phead;
	}else {
		unsigned int i;
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) return phashtable[i];
		}
		return NULL;
	}
}



intitem * intmap::EnumNext(intitem *pitem)
{
	if (hashsize==0) {
		return pitem->pnext;
	}else {
		if (pitem->pnext !=NULL) {
			return pitem->pnext;
		}
		unsigned int offset=((unsigned long)pitem->key)%hashsize;
		offset++;
		for (;offset<hashsize;offset++) {
			if (phashtable[offset]!=NULL) return phashtable[offset];
		}
		return NULL;
	}
}

intitem * intmap::AddNoDup(long key, long val)
{
	mlock.lock();
	intitem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (pitem->key==key){
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL) {
			pitem=intitem::GetVoidBuff();
			pitem->key=key;
			pitem->val=val;
			assert(pitem!=NULL);
			pitem->pnext=phead;
			phead=pitem;
		}
	} else {
		unsigned int offset;
		offset=((unsigned long)key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (pitem->key==key){
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL){
			pitem=intitem::GetVoidBuff();
			pitem->key=key;
			pitem->val=val;
			assert(pitem!=NULL);
			pitem->pnext=phashtable[offset];
			phashtable[offset]=pitem;
		}
	};
	mlock.unlock();
	return pitem;
}

intitem * intmap::FindItem(long key)
{
	mlock.lock();
	intitem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (pitem->key==key){
				break;
			}
			pitem=pitem->pnext;
		}
	} else {
		unsigned int offset;
		offset=((unsigned long)key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (pitem->key==key){
				break;
			}
			pitem=pitem->pnext;
		}
	};
	mlock.unlock();

	return pitem;

}

intmap::intmap(int size)
{
	hashsize=0;
	phead=0;
	phashtable=NULL;
	init(size);
}

void intmap::EnumFirst(intitem **ppitem)
{
	if (hashsize==0) {
		*ppitem=phead; 
		return;
	}else {
		unsigned int i;
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) {
				*ppitem=phashtable[i];
				return;
			}
		}
		*ppitem=NULL;
		return;
	}

}

void intmap::EnumNext(intitem **ppitem)
{
	if (hashsize==0) {
		*ppitem=(*ppitem)->pnext;
		return ;
	}else {
		if ((*ppitem)->pnext !=NULL) {
			*ppitem=(*ppitem)->pnext;
			return;
		}
		unsigned int offset=((unsigned long)(*ppitem)->key)%hashsize;
		offset++;
		for (;offset<hashsize;offset++) {
			if (phashtable[offset]!=NULL) {
				*ppitem= phashtable[offset];
				return;
			}
		}
		*ppitem=NULL;
		return ;
	}
}

intitem * intmap::Enum(intitem *&pitem)
{
	if (hashsize==0) {
		if (pitem==NULL) pitem=phead; 
		else pitem=pitem->pnext;
	}else {
		unsigned int offset;
		if (pitem==NULL) {
			for (offset=0;offset<hashsize;offset++) {
				if (phashtable[offset]!=NULL) {
					pitem=phashtable[offset];
					break;
				}
			}
		}else {
			if (pitem->pnext !=NULL) {
				pitem=pitem->pnext;
			}else {
				offset=(((unsigned long)pitem->key)%hashsize)+1;
				pitem=NULL;
				for (;offset<hashsize;offset++) {
					if (phashtable[offset]!=NULL) {
						pitem= phashtable[offset];
						break;
					}
				}
			}
		}
	}
	return pitem;
}

intitem * intmap::EnumKey(long key, intitem *&pitem)
{
	if (pitem==NULL) {
		if (hashsize==0) {
			pitem=phead; 
		}else {
			pitem=phashtable[((unsigned long)key)%hashsize];
		}
		while(pitem) {
			if (pitem->key==key) {
				break;
			}
			pitem=pitem->pnext;
		}
	}else {
		pitem=pitem->pnext;
		while(pitem) {	
			if (pitem->key==key) break;
			pitem=pitem->pnext;
		};
	}
	return pitem;
}



int intmap::RemoveAllDup(long key)
{
	mlock.lock();
	int ret=0;
	intitem *pitem;
	intitem *p2;
	intitem *p;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (pitem->key==key) {
				if (pitem==phead) {
					p2=phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				intitem::PutVoidItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}else {
		unsigned int offset;
		offset=((unsigned long)key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (pitem->key==key) {
				if (pitem==phashtable[offset]) {
					p2=phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				intitem::PutVoidItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	return ret;
}

int intmap::RemoveVal(long key, long val)
{
	int ret=0;
	mlock.lock();
	intitem *pitem;
	intitem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if ((pitem->key==key)&&(pitem->val==val)) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				intitem::PutVoidItem(pitem);
				ret++;
				pitem=p2->pnext;
			}else {
				p2=pitem;
				pitem=pitem->pnext;
			}
		}

	}else {
		unsigned int offset;
		offset=((long unsigned)key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if ((pitem->key==key)&&(pitem->val==val)) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				intitem::PutVoidItem(pitem);
				ret++;
				pitem=p2->pnext;
			}else {
				p2=pitem;
				pitem=pitem->pnext;
			}
		}
	}
	mlock.unlock();
	return ret;
}

void intmap::EnumFirstKey(long key, intitem **ppitem)
{
	intitem *p=NULL;
	if (hashsize==0) {
		p=phead; 
	}else {
		p=phashtable[((unsigned long)key)%hashsize];
	}
	while(p!=NULL) {
		if (p->key==key) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
	return;
}

void intmap::EnumNextKey(long key, intitem **ppitem)
{
	intitem *p;
	p=(*ppitem)->pnext;
	while(p!=NULL) {
		if (p->key==key) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
}

intitem *intmap::AddAscent(long key, long val)
{
	assert (hashsize==0);
	mlock.lock();
	intitem *p1,*p2,*pitem;
	pitem=intitem::GetVoidBuff();
	assert(pitem!=NULL);
	pitem->key=key;
	pitem->val=val;
	p1=p2=phead;
	while(p1!=NULL) {
		if (p1->key>=key){
			break;
		}
		p2=p1;
		p1=p1->pnext;
	}
	pitem->pnext=p1;
	if (p1==phead) {
		phead=pitem;
	}else {
		assert(p2!=NULL);
		p2->pnext=pitem;
	}
	mlock.unlock();
	return pitem;

}


stritem* stritem::GetVoidBuff()
{
	return (stritem*)itembuff.GetVoidBuff();
}

void stritem::PutVoidItem(stritem *pitem)
{
	itembuff.PutVoidBuff((char*)pitem);
}


strmap::strmap()
{
	hashsize=0;
	phead=0;
	phashtable=NULL;
}

strmap::~strmap()
{
	RemoveAll();
	mlock.lock();
	if (hashsize!=0){
		if(phashtable!=NULL)
			free(phashtable);
		hashsize=0;
		phashtable=NULL;
	}
	mlock.unlock(); 
}

void strmap::init(int size)
{
	mlock.lock();
	assert(size>=1);
	assert(phashtable==NULL);
	phashtable=(stritem**)malloc(size*(sizeof(strmap*)));
	hashsize=size;
	assert(phashtable!=NULL);
	unsigned int i;
	for(i=0;i<hashsize;i++) {
		phashtable[i]=NULL;
	}
	mlock.unlock();
}

stritem *strmap::Add(const char* key, long val)
{
	mlock.lock();
	stritem *pitem;
	pitem=stritem::GetVoidBuff();
	assert(pitem!=NULL);
	
	memccpy(pitem->key,key,0,StrMapMaxStrLen);
	pitem->val=val;
	if (hashsize==0){
		pitem->pnext=phead;
		phead=pitem;
	} else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem->pnext=phashtable[offset];
		phashtable[offset]=pitem;
	};
	mlock.unlock();
	return pitem;
}


int strmap::Remove(const char* key)
{
	int ret=0;
	mlock.lock();
	stritem *pitem;
	stritem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				stritem::PutVoidItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}

	}else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				stritem::PutVoidItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	return ret;
}


void strmap::LockMap()
{
	mlock.lock();
}

void strmap::UnlockMap()
{
	mlock.unlock();
}

stritem * strmap::EnumFirst()
{
	if (hashsize==0) {
		return phead;
	}else {
		unsigned int i;
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) return phashtable[i];
		}
		return NULL;
	}
}



stritem * strmap::EnumNext(stritem *pitem)
{
	if (hashsize==0) {
		return pitem->pnext;
	}else {
		if (pitem->pnext !=NULL) {
			return pitem->pnext;
		}
		unsigned int offset=HashKey(pitem->key)%hashsize;
		offset++;
		for (;offset<hashsize;offset++) {
			if (phashtable[offset]!=NULL) return phashtable[offset];
		}
		return NULL;
	}
}

stritem * strmap::AddNoDup(const char* key, long val)
{
	mlock.lock();
	stritem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0){
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL) {
			pitem=stritem::GetVoidBuff();
			memccpy(pitem->key,key,0,StrMapMaxStrLen);
			pitem->val=val;
			assert(pitem!=NULL);
			pitem->pnext=phead;
			phead=pitem;
		}
	} else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0){
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL){
			pitem=stritem::GetVoidBuff();
			memccpy(pitem->key,key,0,sizeof(pitem->key));
			pitem->val=val;
			assert(pitem!=NULL);
			pitem->pnext=phashtable[offset];
			phashtable[offset]=pitem;
		}
	};
	mlock.unlock();
	return pitem;
}

stritem * strmap::FindItem(const char* key)
{
	mlock.lock();
	stritem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0){
				break;
			}
			pitem=pitem->pnext;
		}
	} else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0){
				break;
			}
			pitem=pitem->pnext;
		}
	};
	mlock.unlock();

	return pitem;

}

strmap::strmap(int size)
{
	hashsize=0;
	phead=0;
	phashtable=NULL;
	init(size);
}

void strmap::EnumFirst(stritem **ppitem)
{
	if (hashsize==0) {
		*ppitem=phead; 
		return;
	}else {
		unsigned int i;
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) {
				*ppitem=phashtable[i];
				return;
			}
		}
		*ppitem=NULL;
		return;
	}

}

void strmap::EnumNext(stritem **ppitem)
{
	if (hashsize==0) {
		*ppitem=(*ppitem)->pnext;
		return ;
	}else {
		if ((*ppitem)->pnext !=NULL) {
			*ppitem=(*ppitem)->pnext;
			return;
		}
		unsigned int offset=HashKey((*ppitem)->key)%hashsize;
		offset++;
		for (;offset<hashsize;offset++) {
			if (phashtable[offset]!=NULL) {
				*ppitem= phashtable[offset];
				return;
			}
		}
		*ppitem=NULL;
		return ;
	}
}


stritem* strmap::Enum(stritem *&pitem)
{
	if (hashsize==0) {
		if (pitem==NULL) pitem=phead;
		else pitem=pitem->pnext;
	}else {
		unsigned int offset;
		if (pitem==NULL) {
			for (offset=0;offset<hashsize;offset++) {
				if (phashtable[offset]!=NULL) {
					pitem=phashtable[offset];
					break;;
				}
			}
		}else {
			if (pitem->pnext!=NULL) {
				pitem=pitem->pnext;
			}else {
				offset=(HashKey(pitem->key)%hashsize) +1;
				pitem=NULL;
				for (;offset<hashsize;offset++) {
					if (phashtable[offset]!=NULL) {
						pitem= phashtable[offset];
						break;
					}
				}
			}
		}
	}
	return pitem;
}
stritem* strmap::EnumKey(const char* key,stritem *&pitem)
{
	if (pitem==NULL) {
		if (hashsize==0) {
			pitem=phead; 
		}else {
			pitem=phashtable[HashKey(key)%hashsize];
		}
		while(pitem) {
			if (strcmp(pitem->key,key)==0) {
				break;
			}
			pitem=pitem->pnext;
		}
	}else {
		pitem=pitem->pnext;
		while(pitem){
			if (strcmp(pitem->key,key)==0) break;
			pitem=pitem->pnext;
		};
	}
	return pitem;
}

int strmap::RemoveAllDup(const char* key)
{
	mlock.lock();
	int ret=0;
	stritem *pitem;
	stritem *p2;
	stritem *p;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0) {
				if (pitem==phead) {
					p2=phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				stritem::PutVoidItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (strcmp(pitem->key,key)==0) {
				if (pitem==phashtable[offset]) {
					p2=phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				stritem::PutVoidItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	return ret;
}

int strmap::RemoveVal(const char* key, long val)
{
	int ret=0;
	mlock.lock();
	stritem *pitem;
	stritem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if ((strcmp(pitem->key,key)==0)&&(pitem->val==val)) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				stritem::PutVoidItem(pitem);
				ret++;
				pitem=p2->pnext;
			}else {
				p2=pitem;
				pitem=pitem->pnext;
			}
		}

	}else {
		unsigned int offset;
		offset=HashKey(key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if ((strcmp(pitem->key,key)==0)&&(pitem->val==val)) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				stritem::PutVoidItem(pitem);
				ret++;
				pitem=p2->pnext;
			}else {
				p2=pitem;
				pitem=pitem->pnext;
			}
		}
	}
	mlock.unlock();
	return ret;
}

void strmap::EnumFirstKey(const char* key, stritem **ppitem)
{
	stritem *p=NULL;
	if (hashsize==0) {
		p=phead; 
	}else {
		p=phashtable[HashKey(key)%hashsize];
	}
	while(p!=NULL) {
		if (strcmp(p->key,key)==0) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
	return;
}

void strmap::EnumNextKey(const char* key, stritem **ppitem)
{
	stritem *p;
	p=(*ppitem)->pnext;
	while(p!=NULL) {
		if (strcmp(p->key,key)==0) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
}

stritem *strmap::AddAscent(const char* key, long val)
{
	assert (hashsize==0);
	mlock.lock();
	stritem *p1,*p2,*pitem;
	pitem=stritem::GetVoidBuff();
	assert(pitem!=NULL);
	memccpy(pitem->key,key,0,sizeof(pitem->key));
	pitem->val=val;
	p1=p2=phead;
	while(p1!=NULL) {
		if (strcmp(pitem->key,key)>=0){
			break;
		}
		p2=p1;
		p1=p1->pnext;
	}
	pitem=p1;
	if (p2==NULL) {
		phead=pitem;
	}else {
		p2->pnext=pitem;
	}
	mlock.unlock();
	return pitem;

}

uint32_t strmap::HashKey(const char* key)
{
	uint32_t nHash = 0;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
}

int intmap::RemoveAll()
{
	mlock.lock();
	int ret=0;
	intitem *pitem;
	intitem *p2;
	unsigned int i;
	if (hashsize!=0){
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) {
				pitem=phashtable[i];
				while (pitem!=NULL) {
					p2=pitem;
					pitem=pitem->pnext;
					ret++;
					intitem::PutVoidItem(p2);
				}
				phashtable[i]=NULL;
			}
		}
	}else {
		if (phead !=NULL) {
			pitem=phead;
			while(pitem!=NULL) {
				p2=pitem;
				pitem=pitem->pnext;
				ret++;
				intitem::PutVoidItem(p2);
			}
			phead=NULL;
		}
	}
	mlock.unlock();
	return ret;
}

int strmap::RemoveAll()
{
	mlock.lock();
	int ret=0;
	stritem *pitem;
	stritem *p2;
	unsigned int i;
	if (hashsize!=0){
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) {
				pitem=phashtable[i];
				while (pitem!=NULL) {
					p2=pitem;
					pitem=pitem->pnext;
					ret++;
					stritem::PutVoidItem(p2);
				}
				phashtable[i]=NULL;
			}
		}
	}else {
		if (phead !=NULL) {
			pitem=phead;
			while(pitem!=NULL) {
				p2=pitem;
				pitem=pitem->pnext;
				ret++;
				stritem::PutVoidItem(p2);
			}
			phead=NULL;
		}
	}
	mlock.unlock(); 
	return ret;
}

intlst::intlst()
{
	phead=NULL;
	ptail=NULL;
}

intlst::~intlst()
{
	RemoveAll();
}


void intlst::RemoveAll()
{
	intitem *pitem;
	intitem *p2;
	mlock.lock();
	if (phead !=NULL) {
		pitem=phead;
		while(pitem!=NULL) {
			p2=pitem;
			pitem=pitem->pnext;
			intitem::PutVoidItem(p2);
		}
		phead=NULL;
		ptail=NULL;
	}
	mlock.unlock();
}

intitem *intlst::AddHead(int key, int val)
{
	intitem *pitem;
	pitem=intitem::GetVoidBuff();
	assert(pitem!=NULL);
	mlock.lock();
	pitem->key=key;
	pitem->val=val;
	pitem->pprev=NULL;
	if (phead!=NULL) {
		pitem->pnext=phead;
		phead->pprev=pitem;
	}else {
		pitem->pnext=NULL;
		ptail=pitem;
	}
	phead=pitem;
	mlock.unlock();
	return pitem;
}

intitem *intlst::AddTail(int key, int val)
{
	intitem *pitem;
	pitem=intitem::GetVoidBuff();
	assert(pitem!=NULL);
	pitem->key=key;
	pitem->val=val;
	pitem->pnext=NULL;
	mlock.lock();
	if (ptail!=NULL) {
		pitem->pprev=ptail;
		ptail->pnext=pitem;
	}else {
		pitem->pprev=NULL; 
		phead=pitem;
	}
	ptail=pitem;
	mlock.unlock();
	return pitem;
}



bool intlst::GetHead(int *pkey, int *pval)
{
	intitem *pitem;
	bool ret;
	mlock.lock();
	if (phead!=NULL){
		pitem=phead;
		*pkey=pitem->key;
		*pval=pitem->val;
		phead=pitem->pnext;
		if (phead==NULL) {
			ptail=NULL;
		} else {
			phead->pprev=NULL;
		}
		intitem::PutVoidItem(pitem);
		ret=true;
	}else {
		ret=false;
	}
	mlock.unlock();
	return ret;
}

bool intlst::GetTail(int *pkey, int *pval)
{
	intitem *pitem;
	bool ret;
	mlock.lock();
	if (ptail!=NULL){
		pitem=ptail;
		*pkey=pitem->key;
		*pval=pitem->val;
		ptail=pitem->pprev;
		if (ptail==NULL) {
			phead=NULL;
		} else {
			ptail->pnext=NULL;
		}
		intitem::PutVoidItem(pitem);
		ret=true;
	}else {
		ret=false;
	}
	mlock.unlock();
	return ret;

}

intitem* intlst::AddAscent(long key, long val)
{
	intitem *p1,*pitem;
	pitem=intitem::GetVoidBuff();
	assert(pitem!=NULL);
	pitem->key=key;
	pitem->val=val;
	
	mlock.lock();
	if (ptail==NULL) {
		ptail=phead=pitem;
		pitem->pnext =NULL;
		pitem->pprev =NULL;
	}else {
		p1=ptail;
		while(p1!=NULL) {
			if (p1->key<=key){
				break;
			}
			p1=p1->pprev;
		}
		if (p1==NULL){
			pitem->pprev=NULL;
			pitem->pnext=phead;
			phead->pprev=pitem;
			phead=pitem;
		}else {
			pitem->pnext=p1->pnext ;
			pitem->pprev=p1;
			if (p1->pnext==NULL) {
				ptail=pitem;
			}else {
				p1->pnext->pprev=pitem;
			}
			p1->pnext=pitem;
		}
	}
	mlock.unlock(); 
	return pitem;
}

int intlst::RemoveVal(long val)
{
	int ret=0;
	mlock.lock();
	intitem *pitem;
	intitem *p2;
	pitem=phead;
	while(pitem!=NULL) {
		if ((pitem->val==val)) {
			p2=pitem->pnext;
			if (pitem->pnext!=NULL) {
				pitem->pnext->pprev=pitem->pprev;
			}else {
				ptail=pitem->pprev;
			}
			if (pitem->pprev!=NULL) {
				pitem->pprev->pnext=pitem->pnext;
			}else {
				phead=pitem->pnext;
			}
			intitem::PutVoidItem(pitem);
			ret++;
			pitem=p2;
		}else {
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	return ret;

}

stritem * strmap::FindItemVal(const char* key, long val)
{
	LockMap();
	stritem *PSitem;
	stritem *ret;
	EnumFirstKey(key,&PSitem);
	ret=NULL;
	while(PSitem) {
		if (PSitem->val==val) {
			ret=PSitem;
			break;
		}
		EnumNextKey(key,&PSitem);
	}
	UnlockMap(); 
	return ret;
}

int intmap::GetCounter()
{
	intitem *PIitem;
	int i;
	i=0;
	LockMap();
	EnumFirst(&PIitem);
	while(PIitem) {
		i++;
		EnumNext(&PIitem);
	}
	UnlockMap();
	return i;
}

int strmap::GetCounter()
{
	stritem *PIitem;
	int i;
	i=0;
	LockMap();
	EnumFirst(&PIitem);
	while(PIitem) {
		i++;
		EnumNext(&PIitem);
	}
	UnlockMap();
	return i;
}

void intlst::RemoveItem(intitem **ppitem)
{

	mlock.lock();
	intitem *pitem;
	intitem *p2;
	pitem=*ppitem;
	if(pitem!=NULL) {
		p2=pitem->pnext;
		if (pitem->pnext!=NULL) {
			pitem->pnext->pprev=pitem->pprev;
		}else {
			ptail=pitem->pprev;
		}
		if (pitem->pprev!=NULL) {
			pitem->pprev->pnext=pitem->pnext;
		}else {
			phead=pitem->pnext;
		}
		intitem::PutVoidItem(pitem);
		*ppitem=p2;
	}
	mlock.unlock();
}

int intlst::GetCounter()
{
	intitem *PIitem;
	int i;
	i=0;
	LockLst();
	EnumFirst(&PIitem);
	while(PIitem) {
		i++;
		EnumNext(&PIitem);
	}
	UnlockLst();
	return i;
}



intitem * intlst::FindItem(long key)
{
	mlock.lock();
	intitem *pitem;
	pitem=phead;
	while(pitem!=NULL) {
		if (pitem->key==key){
			break;
		}
		pitem=pitem->pnext;
	}
	mlock.unlock();
	return pitem;
}

stritem * strmap::RemoveItem(stritem **ppitem)
{
	if (ppitem==NULL) return NULL;
	mlock.lock();
	int ret=0;
	stritem *pitem;
	stritem *p2;
	stritem *p;
	stritem *pos;
	pos=*ppitem;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (pitem==pos) {
				if (pitem==phead) {
					p2=phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				stritem::PutVoidItem(p);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}else {
		unsigned int offset;
		offset=HashKey(pos->key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (pitem==pos) {
				if (pitem==phashtable[offset]) {
					p2=phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				stritem::PutVoidItem(p);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	mlock.unlock();
	*ppitem=pitem;
	return pitem;
}

bool intlst::MergeToEnd(intlst *plst)
{
	intitem *newh;
	intitem *newt;
	
	plst->LockLst();
	newh=plst->phead;
	newt=plst->ptail;
	plst->phead=NULL;
	plst->ptail=NULL;
	plst->UnlockLst();
	if (newh==NULL) return true;

	mlock.lock();
	if (ptail==NULL) {
		phead=newh;
		ptail=newt; 
	}else {
		ptail->pnext=newh;
		newh->pprev=ptail;
		ptail=newt;
	}
	mlock.unlock();
	return true;
}


int strmap::GetMaxBucket() 
{
	int maxbkt=0;
	if (hashsize==0){
		maxbkt=GetCounter();
	}else {
		uint32_t i;
		for (i=0;i<hashsize;i++) {
			int bsz=0;
			stritem *pi=phashtable[i];
			while(pi) {
				bsz++;
				pi=pi->pnext;
			}
			if (bsz>maxbkt) maxbkt=bsz;
		}
	}
	return maxbkt;
}
