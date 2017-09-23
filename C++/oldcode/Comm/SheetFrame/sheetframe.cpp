// sheetframe.cpp : implementation file
//

#include "stdafx.h"
#include "sheetframe.h"
#include <afxpriv.h> // for WM_INITIALUPDATE

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const MINSCROLLBARWIDTH = 4*GetSystemMetrics(SM_CXVSCROLL);

const CXOFFSET = 8;		// defined pitch of trapezoid slant
const CXMARGIN = 2;		// left/right text margin
const CYMARGIN = 1;		// top/bottom text margin
const CYBORDER = 1;		// top border thickness

//////////////////
// Compute the the points, rect and region for a tab.
// Input x is starting x pos.
//
int CSheetTab::ComputeRgn(CDC& dc, int x)
{
	CRect& rc = m_origrect;
	rc.SetRectEmpty();
	// calculate desired text rectangle
	dc.DrawText(m_sText, &rc, DT_CALCRECT);
	rc.right  += 2*CXOFFSET + 2*CXMARGIN;						// add margins
	rc.bottom = rc.top + GetSystemMetrics(SM_CYHSCROLL);	// ht = scrollbar ht
	rc += CPoint(x,0);												// shift right
	
	MoveRgn(0);
	return rc.Width();

}

//////////////////
// Given the boundint rect, compute trapezoid region.
// Note that the right and bottom edges not included in rect or
// trapezoid; these are normal rules of geometry. 
//
void CSheetTab::GetTrapezoid(const CRect& rc, CPoint* pts) const
{
	if (mflag &(1<<1)) {//top tab
		pts[0] = CPoint(rc.left,rc.bottom);
		pts[1] = CPoint(rc.left + CXOFFSET,   rc.top+1);
		pts[2] = CPoint(rc.right- CXOFFSET-1, rc.top+1);
		pts[3] = CPoint(rc.right-1, rc.bottom);
	}else {
		pts[0] = rc.TopLeft();
		pts[1] = CPoint(rc.left + CXOFFSET,   rc.bottom);
		pts[2] = CPoint(rc.right- CXOFFSET-1, rc.bottom);
		pts[3] = CPoint(rc.right-1, rc.top);
	}
}


