// synchronal.h: interface for the APICritical class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNCHRONAL_H__E840D861_E319_11D4_BD28_0050041B8FEC__INCLUDED_)
#define AFX_SYNCHRONAL_H__E840D861_E319_11D4_BD28_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//this critical section is just warp the 
//			normal Windows CriticalSection.
class APICritical  
{
private:
	CRITICAL_SECTION m_sect;
public:
	APICritical();
	virtual ~APICritical();
	
#if( _WIN32_WINNT >= 0x0400)
	inline BOOL TryLock() { return TryEnterCriticalSection(&m_sect);};
#endif

	inline void Unlock() {LeaveCriticalSection(&m_sect);};

	inline void Lock(){	EnterCriticalSection(&m_sect);};
	inline LONG GetRecursionCount(){return m_sect.RecursionCount;};

};

#endif // !defined(AFX_SYNCHRONAL_H__E840D861_E319_11D4_BD28_0050041B8FEC__INCLUDED_)
