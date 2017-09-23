// logframe.cpp : implementation file
//

#include "stdafx.h"
//#include "..\..\Servers\FeedCenter\FeedCenter.h"
#include "SmtLog.h"

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

	{
		LOGFONT fnt;
		memset(&fnt,0,sizeof(fnt));
		strcpy(fnt.lfFaceName, "Arial");
		fnt.lfHeight = 14;
		fnt.lfWeight = 400;
		if(m_log_font.m_hObject){
			m_log_font.DeleteObject();
		}
		m_log_font.CreateFontIndirect(&fnt);

		medit.SetFont(&m_log_font);
	}



	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CLogFrame::OnDestroy() 
{
	plog->mCritical.Lock(); 
//	plog->mpEdLog=NULL;
	plog->mpFrame=NULL;
	plog->mCritical.Unlock();
	
	CMDIChildWnd::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CLogFrame::SetLogObject(CTxtLog *plogob)
{
	plog=plogob;
//	plog->mpEdLog=&medit; 

}

void CLogFrame::OnUpadtelog() 
{
	// TODO: Add your command handler code here

	if (plog!=NULL) {
		plog->UpdateLog_Edit(&medit); 
	}
	//mloglinenum++; 
	
}
