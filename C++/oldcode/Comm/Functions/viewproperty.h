#if !defined(AFX_VIEWPROPERTY_H__5BD496F1_50EC_11D4_BCE1_00105A82D2D1__INCLUDED_)
#define AFX_VIEWPROPERTY_H__5BD496F1_50EC_11D4_BCE1_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// viewproperty.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// ViewProperty form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "..\GRIDCTRL\GridCtrl.h"	// Added by ClassView
#include "Property.h" 

class ViewProperty : public CFormView
{
protected:
	ViewProperty();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(ViewProperty)

// Form Data
public:
	//{{AFX_DATA(ViewProperty)
	enum { IDD = IDD_DLGProperty };
	CButton	m_bSort;
	CButton	m_bAdd;
	CButton	m_bSave;
	CButton	m_bDelete;
	CButton	m_bquit;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	int InitProperty(Property * pproperty);
	CGridCtrl mGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ViewProperty)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ArrangeWindow();
	virtual ~ViewProperty();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(ViewProperty)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnBAdd();
	afx_msg void OnBDelete();
	afx_msg void OnBSave();
	afx_msg void OnBQuit();
	afx_msg void OnBSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	Property *mpproperty;
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// frmPorperty frame

class frmPorperty : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmPorperty)
protected:
	frmPorperty();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	int InitProperty(LPCSTR Title,Property *pproperty);
	ViewProperty * mpview;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmPorperty)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmPorperty();

	// Generated message map functions
	//{{AFX_MSG(frmPorperty)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPROPERTY_H__5BD496F1_50EC_11D4_BCE1_00105A82D2D1__INCLUDED_)
