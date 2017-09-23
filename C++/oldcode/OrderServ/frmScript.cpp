// frmScript.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "frmScript.h"
#include "OrderConfig.h"
#include "OrdScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern OrderConfig theCfgFile;
frmScript *frmScript::mfrm=NULL;
/////////////////////////////////////////////////////////////////////////////
// frmScript

IMPLEMENT_DYNCREATE(frmScript, CMDIChildWnd)

frmScript::frmScript()
{
}

frmScript::~frmScript()
{
	mfrm=NULL;
}


BEGIN_MESSAGE_MAP(frmScript, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmScript)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmScript message handlers

BOOL frmScript::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	mGrid.Create(CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST);
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnCount(6);
//	mGrid.SetFixedColumnCount(1);
	mGrid.SetEditable(FALSE);
	mGrid.SetListMode(TRUE);
	InitGrid();
	mGrid.AutoSizeColumns(); 

	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void frmScript::InitGrid()
{
	mGrid.SetItemText(0,0,"time");
	mGrid.SetItemText(0,1,"status");
	mGrid.SetItemText(0,2,"Script");

	intitem *PI=NULL;
	while(theCfgFile.mScriptTask.Enum(PI)) {
		CScriptItem *pitem;
		pitem=(CScriptItem *)PI->val;
		int row;
		row=mGrid.InsertRow(NULL); 
		int ntime=pitem->GetTime();
		if (ntime>0) ntime-=theCfgFile.mtmFirstSecond;  
		mGrid.SetItemTextFmt(row,0,"%02d:%02d",ntime/3600,(ntime%3600)/60);
		mGrid.SetItemText(row,1,pitem->IsTrigged()?"Y":"N");
		mGrid.SetItemText(row,2,pitem->GetCmd());
	}

}

void frmScript::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CMDIChildWnd::OnClose();
}
