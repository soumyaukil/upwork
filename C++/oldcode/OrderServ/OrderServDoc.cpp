// OrderServDoc.cpp : implementation of the COrderServDoc class
//

#include "stdafx.h"
#include "OrderServ.h"

#include "OrderServDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COrderServDoc

IMPLEMENT_DYNCREATE(COrderServDoc, CDocument)

BEGIN_MESSAGE_MAP(COrderServDoc, CDocument)
	//{{AFX_MSG_MAP(COrderServDoc)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderServDoc construction/destruction

COrderServDoc::COrderServDoc()
{
	// TODO: add one-time construction code here

}

COrderServDoc::~COrderServDoc()
{
}

BOOL COrderServDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// COrderServDoc serialization

void COrderServDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// COrderServDoc diagnostics

#ifdef _DEBUG
void COrderServDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COrderServDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COrderServDoc commands

void COrderServDoc::OnFileOpen() 
{
	// TODO: Add your command handler code here
	
}

void COrderServDoc::OnFileClose() 
{
	// TODO: Add your command handler code here
	
}
