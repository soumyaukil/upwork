// OrderServDoc.h : interface of the COrderServDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERSERVDOC_H__8FF81F25_F460_11D3_BC5D_00105A82D2D1__INCLUDED_)
#define AFX_ORDERSERVDOC_H__8FF81F25_F460_11D3_BC5D_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COrderServDoc : public CDocument
{
protected: // create from serialization only
	COrderServDoc();
	DECLARE_DYNCREATE(COrderServDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COrderServDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COrderServDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COrderServDoc)
	afx_msg void OnFileOpen();
	afx_msg void OnFileClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ORDERSERVDOC_H__8FF81F25_F460_11D3_BC5D_00105A82D2D1__INCLUDED_)
