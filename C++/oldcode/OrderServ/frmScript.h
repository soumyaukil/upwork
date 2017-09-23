#if !defined(AFX_FRMSCRIPT_H__A43AC597_41AA_4722_B139_C21F3BE97556__INCLUDED_)
#define AFX_FRMSCRIPT_H__A43AC597_41AA_4722_B139_C21F3BE97556__INCLUDED_

#include "GRIDCTRL\GridCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// frmScript.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// frmScript frame

class frmScript : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmScript)
protected:
	frmScript();           // protected constructor used by dynamic creation

// Attributes
public:
	static frmScript *mfrm;
// Operations
public:
	void InitGrid();
	CGridCtrl mGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmScript)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmScript();

	// Generated message map functions
	//{{AFX_MSG(frmScript)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMSCRIPT_H__A43AC597_41AA_4722_B139_C21F3BE97556__INCLUDED_)
