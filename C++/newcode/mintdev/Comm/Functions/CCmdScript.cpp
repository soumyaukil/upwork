// CCmdScript.cpp: implementation of the CCmdScript class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "CCmdScript.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdScript::CCmdScript()
{

}

CCmdScript::~CCmdScript()
{

}

int RunProcess(char * pcmd,char *retbuff,int retbuffsize,const char * penv, const char * ppath);

BOOL CCmdScript::OnRunCmd(int argc, char **argv)
{
	if (_stricmp(argv[0],"shell")==0) {
		if (argc<2) {
			OnErrorMsg("shell cmd ...");
			return FALSE;
		}
		char str[200];
		char retbff[4000];
		strcpy(str,argv[1]);
		for (int i=2;i<argc;i++) {
			strcat(str," ");
			strcat(str,argv[i]);
		}
		int ret=RunProcess(str,retbff,sizeof(retbff),NULL,NULL);
		if (ret!=0) {
			char errmsg[1000];
			sprintf(errmsg,"shell cmd error:%d,%s",ret,str);
			OnErrorMsg(errmsg);
		}
//		if (strlen(retbff)) {
//			AfxMessageBox(retbff);
//		}
		return TRUE;
	}else {
		OnErrorMsg("Unknow command");
		return FALSE;
	}
	return FALSE;
}

void CCmdScript::OnErrorMsg(char *errmsg)
{
	return;
}

BOOL CCmdScript::RunCmds(LPCSTR pcmds)
{
	char cmd[1000];
	int i,j;
	j=0;
	int state=0;
	for (i=0;pcmds[i];i++) {
		if (pcmds[i]==';'){
			if ((j>0)&&(cmd[j-1]=='\\')){
				cmd[j-1]=';';
			}else {
				cmd[j]=0;
				GetCmd(cmd);
				j=0;
			}
		}else {
			cmd[j]=pcmds[i];
			if (j>=1000) {
				OnErrorMsg("Command String too long");
				return FALSE;// cmd too long;
			}
			j++;
		}
	}
	if (j>0) {
		cmd[j]=0;
		GetCmd(cmd);
		j=0;
	}
	return TRUE;
}

static BOOL isSpace(char p) {
	if ((p==' ')||(p=='\t')||(p=='\r')||(p=='\n')){
		return TRUE;
	}
	return FALSE;
}

BOOL CCmdScript::GetCmd(char *pcmd)
{
	char *args[100];
	int  argc;
	argc=0;

	memset(args,0,sizeof(args));

	int i;
	char *p1;
	p1=NULL;
	for (i=0;pcmd[i];i++) {
		if (isSpace(pcmd[i])) {
			if (p1!=NULL){
				pcmd[i]=0;
				args[argc]=p1;
				argc++;
				p1=NULL;
			}
		}else {
			if (p1==NULL) {
				p1=pcmd+i;
			}
		}
	}
	if (p1) {
		args[argc]=p1;
		argc++;
		p1=NULL;
	}

	if (argc) return OnRunCmd(argc,args);
	return TRUE;
}


int RunProcess(char * pcmd,char *retbuff,int retbuffsize,const char * penv, const char * ppath)
{
     HANDLE hreadFromChild = NULL;
     HANDLE hwriteToParent = NULL;
     HANDLE hwriteToParent2 = NULL;
	 retbuff[0]=0;

     SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE }; // inheritable handle

     if(!::CreatePipe(&hreadFromChild, &hwriteToParent, &sa, 0))
	{ /* pipe failed */
	 // ::GetLastError() will reveal the cause
	 return -1;
	} /* pipe failed */

     if(!::DuplicateHandle(GetCurrentProcess(),     // duplicate from this process
			   hwriteToParent,	    // this handle 
			   GetCurrentProcess(),     // into this process
			   &hwriteToParent2,        // as this handle
			   0,			    // no access flags (subsumed by DUPLICATE_SAME_ACCESS)
			   TRUE,	            // create inheritable
			   DUPLICATE_SAME_ACCESS))  // create duplicate access
	{ /* duplicate failed */
	 DWORD err = ::GetLastError();
	 ::CloseHandle(hreadFromChild);
	 ::CloseHandle(hwriteToParent);
	 ::SetLastError(err);
	 return -1;
	} /* duplicate failed */

     STARTUPINFO startup;
     PROCESS_INFORMATION procinfo;

     ::ZeroMemory(&startup, sizeof(startup));

     startup.cb = sizeof(startup);
     startup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
     startup.wShowWindow = SW_MINIMIZE; // hidden console window
     startup.hStdInput = NULL; // not used
     startup.hStdOutput = hwriteToParent;
     startup.hStdError = hwriteToParent2;

     // We want a non-inherited read handle. DuplicateHandle with a
     // NULL target fixes the read side to be non-inheritable
     ::DuplicateHandle(::GetCurrentProcess(),    // in this process
		       hreadFromChild,           // child read handle
		       ::GetCurrentProcess(),    // to this process
		       NULL,                     // modify existing handle
		       0,                        // flags
		       FALSE,                    // not inheritable
		       DUPLICATE_SAME_ACCESS);   // same handle access

     BOOL started = ::CreateProcess(NULL,        // command is part of input string
				    pcmd,         // (writeable) command string
				    NULL,        // process security
				    NULL,        // thread security
				    TRUE,        // inherit handles flag
				    0,           // flags
				    (void*)penv,        // inherit environment
				    ppath,        // inherit directory
				    &startup,    // STARTUPINFO
				    &procinfo);  // PROCESS_INFORMATION

     if(!started)	{ /* failed to start */
	 DWORD err = ::GetLastError(); // preserve across CloseHandle calls
	 ::CloseHandle(hreadFromChild);
	 ::CloseHandle(hwriteToParent);
	 ::CloseHandle(hwriteToParent2);
	 ::SetLastError(err);
	 return -1;
	} /* failed to start */

     // Now close the output pipes so we get true EOF/broken pipe
     ::CloseHandle(hwriteToParent);
     ::CloseHandle(hwriteToParent2);
     

	 DWORD lasttick;
	 const DWORD ntimeout =100000;
	 lasttick=GetTickCount(); 
     unsigned long bytesRead=0;
	 int buffbegin=0;
     while((retbuffsize-buffbegin)>0) {
		 if (::ReadFile(hreadFromChild, retbuff+buffbegin, retbuffsize-buffbegin, &bytesRead, NULL)==0) break;
		 if(bytesRead <= 0)
			break; // EOF condition
		 if ((GetTickCount()-lasttick)>ntimeout) break;
		 buffbegin+=bytesRead;
	 };

	 if (buffbegin>=retbuffsize) {
		 retbuff[buffbegin-1]=0;
		 char discardbuff[1000];
		 //discard the left buffer;
		 while(::ReadFile(hreadFromChild, discardbuff, sizeof(discardbuff), &bytesRead, NULL)>0){
			 if ((GetTickCount()-lasttick)>ntimeout) break;
		 };
	 }else {
		 retbuff[buffbegin]=0; 
	 }


      ::CloseHandle(hreadFromChild);

	unsigned long exitcode;
	exitcode=STILL_ACTIVE;
//	int i=0;
	while(exitcode==STILL_ACTIVE){
		GetExitCodeProcess(procinfo.hProcess, &exitcode);
//		i++;
		if ((GetTickCount()-lasttick)>ntimeout) break;
//		if (i>200) break;
		Sleep(10);

	}
	return exitcode;     
}
