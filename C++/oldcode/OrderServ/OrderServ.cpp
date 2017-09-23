// OrderServ.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OrderServ.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "OrderServDoc.h"
#include "OrderServView.h"
//#include "RstTrader.h"
#include "Functions\PkgBuff.h"
#include "MSocket\Msocket.h"
#include "Functions\encrypt.h"
#include "Functions\InputBox.h"
#include "Functions\Perf.h"
#include "OrdScript.h"

#include "OrderDB.h"
#include "account.h"
#include "mgmt\frmMgmt.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "dlgIPBox.h"
#include "dlgBalance.h"
#include "OrderRouteCfg.h"
#include "dlgpopmsg.h"

//extern CPkgBuff thePkgbuf;
#include "InfoPort.h"
extern OrderConfig theCfgFile;
extern OrderDB theOrderDB;
extern InfoPort theInfoPort;
/////////////////////////////////////////////////////////////////////////////
// COrderServApp

BEGIN_MESSAGE_MAP(COrderServApp, CWinApp)
	//{{AFX_MSG_MAP(COrderServApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_VIEW_CFG, OnViewCfg)
	ON_COMMAND(ID_VIEW_LOG, OnViewLog)
	ON_COMMAND(ID_DATA_LOADEQUITY, OnDataLoadequity)
	ON_COMMAND(ID_DATA_SAVEEQUITY, OnDataSaveequity)
	ON_COMMAND(ID_DATA_LSHTLST, OnDataLshtlst)
	ON_COMMAND(ID_DATA_SAVEDATA, OnDataSavedata)
	ON_COMMAND(ID_TOOLS_SECIFO, OnToolsSecifo)
	ON_COMMAND(ID_TOOLS_EQUITY, OnToolsEquity)
	ON_COMMAND(ID_VIEW_TRADERS, OnViewTraders)
	ON_COMMAND(ID_TOOLS_BALANCE, OnToolsBalance)
	ON_COMMAND(ID_VIEW_LOGINLOG, OnViewLoginlog)
	ON_COMMAND(ID_DATA_EXPORTPOSITION, OnDataExportposition)
	ON_COMMAND(ID_TOOLS_LOADLOGINCTRL, OnToolsLoadloginctrl)
	ON_COMMAND(ID_DATA_COUNTING, OnDataCounting)
	ON_COMMAND(ID_VIEW_THREADS, OnViewThreads)
	ON_COMMAND(ID_DATA_RECOVER_SAVETRADES, OnDataRecoverSavetrades)
	ON_COMMAND(ID_AAA_TEST, OnAaaTest)
	ON_COMMAND(ID_TOOLS_LOADMMUNTRADEABLE, OnToolsLoadmmuntradeable)
	ON_COMMAND(ID_DATA_SOD_QUEST, OnDataSodQuest)
	ON_COMMAND(ID_ROUTE_CONFIG, OnRouteConfig)
	ON_COMMAND(ID_DATA_SOD_BPEQUITY, OnDataSodBpequity)
	ON_COMMAND(ID_TOOLS_LOADROUTECTRL, OnToolsLoadroutectrl)
	ON_COMMAND(ID_TOOLS_COMMAND, OnToolsCommand)
	ON_COMMAND(ID_TOOLS_SCRIPTLIST, OnToolsScriptlist)
	ON_COMMAND(ID_LogFile, OnLogFile)
	ON_COMMAND(ID_TOOLS_DBRECONN, OnToolsDbreconn)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COrderServApp construction

COrderServApp::COrderServApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COrderServApp object

COrderServApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COrderServApp initialization


