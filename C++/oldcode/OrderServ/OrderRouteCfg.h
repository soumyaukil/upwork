#if !defined(AFX_ORDERROUTECFG_H__34137D51_47D4_11D6_BDF6_0050041B8FEC__INCLUDED_)
#define AFX_ORDERROUTECFG_H__34137D51_47D4_11D6_BDF6_0050041B8FEC__INCLUDED_

#include "OrderRouter.h"	// Added by ClassView
#include "GRIDCTRL\GridCtrl.h"	// Added by ClassView
#include "FUNCTIONS\ControlPos.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OrderRouteCfg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// OrderRouteCfg1 dialog

class OrderRouteCfg1 : public CDialog
{
// Construction
public: 
	OrderRouter *mproute; 
	char  mRouteName[10];
	DWORD mIP;
	WORD mPort;
	char mUserID[10];
	char mUserPWD[20];
	int  mtmstart;
	int  mtmstop;
	int  mautoconnect;

public:
	OrderRouteCfg1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(OrderRouteCfg1)
	enum { IDD = IDD_ROUTECFG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(OrderRouteCfg1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(OrderRouteCfg1)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERROUTECFG_H__34137D51_47D4_11D6_BDF6_0050041B8FEC__INCLUDED_)
