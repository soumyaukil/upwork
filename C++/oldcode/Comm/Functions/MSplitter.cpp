// MSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "MSplitter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CMSplitter_Info::CMSplitter_Info()
{
	m_Parent=NULL;
	m_FirstChild=NULL;
	m_Prev=NULL;
	m_Next=NULL;
	
	m_Splitter_Wnd=NULL;
	
	m_nTotalChild=0;
	m_Percent=100;
	m_type=0;

}

CMSplitter_Info::~CMSplitter_Info()
{
	if (m_Parent) {
		if (m_Parent->m_FirstChild==this) {
			m_Parent->m_FirstChild=m_Next; 
		}
		m_Parent->m_nTotalChild--;
	}
	
	if (m_Prev) {
		m_Prev->m_Next=m_Next;
	}
	
	if (m_Next) {
		m_Next->m_Prev=m_Prev;
	}
	
	while(m_FirstChild!=NULL) {
		CMSplitter_Info *pifo=m_FirstChild->m_Next;
		delete(m_FirstChild);
		m_FirstChild=pifo;
	}

	if (IsWindow(m_wnd)) {
		DestroyWindow(m_wnd);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMSplitter

CMSplitter::CMSplitter()
{
	m_SplitterHead.SetSplitter(this);
	m_BoardSize=4;

	m_CurrentSplitter=NULL;

}

CMSplitter::~CMSplitter()
{
}


BEGIN_MESSAGE_MAP(CMSplitter, CWnd)
	//{{AFX_MSG_MAP(CMSplitter)
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMSplitter message handlers

CMSplitter_Info *CMSplitter_Info::AddWnd(HWND hwnd, int nidx)
{
	CMSplitter_Info *pifo= new CMSplitter_Info;
	pifo->SetSplitter(m_Splitter_Wnd);
	pifo->m_wnd=hwnd;
	pifo->m_Parent=this;
	m_nTotalChild++;

	if (m_FirstChild==NULL) {
		m_FirstChild=pifo;
	}else if (nidx==0) {
		pifo->m_Next=m_FirstChild;
		m_FirstChild->m_Prev=pifo; 
		m_FirstChild=pifo;
	}else {
		CMSplitter_Info *p;
		p=m_FirstChild;
		int i=1;
		while(i<nidx) {
			if (p->Next()==NULL) break;
			p=p->Next();
			i++;
		}
		pifo->m_Next=p->Next();
		pifo->m_Prev=p;
		if (p->m_Next!=NULL) {
			pifo->m_Next->m_Prev=pifo;
		}
		p->m_Next=pifo;
		
	}
	return pifo;

}

CMSplitter_Info* CMSplitter_Info::GetChild(int nidx)
{
	if (nidx==0) return m_FirstChild;
	int i;
	CMSplitter_Info* pret=m_FirstChild;
	for (i=0;i<nidx;i++) {
		if (pret==NULL) return NULL;
		pret=pret->Next();
	}
	return pret;
}

void CMSplitter::RecalcLayout()
{

	CRect rec;
	GetClientRect(&rec);
	m_SplitterHead.SizeAlloc(rec,NULL);
	Invalidate();
//	m_SplitterHead.m_rec
//	m_SplitterHead.m_rect_border=rec;

}

void CMSplitter_Info::SizeAlloc(LPRECT prect, LPRECT pSplitter)
{
	m_rect_wnd=prect;
	if (pSplitter!=NULL) m_rect_splitter= pSplitter;
	else m_rect_splitter.SetRectEmpty();
	if (IsWindow(m_wnd)) {
		MoveWindow(m_wnd,m_rect_wnd.left, m_rect_wnd.top,
					m_rect_wnd.Width(),m_rect_wnd.Height(),FALSE);
	}

	double totalratio;
	totalratio=0;
	CMSplitter_Info *pifo;
	pifo=m_FirstChild;
	while(pifo) {
		totalratio+=pifo->m_Percent;
		pifo=pifo->Next();
	}
	
	if (totalratio<=0) return;

	int totalsize;

	totalsize=(m_type==0)?m_rect_wnd.Width():m_rect_wnd.Height();
	totalsize-=(m_nTotalChild-1)*m_Splitter_Wnd->m_BoardSize;
	if (totalsize<0) totalsize=0;
	pifo=m_FirstChild;
	CRect recwnd;
	CRect recsplit;
	recwnd=recsplit=prect;
	int lastpos=(m_type==0)?recwnd.left:recwnd.top;
	int endpos =(m_type==0)?recwnd.right:recwnd.bottom;
	while(pifo) {
		int newsz;
		newsz=(int)(totalsize*pifo->m_Percent/totalratio);
		if (m_type==0) {
			recwnd.left=lastpos;
			if (pifo->Next()) {
				recwnd.right=lastpos+newsz;
				if (recwnd.right>endpos) recwnd.right=endpos;
				recsplit.left=recwnd.right;
				recsplit.right=recsplit.left+m_Splitter_Wnd->m_BoardSize;
				if (recsplit.right>endpos)  recsplit.right=endpos;
				lastpos=recsplit.right;
			}else {
				recwnd.right=endpos;
				recsplit.SetRectEmpty(); 
			}
		}else{
			recwnd.top=lastpos;
			if (pifo->Next()) {
				recwnd.bottom=lastpos+newsz;
				if (recwnd.bottom>endpos) recwnd.bottom=endpos;
				recsplit.top=recwnd.bottom;
				recsplit.bottom=recsplit.top+m_Splitter_Wnd->m_BoardSize;
				if(recsplit.bottom>endpos) recsplit.bottom=endpos;
				lastpos=recsplit.bottom;
			}else {
				recwnd.bottom=m_rect_wnd.bottom;
				recsplit.SetRectEmpty(); 
			}
		}
		pifo->SizeAlloc(recwnd,recsplit); 
		pifo=pifo->Next();
	}
}

void CMSplitter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	RecalcLayout();
}

void CMSplitter::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (m_CurrentSplitter==NULL) {
		m_CurrentSplitter=m_SplitterHead.GetSplitterByPt(point);
		if (m_CurrentSplitter) {
			m_lastpt=point;
			m_lastSpTopLeft=m_CurrentSplitter->m_rect_splitter.TopLeft(); 
			m_lastmovetype=m_CurrentSplitter->GetMoveType();

			int totalsz=0;
			CMSplitter_Info *pifo;
			pifo=m_CurrentSplitter;
			m_pc1=m_CurrentSplitter->m_Percent; 
			totalsz+=(m_lastmovetype==0)?pifo->m_rect_wnd.Width():pifo->m_rect_wnd.Height();
			pifo=m_CurrentSplitter->Next();
			if(pifo) {
				m_pc2=pifo->m_Percent;
				totalsz+=(m_lastmovetype==0)?pifo->m_rect_wnd.Width():pifo->m_rect_wnd.Height();
			}else {
				m_pc2=0;
			}

			if (totalsz)m_pixperpc=(m_pc1+m_pc2)/totalsz;
			else m_pixperpc=0;
			if (pifo->GetMoveType()==0) {
				SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			}else {
				SetCursor(LoadCursor(NULL,IDC_SIZENS));
			}

			SetCapture();
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMSplitter::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_CurrentSplitter) {// move splitter;
		int dlt;
		if (m_lastmovetype==0) {//horize
			dlt=point.x-m_lastpt.x;
		}else {//vertical;
			dlt=point.y-m_lastpt.y;
		}
		double dltpercent=dlt*m_pixperpc;
		double npc1,npc2;
		npc2=m_pc2-dltpercent;
		npc1=m_pc1+dltpercent;
		if (npc1<0){
			npc1=0;
			npc2=m_pc1+m_pc2;
		};
		if (npc2<0) {
			npc2=0;
			npc1=m_pc1+m_pc2;
		}
		m_CurrentSplitter->m_Percent=npc1;

		CMSplitter_Info *pifo;
		pifo=m_CurrentSplitter->Next();
		if(pifo) {
			pifo->m_Percent=npc2;
		}
		RecalcLayout();
	}else {
		CMSplitter_Info *pifo=m_SplitterHead.GetSplitterByPt(point);
		if (pifo) {
			if (pifo->GetMoveType()==0) {
				SetCursor(LoadCursor(NULL,IDC_SIZEWE));
			}else {
				SetCursor(LoadCursor(NULL,IDC_SIZENS));
			}
		}



	}
	CWnd::OnMouseMove(nFlags, point);
}

void CMSplitter::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_CurrentSplitter) {
		ReleaseCapture();
		m_CurrentSplitter=NULL;
	}
	CWnd::OnLButtonUp(nFlags, point);
}

