// frmlevel1.cpp : implementation file
//
#define _FRM_LV1_FILE
#include "stdafx.h"
#include "..\..\Servers\RiskCtrl\riskctrl.h"
#include "ViewLv1.h" 

#include "frmlevel1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCriticalSection frmLevel1::mFrmCritical; // the critical window;
ViewLv1 * frmLevel1::pviewL1;  // the level1 window

/////////////////////////////////////////////////////////////////////////////
// frmLevel1

IMPLEMENT_DYNCREATE(frmLevel1, CMDIChildWnd)

frmLevel1::frmLevel1()
{
} 

frmLevel1::~frmLevel1()
{
}


BEGIN_MESSAGE_MAP(frmLevel1, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmLevel1)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmLevel1 message handlers

BOOL frmLevel1::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	pContext->m_pNewViewClass=RUNTIME_CLASS(ViewLv1); 
	mFrmCritical.Lock();
	pviewL1=(ViewLv1*)CreateView(pContext);
	mFrmCritical.Unlock();
	/*	theCfgFile.mCritical.Lock();
	mpnext=theCfgFile.mpTimeSellHead;
	theCfgFile.mpTimeSellHead=this;
	theCfgFile.mCritical.Unlock();
*/
	return TRUE;

//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void frmLevel1::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	mFrmCritical.Lock();
	pviewL1=NULL;
	mFrmCritical.Unlock();
	
	CMDIChildWnd::OnClose();
}