//////////////////
// Draw tab in normal or highlighted state
//
int CSheetTab::Draw(CDC& dc, CFont& font, BOOL bSelected)
{
	COLORREF bgColor = GetSysColor(bSelected? COLOR_WINDOW     : COLOR_3DFACE);
	COLORREF fgColor = GetSysColor(bSelected? COLOR_WINDOWTEXT : COLOR_BTNTEXT);

	CBrush brush(bgColor);					 // background brush
	dc.SetBkColor(bgColor);					 // text background
	dc.SetTextColor(fgColor);				 // text color = fg color

	CPen blackPen(PS_SOLID, 1, RGB(0,0,0));	// black
	CPen shadowPen(PS_SOLID, 1, GetSysColor(COLOR_3DSHADOW));

	// Fill trapezoid
	CPoint pts[4];
	CRect rc = m_rect;
	GetTrapezoid(rc, pts);
	CPen* pOldPen = dc.SelectObject(&blackPen);
	dc.FillRgn(&m_rgn, &brush);

	// Draw edges. This is requires two corrections:
	// 1) Trapezoid dimensions don't include the right and bottom edges,
	// so must use one pixel less on bottom (cybottom)
	// 2) the endpoint of LineTo is not included when drawing the line, so
	// must add one pixel (cytop)
	//
	pts[1].y--;			// correction #1: true bottom edge y-coord
	pts[2].y--;			// ...ditto
	pts[3].y--;			// correction #2:	extend final LineTo
	dc.MoveTo(pts[0]);						// upper left
	dc.LineTo(pts[1]);						// bottom left
	dc.SelectObject(&shadowPen);			// bottom line is shadow color
	dc.MoveTo(pts[1]);						// line is inside trapezoid bottom
	dc.LineTo(pts[2]);						// ...
	dc.SelectObject(&blackPen);			// upstroke is black
	dc.LineTo(pts[3]);						// y-1 to include endpoint
	if (!bSelected) {
		// if not highlighted, upstroke has a 3D shadow, one pixel inside
		pts[2].x--;		// offset left one pixel
		pts[3].x--;		// ...ditto
		dc.SelectObject(&shadowPen);
		dc.MoveTo(pts[2]);
		dc.LineTo(pts[3]);
	}
	dc.SelectObject(pOldPen);

	// draw text
	rc.DeflateRect(CXOFFSET + CXMARGIN, CYMARGIN);
	CFont* pOldFont = dc.SelectObject(&font);
	dc.DrawText(m_sText, &rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	dc.SelectObject(pOldFont);

	return m_rect.right;
}


/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl

CSheetTabCtrl::CSheetTabCtrl()
{
	m_iCurItem = 0;
	m_dwFtabStyle = 0;
	m_nDesiredWidth = 0;
	mMoveflag=0; 
	mFirstTab=0; 
	mflag=0;
}

CSheetTabCtrl::~CSheetTabCtrl()
{
	while (!m_lsTabs.IsEmpty())
	delete (CSheetTab*)m_lsTabs.RemoveHead();
}


BEGIN_MESSAGE_MAP(CSheetTabCtrl, CWnd)
	//{{AFX_MSG_MAP(CSheetTabCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSheetTabCtrl message handlers

int CSheetTabCtrl::AddItem(LPCTSTR lpszText,int pos)
{
	POSITION p1;
	p1=m_lsTabs.FindIndex( pos);
	if (p1==NULL) {
		m_lsTabs.AddTail(new CSheetTab(lpszText,mflag));
	}else{
		m_lsTabs.InsertBefore(p1, new CSheetTab(lpszText,mflag));
	}
	return m_lsTabs.GetCount() - 1;
}

BOOL CSheetTabCtrl::CreateFromStatic(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	rc.bottom = rc.top + GetDesiredHeight();
	return Create(WS_CHILD|WS_VISIBLE,rc, pParent, nID);

}
BOOL CSheetTabCtrl::Create(DWORD dwStyle, const RECT& rc,
	CWnd* pParent, UINT nID, DWORD dwFtabStyle)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	m_dwFtabStyle = dwFtabStyle;

	static LPCTSTR lpClassName = _T("LiuSheetTab");
	static BOOL bRegistered = FALSE; // registered?
	if (!bRegistered) {
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		if (!AfxRegisterClass(&wc)) {
			TRACE("*** CFolderTabCtrl::AfxRegisterClass failed!\n");
			return FALSE;
		}
		bRegistered = TRUE;
	}
	if (!CWnd::CreateEx(0, lpClassName, NULL, dwStyle, rc, pParent, nID))
		return FALSE;

	// initialize fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = GetSystemMetrics(SM_CYHSCROLL)-CYMARGIN;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	m_fontNormal.CreateFontIndirect(&lf);
	//lf.lfHeight = 1;
	m_fontSelected.CreateFontIndirect(&lf);

	return TRUE;
}

//////////////////
// copy a font
//
static void CopyFont(CFont& dst, CFont& src)
{
	dst.DeleteObject();
	LOGFONT lf;
	VERIFY(src.GetLogFont(&lf));
	dst.CreateFontIndirect(&lf);
}

//////////////////
// Set normal, selected fonts
//
void CSheetTabCtrl::SetFonts(CFont& fontNormal, CFont& fontSelected)
{
	CopyFont(m_fontNormal, fontNormal);
	CopyFont(m_fontSelected, fontSelected);
}

BOOL CSheetTabCtrl::Load(UINT nIDRes)
{
	CString s;
	if (!s.LoadString(nIDRes))
		return FALSE;

	CString sTab;
	for (int i=0; AfxExtractSubString(sTab, s, i); i++) {
		AddItem(sTab);
	}
	RecomputeLayout();
	return TRUE;
}



CSheetTab * CSheetTabCtrl::GetTab(int iPos)
{
	POSITION pos = m_lsTabs.FindIndex(iPos);
	return pos ? static_cast<CSheetTab*>(m_lsTabs.GetAt(pos)) : NULL;

}

