// CCmdScript.h: interface for the CCmdScript class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCmdScript_H__E2874481_67A8_44AE_BE5D_4E0AEF26A15D__INCLUDED_)
#define AFX_CCmdScript_H__E2874481_67A8_44AE_BE5D_4E0AEF26A15D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCmdScript  
{
	BOOL GetCmd(char *pcmd);
public:
	BOOL RunCmds(LPCSTR pcmds);
	virtual void OnErrorMsg(char *errmsg);
	virtual BOOL OnRunCmd(int argc, char **argv);
	CCmdScript();
	virtual ~CCmdScript();

};



#endif // !defined(AFX_CCmdScript_H__E2874481_67A8_44AE_BE5D_4E0AEF26A15D__INCLUDED_)
