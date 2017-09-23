// DirWatcher.h: interface for the DirWatcher class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRWATCHER_H__DA40C917_7A21_11D6_BE35_0050041B8FEC__INCLUDED_)
#define AFX_DIRWATCHER_H__DA40C917_7A21_11D6_BE35_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class DirWatcher  
{
	HANDLE mhDir;
	char mbuffer[2048];
	int mbuflen;
	static HANDLE m_hCompPort;
	static UINT WorkProc(LPVOID pParam);
	OVERLAPPED movlap;
	virtual VOID CallBack( DWORD dwErrorCode,DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped  );
public:
	static BOOL ClearnCompPort();
	static BOOL InitCompPort();
public:
	virtual BOOL OnRmoveFile(LPCSTR pfile);
	virtual BOOL OnModifyFile(LPCSTR pfile);
	virtual BOOL OnAddFile(LPCSTR pfile);
	
	BOOL Watch();
	BOOL StopWatch();
	BOOL AddWatch(LPCSTR pdir);
	CString mDirPath;
	DirWatcher();
	virtual ~DirWatcher();
};

#endif // !defined(AFX_DIRWATCHER_H__DA40C917_7A21_11D6_BE35_0050041B8FEC__INCLUDED_)
