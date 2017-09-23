// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SmtLog.h"
//don't use the CRC32, some problem here.
//#include "CRC32.h"

#define CRC32_LEN sizeof(unsigned long)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTxtLog::CTxtLog()
{
	m_hfp=INVALID_HANDLE_VALUE;
	m_buffsize=1024000;
	m_buffbegin_file=0;
	m_buffbegin_viewer=0; 
	m_buffend=0;
	m_pbuff=NULL;
	m_DirtyFlag=FALSE;
	mpFrame=NULL;
	mlogfilename[0]=0;
	m_Viewer_Message_In_Queue=FALSE;
	m_realtimeflush=TRUE;
	m_addlog_callback=NULL;
}

CTxtLog::~CTxtLog()
{
	mCritical.Lock();
	Flush(TRUE);
	if (m_pbuff!=NULL){
		free(m_pbuff);
		m_pbuff=NULL;
	}
	if (m_hfp!=INVALID_HANDLE_VALUE){
		CloseHandle(m_hfp);
		m_hfp=NULL;
	}

		
	mCritical.Unlock();
}

int CTxtLog::AddLogStr(LPCSTR plogstr)
{	
	mCritical.Lock();
	if (m_addlog_callback) {
		m_addlog_callback(plogstr);
	}
	if (mpFrame ||(m_hfp!=INVALID_HANDLE_VALUE)) {
		if (m_pbuff==NULL) {
			m_pbuff=(char*)malloc(m_buffsize); 
			m_buffbegin_file=0;
			m_buffbegin_viewer=0; 
			m_buffend=0;
		}

		int len=strlen(plogstr);
		if ((m_buffend+len+4)>=m_buffsize){
			Flush(FALSE);
		}

		if ((m_buffend+len+4)<m_buffsize){
			memcpy(m_pbuff+m_buffend,plogstr,len);
			m_buffend+=len;
			m_pbuff[m_buffend]='\xd';m_buffend++;
			m_pbuff[m_buffend]='\xa';m_buffend++;
			m_DirtyFlag=TRUE;
		}
		if (mpFrame!=NULL)	{
			if (!m_Viewer_Message_In_Queue) {
				mpFrame->PostMessage(WM_COMMAND,200,0);
				m_Viewer_Message_In_Queue=TRUE;
			}
		}else {
			m_buffbegin_viewer=m_buffend; 
		}
		if (m_hfp==INVALID_HANDLE_VALUE) {
			m_buffbegin_file=m_buffend; 
		}
		if (m_realtimeflush) {
			Flush(FALSE);
		}
	}
	mCritical.Unlock();

	return 0;
}

int CTxtLog::ShowLogWindow(CMDIFrameWnd *pFrame,long nID)
{
	if (mpFrame !=NULL) return 0; 

	CLogFrame *pfrm=(CLogFrame *)pFrame->CreateNewChild(RUNTIME_CLASS(CLogFrame), nID, NULL, NULL);
	ASSERT(pfrm !=NULL);
	pfrm->SetLogObject(this);
	SetLogWindow(pfrm); 
	return 0;
}

BOOL CTxtLog::OpenLogFile(LPCSTR logfilename)
{

	BOOL bret=TRUE;
	mCritical.Lock(); 
	if (m_hfp!=INVALID_HANDLE_VALUE)
	{
		Flush(TRUE);
		CloseHandle(m_hfp);
	}

	DWORD dwDesireaccess;
	dwDesireaccess=GENERIC_READ|GENERIC_WRITE;
	m_hfp=CreateFile(logfilename,dwDesireaccess,FILE_SHARE_READ,NULL,
				OPEN_ALWAYS,             // existing file only 
                FILE_ATTRIBUTE_NORMAL,     // normal file 
                NULL);

	if (m_hfp!=INVALID_HANDLE_VALUE) {
		SetFilePointer(m_hfp,0,0,FILE_END);	
		strncpy(mlogfilename,logfilename,sizeof(mlogfilename)-1);
		mlogfilename[sizeof(mlogfilename)-1]=0;
	}else {
		bret=FALSE;
	}

	mCritical.Unlock();
	return bret;

}

