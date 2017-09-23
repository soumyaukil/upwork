#ifndef _APICritical_H
#define _APICritical_H

#pragma once

//this critical section is just warp the 
//			normal Windows CriticalSection.
class APICritical  
{
private:
	CRITICAL_SECTION m_sect;
public:
	inline APICritical() {InitializeCriticalSection(&m_sect);}
	inline APICritical::~APICritical(){DeleteCriticalSection(&m_sect);}

#if( _WIN32_WINNT >= 0x0400)
	inline BOOL TryLock() { return TryEnterCriticalSection(&m_sect);};
#endif

	inline void Unlock() {LeaveCriticalSection(&m_sect);};

	inline void Lock(){	EnterCriticalSection(&m_sect);};
	inline LONG GetRecursionCount(){return m_sect.RecursionCount;};

};

class APILock
{
	APICritical *mpLock;
public:
	APILock(APICritical *lock) {mpLock=lock; mpLock->Lock();};
	~APILock() { mpLock->Unlock();};
};



class APIAtomic
{
	long mlockval;
public:
	inline APIAtomic() {mlockval=0;};
	inline void Unlock(){ InterlockedDecrement(&mlockval); };
	inline void Lock(){ 
		while (InterlockedIncrement(&mlockval) >1) {
			InterlockedDecrement(&mlockval);
			Sleep(0);
		}
	};
};

//maybe we should use the Assembly code to write this function;
inline void APILockVal(long *pval) {
	while (InterlockedIncrement(pval) >1) {
		InterlockedDecrement(pval);
		Sleep(0);
	}
}

//maybe we should use the Assembly code to write this function;
inline void APIUnlockVal(long *pval) { InterlockedDecrement(pval);}




#endif 