void CSheetTabCtrl::RecomputeLayout()
{
	CClientDC dc(this);
	CFont* pOldFont = dc.SelectObject(&m_fontNormal);
	int x = 0;
	int n = GetItemCount();
	CSheetTab* pTab;
	pTab=NULL;
	m_nDesiredWidth=0;
	for (int i=0; i<n; i++) {
		pTab = GetTab(i);
		if (pTab) 
			x += pTab->ComputeRgn(dc, x) - CXOFFSET;
	}
	dc.SelectObject(pOldFont);

	if (pTab) {
		CRect rc = pTab->GetRect();
		m_nDesiredWidth = rc.right;
	}

}

void CSheetTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	CSheetTab* pCurTab = NULL;

	// draw all the normal (non-selected) tabs
	int n = GetItemCount();
	for (int i=0; i<n; i++) {
		CSheetTab* pTab = GetTab(i);
		ASSERT(pTab);
		if (i==m_iCurItem) {
			pCurTab = pTab;
		} else if (pTab->Draw(dc, m_fontNormal, FALSE) > rc.right)
			break;
	}
	// draw selected tab last so it will be "on top" of the others
	if (pCurTab)
		pCurTab->Draw(dc, m_fontSelected, TRUE);

	// draw border: line along the top edge, excluding seleted tab
	CRect rcCurTab(0,0,0,0);
	if (pCurTab)
		rcCurTab = pCurTab->GetRect();

	CPen blackPen(PS_SOLID, 1, RGB(0,0,0));	// black
	CPen* pOldPen = dc.SelectObject(&blackPen);
	int yborder;
	if (mflag&(1<<1)) yborder=rcCurTab.bottom;
	else yborder=rcCurTab.top;
	dc.MoveTo(rcCurTab.right, yborder);
	dc.LineTo(rc.right, yborder);
	if (m_dwFtabStyle & FTS_FULLBORDER) {
		dc.MoveTo(rc.right-1, rc.top);
		dc.LineTo(rc.right-1, rc.bottom-1);
		dc.LineTo(rc.left,  rc.bottom-1);
		dc.LineTo(rc.left,  rc.top);
	} else {
		dc.MoveTo(rc.left, yborder);
	}
	dc.LineTo(rcCurTab.left,yborder);
	if (mMoveflag&1)	DrawButton(dc,&mRectbLeft,0);
	if (mMoveflag&2)	DrawButton(dc,&mRectbRight,1);
	
	dc.SelectObject(pOldPen);
}

void CSheetTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (mMoveflag&1) {// the move left 
		if (mRectbLeft.PtInRect(point)) {
			MoveTab(mFirstTab-1);
			return;
		}
	}
	if (mMoveflag&2) {// the move left 
		if (mRectbRight.PtInRect(point)) {
			MoveTab(mFirstTab+1);
			return;
		}
	}
	int iTab = HitTest(point);
	if (iTab>=0 && iTab!=m_iCurItem) {
		SelectItem(iTab);
		NMSHEETTAB nm;
		nm.hwndFrom = m_hWnd;
		nm.idFrom = GetDlgCtrlID();
		nm.code = FTN_SHEETCHANGED;
		nm.iItem = iTab;
		nm.pItem = GetTab(iTab);
		CWnd* pParent = GetParent();
		pParent->SendMessage(WM_NOTIFY, nm.idFrom, (LPARAM)&nm);
	}
}

int CSheetTabCtrl::HitTest(CPoint pt)
{
	CRect rc;
	GetClientRect(&rc);
	if (rc.PtInRect(pt)) {
		int n = GetItemCount();
		for (int i=0; i<n; i++) {
			if (GetTab(i)->HitTest(pt))
				return i;
		}
	}
	return -1;

}

int CSheetTabCtrl::SelectItem(int iTab)
{
	if (iTab<0 || iTab>=GetItemCount())
		return -1;		// bad
	if (iTab==m_iCurItem)
		return iTab;	// already selected

	InvalidateTab(m_iCurItem);		// invalidate old tab (repaint)
	m_iCurItem = iTab;				// set new selected tab
	InvalidateTab(m_iCurItem);		// repaint new tab
	
	return m_iCurItem;

}

void CSheetTabCtrl::InvalidateTab(int iTab, BOOL bErase)
{
	InvalidateRect(GetTab(iTab)->GetRect(), bErase);
}
/////////////////////////////////////////////////////////////////////////////
// CSheetFrame

CSheetFrame::CSheetFrame()
{
	m_cxSheetTabCtrl=hide;
}

