// dragdroptextbox.cpp : implementation file
//

#include "stdafx.h"
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h >       // MFC OLE automation classes
#include <afxpriv.h>
#include "dragdroptextbox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	BUTTON_EVENT		1
#define	BUTTON_EVENT_TIME	100

#define	MINIMUM_MOVE_SQUARE	100	//	10 pixels

IMPLEMENT_DYNAMIC(CDragDropTextBox, CEdit)	//	Want info for drag

/////////////////////////////////////////////////////////////////////////////
// CDragDropTextBox

CDragDropTextBox::CDragDropTextBox()
		:	m_TargetDrop(NULL),
		m_TimerID(0)
{
	m_StartPoint = CPoint(-1, -1);

	//	make us
	m_TargetDrop = new ECMaterialDropText();

}

CDragDropTextBox::~CDragDropTextBox()
{
}


BEGIN_MESSAGE_MAP(CDragDropTextBox, CEdit)
	//{{AFX_MSG_MAP(CDragDropTextBox)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDragDropTextBox message handlers

void CDragDropTextBox::InitDrag()
{
		if(m_TargetDrop)
		m_TargetDrop->Register(this, CF_TEXT);

}

void CDragDropTextBox::OnDestroy() 
{
	if(m_TargetDrop)
	{
		//	stop drag and drop
		m_TargetDrop->Revoke();

		//	KILL!
		delete m_TargetDrop;
	}
	m_TargetDrop = NULL;


	CEdit::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CDragDropTextBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_StartPoint = 	point;
	m_TimerID = SetTimer(BUTTON_EVENT, BUTTON_EVENT_TIME, NULL);
	CEdit::OnLButtonDown(nFlags, point);
}

void CDragDropTextBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_StartPoint.x = -100;
	m_StartPoint.y = -100;
	
	if(m_TimerID)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}

	CEdit::OnLButtonUp(nFlags, point);
}

void CDragDropTextBox::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_TimerID > 0)
	{
		//	check if we really moved enough
/*		int iX = m_StartPoint.x - point.x;
		int iY = m_StartPoint.y - point.y;
		if((iX*iX + iY*iY) > MINIMUM_MOVE_SQUARE)
*/
		CRect rect;
		GetClientRect( rect);
		if (!rect.PtInRect(point))
		{
			CString iText;
			GetWindowText(iText);
			if (iText.IsEmpty()!=TRUE) {
				COleDataSource*	pSource = new COleDataSource();
				if(pSource)
				{
					CSharedFile	sf(GMEM_MOVEABLE|GMEM_DDESHARE|GMEM_ZEROINIT);
					//	write name to clipboard
					sf.Write(iText, iText.GetLength());

					HGLOBAL hMem = sf.Detach();
					if (!hMem) 
						return;
					pSource->CacheGlobalData(CF_TEXT, hMem);

					//	Do drag and drop!
					DROPEFFECT pfect;
					pfect=pSource->DoDragDrop();
					switch (pfect) {
					case DROPEFFECT_COPY:
						break;
					case DROPEFFECT_MOVE:
						SetWindowText("");
						break;
					}


					//	free source
					delete pSource;
				}
			}
		}
	}
	CEdit::OnMouseMove(nFlags, point);
}

void CDragDropTextBox::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == BUTTON_EVENT)
	{
		POINT pt;
		::GetCursorPos(&pt);
		CRect iRect;
		GetWindowRect(iRect);
		if(!(iRect.PtInRect(pt)))
		{
			KillTimer(nIDEvent);
			m_TimerID = 0;
		}
	}
	

	CEdit::OnTimer(nIDEvent);
}