BOOL COrderServApp::InitInstance()
{
	void CrashReportInstall();
	CrashReportInstall();

	
	AfxEnableControlContainer();

	Perf_Clear();
	if (theCfgFile.InitCfg()!=0) {
		return FALSE;
	};
	if(theCfgFile.ReadCfgFile()!=0) {
		return FALSE;
	}

	static char stClassNames[100];
	sprintf(stClassNames,"OM:%s%s",theCfgFile.mCompanyID,theCfgFile.mCompanyIDEx); 


	WNDCLASS wndcls;    
	memset(&wndcls, 0, sizeof(WNDCLASS));
	wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
	wndcls.lpfnWndProc = AfxWndProc;
	wndcls.hInstance = AfxGetInstanceHandle();
	wndcls.hIcon = LoadIcon( IDR_MAINFRAME);//or AFX_IDI_STD_FRAME; 
//	wndcls.hCursor = LoadCursor( wndcls.hInstance, IDC_ARROW );
	wndcls.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wndcls.lpszMenuName = NULL;//You may need to fix this
	wndcls.lpszClassName = stClassNames; // my class name
	// Register name, exit if it fails
	if(!AfxRegisterClass(&wndcls)) {
		AfxMessageBox( _T("Failed to register window class!"), MB_ICONSTOP | MB_OK );
		return FALSE;
	}

  // Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

/*	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_ORDERSTYPE,
		RUNTIME_CLASS(COrderServDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(COrderServView));
	AddDocTemplate(pDocTemplate);
*/

	HWND hWnd = FindWindowEx( NULL, NULL, stClassNames,NULL);
	if ( hWnd != NULL ) {
		ShowWindow( hWnd, SW_RESTORE );
		BringWindowToTop( hWnd );
		SetForegroundWindow( hWnd );
		return FALSE;
	}
		
	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
// pMainFrame->SetWindowText(str);
//	pMainFrame->SetTitle(str); 
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	BOOL ret;
	ret= InitSystem();
	if (ret==FALSE) {
		AfxMessageBox("program error in init system:! please shutdown first.");

	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnStifo();
	afx_msg void OnBCrash();
	afx_msg void OnBPref();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_STIFO, OnStifo)
	ON_BN_CLICKED(IDC_BCrash, OnBCrash)
	ON_BN_CLICKED(IDC_BPref, OnBPref)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void COrderServApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// COrderServApp message handlers


void COrderServApp::OnFileNew() 
{
	// TODO: Add your command handler code here
	CWinApp::OnFileNew();	
}

void COrderServApp::OnFileOpen() 
{
	// TODO: Add your command handler code here
//	CWinApp::OnFileOpen();  
}





int COrderServApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	theInfoPort.exit(); 
	Msocket::M_SocketShutdown();
	theOrderDB.CloseConnection();
	theCfgFile.Exit();

	void CrashReportUninstall();
	CrashReportUninstall();

	return CWinApp::ExitInstance();
}

BOOL COrderServApp::InitSystem()
{
	//----Init the Configure file;
	
	OnViewLoginlog(); 
	OnViewLog(); 
	
	////
	//Temporary commented
	/*if((*(DWORD*)(theCfgFile.mlicenseDeCode+2)) !=
		(*(DWORD*)(theCfgFile.mCompanyID))) {
		AfxMessageBox("Invalid linense");
		return FALSE;
	}
	DWORD msysid;
	encrypt en;
	msysid=en.GetSysID();
	if ((*(DWORD*)(theCfgFile.mlicenseDeCode+6)) !=	msysid) {
		AfxMessageBox("Invalid linense");
		return FALSE;
	}

	CTime curr;
	CTime validto;
	validto=theCfgFile.GetExpTime();
	curr=CTime::GetCurrentTime();
	if (curr>validto) {
		char str[1000];
		sprintf(str,"Your software was expired.");
		theCfgFile.mlog.AddLogStr(str);
		dlgPopMsg *pop;
		pop=new dlgPopMsg;
		pop->stmsg=str;
		pop->Create(dlgPopMsg::IDD);
		pop->ShowWindow(SW_SHOW);
		return FALSE;
	}
	if ((validto.GetTime()-curr.GetTime())<24*3600*7) {
		char str[1000];
		sprintf(str,"Your software will expire at %d/%d/%d \r\n"
			"Please contact to your software provider to upgrade!",validto.GetYear() ,validto.GetMonth(),validto.GetDay());
		theCfgFile.mlog.AddLogStr(str);
		dlgPopMsg *pop;
		pop=new dlgPopMsg;
		pop->stmsg=str;
		pop->Create(dlgPopMsg::IDD);
		pop->ShowWindow(SW_SHOW);
	}*/

	time_t tm1;
	time(&tm1);
	theCfgFile.mlog.AddLogStr(ctime(&tm1));
	theCfgFile.mlog.AddLogStr("Connect to Database...");
	BOOL dbret;
	dbret =theOrderDB.ConnectOrderSV(theCfgFile.mdbConnectString );
	if (dbret==FALSE) {
		AfxMessageBox("Can't Open Database!");
		return FALSE;
	}

	//Temporary commented
	/*char str1[1000];
	theOrderDB.cfgGet("SYSTEM","NAME",theCfgFile.mSystemName,sizeof(theCfgFile.mSystemName));
	sprintf(str1,"sysname:%s",theCfgFile.mSystemName);
	theCfgFile.mlog.AddLogStr(str1);
	sprintf(str1,"Order Manager: %s        VER %d:%d",theCfgFile.mSystemName,theCfgFile.m_Version>>8,theCfgFile.m_Version&0xff);

	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	pFrame->SetTitle(str1);
	pFrame->SetWindowText(str1);*/


	//Temporary commented
	//Check the database version and modify the database;
	//CheckDatabase();


	theCfgFile.mlog.AddLogStr("Doing Daily inituralize....");

	//Temp commented
	TRY {
		theOrderDB.mdb.SetQueryTimeout(theCfgFile.msqlInitTimeOut);
		theOrderDB.mdb.ExecuteSQL("DO " 
								"$$ " 
								"Declare " 
									"temp INTEGER; " 
								"BEGIN " 
									"select DayInit() into temp; " 
								"END "
								"$$; ");
		//theOrderDB.mdb.ExecuteSQL("exec DayInit");//sql server
		theOrderDB.mdb.SetQueryTimeout(theCfgFile.msqlTimeOut);

	}CATCH(CDBException, e) {
		char msg[1000];
		e->GetErrorMessage(msg,sizeof(msg));
		theCfgFile.mlog.AddLogStr(msg); 
//		e.Delete() ;
		return FALSE;
	}END_CATCH
	
	if (theCfgFile.init()==FALSE) {
		return FALSE;
	};
	

	//-----Init Branch information;
	theCfgFile.mlog.AddLogStr("Load Branch");
//	BranchItem::Init(theCfgFile.mBranchNumber);
	theCfgFile.mpItemPool->DBLoadBranchItem(); 
	
	theCfgFile.mlog.AddLogStr("Load Account");
	Account::init(theCfgFile.mAccountNumber);
	theCfgFile.mlog.AddLogStr("Load Trade");
	Trader::Init(theCfgFile.mTraderNumber); 
	theCfgFile.mlog.AddLogStr("Load Trade_Account realationship");
	AC_TR_relation::Init();
	theCfgFile.mlog.AddLogStr("Load Deal Information");
	itemDeal::Init();

	char pathname[1000];
	CTime t1=CTime::GetCurrentTime();
	sprintf(pathname,"%sOSVLOG%04d%02d%02d.txt",theCfgFile.mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay());
	Account::RecoverFromLog(pathname);
	
	//-----recalcuate equity;
	Account::sMapAccid.LockMap();
	intitem *pintitem;
	Account::sMapAccid.EnumFirst(&pintitem);
	Account *pacc;
	while(pintitem) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);
