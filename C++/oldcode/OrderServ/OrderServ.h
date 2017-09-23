// OrderServ.h : main header file for the ORDERSERV application
//

#if !defined(AFX_ORDERSERV_H__8FF81F1D_F460_11D3_BC5D_00105A82D2D1__INCLUDED_)
#define AFX_ORDERSERV_H__8FF81F1D_F460_11D3_BC5D_00105A82D2D1__INCLUDED_

#include "ClientPortDaemon.h"	// Added by ClassView
//#include "ClientPortMan.h"	// Added by ClassView
//#include "LiuSocket.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "OrderConfig.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// COrderServApp:
// See OrderServ.cpp for the implementation of this class
//

class COrderServApp : public CWinApp
{
public:
	BOOL CheckDatabase();
	CClientPortDaemon mclientport1; //the client port daemon
	CClientPortDaemon mclientport2; //the client port daemon
	CClientPortDaemon mclientport3; //the client port daemon
//	CDatabase mdb;				   //the database	
	CString mAppPath;
//	CClientPortMan mClientPort;
//	CLiuSocket mLsocket;

	COrderServApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderServApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(COrderServApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnViewCfg();
	afx_msg void OnViewLog();
	afx_msg void OnDataLoadequity();
	afx_msg void OnDataSaveequity();
	afx_msg void OnDataLshtlst();
	afx_msg void OnDataSavedata();
	afx_msg void OnToolsSecifo();
	afx_msg void OnToolsEquity();
	afx_msg void OnViewTraders();
	afx_msg void OnToolsBalance();
	afx_msg void OnViewLoginlog();
	afx_msg void OnDataExportposition();
	afx_msg void OnToolsLoadloginctrl();
	afx_msg void OnDataCounting();
	afx_msg void OnViewThreads();
	afx_msg void OnDataRecoverSavetrades();
	afx_msg void OnAaaTest();
	afx_msg void OnToolsLoadmmuntradeable();
	afx_msg void OnDataSodQuest();
	afx_msg void OnRouteConfig();
	afx_msg void OnDataSodBpequity();
	afx_msg void OnToolsLoadroutectrl();
	afx_msg void OnToolsCommand();
	afx_msg void OnToolsScriptlist();
	afx_msg void OnLogFile();
	afx_msg void OnToolsDbreconn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitSystem();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERSERV_H__8FF81F1D_F460_11D3_BC5D_00105A82D2D1__INCLUDED_)