CSheetFrame::~CSheetFrame()
{
}


BEGIN_MESSAGE_MAP(CSheetFrame, CWnd)
	//{{AFX_MSG_MAP(CSheetFrame)
	ON_WM_CREATE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSheetFrame message handlers

BOOL CSheetFrame::Create(CWnd* pParent, CRuntimeClass* pViewClass,
	CCreateContext* pcc, UINT nIDRes, int cxSheetTabCtrl, DWORD dwStyle)
{
	ASSERT(pParent);
	ASSERT(dwStyle & WS_CHILD);

	static LPCTSTR lpClassName = NULL;
	if (!lpClassName) {
		lpClassName = _T("PxlFolderFrame");
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = 0;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = NULL; //CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		VERIFY(AfxRegisterClass(&wc));
	}
	m_nIDRes = nIDRes;
	SetSheetTabWidth(cxSheetTabCtrl);

	CRect rc(0,0,0,0);
	if (!CreateEx(0,lpClassName,NULL,dwStyle,rc,pParent,AFX_IDW_PANE_FIRST))
		return FALSE;
	
	CWnd* pView = (CWnd*)pViewClass->CreateObject();
	ASSERT(pView);
	if (!pView->Create(NULL, NULL, dwStyle, rc, this, AFX_IDW_PANE_FIRST, pcc))
		return FALSE;

	// pView->SendMessage(WM_INITIALUPDATE);
	return TRUE;
}

CScrollBar * CSheetFrame::GetScrollBar(int nBar) const
{
	return nBar==SB_HORZ ? const_cast<CScrollBar*>(&m_wndSBHorz)
		: nBar==SB_VERT ? const_cast<CScrollBar*>(&m_wndSBVert) : NULL;
}

int CSheetFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rc(0,0,0,0);
	VERIFY(m_wndSBHorz.Create(WS_VISIBLE|WS_CHILD|SBS_HORZ,
		rc, this, AFX_IDW_HSCROLL_FIRST));
	VERIFY(m_wndSBVert.Create(WS_VISIBLE|WS_CHILD|SBS_VERT,
		rc, this, AFX_IDW_HSCROLL_FIRST+1));
	VERIFY(m_wndSBBox.Create(WS_VISIBLE|WS_CHILD|SBS_SIZEBOX,
		rc, this,-1));
	
	CSheetTabCtrl& ftc = m_wndSheetTabCtrl;
	VERIFY(ftc.Create(WS_CHILD|WS_VISIBLE, rc, this, 1));
	if (m_nIDRes)
		ftc.Load(m_nIDRes);

	ShowControls(m_cxSheetTabCtrl);

	return 0;
}



BOOL CSheetFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (GetView()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;
	return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CSheetFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	GetView()->SendMessage(WM_HSCROLL,
		MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
}

void CSheetFrame::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default
	GetView()->SendMessage(WM_VSCROLL,
	MAKELONG(nSBCode, nPos), (LPARAM)pScrollBar->m_hWnd);
}

void CSheetFrame::OnSize(UINT nType, int cx, int cy) 
{
	CWnd* pView = GetView();
	if (!pView)
		return;

	CRect rc(0,0,cx,cy);

	// First, size the view.
	// 
	int cxSheetTabCtrl = m_cxSheetTabCtrl;
	if (cxSheetTabCtrl > (rc.Width()- MINSCROLLBARWIDTH))
		// make room for horz scrollbar
		cxSheetTabCtrl = rc.Width()- MINSCROLLBARWIDTH;
	int cyHScroll = GetSystemMetrics(SM_CYHSCROLL);
	int cxVScroll = GetSystemMetrics(SM_CXVSCROLL);
	if (cxSheetTabCtrl>=0) {
		rc.right  -= cxVScroll;
		rc.bottom -= cyHScroll;
	}
	pView->MoveWindow(rc);

	// Next, move the scroll bars: horz, vert and box
	// 
	if (cxSheetTabCtrl<0)
		return;
	rc.left   = rc.right;
	rc.right += cxVScroll;
	m_wndSBVert.MoveWindow(rc);

	rc.left   = max(cxSheetTabCtrl,0);
	rc.right -= cxVScroll;
	rc.top    = rc.bottom;
	rc.bottom+= cyHScroll;
	m_wndSBHorz.MoveWindow(rc);

	if (cxSheetTabCtrl>=0) {
		CRect rc2 = rc;
		rc2.left  = 0;
		rc2.right = cxSheetTabCtrl;
		m_wndSheetTabCtrl.MoveWindow(rc2);
	}

	rc.left = rc.right;
	rc.right+=cxVScroll;
	m_wndSBBox.MoveWindow(rc);
	
}

