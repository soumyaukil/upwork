#if !defined(AFX_DLGIPBOX_H__D501C891_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_)
#define AFX_DLGIPBOX_H__D501C891_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dlgIPBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dlgIPBox dialog

class dlgIPBox : public CDialog
{
// Construction
public:
	WORD GetPort();
	void SetPort(WORD nport);
	void SetTitle(char *ptitle);
	BOOL SetIP(DWORD nip);
	DWORD GetIP();
	char mTitle	[200];
	char mIPStr	[100];
	WORD mPort	;

	dlgIPBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dlgIPBox)
	enum { IDD = IDD_DIGIPINPUT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(dlgIPBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(dlgIPBox)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIPBOX_H__D501C891_8A6F_11D5_BD5D_0050041B8FEC__INCLUDED_)