//		pacc->ReCalculatePosition(); 
		pacc->TktRecaluate(FALSE,FALSE);
		pacc->mitemEquity.mCurrHoldBP=pacc->ReCalOrderHoldBP();
		Account::sMapAccid.EnumNext(&pintitem);
	}
	Account::sMapAccid.UnlockMap();

	//-----Init for the Socket Connection
	dabuff::InitBuff(BF_NU);
	Msocket::M_SocketStartUp();
	Msocket::M_StartThread();
	Msocket::M_StartThread();
	Msocket::M_StartThread();
	Msocket::M_StartThread();	

	Msocket::M_StartThread();
	Msocket::M_StartThread();
	Msocket::M_StartThread();
	Msocket::M_StartThread();
	mclientport1.Listen(theCfgFile.mServerPort1);
	mclientport2.Listen(theCfgFile.mServerPort2); 
	mclientport3.Listen(theCfgFile.mServerPort3); 
	OrderRouter *ptest;
	ptest=theCfgFile.GetRoute("TEST");
	
	if ((ptest)){
		if (theCfgFile.mAutoConnectTest){
			ptest->ConnectToServer(); 
		}
		ptest->mbTestRoute=TRUE; 
	}

	theCfgFile.LoadRouteCtrl();
		
	theInfoPort.Init();
	
	CMDIFrameWnd *pfrm;
	pfrm=(CMDIFrameWnd *)GetMainWnd(); 
	pfrm->SetTimer(100,1000,NULL);


	return TRUE;
}

void COrderServApp::OnViewCfg() 
{
	theCfgFile.ShowConfigFile(); 
}

void COrderServApp::OnViewLog() 
{
	CMDIFrameWnd *pfrm;
	pfrm=(CMDIFrameWnd *)GetMainWnd(); 
	theCfgFile.mlog.ShowLogWindow(pfrm,IDR_LOG);
}

//DEL void COrderServApp::OnAaaTmp1() 
//DEL {
//DEL 		Account * item;
//DEL 		AC_TR_relation *ptmp;
//DEL 		intitem *pintitem;
//DEL 		char st[1000];
//DEL 		Account::sMapAccid.EnumFirst(&pintitem);
//DEL 		theCfgFile.mlog.AddLogStr("Acct_access list"); 
//DEL 		while(pintitem!=NULL){
//DEL 			item=(Account*)pintitem->val;//Account::mapid2account.GetNextAssoc(pos,(void*&)accid,(void*&)item);
//DEL 			sprintf(st,"Acc:%d,%s",item->mitemifo.maccid,item->mitemifo.mname);
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 			ptmp= item->pFistAcc;
//DEL 			while(ptmp!=NULL) {
//DEL 				sprintf(st,"Tr_Add:%d,%d,%d",ptmp->mtrid,ptmp->maccid,ptmp->mstatus);
//DEL 				ptmp=ptmp->pnextac;
//DEL 			}
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 			Account::sMapAccid.EnumNext(&pintitem);
//DEL 		}
//DEL 
//DEL 	
//DEL }


