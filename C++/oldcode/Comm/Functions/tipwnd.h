#if !defined(AFX_TIPWND_H__F6D0AE91_694E_11D4_BCF9_00105A82D2D1__INCLUDED_)
#define AFX_TIPWND_H__F6D0AE91_694E_11D4_BCF9_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// tipwnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// TipWnd window

class TipWnd : public CWnd
{
// Construction
public:
	TipWnd();

// Attributes
public:
	CWnd *m_pParentWnd;
	CRect m_rectTitle;
    CRect m_rectHover;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(TipWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	void Show(LPPOINT ppos,LPCSTR str);
	void Show(CRect rectTitle,LPCSTR lptitletext,
		int xoffset, LPRECT lpHoverRect, LPLOGFONT lpLogFont1=NULL);
	void Hide();
	BOOL Create(CWnd *pParentWnd);
	virtual ~TipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(TipWnd)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPWND_H__F6D0AE91_694E_11D4_BCF9_00105A82D2D1__INCLUDED_)
