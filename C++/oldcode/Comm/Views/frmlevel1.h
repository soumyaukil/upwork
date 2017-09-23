#if !defined(AFX_FRMLEVEL1_H__165E5F11_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
#define AFX_FRMLEVEL1_H__165E5F11_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// frmlevel1.h : header file
//
#include "ViewLv1.h"

/////////////////////////////////////////////////////////////////////////////
// frmLevel1 frame

class frmLevel1 : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmLevel1)
protected:
	frmLevel1();           // protected constructor used by dynamic creation
// Attributes
public:
	static ViewLv1 *pviewL1;  // the level1 window
	static CCriticalSection mFrmCritical; // the critical section

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmLevel1)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmLevel1();

	// Generated message map functions
	//{{AFX_MSG(frmLevel1)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*#ifdef _FRM_LV1_FILE
	CCriticalSection gmFrmL1; // the critical section
#else 
	extern CCriticalSection gmFrmL1;
#endif
*/

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMLEVEL1_H__165E5F11_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
