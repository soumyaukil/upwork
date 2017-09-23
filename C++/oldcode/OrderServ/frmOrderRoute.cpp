// frmOrderRoute.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "OrderRouter.h"
#include "frmOrderRoute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "OrderRouteCfg.h"
extern OrderConfig theCfgFile;

frmOrderRoute * frmOrderRoute::mfrm=NULL;
/////////////////////////////////////////////////////////////////////////////
// frmOrderRoute

IMPLEMENT_DYNCREATE(frmOrderRoute, CMDIChildWnd)

frmOrderRoute::frmOrderRoute()
{
}

frmOrderRoute::~frmOrderRoute()
{
}


BEGIN_MESSAGE_MAP(frmOrderRoute, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmOrderRoute)
	ON_COMMAND(ID_ROUTE_CONFIG, OnRouteConfig)
	ON_COMMAND(ID_ROUTE_CONNECTALL, OnRouteConnectall)
	ON_COMMAND(ID_ROUTE_DISCONNECT, OnRouteDisconnect)
	ON_COMMAND(ID_ROUTE_DISCONNECTALL, OnRouteDisconnectall)
	ON_COMMAND(ID_ROUTE_CONNECT, OnRouteConnect)
	ON_NOTIFY(NM_RCLICK, AFX_IDW_PANE_FIRST, OnRMouseDown)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmOrderRoute message handlers

BOOL frmOrderRoute::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	mGrid.Create(CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnCount(8);
	mGrid.SetFixedColumnCount(1);
	mGrid.SetEditable(FALSE);

	InitGrid();
	mfrm=this;
	return TRUE;
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void frmOrderRoute::InitGrid()
{
	OrderRouter *por;
	stritem *PIitem;
	theCfgFile.mRoutes.EnumFirst(&PIitem);
	int rowno;
	mGrid.SetRowCount(1);
	mGrid.SetItemText(0,0," ");
	mGrid.SetItemText(0,1,"Route");
	mGrid.SetItemText(0,2,"Connect");
	mGrid.SetItemText(0,3,"Enable");
	mGrid.SetItemText(0,4,"ver");
	mGrid.SetItemText(0,5,"Exes");
	mGrid.SetItemText(0,6,"exFilter");
	mGrid.SetItemText(0,7,"Hide");
	mGrid.SetHeaderSort(TRUE);

	while(PIitem){
		por=(OrderRouter *)PIitem->val;
		rowno=mGrid.InsertRow("");
		showroute(rowno,por);
		theCfgFile.mRoutes.EnumNext(&PIitem);
	}
	mGrid.AutoSizeColumns(); 

}

BOOL frmOrderRoute::showroute(int row, OrderRouter *por)
{
	mGrid.SetItemText(row,1,por->mRouteName);
	mGrid.SetItemData(row,0,(long)por);
	if (por->IsConnected()) {
		mGrid.SetItemText(row,2,"Y");
		mGrid.SetItemBkColour(row,2,RGB(0,255,0)); 
		mGrid.SetItemTextFmt(row,4,"%d.%d",por->mServerVersion>>8,por->mServerVersion&0xff);
	}else {
		mGrid.SetItemText(row,2,"");
		mGrid.SetItemBkColour(row,2,RGB(255,255,255));
		mGrid.SetItemText(row,4,"");
	}

	if (por->IsEnabled()) {
		mGrid.SetItemText(row,3,"Y");
		mGrid.SetItemBkColour(row,3,RGB(0,255,0)); 
	}else {
		mGrid.SetItemText(row,3,"");
		mGrid.SetItemBkColour(row,3,RGB(255,255,255)); 
	}

	char stExFilter[30];
	por->GetDisableExch(stExFilter);
	mGrid.SetItemText(row,6,stExFilter);
	mGrid.SetItemText(row,7,por->m_bHideRoute?"Y":"");



	return TRUE;
}

void frmOrderRoute::OnRMouseDown(NMHDR *pNotifyStruct, LRESULT *result)
{
/*	NM_GRIDVIEW *pifo;
	pifo=(NM_GRIDVIEW *)pNotifyStruct;
	int norderid;
	norderid=mpGridOrders->GetItemData(pifo->iRow,0);
	itemOrder *porder;
	porder=itemOrder::FindItem(norderid);
*/	
	CMenu popmenu;
	CMenu *pmenu;
	CPoint screenpoint;
	GetCursorPos(&screenpoint);
	
	popmenu.LoadMenu(IDR_MenuPopUp);
	pmenu=popmenu.GetSubMenu(0);
//	ClientToScreen(&screenpoint);
	pmenu->TrackPopupMenu(TPM_LEFTALIGN,screenpoint.x,screenpoint.y,this);
	pmenu->DestroyMenu();
	popmenu.DestroyMenu(); 
	
}

void frmOrderRoute::OnRouteConfig() 
{
	// TODO: Add your command handler code here
	CCellID cid;
	cid=mGrid.GetFocusCell();
	if (cid.row<=0) return;
	CString route;
	route=mGrid.GetItemText(cid.row,1);
	OrderRouter *por;
	por=theCfgFile.GetRoute(route);
	if (por) {
		por->DoConfig();	
	}else {
		AfxMessageBox("Invalid route");
	};
	
}

void frmOrderRoute::OnRouteConnectall() 
{
	theCfgFile.ConnectAllRoute();
}

void frmOrderRoute::OnRouteDisconnect() 
{
	CCellID cid;
	cid=mGrid.GetFocusCell();
	if (cid.row<=0) return;
	CString route;
	route=mGrid.GetItemText(cid.row,1);
	OrderRouter *por;
	por=theCfgFile.GetRoute(route);
	if (por) {
		por->mautoconnect=0; // don't auto connect once
		por->DisconnectServer();	
	}else {
		AfxMessageBox("Invalid route");
	};
	
}

void frmOrderRoute::OnRouteDisconnectall() 
{
	if (AfxMessageBox("Disconnect all Route?",MB_OKCANCEL)==IDCANCEL) {
		return;
	}
	theCfgFile.DisConnectAllRoute();
	
}

void frmOrderRoute::OnRouteConnect() 
{
	CCellID cid;
	cid=mGrid.GetFocusCell();
	if (cid.row<=0) return;
	CString route;
	route=mGrid.GetItemText(cid.row,1);
	OrderRouter *por;
	por=theCfgFile.GetRoute(route);
	if (por) {
		por->ConnectToServer();	
	}else {
		AfxMessageBox("Invalid route!");
	};
	
}


void frmOrderRoute::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	mfrm=NULL;
	CMDIChildWnd::OnClose();
}

LRESULT frmOrderRoute::OnRouteMSG(WPARAM wparam, LPARAM lparam)
{
	OrderRouter *por;
	por=theCfgFile.GetRoute((LPCSTR)lparam);
	if (por==NULL) return 0;
	int row,totalrow;
	totalrow=mGrid.GetRowCount();
	for (row=0;row<totalrow;row++){
		if (mGrid.GetItemData(row,0)==(long)por){
			showroute(row,por);
			mGrid.RedrawRow(row); 
			break;
		}
	}
	return 1;
}
