// OrderServView.cpp : implementation of the COrderServView class
//

#include "stdafx.h"
#include "OrderServ.h"

#include "OrderServDoc.h"
#include "OrderServView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderServView

IMPLEMENT_DYNCREATE(COrderServView, CView)

BEGIN_MESSAGE_MAP(COrderServView, CView)
	//{{AFX_MSG_MAP(COrderServView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderServView construction/destruction

COrderServView::COrderServView()
{
	// TODO: add construction code here

}

COrderServView::~COrderServView()
{
}

BOOL COrderServView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COrderServView drawing

void COrderServView::OnDraw(CDC* pDC)
{
	COrderServDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// COrderServView printing

BOOL COrderServView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COrderServView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COrderServView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// COrderServView diagnostics

#ifdef _DEBUG
void COrderServView::AssertValid() const
{
	CView::AssertValid();
}

void COrderServView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COrderServDoc* COrderServView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COrderServDoc)));
	return (COrderServDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COrderServView message handlers