BOOL CSheetFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
//	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style |= WS_CLIPCHILDREN;
	cs.style &= ~WS_BORDER;
   return CWnd::PreCreateWindow(cs);
}

void CSheetFrame::SetSheetTabWidth(int width)
{
	if (width>0) {
		CRect rc;
		GetClientRect(&rc);
//		if (width > (rc.Width()- MINSCROLLBARWIDTH)) 
//			width = rc.Width()- MINSCROLLBARWIDTH;
	}
	if (width != m_cxSheetTabCtrl) {
		m_cxSheetTabCtrl = width;
		UpdateBarSize();
	}
}

void CSheetFrame::ShowControls(int width)
{
	if (width == bestFit)
		width = m_wndSheetTabCtrl.GetDesiredWidth();
	SetSheetTabWidth(width);

	int iShow = m_cxSheetTabCtrl>0 ? SW_SHOW : SW_HIDE;
	m_wndSBVert.ShowWindow(iShow);
	m_wndSBHorz.ShowWindow(iShow);
	m_wndSBBox.ShowWindow(iShow);
	m_wndSheetTabCtrl.ShowWindow(iShow);
	if (iShow==SW_SHOW)
		UpdateBarSize();

}

void CSheetFrame::UpdateBarSize()
{
	if (IsWindow(m_hWnd)) { 
	CRect rc;
	GetClientRect(&rc);
	SendMessage(WM_SIZE, 0, MAKELPARAM(rc.Width(), rc.Height()));
	}

}
/////////////////////////////////////////////////////////////////////////////
// CSheetView

IMPLEMENT_DYNCREATE(CSheetView, CView)

CSheetView::CSheetView()
{
}

CSheetView::~CSheetView()
{
}


BEGIN_MESSAGE_MAP(CSheetView, CView)
	//{{AFX_MSG_MAP(CSheetView)
	  ON_NOTIFY(FTN_SHEETCHANGED, 1, OnChangedTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSheetView drawing

//DEL void CSheetView::OnInitialUpdate()
//DEL {
//DEL //	CScrollView::OnInitialUpdate();
//DEL 
//DEL 	CSize sizeTotal;
//DEL 	// TODO: calculate the total size of this view
//DEL 	sizeTotal.cx = sizeTotal.cy = 100;
//DEL //	SetScrollSizes(MM_TEXT, sizeTotal);
//DEL }

void CSheetView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CSheetView diagnostics

#ifdef _DEBUG
void CSheetView::AssertValid() const
{
	CView::AssertValid();
}

void CSheetView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSheetView message handlers

CSheetFrame * CSheetView::GetSheetFrame() const
{
	CSheetFrame* pSheetFrame = (CSheetFrame*)GetParent();
	ASSERT(pSheetFrame);
//	ASSERT_KINDOF(CSheetFrame, pSheetFrame);
	return pSheetFrame;
}

CScrollBar* CSheetView::GetScrollBarCtrl(int nBar) const
{
	// TODO: Add your specialized code here and/or call the base class
	CSheetFrame * psheet;
	psheet=GetSheetFrame();
	return psheet->GetScrollBar(nBar);

//	return CScrollView::GetScrollBarCtrl(nBar);
}


void CSheetView::OnChangedTab(NMSHEETTAB* nmtab, LRESULT* pRes)
{
	OnChangeSheet(nmtab->iItem);
}

void CSheetView::OnChangeSheet(int i)
{

}

void CSheetTabCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
//	MoveTab(mFirstTab);
	CRect rect1;
	GetClientRect(rect1);

	mRectbLeft =mRectbRight=rect1; 
	mRectbLeft.right=rect1.left+10;
	mRectbLeft.top=mRectbRight.top=rect1.bottom-10; 
	mRectbRight.right--;
	mRectbRight.left=mRectbRight.right-10;

	CSheetTab *ptab;
	mMoveflag=0;
	ptab=GetItem(mFirstTab);
	if(ptab!=NULL){
		CRect offsetrect;
		offsetrect=ptab->m_origrect;
		if (mFirstTab>0) mMoveflag|=1;
		if (rect1.Width()<(m_nDesiredWidth-offsetrect.left)) {
			if (mFirstTab<GetItemCount()-1) mMoveflag|=2;
		}
	}

/*	if (rect1.Width()<GetDesiredWidth()) {
		mMoveflag=3;
	}else {
		mMoveflag=0;
	}

	int sz=GetSystemMetrics(SM_CYHSCROLL)/2;
	mRectbLeft.top=rect1.bottom-sz; 
	mRectbLeft.right=rect1.left+sz;
	mRectbLeft.bottom--; 
	mRectbLeft.left++;
	mRectbRight.left=rect1.right -sz;
	mRectbRight.top =rect1.bottom -sz; 
	mRectbRight.bottom--;
	mRectbRight.right--; 
*/
	// TODO: Add your message handler code here
	
}

