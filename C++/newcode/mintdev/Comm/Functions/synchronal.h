// synchronal.h: interface for the APICritical class.
//
//////////////////////////////////////////////////////////////////////

#include <mutex>

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
	std::mutex m_sect;
public:
	APICritical();
	virtual ~APICritical();
	
#if( _WIN32_WINNT >= 0x0400)
	inline BOOL TryLock() { return m_sect.try_lock();};
#endif

	inline void Unlock() { m_sect.unlock();};

	inline void Lock(){	m_sect.lock();};

};

#endif // !defined(AFX_SYNCHRONAL_H__E840D861_E319_11D4_BD28_0050041B8FEC__INCLUDED_)
