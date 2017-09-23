// dlgPopMsg.cpp : implementation file
//

#include "stdafx.h"
#include "orderserv.h"
#include "dlgPopMsg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
extern COrderServApp theApp;
/////////////////////////////////////////////////////////////////////////////
// dlgPopMsg dialog


dlgPopMsg::dlgPopMsg(CWnd* pParent /*=NULL*/)
	: CDialog(dlgPopMsg::IDD, pParent)
{
	//{{AFX_DATA_INIT(dlgPopMsg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void dlgPopMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(dlgPopMsg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(dlgPopMsg, CDialog)
	//{{AFX_MSG_MAP(dlgPopMsg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// dlgPopMsg message handlers

void dlgPopMsg::OnCancel() 
{
	CDialog::DestroyWindow();
}

void dlgPopMsg::OnOK() 
{
	CDialog::DestroyWindow();
}

void dlgPopMsg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	delete this;	
}

BOOL dlgPopMsg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CStatic *pstatic;
	pstatic=(CStatic*)GetDlgItem(IDC_STICON);
	pstatic->SetIcon(theApp.LoadStandardIcon(IDI_ASTERISK)); 

	pstatic=(CStatic*)GetDlgItem(IDC_EDMSG);
	pstatic->SetWindowText(stmsg); 
	
	
	return TRUE;  
}
