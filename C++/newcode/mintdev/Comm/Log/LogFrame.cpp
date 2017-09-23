// logframe.cpp : implementation file
//

#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogFrame

IMPLEMENT_DYNCREATE(CLogFrame, CMDIChildWnd)

CLogFrame::CLogFrame()
{
	plog =NULL;
}

CLogFrame::~CLogFrame()
{
}


BEGIN_MESSAGE_MAP(CLogFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CLogFrame)
	ON_WM_DESTROY()
	ON_COMMAND(200, OnUpadtelog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogFrame message handlers

BOOL CLogFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect rect;
	GetClientRect(rect);
	medit.Create(ES_MULTILINE|WS_VSCROLL|WS_HSCROLL ,rect,this,AFX_IDW_PANE_FIRST );
	medit.ShowWindow(SW_SHOW);
	medit.SetLimitText(0);
//	CFont f1;
//	f1.CreatePointFont(200,"Courier New");
//	medit.SetFont(&f1);
	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CLogFrame::OnDestroy() 
{
	plog->mCritical.Lock(); 
	plog->mpEdLog=NULL;
	plog->mpFrame=NULL;
	plog->mCritical.Unlock();
	
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CLogFrame::SetLogObject(CStreamLog *plogob)
{
	plog=plogob;
	plog->mpEdLog=&medit; 

}

void CLogFrame::OnUpadtelog() 
{
	// TODO: Add your command handler code here
	if (plog!=NULL) {
		plog->mCritical.Lock(); 
		if (plog->mcurrlen!=0){ 
			int totalline;
			medit.SetSel(0x7fffffff, -1);
//			medit->ReplaceSel ("\xd\xa");
			medit.ReplaceSel(plog->mpbuff);
			medit.SetSel(0x7fffffff, -1,TRUE);
			plog->mcurrlen=0;
			totalline=medit.GetLineCount();
			if (totalline>=1000) {
				medit.SetSel(0,medit.LineIndex(totalline-500),TRUE);
				medit.ReplaceSel("");
				//mloglinenum=mpLog->GetLineCount();3
			}
		} 
		plog->mCritical.Unlock();
	}
	//mloglinenum++; 
	
}
