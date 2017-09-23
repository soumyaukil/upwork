#if !defined(AFX_LOGFRAME_H__567B97C4_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_)
#define AFX_LOGFRAME_H__567B97C4_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// logframe.h : header file
//

#ifndef afx_msg

#define afx_msg         // intentional placeholder

#endif



/////////////////////////////////////////////////////////////////////////////
// CLogFrame frame
class CStreamLog;

class CLogFrame : public CMDIChildWnd
{
protected:
	CLogFrame();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void SetLogObject(CStreamLog * plog);
	CEdit medit;
	CStreamLog * plog;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CLogFrame();

	// Generated message map functions
	//{{AFX_MSG(CLogFrame)
	afx_msg void OnDestroy();
	afx_msg void OnUpadtelog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGFRAME_H__567B97C4_5D79_11D4_BCF7_00105A82D2D1__INCLUDED_)
