#include "windows.h"

#include "GMap.h"
#include "FixLenMem.h"

//#pragma init_seg(lib)

static FixLenMem *pGmapIntBuff=NULL;

//////////////////////////////////////////////////////////////////////
// 
//////////////////////////////////////////////////////////////////////




GMap::GMap()
{
	InitializeCriticalSection(&m_sect);
	hashsize=0;
	phead=NULL;
	phashtable=NULL;
}

GMap::GMap(int size)
{
	InitializeCriticalSection(&m_sect);
	hashsize=0;
	phead=0;
	phashtable=NULL;
	init(size);
}

GMap::~GMap()
{
	RemoveAll();
	LockMap();
	if (hashsize!=0){
		if(phashtable!=NULL)
			free(phashtable);
		hashsize=0;
		phashtable=NULL;
	}
	UnlockMap(); 
	DeleteCriticalSection(&m_sect);

}

void GMap::init(int size)
{
	LockMap();
	if (size>0) {
		if (hashsize!=0){
			::MessageBox(NULL,"GMap Init Error!",NULL,0);
		}
		phashtable=(gmapitem**)malloc(size*(sizeof(GMap*)));
		hashsize=size;
	}
	unsigned int i;
	for(i=0;i<hashsize;i++) {
		phashtable[i]=NULL;
	}
	UnlockMap();
}

gmapitem *GMap::Add(void* key, void* val)
{
	LockMap();
	gmapitem *pitem;
	pitem=AllocItem();
	pitem->key=key;
	pitem->val=val;
	if (hashsize==0){
		pitem->pnext=phead;
		phead=pitem;
	} else {
		unsigned int offset;
		offset=Hash(key)%hashsize;
		pitem->pnext=phashtable[offset];
		phashtable[offset]=pitem;
	};
	UnlockMap();
	return pitem;
}


int GMap::Remove(void* key)
{
	int ret=0;
	LockMap(); 
	gmapitem *pitem;
	gmapitem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				FreeItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}

	}else {
		unsigned int offset;
		offset=Hash(key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				FreeItem(pitem);
				ret++;
				break;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	UnlockMap();
	return ret;
}

gmapitem * GMap::Enum(gmapitem *&pitem)
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
				offset=(Hash(pitem->key)%hashsize)+1;
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


gmapitem * GMap::EnumFirst()
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



gmapitem * GMap::EnumNext(gmapitem *pitem)
{
	if (hashsize==0) {
		return pitem->pnext;
	}else {
		if (pitem->pnext !=NULL) {
			return pitem->pnext;
		}
		unsigned int offset=Hash(pitem->key)%hashsize;
		offset++;
		for (;offset<hashsize;offset++) {
			if (phashtable[offset]!=NULL) return phashtable[offset];
		}
		return NULL;
	}
}

gmapitem * GMap::AddNoDup(void* key, void* val)
{
	LockMap();
	gmapitem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0){
				pitem->key=key;
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL) {
			pitem=AllocItem();
			pitem->key=key;
			pitem->val=val;
			pitem->pnext=phead;
			phead=pitem;
		}
	} else {
		unsigned int offset;
		offset=Hash(key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0){
				pitem->key=key;
				pitem->val=val;
				break;
			}
			pitem=pitem->pnext;
		}
		if (pitem==NULL){
			pitem=AllocItem();
			pitem->key=key;
			pitem->val=val;
			pitem->pnext=phashtable[offset];
			phashtable[offset]=pitem;
		}
	};
	UnlockMap();
	return pitem;
}

gmapitem * GMap::FindItem(void* key)
{
	LockMap();
	gmapitem *pitem;
	if (hashsize==0){
		pitem=phead;
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0){
				break;
			}
			pitem=pitem->pnext;
		}
	} else {
		unsigned int offset;
		offset=Hash(key)%hashsize;
		pitem=phashtable[offset];
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0){
				break;
			}
			pitem=pitem->pnext;
		}
	};
	UnlockMap();

	return pitem;

}


void GMap::EnumFirst(gmapitem **ppitem)
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

void GMap::EnumNext(gmapitem **ppitem)
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