//DEL void COrderServApp::OnAaaOnlinetrader() 
//DEL {
//DEL 	intitem *pitem;
//DEL 	char str[1000];
//DEL 	Trader::mOnlineAdmin.LockMap();
//DEL 	theCfgFile.mlog.AddLogStr("Online Administrator:");
//DEL 	pitem=Trader::mOnlineAdmin.EnumFirst();
//DEL 	while(pitem!=NULL) {
//DEL 		CClient *pclient;
//DEL 		pclient=(CClient*)pitem->val;
//DEL 		if (pclient->mpAcceptBuff
//DEL 		sprintf(str,"%d:%s",pclient->mptrifo->minfo.mtrid,pclient->mptrifo->minfo.mtracno);
//DEL 		theCfgFile.mlog.AddLogStr(str);
//DEL 		pitem=Trader::mOnlineAdmin.EnumNext(pitem);
//DEL 		
//DEL 	}
//DEL 	Trader::mOnlineAdmin.UnlockMap();
//DEL 
//DEL 	Trader::mOnlineTrader.LockMap();
//DEL 	theCfgFile.mlog.AddLogStr("Online Trader:");
//DEL 	pitem=Trader::mOnlineTrader.EnumFirst();
//DEL 	while(pitem!=NULL) {
//DEL 		CClient *pclient;
//DEL 		pclient=(CClient*)pitem->val;
//DEL 		sprintf(str,"%d:%s",pclient->mptrifo->minfo.mtrid,pclient->mptrifo->minfo.mtracno);
//DEL 		theCfgFile.mlog.AddLogStr(str);
//DEL 		pitem=Trader::mOnlineTrader.EnumNext(pitem);
//DEL 		
//DEL 	}
//DEL 	Trader::mOnlineTrader.UnlockMap();
//DEL 
//DEL }

//DEL void COrderServApp::OnAaaOrders() 
//DEL {
//DEL 		itemOrder *pitem;
//DEL 		char st[1000];
//DEL 		theCfgFile.mlog.AddLogStr(" orders list"); 
//DEL 		intitem *pintitem;
//DEL 		pintitem=itemOrder::sMapOrderID.EnumFirst();
//DEL 		while (pintitem!=NULL) {
//DEL 			pitem=(itemOrder *)pintitem->val;
//DEL 			sprintf(st,"id:%d,trid:%d,accid:%d,brid:%d,Secsym:%s,qty:%d,price:%g",
//DEL 				pitem->morderid,pitem->mtrid,pitem->maccid,pitem->mbrid,
//DEL 					pitem->msecsym,pitem->mqty,pitem->mprice);
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 			pintitem=itemOrder::sMapOrderID.EnumNext(pintitem); 
//DEL 		}
//DEL 
//DEL }


//DEL void COrderServApp::OnAaaTrades() 
//DEL {
//DEL 	Account *pacc;
//DEL 	itemTrade *pitem;
//DEL 	char st[1000];
//DEL 	theCfgFile.mlog.AddLogStr("Trades list"); 
//DEL 
//DEL 	Account::sMapAccid.LockMap();
//DEL 	intitem *pintitemacc;
//DEL 	intitem *pinttrade;
//DEL 	Account::sMapAccid.EnumFirst(&pintitemacc);
//DEL 	while (pintitemacc){
//DEL 		pacc=(Account*)pintitemacc->val;
//DEL 		pacc->mLstTrades.EnumFirst(&pinttrade);
//DEL 		while(pinttrade) {
//DEL 			pitem=(itemTrade*)pinttrade->val;
//DEL 			sprintf(st,"id:%d,trid:%d,accid:%d,brid:%d,Secsym:%s,qty:%d,price:%g",
//DEL 				pitem->mtradeid,pitem->mtrid,pitem->maccid,pitem->mbrid,
//DEL 					pitem->msecsym,pitem->mqty,pitem->mprice);
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 
//DEL 			pacc->mLstTrades.EnumNext(&pinttrade);
//DEL 		}
//DEL 
//DEL 
//DEL 		Account::sMapAccid.EnumNext(&pintitemacc);
//DEL 	}
//DEL 	Account::sMapAccid.UnlockMap();
//DEL 
//DEL /*	itemTrade *pitem;
//DEL 	char st[1000];
//DEL 	theCfgFile.mlog.AddLogStr(" Trades list"); 
//DEL 	intitem *pintitem;
//DEL 	pintitem=itemTrade::sMapTradeID.EnumFirst();
//DEL 	while (pintitem!=NULL) {
//DEL 		pitem=(itemTrade *)pintitem->val;
//DEL 		sprintf(st,"id:%d,trid:%d,accid:%d,brid:%d,Secsym:%s,qty:%d,price:%g",
//DEL 			pitem->mtradeid,pitem->mtrid,pitem->maccid,pitem->mbrid,
//DEL 				pitem->msecsym,pitem->mqty,pitem->mprice);
//DEL 		theCfgFile.mlog.AddLogStr(st); 
//DEL 		pintitem=itemTrade::sMapTradeID.EnumNext(pintitem); 
//DEL 	}
//DEL */
//DEL }



