#if !defined(AFX_VIEWLV1_H__165E5F12_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
#define AFX_VIEWLV1_H__165E5F12_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// viewlv1.h : header file
//
#include "..\gridctrl\GridCtrl.h"
#include "L1Grid.h"
#include "..\Functions\secwatchlst.h"

/////////////////////////////////////////////////////////////////////////////
// ViewLv1 view

class ViewLv1 : public CView
{
protected:
	ViewLv1();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ViewLv1)

// Attributes
public:
	secwatchlst mlv1watchlist;

	L1Grid * mpGrid;
//	CGridCtrl * mpGrid;
// Operations
public:
	int AddSecSem(LPCSTR secsym);
	int ShowL1Info(LPCSTR secsym,double askprice,int asksize,double bidprice,int bidsize,BYTE Condition);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViewLv1)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL
	afx_msg void labelchange( NMHDR * pNotifyStruct, LRESULT * result );


// Implementation
protected:
	virtual ~ViewLv1();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(ViewLv1)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWLV1_H__165E5F12_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