int GMap::RemoveAllDup(void * key)
{
	LockMap();
	int ret=0;
	gmapitem *pitem;
	gmapitem *p2;
	gmapitem *p;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0) {
				if (pitem==phead) {
					p2=phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				FreeItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}else {
		unsigned int offset;
		offset=Hash(key)%hashsize;
		pitem=p2=phashtable[offset];
		while(pitem!=NULL) {
			if (Compare(pitem->key,key)==0) {
				if (pitem==phashtable[offset]) {
					p2=phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				p=pitem;
				pitem=pitem->pnext;
				FreeItem(p);
				ret++;
				continue;
			}
			p2=pitem;
			pitem=pitem->pnext;
		}
	}
	UnlockMap();
	return ret;
}

int GMap::RemoveVal(void * key, void * val)
{
	int ret=0;
	LockMap();
	gmapitem *pitem;
	gmapitem *p2;
	if (hashsize==0){
		pitem=p2=phead;
		while(pitem!=NULL) {
			if ((Compare(pitem->key,key)==0)&&(pitem->val==val)) {
				if (pitem==phead) {
					phead=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				FreeItem(pitem);
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
			if ((Compare(pitem->key,key)==0)&&(pitem->val==val)) {
				if (pitem==phashtable[offset]) {
					phashtable[offset]=pitem->pnext;
				}else {
					p2->pnext=pitem->pnext;
				}
				FreeItem(pitem);
				ret++;
				pitem=p2->pnext;
			}else {
				p2=pitem;
				pitem=pitem->pnext;
			}
		}
	}
	UnlockMap();
	return ret;
}

void GMap::EnumFirstKey(void * key, gmapitem **ppitem)
{
	gmapitem *p=NULL;
	if (hashsize==0) {
		p=phead; 
	}else {
		p=phashtable[Hash(key)%hashsize];
	}
	while(p!=NULL) {
		if (Compare(p->key,key)==0) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
	return;
}

void GMap::EnumNextKey(void * key, gmapitem **ppitem)
{
	gmapitem *p;
	p=(*ppitem)->pnext;
	while(p!=NULL) {
		if (Compare(p->key,key)==0) {
			*ppitem=p;
			return;
		}
		p=p->pnext;
	}
	*ppitem=NULL;
}

gmapitem *GMap::AddAscent(void * key, void * val)
{
	LockMap();
	gmapitem *p1,*p2,*pitem;
	pitem=AllocItem();
	pitem->key=key;
	pitem->val=val;
	p1=p2=phead;
	while(p1!=NULL) {
		if (Compare(p1->key,key)>=0){
			break;
		}
		p2=p1;
		p1=p1->pnext;
	}
	pitem->pnext=p1;
	if (p1==phead) {
		phead=pitem;
	}else {
		p2->pnext=pitem;
	}
	UnlockMap();
	return pitem;

}





int GMap::RemoveAll()
{
	LockMap();
	int ret=0;
	gmapitem *pitem;
	gmapitem *p2;
	unsigned int i;
	if (hashsize!=0){
		for (i=0;i<hashsize;i++) {
			if (phashtable[i]!=NULL) {
				pitem=phashtable[i];
				while (pitem!=NULL) {
					p2=pitem;
					pitem=pitem->pnext;
					ret++;
					FreeItem(p2);
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
				FreeItem(p2);
			}
			phead=NULL;
		}
	}
	UnlockMap();
	return ret;
}

gmapitem * GMap::AllocItem()
{
	if (pGmapIntBuff==NULL) pGmapIntBuff=new FixLenMem(sizeof(gmapitem),102300);
	return (gmapitem *)pGmapIntBuff->GetVoidBuff();
}

void GMap::FreeItem(gmapitem * pitem)
{
	pGmapIntBuff->PutVoidBuff((char*)pitem);
}

unsigned int GMap::Hash(void *pkey)
{
	return (unsigned int )pkey;
};

int GMap::Compare(void *pkey1,void *pkey2)
{
	return (int)pkey1-(int)pkey2;
};


unsigned int GMap_str::Hash(void *pkey)
{
	unsigned int nHash = 0;
	char *key=(char*)pkey;
	while (*key)
		nHash = (nHash<<5) + nHash + *key++;
	return nHash;
};

int GMap_str::Compare(void *pkey1,void *pkey2)
{
	return strcmp((char*)pkey1,(char*)pkey2);
};

