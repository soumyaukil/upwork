#if !defined(AFX_SPINBUTTON_H__0C239811_5112_11D5_BD3C_0050041B8FEC__INCLUDED_)
#define AFX_SPINBUTTON_H__0C239811_5112_11D5_BD3C_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// spinbutton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SpinButton window

#define WM_SPINBUT WM_USER+123
class SpinButton : public CWnd
{
// Construction
	int mstatus;
	int mreptstat;
	int mrepval;
	HWND moldHWND;
public:
	SpinButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SpinButton)
	//}}AFX_VIRTUAL

// Implementation
public:
	void DrawButton(CDC *pdc, LPRECT prec,int stat);
	void DrawDnArrow(CDC *pdc, LPRECT dwnrec);
	void DrawUpArrow(CDC *pdc,LPRECT uprect);
	BOOL CreateFromStatic(UINT nID, CWnd *pParent);
	virtual ~SpinButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(SpinButton)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPINBUTTON_H__0C239811_5112_11D5_BD3C_0050041B8FEC__INCLUDED_)
