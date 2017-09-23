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

class CStreamLog  
{
public:
	int Flush();
	int AddLogStr(LPCSTR plogstr);
	int AddLogStrFmt(LPCSTR szFmt, ...);
	//if ntime==0; get the computer time;
	//this funtion add a log with time;
	int AddLogStrFmtTm(long ntime,LPCSTR szFmt, ...);

	BOOL CloseLogFile();
	BOOL OpenLogFile(LPCSTR logfilename);
	int ShowLogWindow(CMDIFrameWnd *pFrame,long nID);
	CStreamLog();
	virtual ~CStreamLog();

	friend CLogFrame;

protected:
	CEdit	  * mpEdLog;			//the edit 
	CLogFrame * mpFrame;
	CCriticalSection mCritical; //the lock for this log
	//CLogFrame * plogFrame;      //the logframe of this log object
	char      * mpbuff;			//the buff of tempdata exchange 
	long	  mbflen;
	long	  mcurrlen;
	CFile	  mLogFile;

};

#endif // !defined(AFX_LOG_H__567B97C3_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_)
