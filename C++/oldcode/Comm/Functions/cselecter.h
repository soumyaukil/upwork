#if !defined(AFX_COLUMESELECTER_H__0FA60A63_8DBE_42B9_87D5_3DD8CE22F445__INCLUDED_)
#define AFX_COLUMESELECTER_H__0FA60A63_8DBE_42B9_87D5_3DD8CE22F445__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// columeselecter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCSelecter window

class CCSelecter : public CWnd
{
// Construction
public:
	CCSelecter();

protected:
	CListBox mlist1;
	CListBox mlist2;
	CButton	 mbutadd;
	CButton  mbutrem;
	CButton  mbutup;
	CButton  mbutdn;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSelecter)
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL CreateFromStatic(UINT nid, CWnd *pParent);
	int GetItems(int *sel);
	void SetItems(char **items,int *sel);
	void OnDown();
	void OnUp();
	void OnRemove();
	void OnAdd();
	BOOL Create(RECT &rc,CWnd *pParent, UINT nid);
	virtual ~CCSelecter();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCSelecter)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLUMESELECTER_H__0FA60A63_8DBE_42B9_87D5_3DD8CE22F445__INCLUDED_)
