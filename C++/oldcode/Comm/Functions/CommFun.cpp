// CommFun.cpp: implementation of the CCommFun class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommFun.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCommFun::CCommFun()
{

}

CCommFun::~CCommFun()
{

}

char* CCommFun::Expath_Plus(char *name)
{
	static char	m_sPathName[600];
	char	buf[500];
	LPSTR	m_lExtendPoint;

	GetModuleFileName(NULL,buf,sizeof(buf)-1);	buf[sizeof(buf)-1]=0;
	GetFullPathName(buf,sizeof(buf)-1,m_sPathName,&m_lExtendPoint);

	*m_lExtendPoint = 0;
			

/*
	switch( *name ) {
		case 'L':
		case 'l':
				strcat( m_sPathName,"..\\Log\\");
			break;
		case 'F':
		case 'f':
				strcat( m_sPathName,"..\\File\\");
			break;
		default:
			break;
	}
*/
	return(m_sPathName);
}

char* CCommFun::erase_space(char *buf)
{
	for( int i=strlen(buf)-1; 
		 i>=0 && (*(buf+i)==' '||*(buf+i)==10||*(buf+i)==13||*(buf+i)=='	'); 
		 i-- ) 
		 *(buf+i)=0;

	for( char* tmp=buf; 
		 (*tmp==' '||*tmp=='	'||*tmp==10||*tmp==13)&&strlen(tmp)>0;
		 ) 	
		 tmp++;

	i = strlen(tmp);

	strncpy( buf,tmp,i ); buf[i]=0;

	return buf;
}

char* CCommFun::charcurtime()
{
	static char buf[100];
	long t;

	time(&t);
	struct tm *m=localtime(&t);

	sprintf(buf,"%02d/%02d/%04d %02d:%02d:%02d",
		m->tm_mon+1,
		m->tm_mday,
		m->tm_year+1900,
		m->tm_hour,
		m->tm_min,
		m->tm_sec);

	return (buf);
}
#define TIME_POST_SLEEP		10

BOOL CCommFun::PostThreadMessage_(DWORD idThread,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	for( ;; ) {
		if( ::PostThreadMessage(idThread,Msg,wParam,lParam) == TRUE )
			break;
		else {
			if( GetLastError()==ERROR_INVALID_THREAD_ID )
				return FALSE;
			else
				Sleep(TIME_POST_SLEEP);
		}
	}
	return TRUE;
}


BOOL CCommFun::DBopt(double ft, char flag, double lt)
{
#define  COMMFUN_sml  0.0000001

	switch( flag ) {
		case '=':
			 if( ( (ft+COMMFUN_sml)>=lt ) && ( (ft-COMMFUN_sml)<=lt ) )
				 return TRUE;
			 break;
		case '>':
			 if( (ft-COMMFUN_sml) > lt )
				 return TRUE;
			 break;
		case '<':
			 if( (ft+COMMFUN_sml) < lt )
				 return TRUE;
	}

	return FALSE;
}

void CCommFun::strupr(char *buf)
{
	int len=strlen(buf);

	for( int i=0; i<len; i++ ) {
		 if( buf[i]>='a' && buf[i]<='z' )
			 buf[i] += ('A'-'a');
	}
}
