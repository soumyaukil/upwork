// Config.cpp: implementation of the CConfig class.
//
//////////////////////////////////////////////////////////////////////

//#include "QuoteServer.h"
#include "Config.h"
#include <cstdio>
#include <cstring>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <cstdlib>
#include <fstream>
#include <iostream>
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
	time_t t1;
	t1=time(NULL);
	tm *tm1=localtime(&t1);
	if ((tm1->tm_wday==0)||(tm1->tm_wday==6)){//sunday and saturday
		mIsWorkDay=0;
	}else {
		mIsWorkDay=1;
	}
}

CConfig::~CConfig()
{

}

void CConfig::ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
            std::not1(std::ptr_fun<int, int>(std::isspace))));
}

// trim from end (in place)
void CConfig::rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
            std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());

}

// trim from both ends (in place)
void CConfig::trim(std::string &s) {    
    ltrim(s);
    rtrim(s);
}

//DEL int CConfig::GetCfg(const char* id, const char* subid, const char* value)
//DEL {
//DEL 	return 0;
//DEL }

int CConfig::ReadCfgFile(const char* filename)
{

//--------------------------------------------------------------
	char profilename[400];
//	char buf[200];
	if (filename==NULL) {
		sprintf(profilename,"%s%s",mAppPath,mAppCfgFile); 
	}else {
		strcpy(profilename,filename);
	}
        std::ifstream profile (profilename);

	if( !profile.is_open() ) {
		std::cerr << "Open Configure File Error!";
		return -1;
	}
	
	std::string str,id,subid,value;
	int serpert1,serpert2;
	while(getline(profile, str)) {
		trim(str);
		if (str.empty()==false){
			if (str[0]!='#') {
				serpert1 =str.find(':',0);
				if (serpert1 ==-1) {
                                        rtrim(str);
					//str.TrimRight(); 
					OnGetCfg("","",str.data()); 
				}else {
					id=str.substr(0,serpert1);
					rtrim(id);
					serpert2=str.find(':',serpert1+1 );
					if (serpert2==-1) {
						value=str.substr(serpert1+1);
                                                trim(value);
						OnGetCfg(id.data(),"",value.data());
					}else{
						subid=str.substr(serpert1+1,serpert2-serpert1-1 );
						trim(subid);
						value=str.substr(serpert2+1);
						trim(value);
						OnGetCfg(id.data(),subid.data(),value.data());
					}
				}
			}
		}
	};
//	ar.Close();
	profile.close();
	return 0;
}

/*int CConfig::InitCfg(const char* pcfgfile,HINSTANCE hModule)
{
	int len;
	char modulename[300];
	char modulepath[300];
	char * pfilename;
	len=GetModuleFileName(hModule,modulename,sizeof(modulename))   ;
	assert(len!=0);

	m_FullVersion=GetFileVersion(modulename);
	m_Version=(WORD)(m_FullVersion>>16);
/*	
	
	CMDVersion mdv;
	mdv.GetFileVersionInfo(modulename);
	CString mvertmp=mdv.GetValue("PRODUCTVERSION");
	CString tmp;
	int v1,v2,v3,v4;
	v1=v2=v3=v4=0;
	sscanf(mvertmp,"%d,%d,%d,%d",&v1,&v2,&v3,&v4);
	m_Version=(v1<<8)+v2;
	m_FullVersion=(((m_Version<<8)+v3)<<8)+v4;

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
		assert(strlen(pcfgfile)<=19);
		memccpy(mAppCfgFile,pcfgfile,0,20);
		mAppCfgFile[19]=0;
	}
	return 0;
}*/

time_t CConfig::GetTodayFirstSec()
{
	struct tm when;
	time_t now;
	time( &now );
	when = *localtime( &now );

/*	
	when.tm_hour=0;
	when.tm_min =0;
	when.tm_sec=0; 
*/
	return(now -((when.tm_hour*60+when.tm_min)*60+when.tm_sec));//	(mktime(&when));
}

bool CConfig::GetIpPortVal(const char* str, uint32_t *ip, uint16_t *port)
{
	uint32_t mip;
	uint16_t mport;
	char st[100];
	strncpy(st,str,sizeof(st));
	char *pst=strtok(st," ,;\t");
	if (pst!=NULL){
		uint32_t a1,a2,a3,a4;
		if (sscanf(pst,"%d.%d.%d.%d",&a1,&a2,&a3,&a4)!=4) return false;
		mip=((a4&0xff)<<24)+((a3&0xff)<<16)+((a2&0xff)<<8)+(a1&0xff);
	};
	pst=strtok(NULL,",;\t");
	if (pst!=NULL){
		mport=0;
		//sscanf(pst,"%d",&mport);
		mport=atoi(pst);
	};
	
	

/*

	CString tmpst;
	mip=0;
	mport=0;
	uint32_t a1,a2,a3,a4;
	if (AfxExtractSubString(tmpst,str,0,',')==NULL)
		return false;
//	mip=inet_addr(tmpst);
	tmpst.TrimLeft();
	tmpst.TrimRight(); 
	if (sscanf((const char*)tmpst,"%d.%d.%d.%d",&a1,&a2,&a3,&a4)!=4) return false;
	mip=((a4&0xff)<<24)+((a3&0xff)<<16)+((a2&0xff)<<8)+(a1&0xff);

	if (AfxExtractSubString(tmpst,str,1,',')==NULL)
		return false;
	mport=atoi(tmpst);
*/
	if (mport==0) return false;
	if (mip==0) return false;
	*ip=mip;
	*port=mport;
	return true;
}

