// dlgBalance.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "dlgBalance.h"
#include "dlgIPBox.h"
#include "OrderConfig.h"
#include "Functions\strfuncs.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern OrderConfig theCfgFile;
/////////////////////////////////////////////////////////////////////////////
// dlgBalance dialog


dlgBalance::dlgBalance(CWnd* pParent /*=NULL*/)
	: CDialog(dlgBalance::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlgBalance)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlgBalance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlgBalance)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlgBalance, CDialog)
	//{{AFX_MSG_MAP(dlgBalance)
	ON_BN_CLICKED(IDC_BRefresh, OnBRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlgBalance message handlers

//DEL void dlgBalance::OnBQuote() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	int i;
//DEL 	int servpos=-1;
//DEL 	for (i=0;i<theCfgFile.mServerNumber;i++){
//DEL 		if (theCfgFile.mServerIDlst[i]==20){
//DEL 			servpos=i;
//DEL 			break;
//DEL 		}
//DEL 	}
//DEL 	if (servpos==-1) return;
//DEL 	
//DEL 	dlgIPBox dlg;
//DEL 	dlg.SetTitle("Test(Training) Route Server config!");
//DEL 	dlg.SetIP(theCfgFile.mServerIPlst[servpos]);
//DEL 	dlg.SetPort(theCfgFile.mServerPortlst[servpos]);
//DEL 	if (dlg.DoModal()==IDCANCEL) {
//DEL 		return;
//DEL 	}
//DEL 	theCfgFile.mServerIPlst[servpos]=dlg.GetIP();
//DEL 	theCfgFile.mServerPortlst[servpos]=dlg.GetPort();
//DEL 	
//DEL 	
//DEL }

//DEL void dlgBalance::OnBQuote1() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	int i;
//DEL 	int servpos=-1;
//DEL 	for (i=0;i<theCfgFile.mServerNumber;i++){
//DEL 		if (theCfgFile.mServerIDlst[i]==21){
//DEL 			servpos=i;
//DEL 			break;
//DEL 		}
//DEL 	}
//DEL 	if (servpos==-1) return;
//DEL 	
//DEL 	dlgIPBox dlg;
//DEL 	dlg.SetTitle("Test(Training) Route Server config!");
//DEL 	dlg.SetIP(theCfgFile.mServerIPlst[servpos]);
//DEL 	dlg.SetPort(theCfgFile.mServerPortlst[servpos]);
//DEL 	if (dlg.DoModal()==IDCANCEL) {
//DEL 		return;
//DEL 	}
//DEL 	theCfgFile.mServerIPlst[servpos]=dlg.GetIP();
//DEL 	theCfgFile.mServerPortlst[servpos]=dlg.GetPort();
//DEL }

void dlgBalance::OnBRefresh() 
{
	// TODO: Add your control notification handler code here
	int i;
	int servpos=-1;
	char str[100];
	for (i=0;i<theCfgFile.mServerNumber;i++){
		if (theCfgFile.mServerIDlst[i]==20){
			CStrFunc::FormatIPAddr(theCfgFile.mServerIPlst[i],str);
			SetDlgItemText(IDC_EDIP1,str);
			sprintf(str,"%d",theCfgFile.mServerPortlst[i]);
			SetDlgItemText(IDC_EDPORT1,str);	
			
		}else if (theCfgFile.mServerIDlst[i]==21){
			CStrFunc::FormatIPAddr(theCfgFile.mServerIPlst[i],str);
			SetDlgItemText(IDC_EDIP2,str);
			sprintf(str,"%d",theCfgFile.mServerPortlst[i]);
			SetDlgItemText(IDC_EDPORT2,str);	
		}
	}
}

BOOL dlgBalance::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	OnBRefresh();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void dlgBalance::OnOK() 
{
	// TODO: Add extra validation here
	int i;
	int servpos=-1;
	char str[100];
	for (i=0;i<theCfgFile.mServerNumber;i++){
		if (theCfgFile.mServerIDlst[i]==20){
			GetDlgItemText(IDC_EDIP1,str,sizeof(str));
			theCfgFile.mServerIPlst[i]=inet_addr(str);
			GetDlgItemText(IDC_EDPORT1,str,sizeof(str));
			theCfgFile.mServerPortlst[i]=atoi(str);
		}else if (theCfgFile.mServerIDlst[i]==21){
			GetDlgItemText(IDC_EDIP2,str,sizeof(str));
			theCfgFile.mServerIPlst[i]=inet_addr(str);
			GetDlgItemText(IDC_EDPORT2,str,sizeof(str));
			theCfgFile.mServerPortlst[i]=atoi(str);
		}
	}
	
	CDialog::OnOK();
}
