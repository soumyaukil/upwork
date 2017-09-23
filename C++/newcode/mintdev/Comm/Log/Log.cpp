// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStreamLog::CStreamLog()
{
	mbflen=0;
	mcurrlen=0;
	mpbuff=NULL;
	mpEdLog=NULL;
	mpFrame=NULL;

}

CStreamLog::~CStreamLog()
{
	mCritical.Lock();
	if (mLogFile.m_hFile!=CFile::hFileNull) {
		mLogFile.Close();
	}

	if (mpbuff!=NULL) free (mpbuff);
	mpbuff=NULL;
	mCritical.Unlock();
}

int CStreamLog::AddLogStr(LPCSTR plogstr)
{
	mCritical.Lock();

	if (mLogFile.m_hFile!=CFile::hFileNull ) {
		mLogFile.Write(plogstr,strlen(plogstr));
		mLogFile.Write("\xd\xa",2);
	}
	if ((mpbuff!=NULL)&&(mpFrame!=NULL))	{
		int mlen;int clen;
		mlen=strlen(plogstr);
		clen=mcurrlen;
		if (mlen<(mbflen-mcurrlen-2)){
			strcpy(mpbuff+mcurrlen,plogstr);
			mcurrlen+=mlen;
			mpbuff[mcurrlen]=0xd;
			mcurrlen++;
			mpbuff[mcurrlen]=0xa;
			mcurrlen++;
			mpbuff[mcurrlen]=0;

			if (clen==0)
				mpFrame->PostMessage(WM_COMMAND,200,0);
		}
		
	}
	mCritical.Unlock();
	return 0;
}

int CStreamLog::ShowLogWindow(CMDIFrameWnd *pFrame,long nID)
{
	if (mpEdLog!=NULL) return 0;
//	CMiniFrameWnd* pFrame = STATIC_DOWNCAST(CMainFrame, pFrame);
	// create a new MDI child window
//	CLogFrame *pchild;
	mpFrame=(CLogFrame *)pFrame->CreateNewChild(RUNTIME_CLASS(CLogFrame), nID, NULL, NULL);
	ASSERT(mpFrame !=NULL);
	if (mpbuff==NULL) {
		mpbuff=(char*)malloc(102400);
		mbflen=102400;
	}
	mcurrlen=0;
	mpFrame->SetLogObject(this);
	return 0;
}

BOOL CStreamLog::OpenLogFile(LPCSTR logfilename)
{
	mCritical.Lock(); 
	if (mLogFile.m_hFile!=CFile::hFileNull){
		mLogFile.Close();
	}
	mLogFile.Open(logfilename,CFile::modeWrite |CFile::modeCreate |CFile::modeNoTruncate|CFile::shareDenyNone);
	mLogFile.SeekToEnd();
	mCritical.Unlock();
	return TRUE;
}

BOOL CStreamLog::CloseLogFile()
{
	mCritical.Lock();
	if (mLogFile.m_hFile!=CFile::hFileNull) {
		mLogFile.Close();
	}
	mCritical.Unlock();
	return TRUE;
}

int CStreamLog::AddLogStrFmt(LPCSTR szFmt, ...)
{
    CString strText;

    va_list argptr;

    va_start(argptr, szFmt);
    strText.FormatV(szFmt, argptr);
    va_end(argptr);

    AddLogStr(strText);
	return TRUE;
}

int CStreamLog::AddLogStrFmtTm(long ntime, LPCSTR szFmt, ...)
{
	time_t tTime = ntime;
	if (tTime <= 0) {
		time(&tTime);
	}
	char buff[10000];
	int len=0;
	struct tm* ptmTemp = localtime(&tTime);
	if (ptmTemp!= NULL){
		len+=_tcsftime(buff, 20, "%m/%d/%y %H:%M:%S ", ptmTemp);
	}

    va_list argptr;
    va_start(argptr, szFmt);
	_vsnprintf( buff+len, sizeof(buff)-len-1, szFmt, argptr);
    va_end(argptr);
	
    AddLogStr(buff);
	return TRUE;

}

int CStreamLog::Flush()
{
	mCritical.Lock();
	if (mLogFile.m_hFile!=CFile::hFileNull ) {
		mLogFile.Flush();
	}
	mCritical.Unlock();
	return 0;

}
