#if !defined(AFX_SHEETFRAME_H__B7060901_0F23_11D4_BC88_00105A82D2D1__INCLUDED_)
#define AFX_SHEETFRAME_H__B7060901_0F23_11D4_BC88_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// sheetframe.h : header file
//
#define FTS_FULLBORDER	0x1	

class CSheetTab {
private:
	CString	m_sText; // tab text
	CRect		m_rect;			// bounding rect
	CRgn		m_rgn;			// polygon region to fill (trapezoid)
	void		GetTrapezoid(const CRect& rc, CPoint* pts) const;
	int			mflag;
	friend class CFolderTabCtrl;

public:
	CRect		m_origrect; // the origin rect;
	void MoveRgn(int offset);
	BOOL		HitTest(CPoint pt)			{ return m_rgn.PtInRegion(pt); }

	int		Draw(CDC& dc, CFont& font, BOOL bSelected);
	int		ComputeRgn(CDC& dc, int x);
	CRect		GetRect() const				{ return m_rect; }
	CSheetTab(LPCTSTR lpszText,int flag) : m_sText(lpszText) {mflag=flag;}

	LPCTSTR	GetText() const				{ return m_sText; }
	void  	SetText(LPCTSTR lpszText)	{ m_sText = lpszText; }
};

enum { FTN_SHEETCHANGED = 1 };				 // notification: tab changed

struct NMSHEETTAB : public NMHDR {		 // notification struct
	int iItem;										 // item index
	const CSheetTab* pItem;					 // ptr to data, if any
};

/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl window

class CSheetTabCtrl : public CWnd
{
protected:
	CPtrList		m_lsTabs;					 // array of CFolderTabs
	DWORD			m_dwFtabStyle;				 // folder tab style flags
	int			m_iCurItem;					 // current selected tab
	CFont			m_fontNormal;				 // current font, normal ntab
	CFont			m_fontSelected;			 // current font, selected tab
	int			m_nDesiredWidth;			 // exact fit width
	CRect		mRectbLeft;// the left button
	CRect		mRectbRight;// the right button
	int			mMoveflag;
	int		    mFirstTab;// the first tab;
	//bit 1:	top table;
	int			mflag;
// Construction
public:
	CSheetTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetFlag(int flag) {mflag=flag;}
	BOOL RemoveAllItem();
	void MoveTab(int ntab);
	int DrawButton(CDC &dc,CRect *prect,int flag);
	void InvalidateTab(int iTab, BOOL bErase=TRUE);
	int SelectItem(int iTab);
	int HitTest(CPoint pt);
	void RecomputeLayout();
	CSheetTab *GetTab(int iPos);
	BOOL CreateFromStatic(UINT nID, CWnd* pParent);
	virtual BOOL Create(DWORD dwWndStyle, const RECT& rc,
		CWnd* pParent, UINT nID, DWORD dwFtabStyle=0);
	virtual BOOL Load(UINT nIDRes);
	CSheetTab* GetItem(int iPos)		{ return (CSheetTab*)GetTab(iPos); }
	int	GetSelectedItem()				{ return m_iCurItem; }
	int	GetItemCount()					{ return m_lsTabs.GetCount(); }
	int	GetDesiredWidth()				{ return m_nDesiredWidth; }
	int	GetDesiredHeight()			{ return GetSystemMetrics(SM_CYHSCROLL); }
	void SetFonts(CFont& fontNormal, CFont& fontSelected);

	int AddItem(LPCTSTR lpszText,int pos=-1);
	virtual ~CSheetTabCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetTabCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CSheetFrame window

class CSheetFrame : public CWnd
{
// Construction
private:

	CSheetTabCtrl	m_wndSheetTabCtrl;	 // folder tab
	CScrollBar		m_wndSBHorz;			 // horizontal scroll bar..
	CScrollBar		m_wndSBVert;			 // ... vertical
	CScrollBar		m_wndSBBox;				 // ... and box
	int				m_cxSheetTabCtrl;	 // width of folder tab
	UINT				m_nIDRes;				 // resource ID for tab names
public:
	CSheetFrame();
	enum  { bestFit=-1, hide=-2 };
	CSheetTabCtrl & GetSheetTabCtrl()			 { return m_wndSheetTabCtrl; }
	CWnd* GetView(UINT nID=AFX_IDW_PANE_FIRST) { return GetDlgItem(nID); }
	int	GetSheetTabWidth()						 { return m_cxSheetTabCtrl; }



// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetFrame)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd* pParent, CRuntimeClass* pViewClass,CCreateContext* pcc, UINT nIDRes, int cxFolderTabCtrl, DWORD dwStyle= WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN);

	void UpdateBarSize();

	void ShowControls(int width);
	void SetSheetTabWidth(int width);
	CScrollBar * GetScrollBar(int nBar) const;
	virtual ~CSheetFrame();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSheetFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// CSheetView view

class CSheetView : public CView
{
protected:
	CSheetView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSheetView)

// Attributes
public:

// Operations
public:
	virtual void OnChangeSheet(int i);
	CSheetFrame * GetSheetFrame() const;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSheetView)
	public:
	virtual CScrollBar* GetScrollBarCtrl(int nBar) const;
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSheetView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSheetView)
		afx_msg void OnChangedTab(NMSHEETTAB* nmtab, LRESULT* pRes);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHEETFRAME_H__B7060901_0F23_11D4_BC88_00105A82D2D1__INCLUDED_)
