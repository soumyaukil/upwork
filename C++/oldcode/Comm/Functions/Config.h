// Config.h: interface for the CConfig class.
//
//////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <time.h>

#if !defined(AFX_CONFIG_H__C8CBC4A7_598B_11D4_BCF1_00105A82D2D1__INCLUDED_)
#define AFX_CONFIG_H__C8CBC4A7_598B_11D4_BCF1_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CConfig  
{
public:

	int  mIsWorkDay;


public:
	time_t GetYMDHMSVal(const char* str);
// for process the configure string:
	static bool GetIpPortVal(const char* str, uint32_t *ip, uint16_t *port);
	static int GetTimeVal(const char* pval);
	static bool GetBoolVal(const char* str);
	//check a string is today's day;
	//format:yyyy/mm/dd
	static bool IsToday(const char* pval);

	static time_t GetTodayFirstSec();
//  this function format the path related to the apppath;
//  %Y %y %m %d, %H %M %S will be replaced as the today time
//	if ntime==0, it will replace with the today's time;
	bool GetPath(const char* filename, char *pPath,int ntime=0);
	
	const char* GetYMDStr();
	bool FormatTimeHMS(time_t ntime, char *pbuff);
	bool FormatTimeYMDHMS(time_t ntime, char *pbuff);
	int ShowConfigFile(const char* filename=NULL);
	uint16_t m_Version;
	uint32_t m_FullVersion;
	//int InitCfg(const char* filename=NULL,HINSTANCE hModule=NULL);
	char mAppCfgFile[20];
	char mAppName[20];
	char mAppPath[300];

	int ReadCfgFile(const char* filename=NULL);
	virtual int OnGetCfg(const char* id, const char* subid, const char* value)=0;
	CConfig();
	virtual ~CConfig();
};

#endif // !defined(AFX_CONFIG_H__C8CBC4A7_598B_11D4_BCF1_00105A82D2D1__INCLUDED_)