void COrderServApp::OnDataLoadequity() 
{
	if (AfxMessageBox("This will load equity information from the database! continue?",MB_OKCANCEL)==IDCANCEL)
		return;
	Account::LoadEquityInfo();
}

void COrderServApp::OnDataSaveequity() 
{
	if (AfxMessageBox("This will Save equity information to the database! continue?",MB_OKCANCEL)==IDCANCEL)
		return;
	Account::SaveAllEquityInfo(FALSE);
}

//DEL void COrderServApp::OnAaaAccount() 
//DEL {
//DEL 	intitem *pintitem;
//DEL 	Account *pacc;
//DEL 	Account::sMapAccid.EnumFirst(&pintitem);
//DEL 	char str[300];
//DEL 	while(pintitem!=NULL) {
//DEL 		pacc=(Account*)pintitem->val;
//DEL 		sprintf(str,"%d,%s,%f%f%f",
//DEL 				pacc->mitemifo.maccid,pacc->mitemifo.mname,
//DEL 				pacc->mitemEquity.mOpenEQ,pacc->mitemEquity.mOpenBP, pacc->mitemEquity.mOpenNBP);
//DEL 		theCfgFile.mlog.AddLogStr(str);  
//DEL 
//DEL 		Account::sMapAccid.EnumNext(&pintitem);
//DEL 	}
//DEL 
//DEL }

//DEL void COrderServApp::OnAaaDeal() 
//DEL {
//DEL 		itemDeal *pitem;
//DEL 		char st[1000];
//DEL 		theCfgFile.mlog.AddLogStr(" Deal list"); 
//DEL 		itemPassthrufee *ppath; 
//DEL 		intitem *pintitem;
//DEL 		pintitem=itemDeal::sMapDealID.EnumFirst();
//DEL 		while (pintitem!=NULL) {
//DEL 			pitem=(itemDeal*)pintitem->val;
//DEL 			sprintf(st,"id:%d,name:%s,def:%d,sharelmt:%d,shareadj:%g,limitadj:%g,marketadj:%g,"
//DEL 				"baseshare:%d,basecharge:%g,note:%s",
//DEL 				pitem->mdealid,pitem->mname,pitem->mdefaultdeal,pitem->msharelmt,
//DEL 					pitem->mshareadj,pitem->mlimitadj,pitem->mmktadj,pitem->mbaseshare,
//DEL 					pitem->mbasecharge,pitem->mnote);
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 
//DEL 			ppath=pitem->ppathfee;
//DEL 			while(ppath) {
//DEL 				sprintf(st,"%s,%g,%g,%g,%g",ppath->mpath,
//DEL 					ppath->costpertkt,ppath->costperexe,ppath->costpershr,ppath->costpershrA);
//DEL 				theCfgFile.mlog.AddLogStr(st); 
//DEL 				ppath=ppath->pnext ;
//DEL 			}
//DEL 			pintitem=itemDeal::sMapDealID.EnumNext(pintitem); 
//DEL 		}
//DEL 
//DEL 
//DEL }

//DEL void COrderServApp::OnAaaTicket() 
//DEL {
//DEL 	Account *pacc;
//DEL 	itemTicket *pitem;
//DEL 	char st[1000];
//DEL 	theCfgFile.mlog.AddLogStr("Trades list"); 
//DEL 
//DEL 	Account::sMapAccid.LockMap();
//DEL 	intitem *pintitemacc;
//DEL 	intitem *pintticket;
//DEL 	Account::sMapAccid.EnumFirst(&pintitemacc);
//DEL 	while (pintitemacc){
//DEL 		pacc=(Account*)pintitemacc->val;
//DEL 		pacc->mLstTickets.EnumFirst(&pintticket);
//DEL 		while(pintticket) {
//DEL 			pitem=(itemTicket*)pintticket->val;
//DEL 			sprintf(st,"id:%d,tid:%d,oid:%d,"
//DEL 				"accid:%d,brid:%d,Secsym:%s,qty:%d,price:%g,Comm %g",
//DEL 				pitem->mticketid,pitem->mtradeid,pitem->morderid, 
//DEL 				pitem->maccid,pitem->mbrid,
//DEL 					pitem->msecsym,pitem->mqty,pitem->mprice,pitem->mcommision);
//DEL 			theCfgFile.mlog.AddLogStr(st); 
//DEL 			pacc->mLstTickets.EnumNext(&pintticket);
//DEL 		}
//DEL 		Account::sMapAccid.EnumNext(&pintitemacc);
//DEL 	}
//DEL 	Account::sMapAccid.UnlockMap();
//DEL 
//DEL }

void COrderServApp::OnDataLshtlst() 
{
	if (theCfgFile.mpIssueData ) {
		theCfgFile.mpIssueData->LoadIssueInfo();
	}
}