CMSplitter_Info * CMSplitter_Info::GetSplitterByPt(CPoint point)
{
	if (m_rect_splitter.PtInRect(point)) return this;
	CMSplitter_Info *pifo;
	CMSplitter_Info *ret;
	pifo=m_FirstChild;
	while(pifo) {
		ret=pifo->GetSplitterByPt(point);
		if (ret) return ret;
		pifo=pifo->Next();
	}
	return  NULL;
}

void CMSplitter::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	m_SplitterHead.PaintGribber(&dc); 
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CMSplitter::OnCommand(WPARAM wParam, LPARAM lParam)
{
	if (CWnd::OnCommand(wParam, lParam))
		return TRUE;

	// route commands to the splitter to the parent frame window
	return GetParent()->SendMessage(WM_COMMAND, wParam, lParam);
}

BOOL CMSplitter::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	if (CWnd::OnNotify(wParam, lParam, pResult))
		return TRUE;

	// route commands to the splitter to the parent frame window
	*pResult = GetParent()->SendMessage(WM_NOTIFY, wParam, lParam);
	return TRUE;
}



void CMSplitter_Info::PaintGribber(CDC *pdc)
{
	if ((m_rect_splitter.Width()>0)
		&&(m_rect_splitter.Height()>0)){
		pdc->FillSolidRect(m_rect_splitter,RGB(128,128,128)); 
		pdc->Draw3dRect(m_rect_splitter,RGB(200,200,200),RGB(100,100,100));  
	}
	CMSplitter_Info *pifo;
	pifo=m_FirstChild;
	while(pifo) {
		pifo->PaintGribber(pdc);
		pifo=pifo->Next();
	}
}
