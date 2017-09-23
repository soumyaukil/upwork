// Config.cpp: implementation of the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "QuoteServer.h"
#include "Config.h"
#include "MDVersion.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConfig::CConfig()
{

}

CConfig::~CConfig()
{

}

int CConfig::GetCfg(LPCSTR id, LPCSTR subid, LPCSTR value)
{

	return 0;
}

int CConfig::ReadCfgFile(LPCSTR filename)
{
//--------------------------------------------------------------
	CString profilename;//=mAppPath + _T("OrderServ.cfg");
	CFile profile;
	char buf[512];
	if( !profile.Open( profilename, CFile::modeRead) ) {
		AfxMessageBox("Open Configure File Error!");
		return -1;
	}
	CArchive ar( &profile, CArchive::load, 512, buf );
//	int i;
	CString str,id,subid,value;
	int serpert1,serpert2;
	while(ar.ReadString(str)) {
		str.TrimLeft();
		if (str.IsEmpty()==FALSE){
			if (str.GetAt(0)!='#') {
				serpert1 =str.Find(':',0);
				if (serpert1 ==-1) {
					str.TrimRight(); 
					GetCfg(NULL,NULL,str); 
				}else {
					id=str.Mid(0,serpert1);
					id.TrimRight();
					serpert2=str.Find(':',serpert1+1 );
					if (serpert2==-1) {
						value=str.Mid(serpert1+1);
						value.TrimLeft();
						value.TrimRight();
						GetCfg(id,NULL,value);
					}else{
						subid=str.Mid(serpert1+1,serpert2-serpert1-1 );
						subid.TrimLeft();
						subid.TrimRight();
						value=str.Mid(serpert2+1);
						value.TrimLeft();
						value.TrimRight();
						GetCfg(id,subid,value);
					}
				}
			}
		}
	};
	ar.Close();
	profile.Close();
	return 0;

}

int CConfig::InitCfg(LPCSTR pcfgfile)
{
	int len;
	char modulename[300];
	char modulepath[300];
	char * pfilename;
	len=GetModuleFileName(NULL,modulename,sizeof(modulename))   ;
	if (len==0) {
//		strcpy(mCfgFileName,"IPServer.cfg");
		modulename[0]=0;
	}

	CMDVersion mdv;
	mdv.GetFileVersionInfo(modulename);
	CString mvertmp=mdv.GetValue("PRODUCTVERSION");
	CString tmp;
	AfxExtractSubString(tmp,mvertmp,0,',');
	m_Version=0;
	m_Version=(atoi(tmp)& 0xff);
	m_Version<<=8;

	AfxExtractSubString(tmp,mvertmp,1,',');
	m_Version+=(atoi(tmp)&0xff);
	


	len=GetFullPathName(modulename ,sizeof(modulepath),modulepath,&pfilename);   
	if ((len==0)||(len>=sizeof(modulepath ))) {
		AfxMessageBox("Get the modifile name error!");
	}
	if(pfilename!=NULL) {
		memccpy(mAppName,pfilename,0,20);
		*pfilename=0;
	} else {
		mAppName[0]=0;
	}
	strcpy(mAppPath,modulepath); 
	if (pcfgfile==NULL) {
		strcpy(mAppCfgFile,mAppName);
		strcpy(mAppCfgFile+strlen(mAppName)-3,"cfg");
	}else {
		ASSERT(strlen(pcfgfile)<=19);
		memccpy(mAppCfgFile,pcfgfile,0,20);
		mAppCfgFile[19]=0;
	}

	CString st;
	st.Format("Version %d,%d",HIBYTE(m_Version),LOBYTE(m_Version));
	AfxMessageBox(st);
	//init the quote server;
	return 0;
}
