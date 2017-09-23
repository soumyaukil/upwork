//--NOTICE: NOTICE: NOTICE: NOTICE:
//
//This modual is Copying from Jun Liu's previous work of Shengjing Tek Inc.
//This modual is Written by Jun Liu and coworkers at Shengjing Tek Inc.
//This modual can't be copied or viewed or modify other than Jun Liu.
//

/*ʢ��ӡ����ģ�鶨�壺intitem.h********************
ģ�����ƣ���ϣ���ѯģ��
ģ�鹦�ܣ����ٱ��ѯ.���ù�ϣ�㷨.ʵ���������ַ�����
			���ٲ�ѯ
���:����
����:����,����,
��д����:1998��7��20��
����:����,������,֣����
��������:1998��7��25��
**************************************************/

#if !defined(INTMAP_DEF)
#define INTMAP_DEF

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FixLenMem.h"
#include "afxmt.h"
#define StrMapMaxStrLen 20

/*ʢ��ӡ�����ඨ�壺intitem********************
�����ƣ�intitem,������ϣ���Ԫ��
�๦�ܣ���ϣ��Ԫ
���:����,����,
����:����,����,������,֣����
��д����:1998��7��20��
����:����
��������:1998��7��25��
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

/*ʢ��ӡ�����ඨ�壺intlst********************
�����ƣ�intlst,�����б�
�๦�ܣ��������й���.
���:����,����,
����:����,����,������,֣����
��д����:1998��7��20��
����:����
��������:1998��7��25��
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

/*ʢ��ӡ�����ඨ�壺intmap********************
�����ƣ�intmap,������ϣ��
�๦�ܣ��ṩ����������Ϊ��ֵ�Ĺ�ϣ���ѯ.
���:����,����,
����:����,����,������,֣����
��д����:1998��7��20��
����:����
��������:1998��7��25��
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

	//����һ�������ظ��Ĺ�ϣ��Ԫ
	intitem *Add(long key, long val);
	//����һ�����ظ��Ĺ�ϣ��Ԫ
	intitem *AddNoDup(long key, long val);
	//ɾ����һ��ָ����ֵ�Ĺ�ϣ��Ԫ.
	int Remove(long key);
	//ɾ�������ƶ���ֵ�Ĺ�ϣ��Ԫ;
	int RemoveAllDup(long key);
	//ɾ���ƶ���ֵ��Ԫ��ֵ�Ĺ�ϣ��Ԫ
	int RemoveVal(long key, long val);
	//������ϣ��Ĵ�СΪ1ʱ(�б�),����
	//��ֵ��С˳������.
	intitem *AddAscent(long key,long val);

	void init(int size);
	intmap();
	virtual ~intmap();
};

/*ʢ��ӡ�����ඨ�壺stritem********************
�����ƣ�stritem,�ַ�����ϣ���Ԫ��
�๦�ܣ���ϣ��Ԫ
���:����,����,
����:����,����
��д����:1998��7��20��
����:����
��������:1998��7��25��
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

/*ʢ��ӡ�����ඨ�壺strmap********************
�����ƣ�strmap,�ַ�����ϣ��
�๦�ܣ��ṩ�������ַ���Ϊ��ֵ�Ĺ�ϣ���ѯ.
���:����,����,
����:����,����
��д����:1998��7��20��
����:����
��������:1998��7��25��
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
