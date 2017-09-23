// dlgIPBox.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "dlgIPBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dlgIPBox dialog


dlgIPBox::dlgIPBox(CWnd* pParent /*=NULL*/)
	: CDialog(dlgIPBox::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlgIPBox)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	mPort=0;
	mIPStr[0]=0;
	mTitle[0]=0;
}


void dlgIPBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlgIPBox)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlgIPBox, CDialog)
	//{{AFX_MSG_MAP(dlgIPBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlgIPBox message handlers

BOOL dlgIPBox::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDIP);
	ped->SetWindowText(mIPStr);
	char portst[100];
	ped=(CEdit*)GetDlgItem(IDC_EDPORT);
	sprintf(portst,"%d",mPort);
	ped->SetWindowText(portst);
	SetWindowText(mTitle);
	// TODO: Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}



DWORD dlgIPBox::GetIP()
{
	return inet_addr(mIPStr); 
}

BOOL dlgIPBox::SetIP(DWORD nip)
{
	sprintf(mIPStr,"%d.%d.%d.%d",
			nip&0xff,(nip>>8 )&0xff, 
			(nip>>16)&0xff,(nip>>24)&0xff);
	return TRUE;
}

void dlgIPBox::SetTitle(char *ptitle)
{
	strcpy(mTitle,ptitle); 
}

void dlgIPBox::SetPort(WORD nport)
{
	mPort=nport;
}

WORD dlgIPBox::GetPort()
{
	return mPort;
}

void dlgIPBox::OnOK() 
{
	// TODO: Add extra validation here
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDIP);
	ped->GetWindowText (mIPStr,sizeof(mIPStr));
	DWORD nip;
	nip=inet_addr(mIPStr);
	if ((nip==0)||(nip==0xffffffff)) {
		AfxMessageBox("Wrong IP Address!"); 
		return;
	}

	ped=(CEdit*)GetDlgItem(IDC_EDPORT);
	char portst[20];
	ped->GetWindowText(portst,sizeof(portst));
	mPort=(WORD)atoi(portst);
	if (mPort==0){
		AfxMessageBox("Wrong Port Number");
		return;
	}
	
	CDialog::OnOK();
}
