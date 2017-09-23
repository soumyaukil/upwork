// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__567B97C3_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_)
#define AFX_LOG_H__567B97C3_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxmt.h>
#include "LogFrame.h"
#include "synchronal.h"

typedef void (*Fn_CB_AddLog)(const char *plogstr);


class CTxtLog  
{
public:
	BOOL Flush(BOOL bCommit);
	void UpdateLog_Edit(CEdit *ped);
	int SetLogWindow(CWnd *pwnd);
	void ShowLogFile();
	int AddLogStr(LPCSTR plogstr);
	int AddLogStrFmt(LPCSTR szFmt, ...);
	//if ntime==0; get the computer time;
	//this funtion add a log with time;
	int AddLogStrFmtTm(long ntime,LPCSTR szFmt, ...);

	BOOL CloseLogFile();
	BOOL OpenLogFile(LPCSTR logfilename);
	int ShowLogWindow(CMDIFrameWnd *pFrame,long nID);
	CTxtLog();
	virtual ~CTxtLog();

	friend CLogFrame;

	BOOL  m_realtimeflush;//do realtime flush;
	Fn_CB_AddLog m_addlog_callback;
	
protected:
	CWnd	  * mpFrame;
	APICritical mCritical; //the lock for this log


	HANDLE m_hfp;
	char *m_pbuff;
	int   m_buffsize;//how big this buffer is,
	int   m_buffbegin_file; //the place for write file
	int   m_buffbegin_viewer;//the place for viewer;
	int   m_buffend;
	
	BOOL  m_DirtyFlag;
	BOOL  m_Viewer_Message_In_Queue;//the Viewer need to take the message;

	char  mlogfilename[300];

};

class CPkgLog
{
	APICritical mCritical;
	
	HANDLE m_hfp;
	char *m_pbuff;
	int   m_buffsize;
	int   m_buffbegin;
	int   m_buffend;
	BOOL  m_DirtyFlag;

	char  mlogfilename[300];

public:
	//AddLog, pbuff don't include header;
	BOOL AddLog(WORD len, WORD type, char *pbuff);
	// return 1: sucessful
	// return 0: end of file.
	// return -1:Incomplete record;
	// return -2:Calcuation error;
	int GetRecord(WORD &len, WORD &type,char* & pbuff, unsigned int* pCRC32 = NULL);

	CPkgLog();
	~CPkgLog();
	BOOL Flush(BOOL bCommit);
	BOOL OpenLogFile(LPCSTR logfilename,int nbuffersize,BOOL bReadonly);

	
private:
	int Read_Data();
};
#endif // !defined(AFX_LOG_H__567B97C3_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_)
