#ifndef G_INTMAP_DEF
#define G_INTMAP_DEF

#pragma once

struct gmapitem
{
public:
	gmapitem *pnext;
	void* key;
	void* val;
};

class GMap  
{

private:
	CRITICAL_SECTION m_sect;
	unsigned int hashsize;
	union{
		gmapitem **phashtable;
		gmapitem *phead;
	};
protected:
	virtual gmapitem * AllocItem();
	virtual void FreeItem(gmapitem * pitem);
	virtual unsigned int Hash(void *pkey);
	virtual int Compare(void *pkey1,void *pkey2);
public:
	GMap();
	GMap(int size);
	void init(int size);
	virtual ~GMap();


	int GetCounter();
	int RemoveAll();
	gmapitem * Enum(gmapitem *&pitem);
	void EnumFirst(gmapitem ** ppitem);
	void EnumNext(gmapitem **ppitem);
	gmapitem * EnumFirst();
	gmapitem * EnumNext(gmapitem *pitem);

	void EnumFirstKey(void* key, gmapitem **ppitem);
	void EnumNextKey(void* key,gmapitem **ppitem);

	gmapitem * FindItem ( void* key );
	inline void LockMap() {  EnterCriticalSection(&m_sect);};
	inline void UnlockMap(){ LeaveCriticalSection(&m_sect);};

	gmapitem *Add(void* key, void* val);
	gmapitem *AddNoDup(void* key, void*val);
	int Remove(void* key);
	int RemoveAllDup(void*key);
	int RemoveVal(void* key, void*val);
	gmapitem *AddAscent(void* key,void*val);
};

class GMap_str: public GMap
{
	virtual unsigned int Hash(void *pkey);
	virtual int Compare(void *pkey1,void *pkey2);

};


#endif 
