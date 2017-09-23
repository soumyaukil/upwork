// OrderRouteCfg.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "OrderRouteCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern OrderConfig theCfgFile;
/////////////////////////////////////////////////////////////////////////////
// OrderRouteCfg dialog



/////////////////////////////////////////////////////////////////////////////
// OrderRouteCfg1 dialog


OrderRouteCfg1::OrderRouteCfg1(CWnd* pParent /*=NULL*/)
	: CDialog(OrderRouteCfg1::IDD, pParent)
{
	//{{AFX_DATA_INIT(OrderRouteCfg1)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void OrderRouteCfg1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(OrderRouteCfg1)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(OrderRouteCfg1, CDialog)
	//{{AFX_MSG_MAP(OrderRouteCfg1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// OrderRouteCfg1 message handlers

BOOL OrderRouteCfg1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CStatic *pst;
	pst=(CStatic*)GetDlgItem(IDC_STRoute);
	pst->SetWindowText(mRouteName);
	// TODO: Add extra initialization here
	char str[100];
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDIP);
	DWORD nip;
	nip=mIP;
	sprintf(str,"%d.%d.%d.%d",nip&0xff,(nip>>8 )&0xff, 
			(nip>>16)&0xff,(nip>>24)&0xff);
	ped->SetWindowText(str);
	
	ped=(CEdit*)GetDlgItem(IDC_EDPORT);
	sprintf(str,"%d",mPort);
	ped->SetWindowText(str);

	ped=(CEdit*)GetDlgItem(IDC_EDUID);
	ped->SetWindowText(mUserID);
	
	ped=(CEdit*)GetDlgItem(IDC_EDPWD);
	ped->SetWindowText(mUserPWD);

	ped=(CEdit*)GetDlgItem(IDC_EDSTART);
	sprintf(str,"%02d:%02d",mtmstart/60,mtmstart%60);
	ped->SetWindowText(str);

	ped=(CEdit*)GetDlgItem(IDC_EDSTOP);
	sprintf(str,"%02d:%02d",mtmstop/60,mtmstop%60);
	ped->SetWindowText(str);
	
	CButton *pbut;
	pbut=(CButton*)GetDlgItem(IDC_CAutoConnect);
	pbut->SetCheck(mautoconnect?1:0); 

	CheckDlgButton(IDC_CHKCancelReplace,mproute->mCxlReplaceEnable); 
	CheckDlgButton(IDC_CHKHide,mproute->m_bHideRoute); 
	CheckDlgButton(IDC_CHKDisableCustomerOrder,mproute->m_bDisableCustomerOrder);

	char stexfilter[30];
	mproute->GetDisableExch(stexfilter);
	SetDlgItemText(IDC_EDEXCHFILTER,stexfilter);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void OrderRouteCfg1::OnOK() 
{
	char str[100];
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDIP);
	DWORD nip;
	nip=mIP;
	ped->GetWindowText(str,sizeof(str));
	mIP=inet_addr(str);

	ped=(CEdit*)GetDlgItem(IDC_EDPORT);
	ped->GetWindowText(str,sizeof(str));
	mPort=atoi(str);

	ped=(CEdit*)GetDlgItem(IDC_EDUID);
	ped->GetWindowText(mUserID,sizeof(mUserID));
	
	ped=(CEdit*)GetDlgItem(IDC_EDPWD);
	ped->GetWindowText(mUserPWD,sizeof(mUserPWD));



	ped=(CEdit*)GetDlgItem(IDC_EDSTART);
	ped->GetWindowText(str,sizeof(str));
	mtmstart=CConfig::GetTimeVal(str);  

	ped=(CEdit*)GetDlgItem(IDC_EDSTOP);
	ped->GetWindowText(str,sizeof(str));
	mtmstop=CConfig::GetTimeVal(str);  

	CButton *pbut;
	pbut=(CButton*)GetDlgItem(IDC_CAutoConnect);
	if (pbut->GetCheck()!=0){
		mautoconnect=1;
	}else {
		mautoconnect=0;
	}

	if (IsDlgButtonChecked(IDC_CHKCancelReplace)){
		mproute->mCxlReplaceEnable=1;
	}else {
		mproute->mCxlReplaceEnable=0;
	}

	if (IsDlgButtonChecked(IDC_CHKHide)){
		mproute->m_bHideRoute=TRUE;
	}else {
		mproute->m_bHideRoute=FALSE;
	}
	mproute->m_bDisableCustomerOrder=IsDlgButtonChecked(IDC_CHKDisableCustomerOrder);

	char stexfilter[30];
	GetDlgItemText(IDC_EDEXCHFILTER,stexfilter,sizeof(stexfilter));
	mproute->SetDisableExch(stexfilter);
	
	CDialog::OnOK();
}
