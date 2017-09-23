#if !defined(AFX_FRMMGMT_H__EADD2191_6741_11D5_BD4B_0050041B8FEC__INCLUDED_)
#define AFX_FRMMGMT_H__EADD2191_6741_11D5_BD4B_0050041B8FEC__INCLUDED_

#include "FUNCTIONS\ControlPos.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// frmmgmt.h : header file
//
#include "..\account.h"
/////////////////////////////////////////////////////////////////////////////
// viewSecIfo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "GRIDCTRL\GridCtrl.h"	// Added by ClassView

class viewSecIfo : public CFormView
{
protected:
	viewSecIfo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(viewSecIfo)

// Form Data
public:
	//{{AFX_DATA(viewSecIfo)
	enum { IDD = IDD_SECIFO };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:
	BOOL GetRowIfo(int row, IssueIfo *pifo);
	int SetGridRowStatus(int row,int flag);
	int FindRowInGrid(LPCSTR str);
	void ShowIfo(int row,IssueIfo *pifo);
	BOOL SaveData(IssueIfo *pifo);
	int LoadData(IssueIfo *pifo);
	void OnGridLMouseClick(NMHDR *pNotifyStruct, LRESULT *result);
	void AddIfo(IssueIfo *pifo);
	void InitGrid();
	CControlPos mPoshelper;
	CGridCtrl mGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(viewSecIfo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~viewSecIfo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(viewSecIfo)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBLoadShortLst();
	afx_msg void OnBdel();
	afx_msg void OnBClear();
	afx_msg void OnBUndel();
	afx_msg void OnBChange();
	afx_msg void OnBFind();
	afx_msg void OnBComitChange();
	afx_msg void OnBSort();
	afx_msg void OnFilePrint();
	afx_msg void OnBLoadUnShortLst();
	afx_msg void OnBRemoveAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// frmSecIfo frame

class frmSecIfo : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmSecIfo)
protected:
	frmSecIfo();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	viewSecIfo * pview;
	static frmSecIfo * pfrm;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmSecIfo)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmSecIfo();

	// Generated message map functions
	//{{AFX_MSG(frmSecIfo)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// frmEquity frame
class viewEquity;

class frmEquity : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmEquity)
protected:
	frmEquity();           // protected constructor used by dynamic creation

// Attributes
public:
	static frmEquity *pfrm;
	viewEquity  *pview;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmEquity)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmEquity();

	// Generated message map functions
	//{{AFX_MSG(frmEquity)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// viewEquity form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class viewEquity : public CFormView
{
protected:
	viewEquity();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(viewEquity)

// Form Data
public:
	//{{AFX_DATA(viewEquity)
	enum { IDD = IDD_EQUITY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CControlPos mPoshelper;
	CGridCtrl mGrid;


// Operations
public:
	int FindRowInGrid(LPCSTR str);
	BOOL LoadRowInfo(int row);
	void OnGridLMouseClick(NMHDR *pNotifyStruct, LRESULT *result);
	int ShowIfo(int row,Account *pacc);
	int AddInfo(Account *pacc);
	void InitGrid();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(viewEquity)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~viewEquity();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(viewEquity)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBFind();
	afx_msg void OnBSort();
	afx_msg void OnBChange();
	afx_msg void OnBLoadBuypower();
	afx_msg void OnBLoadEquity();
	afx_msg void OnBComitChange();
	afx_msg void OnFilePrint();
	afx_msg void OnBLoadCCSBalance();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// frmTraders frame

class viewTraders;
class frmTraders : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmTraders)
protected:
	frmTraders();           // protected constructor used by dynamic creation
// Attributes
public:
	static frmTraders *pfrm;
	viewTraders *pview;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmTraders)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmTraders();

	// Generated message map functions
	//{{AFX_MSG(frmTraders)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

class viewTraders : public CFormView
{
protected:
	viewTraders();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(viewTraders)

// Form Data
public:
	//{{AFX_DATA(viewTraders)
	enum { IDD = IDD_TRADERS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CGridCtrl mGrid;
	CControlPos mPoshelper;


// Operations
public:
	int AddIfo(CClient *pclient);
	void InitGrid();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(viewTraders)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~viewTraders();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(viewTraders)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBRefresh();
	afx_msg void OnBKickOut();
	afx_msg void OnBSortAsc();
	afx_msg void OnBSortDec();
	afx_msg void OnBCount();
	afx_msg void OnBBlock();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMMGMT_H__EADD2191_6741_11D5_BD4B_0050041B8FEC__INCLUDED_)
