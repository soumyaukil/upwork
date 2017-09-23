//--NOTICE: NOTICE: NOTICE: NOTICE:
//
//This modual is Copying from Jun Liu's previous work of Shengjing Tek Inc.
//This modual is Written by Jun Liu and coworkers at Shengjing Tek Inc.
//This modual can't be copied or viewed or modify other than Jun Liu.
//


/*ʢ��ӡ����ģ�鶨�壺FixLenMem.h********************
ģ�����ƣ��ڴ����ģ��
ģ�鹦�ܣ������ڴ������ͷ�.����windows�ڴ������
���:����,����,
����:����,����,������,֣����
��д����:1998��7��3��
����:����
��������:1998��7��15��
**************************************************/

#include <thread>
#include <atomic>
#if !defined(FIXLENMEM_DEFINE)
#define FIXLENMEM_DEFINE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*ʢ��ӡ�����ඨ�壺simplelock********************
�����ƣ�simplelock,������
�๦�ܣ��ṩ������Դ������
���:����,����,
����:����,����,������,֣����
��д����:1998��7��3��
����:����
��������:1998��7��15��
**************************************************/
class simplelock
{
	std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
	simplelock();
	void Unlock();
	void Lock();
};

/*ʢ��ӡ�����ඨ�壺FixLenMem**********************
�����ƣ�FixLenMem,�̶������ڴ����
�๦�ܣ��ṩ�����ڴ����,����windows�ڴ������
���:����,����,
����:����,����,������,֣����
��д����:1998��7��3��
����:����
��������:1998��7��15��
**************************************************/

class FixLenMem  
{
private:
	simplelock mThreadLock; //������Դ��.
	int mBlockSize;	 // �ڴ���С
	int mMemLen;	 // ÿ���С
	char *mpVoidMem; // ��������ڴ���п�ָ��.
	char *mpbuff;	 // �ڴ滺��
public:
	inline int GetBlockSize() { return mBlockSize;};
	void FreeAllMem();
	bool Combine(FixLenMem *pfmem);
	int  GetMemLen() { return mMemLen;};
	int CountTotalSize();
	int CountVoidSize();
	void PutVoidBuff(char* pbuff);
	char * GetVoidBuff();
	int init(int MemLen ,int BlockSize);

	FixLenMem();
	FixLenMem(int MemLen, int BlockSize);
	virtual ~FixLenMem();

private:
	int AddBuff();
};

#endif 
