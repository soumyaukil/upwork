// DirWatcher.cpp: implementation of the DirWatcher class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirWatcher.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
HANDLE DirWatcher::m_hCompPort=NULL;

DirWatcher::DirWatcher()
{
	mhDir=INVALID_HANDLE_VALUE;
	mbuflen=0;
}

DirWatcher::~DirWatcher()
{
	StopWatch();
}

BOOL DirWatcher::AddWatch(LPCSTR pdir)
{
	if (mhDir!=INVALID_HANDLE_VALUE) {
		CloseHandle(mhDir);
	}
	mhDir = CreateFile(
	  pdir,                            // pointer to the file name
	  FILE_LIST_DIRECTORY,                // access (read/write) mode
	  FILE_SHARE_READ|FILE_SHARE_WRITE,//|FILE_SHARE_DELETE,  // share mode
	  NULL,                               // security descriptor
	  OPEN_EXISTING,                      // how to create
	  FILE_FLAG_BACKUP_SEMANTICS|FILE_FLAG_OVERLAPPED,         // file attributes
	  NULL                                // file with attributes to copy
	);
	if (mhDir!=INVALID_HANDLE_VALUE) {
		CreateIoCompletionPort(mhDir,m_hCompPort,(DWORD)this,1); 
		mDirPath=pdir;
		return TRUE;
	}else {
		return FALSE;
	}
}

BOOL DirWatcher::StopWatch()
{
	if (mhDir!=INVALID_HANDLE_VALUE) {
		CloseHandle(mhDir);
		mhDir=NULL;
	}
	return TRUE;
}

BOOL DirWatcher::Watch()
{
	BOOL ret;
	DWORD returned;
	returned=0;
	memset(&movlap,0,sizeof(movlap));
	memset(mbuffer,0,sizeof(mbuffer));
//	movlap.hEvent=this; 
	ret=ReadDirectoryChangesW(mhDir,mbuffer,sizeof(mbuffer),FALSE,
		FILE_NOTIFY_CHANGE_CREATION|FILE_NOTIFY_CHANGE_LAST_WRITE
		|FILE_NOTIFY_CHANGE_FILE_NAME,		
		&returned,
		&movlap,
		NULL);
	return ret;

}

VOID DirWatcher::CallBack( DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped)
{
	if (mhDir==NULL) return;

	mbuflen=dwNumberOfBytesTransfered;
	FILE_NOTIFY_INFORMATION *pfni;
	pfni=(FILE_NOTIFY_INFORMATION*)&(mbuffer);
//	char st[100];
//	sprintf(st,"Flag:%d",pfni->Action);
//	AfxMessageBox(st);
	CString str;
	WCHAR wcFileName[ MAX_PATH + 1] = {0};//L"";
	while(pfni) {
		memcpy(	wcFileName, 
		pfni->FileName , 
		//min( MAX_PATH, m_pCurrentRecord->FileNameLength) <-- buggy line
		min( (MAX_PATH * sizeof(WCHAR)), pfni->FileNameLength));
		str=CString( wcFileName );

		switch(pfni->Action) {
		case FILE_ACTION_MODIFIED:
			OnModifyFile(str);
			break;
		case FILE_ACTION_ADDED:
			OnAddFile(str);
			break;
		case FILE_ACTION_REMOVED:
			OnRmoveFile(str);
			break;
		}
		if (pfni->NextEntryOffset!=0) {
			pfni+=pfni->NextEntryOffset;
		}else {
			pfni=NULL;
		}
	} 
	Watch();
}


UINT DirWatcher::WorkProc(LPVOID pParam)
{

    DWORD numBytes;
	DWORD pkey;
    LPOVERLAPPED lpOverlapped;
	DirWatcher *pdw;
    while(m_hCompPort!=NULL) {
        if( !GetQueuedCompletionStatus(m_hCompPort,
                                   &numBytes,
                                   &pkey,//<-- completion Key
                                   &lpOverlapped,
                                   INFINITE) ){
			AfxMessageBox("Wrong in get queued");
			continue;
		}
		if (pkey==NULL) break;
		pdw=(DirWatcher*)pkey;
		pdw->CallBack(0,numBytes,lpOverlapped); 
	}
	return 0;
}

BOOL DirWatcher::InitCompPort()
{
	if (m_hCompPort!=NULL) {
		return FALSE;
	};
	m_hCompPort=CreateIoCompletionPort(INVALID_HANDLE_VALUE,NULL,NULL,1);
	if (m_hCompPort==NULL) {
		return FALSE;
	};
	AfxBeginThread(WorkProc,0);
	return TRUE;
}

BOOL DirWatcher::ClearnCompPort()
{
	if (m_hCompPort!=NULL) {
		PostQueuedCompletionStatus(m_hCompPort, NULL, NULL, NULL);
		SleepEx(1000,TRUE);
		CloseHandle(m_hCompPort);
		m_hCompPort=NULL;
	}
	return TRUE;
}

BOOL DirWatcher::OnAddFile(LPCSTR pfile)
{
	return TRUE;
}

BOOL DirWatcher::OnModifyFile(LPCSTR pfile)
{
	return TRUE;
}

BOOL DirWatcher::OnRmoveFile(LPCSTR pfile)
{
	return TRUE;
}
