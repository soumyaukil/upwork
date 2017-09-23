#if !defined(AFX_DRAGDROPTEXTBOX_H__7F6435F2_6F98_11D4_BCF9_00105A82D2D1__INCLUDED_)
#define AFX_DRAGDROPTEXTBOX_H__7F6435F2_6F98_11D4_BCF9_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// dragdroptextbox.h : header file
//
#include "ECMaterialDropText.h"


/////////////////////////////////////////////////////////////////////////////
// CDragDropTextBox window

class CDragDropTextBox : public CEdit
{
	DECLARE_DYNAMIC(CDragDropTextBox)

// Construction
public:
	CDragDropTextBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDragDropTextBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitDrag();
	virtual ~CDragDropTextBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDragDropTextBox)
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:

	ECMaterialDropText		*m_TargetDrop;
	CPoint					m_StartPoint;
	UINT					m_TimerID;


	friend class ECMaterialDropText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAGDROPTEXTBOX_H__7F6435F2_6F98_11D4_BCF9_00105A82D2D1__INCLUDED_)