//DEL void COrderServApp::OnOrderrouteSoessnet() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("SOES");
//DEL 	if (pr)	pr->ConnectToServer(); 
//DEL }


//DEL void COrderServApp::OnOrderrouteDot() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("DOT");
//DEL 	if (pr)
//DEL 		pr->ConnectToServer(); 
//DEL 	
//DEL }

void COrderServApp::OnDataSavedata() 
{
	if (AfxMessageBox("This will Save equity,position,tickets information to the database! continue?",MB_OKCANCEL)==IDCANCEL)
		return;
	Account::SaveAllData();
}

//DEL void COrderServApp::OnOrderrouteArca() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("ARCA");
//DEL 	if (pr)	pr->ConnectToServer(); 
//DEL }

//DEL void COrderServApp::OnOrderrouteBrut() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("BRUT");
//DEL 	if (pr)	pr->ConnectToServer(); 
//DEL }


//DEL void COrderServApp::OnOrderrouteHrzg() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("HRZG");
//DEL 	if (pr)
//DEL 		pr->ConnectToServer(); 
//DEL }

//DEL void COrderServApp::OnOrderrouteRedi() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("REDI");
//DEL 	if (pr)
//DEL 		pr->ConnectToServer(); 
//DEL }

//DEL void COrderServApp::OnOrderrouteNite() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("NITE");
//DEL 	if (pr)
//DEL 		pr->ConnectToServer(); 
//DEL }

void COrderServApp::OnToolsSecifo() 
{
	if (frmSecIfo::pfrm!=NULL) {
		frmSecIfo::pfrm->MDIActivate();
		if (frmSecIfo::pfrm->IsIconic())
			frmSecIfo::pfrm->MDIRestore();
			return;
	}
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	frmSecIfo::pfrm=(frmSecIfo*)pFrame->CreateNewChild(
		RUNTIME_CLASS(frmSecIfo), IDR_SECIFO, NULL, NULL);
}

void COrderServApp::OnToolsEquity() 
{
	if (frmEquity::pfrm!=NULL) {
		frmEquity::pfrm->MDIActivate();
		if (frmEquity::pfrm->IsIconic())
			frmEquity::pfrm->MDIRestore();
			return;
	}
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	frmEquity::pfrm=(frmEquity*)pFrame->CreateNewChild(
		RUNTIME_CLASS(frmEquity), IDR_EQUITY, NULL, NULL);
}

void COrderServApp::OnViewTraders() 
{
	if (frmTraders::pfrm!=NULL) {
		frmTraders::pfrm->MDIActivate();
		if (frmTraders::pfrm->IsIconic())
			frmTraders::pfrm->MDIRestore();
			return;
	}
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	frmTraders::pfrm=(frmTraders*)pFrame->CreateNewChild(
		RUNTIME_CLASS(frmTraders), IDR_TRADERS, NULL, NULL);
}

//DEL void COrderServApp::OnOrderrouteTest() 
//DEL {
//DEL 
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("TEST");
//DEL 	if (pr)	pr->ConnectToServer(); 
//DEL }

void COrderServApp::OnToolsBalance() 
{
	dlgBalance dlg;
	dlg.DoModal();
}

void COrderServApp::OnViewLoginlog() 
{
	CMDIFrameWnd *pfrm;
	pfrm=(CMDIFrameWnd *)GetMainWnd(); 
	theCfgFile.mLoginLog.ShowLogWindow(pfrm,IDR_LOGINLOG);
	
}

void COrderServApp::OnDataExportposition() 
{
	char fname[400];

	CTime t1;
	t1=CTime::GetCurrentTime();
	sprintf(fname,"%sPositions%04d%02d%02d_%02d%02d%02d.txt",theCfgFile.mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay(),
		t1.GetHour(),t1.GetMinute(),t1.GetSecond());
	Account::SaveAllPositionToFile(fname); 
}

void COrderServApp::OnToolsLoadloginctrl() 
{
	theCfgFile.LoadLogonCtrl();  
}	

void COrderServApp::OnDataCounting() 
{
	CString st;
	CString tmpst;
	int ntotal,nreal;
	
	double dcomm;
	Account::CountAllTickets(ntotal,nreal,dcomm);
	st.Format("Tickets Total:%d,   Real:%d,Comm:%g$",
		ntotal,nreal,dcomm);

	int nadd,ncxl;
	Account::CountAllTrades(ntotal,nreal,nadd,ncxl); 
	tmpst.Format("\r\nTrades Total:%d,  Real:%d,added:%d,canceld:%d",
		ntotal,nreal,nadd,ncxl);
	st+=tmpst;
	
	int nopen,nexed,nexecs;
	Account::CountAllOrders(ntotal,nreal,nopen,nexed,nexecs); 
	tmpst.Format("\r\nOrders Total:%d,  Real:%d,Open:%d,exed:%d,execs:%d",
		ntotal,nreal,nopen,nexed,nexecs	);
	st+=tmpst;

	AfxMessageBox(st);

}