BOOL CTxtLog::CloseLogFile()
{
	mCritical.Lock();
	if (m_hfp!=INVALID_HANDLE_VALUE){
		Flush(TRUE);
		CloseHandle(m_hfp);
		m_hfp = NULL;
	}
	mCritical.Unlock();
	return TRUE;
}

int CTxtLog::AddLogStrFmt(LPCSTR szFmt, ...)
{
    CString strText;

    va_list argptr;

    va_start(argptr, szFmt);
    strText.FormatV(szFmt, argptr);
    va_end(argptr);

    AddLogStr(strText);
	return TRUE;
}

int CTxtLog::AddLogStrFmtTm(long ntime, LPCSTR szFmt, ...)
{
	time_t inTime = ntime;
	if (inTime <= 0) {
		time(&inTime);
	}
	char buff[10000];
	int len=0;
	struct tm* ptmTemp = localtime(&inTime);
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


void CTxtLog::ShowLogFile()
{
	ShellExecute(NULL,"open","notepad",mlogfilename,NULL,SW_SHOWNORMAL ); 
}
int CTxtLog::SetLogWindow(CWnd *pwnd)
{
	mCritical.Lock();
	mpFrame=pwnd;
	if (mpFrame) {
		mpFrame->PostMessage(WM_COMMAND,200,0);
		m_Viewer_Message_In_Queue=TRUE;
	}
	mCritical.Unlock();
	return 0;
}

void CTxtLog::UpdateLog_Edit(CEdit *ped)
{
	mCritical.Lock(); 
	if ((m_buffend-m_buffbegin_viewer)>0){ 
		int totalline;
		ped->SetSel(0x7fffffff, -1);
		m_pbuff[m_buffend]=0; 
		ped->ReplaceSel(m_pbuff+m_buffbegin_viewer);
		m_buffbegin_viewer=m_buffend;
		totalline=ped->GetLineCount();
		if (totalline>=1000) {
			ped->SetSel(0,ped->LineIndex(totalline-500),TRUE);
			ped->ReplaceSel("");
		}
		ped->SetSel(0x7fffffff, -1);
	} 
	m_Viewer_Message_In_Queue=FALSE;
	mCritical.Unlock();
	Flush(FALSE); 
}

BOOL CTxtLog::Flush(BOOL bCommit)
{
	if (m_DirtyFlag==FALSE) return TRUE;
	if (m_pbuff==NULL) return TRUE;
	mCritical.Lock();
	if (m_hfp==INVALID_HANDLE_VALUE) {
		m_buffbegin_file=m_buffend;
	}else {
		DWORD bufflen=(m_buffend-m_buffbegin_file);
		if (bufflen>0) {
			DWORD len;
			BOOL bret=WriteFile(m_hfp,m_pbuff+m_buffbegin_file,bufflen,&len,NULL);
			if (!bret) {
				//AfxMessageBox("Write file error!");
			}else if (len >=0) {
				m_buffbegin_file+=len;
			}
			if (bCommit) {
				FlushFileBuffers(m_hfp);
			}
		}
	}
	m_DirtyFlag =FALSE;
	int movelen;
	movelen=(m_buffbegin_file<m_buffbegin_viewer)?m_buffbegin_file:m_buffbegin_viewer; 
	if (movelen>0) {
		if (movelen==m_buffend) {
			m_buffbegin_file=m_buffbegin_viewer=0;
			m_buffend=0;
		}else {
			memmove(m_pbuff,m_pbuff+movelen,m_buffend-movelen);  
			m_buffbegin_file-=movelen;
			m_buffbegin_viewer-=movelen;
			m_buffend-=movelen;
		}
	}
	if (m_buffsize-m_buffend<1000) {// the buffer is full, discard the information;
		m_buffend=0;
		m_buffbegin_file=0;
		m_buffbegin_viewer=0; 
	}

	mCritical.Unlock();
	return TRUE;
}


//-----------------------------------
// other way to calcuate CRC, it works,
// but we use the len + type to do the CRC, to speed up.
/*
static unsigned int CalculateCRC32(const char* p, int len){
	unsigned int ret;
	ret=0;
	for (int i=0;i<len;i++) {
		ret +=p[i];
	}
	return ret;
}
*/

CPkgLog::CPkgLog()
{
	m_hfp=INVALID_HANDLE_VALUE;
	m_buffsize=0;
	m_buffbegin=m_buffend=0;
	m_pbuff=NULL;
	m_DirtyFlag=FALSE;
}

CPkgLog::~CPkgLog()
{
	Flush(TRUE);
	if (m_pbuff!=NULL){
		free(m_pbuff);
		m_pbuff=NULL;
	}
	if (m_hfp!=INVALID_HANDLE_VALUE) CloseHandle(m_hfp);
}

BOOL CPkgLog::Flush(BOOL bCommit)
{
//	if (m_DirtyFlag==FALSE) return TRUE;
	if (m_hfp==INVALID_HANDLE_VALUE) return TRUE;
	if (m_pbuff==NULL) return TRUE;
	mCritical.Lock();
	if ((m_buffend-m_buffbegin)>0) {
		DWORD bufflen=(m_buffend-m_buffbegin);
		DWORD len;
		BOOL bret=WriteFile(m_hfp,m_pbuff,(m_buffend-m_buffbegin),&len,NULL);
		if (!bret) {
			AfxMessageBox("Write file error 1 !");
		}else if (bufflen==len) {
			m_buffend=m_buffbegin=0;
		}else if (len<=0) {
			AfxMessageBox("Write file error 2!");
		}else {
			memmove(m_pbuff,m_pbuff+m_buffbegin+len,bufflen-len); 
			m_buffbegin=0;
			m_buffend=bufflen-len;
		}
	}else {
		ASSERT(m_buffbegin==0);
		m_buffbegin=0;
		m_buffend=0;
	}
	if (bCommit) {
		FlushFileBuffers(m_hfp);
		m_DirtyFlag =FALSE;
	}
	mCritical.Unlock();
	return TRUE;
};

BOOL CPkgLog::OpenLogFile(LPCSTR logfilename,int buffersize, BOOL bReadonly)
{
	if (m_hfp!=INVALID_HANDLE_VALUE)
	{
		Flush(TRUE);
		CloseHandle(m_hfp);
	}

	DWORD dwDesireaccess;
	DWORD dwShareMode;
	if (bReadonly) {
		dwDesireaccess=GENERIC_READ;
		dwShareMode=FILE_SHARE_READ|FILE_SHARE_WRITE;
	}else {
		dwDesireaccess=GENERIC_READ|GENERIC_WRITE;
		dwShareMode=FILE_SHARE_READ;	
	}
	m_hfp=CreateFile(logfilename,dwDesireaccess,dwShareMode,NULL,
				OPEN_ALWAYS,             // existing file only 
                FILE_ATTRIBUTE_NORMAL,     // normal file 
                NULL);

		
	if (buffersize==0) {
		m_buffsize=1024000;
	}else {
		m_buffsize=buffersize; 
	}
	if (m_hfp==INVALID_HANDLE_VALUE) return FALSE;
	if (m_pbuff==NULL) m_pbuff=(char*)malloc(m_buffsize); 

	return TRUE;

};

BOOL CPkgLog::AddLog(WORD len, WORD type, char *pbuff)
{
	if (m_hfp==INVALID_HANDLE_VALUE) return FALSE;
	mCritical.Lock();
	if ((len+(int)CRC32_LEN)>(m_buffsize-m_buffend)) {
		Flush(FALSE);
	}
	ASSERT((len+(int)CRC32_LEN)<=(m_buffsize-m_buffend));
	if (len>4) {
		char *p=m_pbuff+m_buffend;
		*((WORD*)(p))=len;
		*((WORD*)(p+2))=type;
		
		memcpy(p+4,pbuff,len-4);
		
		unsigned int crc32 =len+type; ///*CRC32::*/CalculateCRC32(p,len);//len
		memcpy(p+len, &crc32, CRC32_LEN ); //CRC
		
		//m_buffend+=len;
		m_buffend+=(len+CRC32_LEN); //CRC
		
		m_DirtyFlag=TRUE;
	}
	mCritical.Unlock();
	return TRUE;
}

// return 1: sucessful
// return 0: end of file.
// return -1:Incomplete record;
// return -2:Calcuation error;
int CPkgLog::GetRecord(WORD &len, WORD &type, char *  &pbuff, unsigned int* pCRC32)
{

	APILock lock(&mCritical);
	int buflen;
	BOOL bendoffile=FALSE;
	buflen=m_buffend-m_buffbegin;
	if (buflen<2) {
		if (Read_Data()<=0) {
			bendoffile=TRUE;
		}
	}else{
		int pkglen;
		char * retbuf;
		retbuf=m_pbuff+m_buffbegin;
		pkglen=*((WORD*)retbuf);
		//if (pkglen<2) pkglen=2;
		ASSERT(pkglen>=2);
		if (pkglen + (int)CRC32_LEN > buflen) {
			if (Read_Data()<=0) {
				bendoffile=TRUE;
			}
		}
	}
	if (bendoffile) {
		if (m_buffend-m_buffbegin>0) {
			AfxMessageBox("Incomplete record!");
			return -1;
		}
		m_buffbegin=m_buffend=0; 
		return 0;
	}else {
		buflen=m_buffend-m_buffbegin;
		if (buflen<2) {
			AfxMessageBox("Incomplete record!");
			//_chsize(fileno(fp),len); to truncate the file.
			m_buffbegin=m_buffend=0; 
			return -1;
		}
		int pkglen;
		char * retbuf;
		retbuf=m_pbuff+m_buffbegin;
		pkglen=*((WORD*)retbuf);
		if (pkglen<2) pkglen=2;
		if (pkglen + (int)CRC32_LEN > buflen) {
			AfxMessageBox("Incomplete record!");
			//_chsize(fileno(fp),len); to truncate the file.
			m_buffbegin=m_buffend=0; 
			return -1;
		}else {
			m_buffbegin+=pkglen;
			len=pkglen;
			type=*((WORD*)(retbuf+2));
			pbuff=retbuf+4;

			unsigned int crc32 = *(unsigned int*)(m_pbuff+m_buffbegin);
			unsigned int newcrc32=len+type;///*CRC32::*/CalculateCRC32(retbuf,len);//len;
			BOOL ret = (newcrc32 == crc32);
			
			if(pCRC32)
			{
				*pCRC32 = ret ? crc32 : -1;
			}


			m_buffbegin += CRC32_LEN;
			if (ret==FALSE) {
				AfxMessageBox("Crc caculation error!");
				return -2;
			}
			return 1;
		}
	}
}

int CPkgLog::Read_Data()
{
	if (m_hfp==INVALID_HANDLE_VALUE) return FALSE;
	if (m_pbuff==NULL) m_pbuff=(char*)malloc(m_buffsize); 
	if (m_buffbegin>0) {
		memmove(m_pbuff,m_pbuff+m_buffbegin,m_buffend-m_buffbegin);
		m_buffend-=m_buffbegin;
		m_buffbegin=0;
	}

	DWORD len;
	BOOL bret=ReadFile(m_hfp,m_pbuff+m_buffend,m_buffsize-m_buffend,&len,NULL);
	if ((bret)&&(len>0)){
		m_buffend+=len;
	}
	return len;
}

