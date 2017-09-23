// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "OrderServ.h"
#include "account.h"

#include "frmOrderRoute.h"

#include "MainFrm.h"
#include "../define/Orderdef.h"
#include "OrderConfig.h"
#include "InfoPort.h"
#include "OrdScript.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define RouteMSG WM_USER+101
extern OrderConfig theCfgFile;
extern InfoPort theInfoPort;
/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(RouteMSG,OnRouteMSG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	time_t t1=time(NULL);
	tm tm1=*localtime(&t1);
	nlasttimestamp=t1-tm1.tm_sec;  //-(tm1.tm_min%10)*60
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	static char str[20];
	sprintf(str,"OM:%s%s",theCfgFile.mCompanyID,theCfgFile.mCompanyIDEx); 
	cs.lpszClass=str; 
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

LRESULT CMainFrame::OnRouteMSG(WPARAM wparam, LPARAM lparam)
{
	
	if (frmOrderRoute::mfrm!=NULL) {
		frmOrderRoute::mfrm->OnRouteMSG(wparam,lparam);
	}
	return 0;
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (AfxMessageBox("Close OrderServer!",MB_OKCANCEL)==IDOK) {	
		if (theCfgFile.mIsClosed==0){ 
			if (AfxMessageBox("SaveData?",MB_YESNO)==IDYES){
				char fname[100];
				CTime t1;
				t1=CTime::GetCurrentTime();
				sprintf(fname,"%sPositions%04d%02d%02d_%02d%02d%02d.txt",theCfgFile.mPathLog,
					t1.GetYear(),t1.GetMonth(),t1.GetDay(),
					t1.GetHour(),t1.GetMinute(),t1.GetSecond());

				Account::SaveAllPositionToFile(fname); 
				Account::SaveAllData();
			}
		}
		CMDIFrameWnd::OnClose();
	}
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	time_t t1;
	time(&t1);

	if ((t1-nlasttimestamp)>=60) {
		tm tm1=*localtime(&t1);
		theCfgFile.mlog.AddLogStrFmt("%%Z,T,%02d/%02d/%02d %02d:%02d:%02d",
			tm1.tm_year-100,tm1.tm_mon+1,tm1.tm_mday,tm1.tm_hour,tm1.tm_min,tm1.tm_sec); 
		nlasttimestamp=t1-tm1.tm_sec;

	}


	if (theCfgFile.mtmQuit!=0){
		if (t1>theCfgFile.mtmQuit) {
			if (theCfgFile.mIsClosed==0) { //didn't do the dayclose,
				theCfgFile.DoDayClose(); 
			}
			if((*(DWORD*)(theCfgFile.mlicenseDeCode+2)) !=
				(*(DWORD*)(theCfgFile.mCompanyID))) {
				AfxMessageBox("Invalid linense");
			}else {
				PostQuitMessage(0); 
				return;
			}
		}
	}
	if (theCfgFile.mtmClose!=0){
		if (t1>theCfgFile.mtmClose) {
			theCfgFile.DoDayClose();
		}
	}
	theCfgFile.AutoConnectRoute();
	
/*	theCfgFile.mRouteARCA.IsEnabled();
	theCfgFile.mRouteSOES.IsEnabled();
	theCfgFile.mRouteISLD.IsEnabled();
	theCfgFile.mRouteBRUT.IsEnabled();
	theCfgFile.mRouteHRZG.IsEnabled();
	theCfgFile.mRouteDOT.IsEnabled();
	theCfgFile.mRouteNITE.IsEnabled();
	theCfgFile.mRouteREDI.IsEnabled();
	theCfgFile.mRouteBAMM.IsEnabled();
	theCfgFile.mRouteGNET.IsEnabled(); 
*/
	intitem *PI=NULL;
	while(theCfgFile.mScriptTask.Enum(PI)) {
		CScriptItem *pitem;
		pitem=(CScriptItem *)PI->val;
		if (pitem->mbworkdayonly){
			if (theCfgFile.mIsWorkDay==0){
				continue;
			}
		}
		pitem->CheckTrigger(t1);
	}


	theInfoPort.GetStatus();

	CMDIFrameWnd::OnTimer(nIDEvent);
}