//DEL void COrderServApp::OnOrderrouteGnet() 
//DEL {
//DEL 	OrderRouter *pr;
//DEL 	pr=theCfgFile.GetRoute("GNET");
//DEL 	if (pr)	pr->ConnectToServer(); 
//DEL }


//DEL void COrderServApp::OnOrderrouteConfig() 
//DEL {
//DEL 	
//DEL //	OrderRouteCfg dlg;
//DEL //	dlg.DoModal();
//DEL }

//DEL void COrderServApp::OnOrderrouteIsld() 
//DEL {
//DEL  	OrderRouter *pr;
//DEL  	pr=theCfgFile.GetRoute("ISLD");
//DEL  	if (pr)	pr->ConnectToServer(); 
//DEL }

void COrderServApp::OnViewThreads() 
{
	// TODO: Add your command handler code here
	char str[100];
	sprintf(str,"Max:%d,Curr:%d\r\n"
				"Sockets:%d",
				Msocket::mMaxThreads,Msocket::mCurrThreads,
				Msocket::CountSocket());
	AfxMessageBox(str);
}

void COrderServApp::OnDataRecoverSavetrades() 
{
	// TODO: Add your command handler code here
	itemTrade::SaveAllToDB();
	itemOrder::SaveAllToDB();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char str[300];
	WORD vno;
	vno=theCfgFile.m_Version;
	sprintf(str,"%d,%d",HIBYTE(vno),LOBYTE(vno));
	CStatic *pstatic;
	pstatic=(CStatic *)GetDlgItem(IDC_STVER);
	pstatic->SetWindowText(str); 

	pstatic=(CStatic *)GetDlgItem(IDC_STIFO);
	DWORD sysid;
	encrypt en;
	sysid=en.GetSysID();
	sprintf(str,"%s,%u",theCfgFile.mCompanyID,sysid);
	pstatic->SetWindowText(str); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COrderServApp::OnAaaTest() 
{
	// TODO: Add your command handler code here
	Account *pacc;
	pacc=Account::FindAcc(1028);
	if (pacc==NULL) return;
	theCfgFile.mlog.AddLogStr(pacc->mitemifo.mname);
	pacc->TktRecaluate(FALSE,FALSE);

}

void COrderServApp::OnToolsLoadmmuntradeable() 
{
	CFile file1;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Screen File";
	fdlg.m_ofn.lpstrFilter="MM untradable list File(*.txt)\0*.txt\0*.*\0*.*\0\0";
//	fdlg.m_ofn.lpstrFile=mlastOpenScreenFile;
	fdlg.m_ofn.Flags|= OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;
//	fdlg.m_ofn.nMaxFile=sizeof(mlastOpenScreenFile);  
	fdlg.m_ofn.lpstrDefExt="scr";  
	if (fdlg.DoModal()!=IDOK) return;

	CString filename;
	int ret;
	filename=fdlg.GetPathName();
	ret=file1.Open(filename,CFile::modeRead);
	if (ret==FALSE) {
		AfxMessageBox("Open File Error!");
		return;
	}
	char buf[1000];
	theCfgFile.mpIssueData->RMAllUnTradable(0); 
	CArchive ar( &file1, CArchive::load, 512, buf );
	CString str,id,subid,value;
//	int serpert1,serpert2;
	while(ar.ReadString(str)) {
		str.TrimLeft();
		str.TrimRight();
		if (str.GetLength()<12) {
			theCfgFile.mpIssueData->AddUnTradable(0,str);
		}else {
			AfxMessageBox("Error Security Symbol!");
			break;
		}
	};
	ar.Close();
	file1.Close();
}

void CAboutDlg::OnStifo() 
{
	// TODO: Add your control notification handler code here
//	encrypt en;
//	en.GetSysID(TRUE);
}

void COrderServApp::OnDataSodQuest() 
{
	// TODO: Add your command handler code here
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Quest SOD file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return ; 
	CString sodfilename;
	sodfilename=fdlg.GetFileName();
	Account::LoadSODQuest(sodfilename); 

}


#include "frmOrderRoute.h" 
void COrderServApp::OnRouteConfig() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	if (frmOrderRoute::mfrm!=NULL) {
		frmOrderRoute::mfrm->ActivateFrame(SW_SHOW);
		return;
	}
	pFrame->CreateNewChild(
		RUNTIME_CLASS(frmOrderRoute), IDR_OrderRoute, NULL, NULL);
	
}

void COrderServApp::OnDataSodBpequity() 
{
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open BP & Equity file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return ; 
	CString sodfilename;
	sodfilename=fdlg.GetFileName();
	Account::LoadSODFile(sodfilename); 
	
}

void COrderServApp::OnToolsLoadroutectrl() 
{
	// TODO: Add your command handler code here
	theCfgFile.LoadRouteCtrl();  
}