bool CConfig::FormatTimeYMDHMS(time_t ntime, char *pbuff)
{
	if (ntime<=0) {
		time(&ntime);
	}

	struct tm* ptmTemp = localtime(&ntime);
	if (ptmTemp == NULL ||
		!strftime(pbuff, 20, "%m/%d/%y %H:%M:%S", ptmTemp))
		pbuff[0] = '\0';
	return true;
}

bool CConfig::FormatTimeHMS(time_t ntime, char *pbuff)
{
	if (ntime<=0) {
		time(&ntime);
	}

	struct tm* ptmTemp = localtime(&ntime);
	if (ptmTemp == NULL ||
		!strftime(pbuff, 10, "%H:%M:%S", ptmTemp))
		pbuff[0] = '\0';
	return true;
}

const char* CConfig::GetYMDStr()
{
	static char dd[20];
	struct tm *mm;
	time_t cctime;
	time(&cctime);	
	mm = localtime(&cctime);
	sprintf(dd,"%02d%02d%02d",(mm->tm_year %100),mm->tm_mon+1,mm->tm_mday);
	return dd;
}

bool CConfig::GetBoolVal(const char* str)
{
	if (str==NULL) return false;
	if ((str[0]=='Y')||(str[0]=='y')||(str[0]=='T')||(str[0]=='t')) { // true or false
		return true;
	}else return false;

}

int CConfig::GetTimeVal(const char* pval)
{
	if (pval==NULL) return 0;
	int h,m;
	h=m=0;
	sscanf(pval,"%d:%d",&h,&m);
	if (h<0) h=0;
	if (m<0) m=0;
	return h*60+m;
}

bool CConfig::IsToday(const char* pval)
{
	if (pval==NULL) return 0;
	int y,m,d;
	if (sscanf(pval,"%d/%d/%d",&y,&m,&d)!=3) return 0;
	time_t t1;
	t1=time(NULL);
	tm *tm1=localtime(&t1);
	if ((tm1->tm_year+1900==y)&&(tm1->tm_mon+1==m) &&(tm1->tm_mday==d)) return 1;
	return 0;
}


bool CConfig::GetPath(const char* filename, char *pPath,int ntime)
{
//  %Y %y %m %d, %H %M %S will be replaced as the today time
	if ((filename==NULL)&&(strlen(filename)==0)){
		sprintf(pPath,"%s/file1",mAppPath);
		return true;
	}
	bool babsolute=false;
	if ((filename[0]=='/')||(filename[0]=='\\')||(filename[1]==':')) {
		babsolute=true;
	}
	int i;
	char *p=pPath;
	if (babsolute==false) {
		sprintf(pPath,"%s",mAppPath);
		while(*p){
			p++;
		}
	}
	tm t1;
	tm t2;
	time_t t;
	time_t tt;
	if (ntime==0) {
		time(&t);
	}else {
		t=ntime;
	}
	t1=*localtime(&t);
	if (t1.tm_wday==0) {//sunday;
		tt=t-48*3600;
	}else if (t1.tm_wday==1) {//monday;
		tt=t-72*3600;
	}else {
		tt=t-24*3600;
	}
	t2=*localtime(&tt);

	

	for (i=0;filename[i];i++) {
		if (filename[i]=='%') {
			switch(filename[i+1]) {
			case 'Y':
				sprintf(p,"%04d",t1.tm_year+1900);
				p+=4;
				i++;
				continue;
			case 'y':
				sprintf(p,"%02d",t1.tm_year-100);
				p+=2;
				i++;
				continue;

			case 'm':
				sprintf(p,"%02d",t1.tm_mon+1);
				p+=2;
				i++;
				continue;
			case 'd':
				sprintf(p,"%02d",t1.tm_mday);
				p+=2;
				i++;
				continue;
			case 'l'://month of previous workday, the letter before m
				sprintf(p,"%02d",t2.tm_mon+1);
				p+=2;
				i++;
				continue;
			case 'c'://day of previous workday, the letter is before d;
				sprintf(p,"%02d",t2.tm_mday);
				p+=2;
				i++;
				continue;

			case 'H':
				sprintf(p,"%02d",t1.tm_hour);
				p+=2;
				i++;
				continue;

			case 'M':
				sprintf(p,"%02d",t1.tm_min);
				p+=2;
				i++;
				continue;
			case 'S':
				sprintf(p,"%02d",t1.tm_sec);
				p+=2;
				i++;
				continue;
			}
		}
		*p=filename[i];
		p++;
	}
	*p=0;
	return true;
}

//this funciton is to read the the data string formated by 
//Format YMDHMS function;
time_t CConfig::GetYMDHMSVal(const char* str)
{
	int y,m,d,h,min,s;
	y=m=d=h=min=s=0;
	time_t ret;
	sscanf(str,"%d/%d/%d %d:%d:%d",&m,&d,&y,&h,&min,&s);
	tm t1;
	memset(&t1,0,sizeof(t1));
	
	t1.tm_year=y+100; 
	t1.tm_mon=m-1;
	t1.tm_mday=d;
	t1.tm_hour=h;
	t1.tm_min=min;
	t1.tm_sec=s;
	
	t1.tm_isdst=-1; 
	ret=mktime(&t1);
	return ret;
}
