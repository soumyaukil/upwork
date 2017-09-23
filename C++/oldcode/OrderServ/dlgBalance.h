#if !defined(AFX_DLGBALANCE_H__D501C892_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_)
#define AFX_DLGBALANCE_H__D501C892_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgBalance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlgBalance dialog

class dlgBalance : public CDialog
{
// Construction
public:
	dlgBalance(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlgBalance)
	enum { IDD = IDD_DLGBALANCE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlgBalance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlgBalance)
	afx_msg void OnBRefresh();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGBALANCE_H__D501C892_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_)