void COrderServApp::OnToolsCommand() 
{
	// TODO: Add your command handler code here
	CInputBox input;
	if (input.Show(NULL,"Command","Input the command to run:")==IDOK) {
		COrdScript scpt;
		scpt.RunCmds(input.GetText());
	}
}

static UINT testcrash(LPVOID lparam) 
{
	char *str;
	str=0;
	str[0]=0;

	return 0;
}

void CAboutDlg::OnBCrash() 
{
	// TODO: Add your control notification handler code here
//*
	AfxBeginThread(testcrash,(LPVOID)this);
/*/
	char *str;
	str=0;
	str[0]=0;
//*/
}

#include "frmScript.h"

void COrderServApp::OnToolsScriptlist() 
{
	CMainFrame* pFrame = STATIC_DOWNCAST(CMainFrame, m_pMainWnd);
	if (frmScript::mfrm!=NULL) {
		frmScript::mfrm->ActivateFrame(SW_SHOW);
		return;
	}
	pFrame->CreateNewChild(
		RUNTIME_CLASS(frmScript), IDR_OrderRoute, NULL, NULL);
	
	
}

void CAboutDlg::OnBPref() 
{
	// TODO: Add your control notification handler code here
	Perf_Show(); 
}

void COrderServApp::OnLogFile() 
{


	char pathname[400];

	CTime t1=CTime::GetCurrentTime();
	sprintf(pathname,"%sOSVLOG%04d%02d%02d.txt",theCfgFile.mPathLog, t1.GetYear(),t1.GetMonth(),t1.GetDay());
	ShellExecute(NULL,"open","notepad",pathname,NULL,SW_SHOWNORMAL ); 
	
}

BOOL COrderServApp::CheckDatabase()
{

	int dbversion;
	dbversion=theOrderDB.ChangeSysID("DBVER",0);
	theCfgFile.mlog.AddLogStrFmt("Current Database Version:%d",dbversion);
	if (dbversion==0) {
		//For sql server
		/*char *str="if not exists (select * from syscolumns "
					" where id=object_id('mdeal') and name='symbolfee') "
					" alter table mdeal add symbolfee float NULL ";*/

		//Postgre
		char *str = "DO "
			"$$ "
			"BEGIN "
			"IF NOT EXISTS(select * from information_schema.columns WHERE table_schema = 'public' AND table_name = 'mdeal' AND column_name = 'symbolfee') THEN "
			"alter table mdeal add symbolfee float NULL; "
			"else "
			"raise NOTICE 'Already exists'; "
			"END IF; "
			"END "
			"$$ " ;
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);
		dbversion=theOrderDB.ChangeSysID("DBVER",1);
	}
	if (dbversion<2) {
		char *str="CREATE TABLE mExchAgree IF NOT EXISTS (trid int ,type int,event int, note varchar(255), logdate timestamp)";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);
		dbversion=theOrderDB.ChangeSysID("DBVER",1);
	}
	if (dbversion<3) {
		char *str="CREATE TABLE mAccCtrlEx (accid int  PRIMARY KEY,flag int,flagex int, maxloss float, maxshare int, maxtotalshare int,maxsymbshare int)";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);
		dbversion=theOrderDB.ChangeSysID("DBVER",1);

	}
	if (dbversion<4){
		char *str="if not exists (select * from syscolumns "
					" where id=object_id('mAccCtrlEx') and name='maxsymbs') "
					" alter table mAccCtrlEx add maxsymbs int NULL,maxunrealizedloss float NULL, starttradetime int NULL,"
					" stoptradetime int NULL ";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);

		str="CREATE TABLE mAccCtrlEx_Symb (accid int, symbol varchar(12),maxpos int,maxloss float,maxorder int)";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);


		dbversion=theOrderDB.ChangeSysID("DBVER",1);
	}
	if (dbversion<5){

		char *str="CREATE TABLE mAccCtrlEx_Route(accid int, mroute varchar(8),mflag int)";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);
		dbversion=theOrderDB.ChangeSysID("DBVER",1);
	}
	if (dbversion<6) { //each Admin may have multiple branch along whith the main branch.
		char *str="CREATE TABLE mAdmBranchList(trid int,brid int, stat1 int)";
		theCfgFile.mlog.AddLogStrFmt("RunSQL:%s",str);
		theOrderDB.mdb.ExecuteSQL(str);
		dbversion=theOrderDB.ChangeSysID("DBVER",1);
	}
	return 0;
}

void COrderServApp::OnToolsDbreconn() 
{
	
	theOrderDB.CloseConnection();
	theCfgFile.mlog.AddLogStrFmtTm(0,"Disconnected to database"); 
	BOOL dbret;
	dbret =theOrderDB.ConnectOrderSV(theCfgFile.mdbConnectString );
	if (dbret==FALSE) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"Connect to database fail!"); 
	}else {
		theCfgFile.mlog.AddLogStrFmtTm(0,"Connect to database successed!"); 
	}

		
}
