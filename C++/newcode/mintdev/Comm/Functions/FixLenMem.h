//--NOTICE: NOTICE: NOTICE: NOTICE:
//
//This modual is Copying from Jun Liu's previous work of Shengjing Tek Inc.
//This modual is Written by Jun Liu and coworkers at Shengjing Tek Inc.
//This modual can't be copied or viewed or modify other than Jun Liu.
//


/*盛京印鉴：模块定义：FixLenMem.h********************
模块名称：内存管理模块
模块功能：快速内存分配和释放.补充windows内存管理功能
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月3日
调试:李旭功
调试日期:1998年7月15日
**************************************************/

#include <thread>
#include <atomic>
#if !defined(FIXLENMEM_DEFINE)
#define FIXLENMEM_DEFINE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*盛京印鉴：类定义：simplelock********************
类名称：simplelock,简单锁类
类功能：提供快速资源共享锁
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月3日
调试:李旭功
调试日期:1998年7月15日
**************************************************/
class simplelock
{
	std::atomic_flag locked = ATOMIC_FLAG_INIT;
public:
	simplelock();
	void Unlock();
	void Lock();
};

/*盛京印鉴：类定义：FixLenMem**********************
类名称：FixLenMem,固定长度内存管理
类功能：提供快速内存分配,补充windows内存管理功能
设计:刘军,李旭功,
作者:刘军,李旭功,焦钟鸣,郑联合
编写日期:1998年7月3日
调试:李旭功
调试日期:1998年7月15日
**************************************************/

class FixLenMem  
{
private:
	simplelock mThreadLock; //共享资源锁.
	int mBlockSize;	 // 内存块大小
	int mMemLen;	 // 每块大小
	char *mpVoidMem; // 欲分配的内存空闲块指针.
	char *mpbuff;	 // 内存缓冲
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
