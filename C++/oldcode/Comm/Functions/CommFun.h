// CommFun.h: interface for the CCommFun class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMFUN_H__431D2DB5_9D32_42BB_A54F_9C1F3A8645CF__INCLUDED_)
#define AFX_COMMFUN_H__431D2DB5_9D32_42BB_A54F_9C1F3A8645CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//this function is for the newchartserver.
//need to drop.
class CCommFun  
{
public:
	 void strupr(char *buf);
	 BOOL DBopt(double ft,char flag,double lt);
	 BOOL PostThreadMessage_(DWORD idThread,UINT Msg,WPARAM wParam,LPARAM lParam);
	 char* charcurtime();
	 char* erase_space(char *buf);
	 char* Expath_Plus(char *name="default");
	CCommFun();
	virtual ~CCommFun();

};

#endif // !defined(AFX_COMMFUN_H__431D2DB5_9D32_42BB_A54F_9C1F3A8645CF__INCLUDED_)
