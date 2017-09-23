//--NOTICE: NOTICE: NOTICE: NOTICE:
//
//This modual is Copying from Jun Liu's previous work of Shengjing Tek Inc.
//This modual is Written by Jun Liu and coworkers at Shengjing Tek Inc.
//This modual can't be copied or viewed or modify other than Jun Liu.
//

/*盛京印鉴：模块定义：intitem.h********************
模块名称：哈希表查询模块
模块功能：快速表查询.利用哈希算法.实现整数和字符串的
			快速查询
设计:刘军
作者:刘军,李旭功,
编写日期:1998年7月20日
调试:刘军,焦钟鸣,郑联合
调试日期:1998年7月25日
**************************************************/

#if !defined(INTMAP_DEF)
#define INTMAP_DEF

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FixLenMem.h"
#include "afxmt.h"
#define StrMapMaxStrLen 20

/*盛京印鉴：类定义：intitem********************
类名称：intitem,整数哈希表的元素
类功能：哈希表元
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月20日
调试:李旭功
调试日期:1998年7月25日
**************************************************/
class intitem
{
public:
	long key;
	long val;
	intitem *pnext;
	intitem *pprev;
	static void PutVoidItem(intitem *pitem);
	static intitem* GetVoidBuff();
private:
	static FixLenMem itembuff;
};

/*盛京印鉴：类定义：intlst********************
类名称：intlst,整数列表
类功能：整数队列管理.
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月20日
调试:李旭功
调试日期:1998年7月25日
**************************************************/
class intlst 
{
private:
	CCriticalSection mlock;
	intitem *phead;
	intitem *ptail;

public:
	BOOL MergeToEnd(intlst *plst);
	intitem * FindItem(long key);
	int GetCounter();
	void RemoveItem(intitem **ppitem);
	int RemoveVal(long val);
	inline void LockLst() { mlock.Lock();};
	inline void UnlockLst(){ mlock.Unlock();};

	intitem* AddAscent(long key, long val);
	inline intitem *Enum(intitem *&ppitem) {
		ppitem=(ppitem==NULL)?phead:ppitem->pnext; 
		return ppitem;
	}
	inline void EnumFirst(intitem **ppitem){ *ppitem=phead;				};
	inline void EnumNext(intitem **ppitem) { *ppitem=(*ppitem)->pnext;	};
	inline void EnumLast(intitem **ppitem) { *ppitem=ptail;				};
	inline void EnumPrev(intitem **ppitem) { *ppitem=(*ppitem)->pprev;	};

	BOOL GetTail(int *pkey, int *pval);
	BOOL GetHead(int *pkey, int *pval);
	intitem *AddTail(int key, int val);
	intitem *AddHead(int key, int val);
	void RemoveAll();
	intlst();
	~intlst();
};

/*盛京印鉴：类定义：intmap********************
类名称：intmap,整数哈希表
类功能：提供快速以整数为键值的哈希表查询.
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月20日
调试:李旭功
调试日期:1998年7月25日
**************************************************/
class intmap  
{

private:
	CCriticalSection mlock;
	unsigned int hashsize;
	union{
		intitem **phashtable;
		intitem *phead;
	};

public:
	int GetCounter();
	inline int GetHashSize() { return hashsize;};
	int RemoveAll();
	void EnumFirst(intitem ** ppitem);
	void EnumNext(intitem **ppitem);
	intitem * EnumFirst();
	intitem * EnumNext(intitem *pitem);
	intitem * Enum(intitem * &pitem);
	intitem * EnumKey(long key, intitem * &pitem);

	void EnumFirstKey(long key, intitem **ppitem);
	void EnumNextKey(long key,intitem **ppitem);

	intmap(int size);
	intitem * FindItem ( long key );
	inline void LockMap() { mlock.Lock();};
	inline void UnlockMap(){ mlock.Unlock();};

	//加入一个可能重复的哈希表元
	intitem *Add(long key, long val);
	//加入一个无重复的哈希表元
	intitem *AddNoDup(long key, long val);
	//删除第一个指定键值的哈希表元.
	int Remove(long key);
	//删除所有制定键值的哈希表元;
	int RemoveAllDup(long key);
	//删除制定键值和元素值的哈希表元
	int RemoveVal(long key, long val);
	//仅当哈希表的大小为1时(列表),按照
	//键值大小顺序排列.
	intitem *AddAscent(long key,long val);

	void init(int size);
	intmap();
	virtual ~intmap();
};

/*盛京印鉴：类定义：stritem********************
类名称：stritem,字符串哈希表的元素
类功能：哈希表元
设计:刘军,李旭功,
作者:刘军,李旭功
编写日期:1998年7月20日
调试:李旭功
调试日期:1998年7月25日
**************************************************/
class stritem
{
public:
	char key[StrMapMaxStrLen];
	long val;
	stritem *pnext;

	static void PutVoidItem(stritem *pitem);
	static stritem* GetVoidBuff();
private:
	static FixLenMem itembuff;
};

/*盛京印鉴：类定义：strmap********************
类名称：strmap,字符串哈希表
类功能：提供快速以字符串为键值的哈希表查询.
设计:刘军,李旭功,
作者:刘军,李旭功
编写日期:1998年7月20日
调试:李旭功
调试日期:1998年7月25日
**************************************************/
class strmap{
private:
	CCriticalSection mlock;
	unsigned int hashsize;
	union{
		stritem **phashtable;
		stritem *phead;
	};
public:
	stritem * RemoveItem(stritem ** ppitem);
	int GetCounter();
	inline int GetHashSize() { return hashsize;};
	stritem *FindItemVal(LPCSTR key,long val);
	int RemoveAll();
	static DWORD HashKey(LPCTSTR key);
	stritem* Enum(stritem *&pitem);
	stritem* EnumKey(LPCSTR key,stritem *&pitem);
	void EnumFirst(stritem ** ppitem);
	void EnumNext(stritem **ppitem);
	stritem * EnumFirst();
	stritem * EnumNext(stritem *pitem);

	void EnumFirstKey(LPCSTR key, stritem **ppitem);
	void EnumNextKey(LPCSTR key,stritem **ppitem);

	stritem * FindItem ( LPCSTR key );
	void UnlockMap();
	void LockMap();
	stritem *Add(LPCSTR key, long val);
	stritem *AddNoDup(LPCSTR key, long val);
	int Remove(LPCSTR key);
	int RemoveAllDup(LPCSTR key);
	int RemoveVal(LPCSTR key, long val);
	stritem *AddAscent(LPCSTR key,long val);
	int GetMaxBucket();
	void init(int size);
	strmap(int size);
	strmap();
	virtual ~strmap();

};

#endif 
