#if !defined(AFX_MSPLITTER_H__B4A44D57_1F31_4741_A19E_F1988AA8FDF0__INCLUDED_)
#define AFX_MSPLITTER_H__B4A44D57_1F31_4741_A19E_F1988AA8FDF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MSplitter.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMSplitter window
class CMSplitter;

class CMSplitter_Info
{
	CMSplitter_Info *m_Parent;
	CMSplitter_Info *m_FirstChild;
	CMSplitter_Info *m_Prev;
	CMSplitter_Info *m_Next;

	int m_nTotalChild;

	
	CMSplitter *m_Splitter_Wnd;
public:
	int m_type; //0 horizonal, 1: vertical;

	CRect m_rect_wnd;
	CRect m_rect_splitter;
	HWND  m_wnd;
	double m_Percent; 

public:
	CMSplitter_Info();
	~CMSplitter_Info();

public:
	void PaintGribber(CDC *pdc);
	CMSplitter_Info * GetSplitterByPt(CPoint point);
	void SizeAlloc(LPRECT prect,LPRECT pSplitter);
	CMSplitter_Info* GetChild(int nidx);
	CMSplitter_Info *AddWnd(HWND hwnd,int nidx);
	CMSplitter_Info *Next() { return m_Next;};
	CMSplitter_Info *Prev() { return m_Prev;};
	CMSplitter_Info *FirstChild() { return m_FirstChild;};
	int GetMoveType() {	return m_Parent->m_type;}
	int		GetChildNum() { return m_nTotalChild;};
	void SetSplitter(CMSplitter *pSplitter) {
		if (m_Splitter_Wnd==NULL) m_Splitter_Wnd = pSplitter;
	}

};


class CMSplitter : public CWnd
{
// Construction
public:
	CMSplitter();
	
// Attributes
public:	
	CMSplitter_Info m_SplitterHead;
	int m_BoardSize;// the splitter boarder size;//usually is 4

private:
	//for mouse move;
	CMSplitter_Info *m_CurrentSplitter;
	CPoint m_lastpt;
	CPoint m_lastSpTopLeft;
	double m_pc1,m_pc2;
	double m_pixperpc;
	int    m_lastmovetype;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMSplitter)
	//}}AFX_VIRTUAL

// Implementation
public:
	void RecalcLayout();
	virtual ~CMSplitter();
	// special command routing to frame

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// Generated message map functions
protected:
	//{{AFX_MSG(CMSplitter)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSPLITTER_H__B4A44D57_1F31_4741_A19E_F1988AA8FDF0__INCLUDED_)
