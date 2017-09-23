// OrdScript.h: interface for the COrdScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDSCRIPT_H__166E632D_B3EA_4700_9CC8_B4492D7F9B6C__INCLUDED_)
#define AFX_ORDSCRIPT_H__166E632D_B3EA_4700_9CC8_B4492D7F9B6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FUNCTIONS\CCmdScript.h"

class COrdScript : public CCmdScript  
{
public:
	char msg[2000];
public:
	BOOL LoadGTCOrder(LPCSTR fname,LPCSTR errfname);
	BOOL SaveGTCOrder(LPCSTR fname);
	BOOL SendGTCOrder();
	void doHelp();
	BOOL OnRunCmd(int argc, char **argv);
	void OnErrorMsg(char *errmsg);
	COrdScript();
	virtual ~COrdScript();

};

class CScriptItem
{
	char *mpcmd;
	int	  mtime;
	BOOL  mbtrigged;
public:
	BOOL  mbworkdayonly;

	BOOL CheckTrigger(int ntime);
	void SetTime(int ntime);
	void SetCmd(LPCSTR pcmd);
	
	int GetTime() {return mtime;}
	const char * GetCmd() {return mpcmd;}
	BOOL IsTrigged() { return mbtrigged;}

	CScriptItem();
	~CScriptItem();
};

#endif // !defined(AFX_ORDSCRIPT_H__166E632D_B3EA_4700_9CC8_B4492D7F9B6C__INCLUDED_)