int CSheetTabCtrl::DrawButton(CDC &dc, CRect *prect, int flag)
{
// draw the arrows.
	CBrush *poldbrush;
	CPen *poldpen;
	CPen blackpen(PS_SOLID,1, RGB(0,0,255));	// black
	CPen whitepen(PS_SOLID,4,RGB(0,255,255));//white pen
	poldpen=dc.SelectObject(&whitepen);
	CBrush arrbrush(RGB(255,255,0));
	poldbrush=dc.SelectObject(&arrbrush);

	static CPoint pts0[]={CPoint(0,5),CPoint(6,9),CPoint(6,7),
					CPoint(9,7),CPoint(9,3),CPoint(6,3),
					CPoint(6,1)};
	static CPoint pts1[]={CPoint(10,5),CPoint(3,9),CPoint(3,7),
					CPoint(0,7),CPoint(0,3),CPoint(3,3),
					CPoint(3,1)};
	CPoint pts[7];
	int i;
	if (flag==0){
		for (i=0;i<7;i++) {
			pts[i].x=pts0[i].x+prect->left;
			pts[i].y=pts0[i].y+prect->top;
		}
	}else {
		for (i=0;i<7;i++) {
			pts[i].x=pts1[i].x+prect->left;
			pts[i].y=pts1[i].y+prect->top;
		}
	}

	dc.Polygon(pts,7);
	dc.SelectObject(&blackpen);
	dc.Polygon(pts,7);
	

	dc.SelectObject(poldpen);
	dc.SelectObject(poldbrush);
	return 0;
}


void CSheetTab::MoveRgn(int offset)
{
	TRACE(m_sText); 
	m_rect= m_origrect+CPoint(offset,0);
	m_rgn.DeleteObject();
	CPoint pts[4];
	GetTrapezoid(m_rect, pts);
	m_rgn.CreatePolygonRgn(pts, 4, WINDING);
}

void CSheetTabCtrl::MoveTab(int ntab)
{

	ASSERT(ntab>=0);
//	ASSERT(ntab<GetItemCount());
	int itemcount;
	itemcount=GetItemCount();
	CSheetTab *ptab;
	ptab=GetItem(ntab);
	if(ptab==NULL) return;
	int i;
	CRect offsetrect;
	offsetrect=ptab->m_origrect;
	for(i=0;i<itemcount;i++) {
		ptab=GetItem(i);
		ptab->MoveRgn(-offsetrect.left);
	}
	mFirstTab=ntab;
	mMoveflag=0;
	if (ntab>0) mMoveflag|=1;
	CRect rectclient;
	GetClientRect(rectclient);
	if (rectclient.Width()<(m_nDesiredWidth-offsetrect.left)) {
		if (ntab<itemcount-1) mMoveflag|=2;
	}
	Invalidate(TRUE);

}


BOOL CSheetTabCtrl::RemoveAllItem()
{
	while (!m_lsTabs.IsEmpty())
		delete (CSheetTab*)m_lsTabs.RemoveHead();
	m_iCurItem = 0;
	m_dwFtabStyle = 0;
	m_nDesiredWidth = 0;
	mMoveflag=0; 
	mFirstTab=0; 

	return TRUE;
}
