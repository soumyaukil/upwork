// viewproperty.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Servers\autotrade\autotrade.h"
#include "viewproperty.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewProperty

IMPLEMENT_DYNCREATE(ViewProperty, CFormView)

ViewProperty::ViewProperty()
	: CFormView(ViewProperty::IDD)
{
	//{{AFX_DATA_INIT(ViewProperty)
	//}}AFX_DATA_INIT
}

ViewProperty::~ViewProperty()
{
}

void ViewProperty::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ViewProperty)
	DDX_Control(pDX, IDC_BSort, m_bSort);
	DDX_Control(pDX, IDC_BAdd, m_bAdd);
	DDX_Control(pDX, IDC_BSave, m_bSave);
	DDX_Control(pDX, IDC_BDelete, m_bDelete);
	DDX_Control(pDX, ID_BQuit, m_bquit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ViewProperty, CFormView)
	//{{AFX_MSG_MAP(ViewProperty)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BAdd, OnBAdd)
	ON_BN_CLICKED(IDC_BDelete, OnBDelete)
	ON_BN_CLICKED(IDC_BSave, OnBSave)
	ON_BN_CLICKED(ID_BQuit, OnBQuit)
	ON_BN_CLICKED(IDC_BSort, OnBSort)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewProperty diagnostics

#ifdef _DEBUG
void ViewProperty::AssertValid() const
{
	CFormView::AssertValid();
}

void ViewProperty::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ViewProperty message handlers

void ViewProperty::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	mGrid.Create(CRect(0,0,0,0),this,1000);
	mGrid.SetColumnCount(3);
	mGrid.SetRowCount(10);
	ArrangeWindow();
//	OnSize(0,0,0);
	// TODO: Add your specialized code here and/or call the base class
	
}

void ViewProperty::OnSize(UINT nType, int cx, int cy) 
{
//	CFormView::OnSize(nType, cx, cy);
	ArrangeWindow(); 
	// TODO: Add your message handler code here
	
}
/////////////////////////////////////////////////////////////////////////////
// frmPorperty

IMPLEMENT_DYNCREATE(frmPorperty, CMDIChildWnd)

frmPorperty::frmPorperty()
{
	mpview=NULL;
}

frmPorperty::~frmPorperty()
{
}


BEGIN_MESSAGE_MAP(frmPorperty, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmPorperty)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmPorperty message handlers

BOOL frmPorperty::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetWindowRect(rect);
	rect.right=rect.left+350; 
	GetParent()->ScreenToClient(rect); 
	MoveWindow(rect);
	pContext->m_pNewViewClass=RUNTIME_CLASS(ViewProperty); 
	mpview=(ViewProperty*)CreateView(pContext);

	return TRUE;
	
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void ViewProperty::OnOK() 
{
	// TODO: Add your control notification handler code here
	
}

void ViewProperty::OnCancel() 
{
	// TODO: Add your control notification handler code here
	
}

void ViewProperty::ArrangeWindow()
{
	CRect rect1;
	CRect rectbquit;
	CPoint pofset;
	if (!IsWindow(mGrid.m_hWnd)){ 
		return ;
	}

	GetClientRect(rect1); 
	m_bquit.GetWindowRect(rectbquit);
//	m_bquit.ClientToScreen(rectbquit);
	ScreenToClient(rectbquit);
	rect1.bottom-=2*rectbquit.Height(); 
	mGrid.MoveWindow(rect1);
	pofset.x=10;
	pofset.y=rect1.bottom+rectbquit.Height()/2;
	pofset=pofset-CPoint(rectbquit.left,rectbquit.top);
	rectbquit.OffsetRect(pofset);
	m_bSort.MoveWindow (rectbquit);
	rectbquit.OffsetRect(CPoint(rectbquit.Width(),0));
	m_bAdd.MoveWindow (rectbquit);
	rectbquit.OffsetRect(CPoint(rectbquit.Width(),0));
	m_bDelete.MoveWindow (rectbquit);
	rectbquit.OffsetRect(CPoint(rectbquit.Width(),0));
	m_bSave.MoveWindow (rectbquit);
	rectbquit.OffsetRect(CPoint(rectbquit.Width(),0));
	m_bquit.MoveWindow (rectbquit);
}

void frmPorperty::OnSize(UINT nType, int cx, int cy) 
{
	//CMDIChildWnd::OnSize(nType, cx, cy);
	if (mpview!=NULL) {
		mpview->MoveWindow(CRect(0,0,cx,cy));
	}
	// TODO: Add your message handler code here
	
}

int frmPorperty::InitProperty(LPCSTR Title, Property *pproperty)
{
	SetWindowText(Title);
	SetTitle(Title);
	return mpview->InitProperty(pproperty);
	
}

int ViewProperty::InitProperty(Property *pproperty)
{
	mGrid.SetColumnCount(2);
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetItemText(0,0,"Name");
	mGrid.SetItemText(0,1,"Value");
	pproperty->GetFistPosition();
	prop_float * pprost;
	int newrow;
	char valst[100];
	while((pprost=pproperty->GetNext())!=NULL) {
		newrow=mGrid.InsertRow(pprost->name);  
		sprintf(valst,"%g",pprost->val ); 
		mGrid.SetItemText(newrow,1,valst);
	}
	mpproperty=pproperty;
	mGrid.SetModified(FALSE);
	return 0;
}

void ViewProperty::OnBAdd() 
{
	// TODO: Add your control notification handler code here
	CCellID cid;
	int newrowno;
	cid=mGrid.GetFocusCell();
	if (cid.row <=0) newrowno=-1;
	else 
		newrowno=cid.row;
	mGrid.InsertRow(" ",newrowno); 
	mGrid.Refresh();
}

void ViewProperty::OnBDelete() 
{
	// TODO: Add your control notification handler code here
	CCellID cid;
	cid=mGrid.GetFocusCell();
	if ( cid.row<=0) return ; 
	mGrid.DeleteRow(cid.row); 
	mGrid.Refresh();
}

void ViewProperty::OnBSave() 
{
	// TODO: Add your control notification handler code here
	int rownu;
	Property tmpproperty;
	CString pname;
	double pval;
	CString tmpst;
	for(rownu=1;rownu<mGrid.GetRowCount();rownu++) {
		pname=mGrid.GetItemText(rownu,0);
		pname.TrimLeft();
		pname.TrimRight();
		tmpst=mGrid.GetItemText(rownu,1);
		tmpst.TrimLeft();
		pval=atof(tmpst);
		tmpproperty.SetDBLProperty(pname,pval);
	}
	mpproperty->SetPropertyBag(&tmpproperty);
	mGrid.SetModified(FALSE);
}

void ViewProperty::OnBQuit() 
{
	// TODO: Add your control notification handler code here
	if (mGrid.GetModified()) {
		int ret;
		ret=AfxMessageBox("Save changed?",MB_YESNOCANCEL);
		if (ret==IDYES) {
			OnBSave();
		}else if (ret==IDCANCEL) {
			return ;
		}

	}
	GetParent()->PostMessage(WM_CLOSE);
}

void ViewProperty::OnBSort() 
{
	// TODO: Add your control notification handler code here
	int colno;
	CCellID cid;
	cid=mGrid.GetFocusCell();
	colno=cid.col;
	CString st1;
	switch (colno) {
	case 0:
		mGrid.SortTextItems(colno,TRUE);	
		break;
	case 1:
		mGrid.SortNumItems(colno,TRUE);	
		break;
	default:
		break;
	}
	mGrid.Refresh(); 
	return ;

}
