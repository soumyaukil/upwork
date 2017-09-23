// ColorPickStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorPickStatic.h"
#include "strfuncs.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ColorPickStatic

ColorPickStatic::ColorPickStatic()
{
}

ColorPickStatic::~ColorPickStatic()
{
}


BEGIN_MESSAGE_MAP(ColorPickStatic, CWnd)
	//{{AFX_MSG_MAP(ColorPickStatic)
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ColorPickStatic message handlers


void ColorPickStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(rect);
	CBrush brush;
	brush.CreateSolidBrush(mcolor); 
	CBrush *poldbrush;
	poldbrush=dc.SelectObject(&brush);
	dc.Rectangle(rect); 
	dc.SelectObject(poldbrush);
}

void ColorPickStatic::OnClicked() 
{
	// TODO: Add your control notification handler code here
	CColorDialog dlg;
	if (dlg.DoModal()==IDOK) {
		mcolor=dlg.GetColor();
		Invalidate();			
//		mbrush.DeleteObject();
//		mbrush.CreateSolidBrush(mcolor); 
	}; 

}

void ColorPickStatic::SetColor(COLORREF ncolor)
{
	mcolor=ncolor;
	if (IsWindow(m_hWnd)) Invalidate();  
}
/////////////////////////////////////////////////////////////////////////////
// PowerStatic

PowerStatic::PowerStatic()
{
	mcolFground=RGB(0,0,0);
	mcolBground=GetSysColor(COLOR_3DFACE);
//	mpenFground.CreatePen(PS_SOLID,1,mcolFground);
//	mbrushBground.CreateSolidBrush(mcolBground); 
	mclickaction=0;
	m_hfont=NULL;
	memset(mfont,0,sizeof(mfont));

}

PowerStatic::~PowerStatic()
{
	if (m_hfont!=NULL) {
		DeleteObject(m_hfont);
	}
}


BEGIN_MESSAGE_MAP(PowerStatic, CStatic)
	//{{AFX_MSG_MAP(PowerStatic)
	ON_CONTROL_REFLECT(BN_CLICKED, OnClicked)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PowerStatic message handlers

//DEL HBRUSH PowerStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
//DEL {
//DEL 	// TODO: Change any attributes of the DC here
//DEL //	pDC->SelectObject(&mpenFground);
//DEL //	pDC->SelectObject(&mbrushBground);
//DEL 	// TODO: Return a non-NULL brush if the parent's handler should not be called
//DEL 	pDC->SetBkColor(mcolBground);
//DEL 	pDC->SetTextColor(mcolFground); 
//DEL //	return (HBRUSH)GetStockObject(GRAY_BRUSH);
//DEL //	return (HBRUSH)mbrushBground;
//DEL 	return NULL;
//DEL }




//DEL void PowerStatic::OnLButtonDown(UINT nFlags, CPoint point) 
//DEL {
//DEL 	// TODO: Add your message handler code here and/or call default
//DEL 	
//DEL 	CStatic::OnLButtonDown(nFlags, point);
//DEL }

void PowerStatic::OnClicked() 
{
	// TODO: Add your control notification handler code here
	if(mclickaction==0) return;
	if(mclickaction==1) {// pick font;
		CFont font1;
		LOGFONT logfont;
		memset(&logfont,0,sizeof(logfont));
		CStrFunc::FillFont(&logfont,mfont);
		//font1.CreatePointFont(mfontsize,mfontname);
		//font1.GetLogFont(&logfont);
		CFontDialog dlg(&logfont);//;//CF_SCREENFONTS|CF_NOSCRIPTSEL|CF_NOSTYLESEL);
//		dlg.m_cf.lpLogFont=&logfont;
//    	dlg.m_cf.Flags|=CF_NOFACESEL|CF_NOSCRIPTSEL|CF_NOSTYLESEL; 
		dlg.m_cf.rgbColors=mcolFground;
		if(dlg.DoModal()==IDOK){;
			dlg.GetCurrentFont(&logfont);
			CStrFunc::GetFontString(&logfont,mfont,sizeof(mfont)); 
			//font1.CreateFontIndirect(&logfont);
			SetFontStr(mfont);
			mcolFground=dlg.m_cf.rgbColors;
		}
		Invalidate();
	}else if (mclickaction==2) {// pick color
		CColorDialog dlg(mcolBground);
//		dlg.SetCurrentColor(mcolBground); 
		if (dlg.DoModal()==IDOK) {
			mcolBground=dlg.GetColor();
			Invalidate();			
		}; 
	}
}

// 0: do nothing; 1: pcik font; 2: pcik color;
void PowerStatic::SetType(int i)
{
	mclickaction=i;
}

void PowerStatic::SetFontStr(LPCSTR stfnt)
{
	memccpy(mfont,stfnt,0,sizeof(mfont));
	if (m_hfont) {
		DeleteObject(m_hfont);
		m_hfont=NULL;

	}
	LOGFONT fnt;
	memset(&fnt,0,sizeof(fnt));
	CStrFunc::FillFont(&fnt,mfont);
	m_hfont=CreateFontIndirect(&fnt); 
//		SendMessage((HWND) hWnd,
//		  WM_SETFONT,               // message to send
//		  (WPARAM) wParam,          // handle to font
//		  (LPARAM) lParam          // redraw option
//		);

}

BOOL PowerStatic::GetFontStr(char *stfnt, int nsize)
{
	memccpy(stfnt,mfont,0,nsize);
	return TRUE;
}

void PowerStatic::SetColBkground(COLORREF col)
{
	mcolBground=col;
//	mbrushBground.DeleteObject();
//	mbrushBground.CreateSolidBrush(mcolBground); 
}

DWORD PowerStatic::GetColBkground()
{
	return mcolBground;
}

DWORD PowerStatic::GetColForgound()
{
	return mcolFground;
}


void PowerStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rec;
	GetClientRect(rec);
	dc.FillSolidRect(rec,mcolBground);
	CString stitle;
	GetWindowText(stitle);
	dc.SetTextColor(mcolFground);
	HFONT oldfnt=NULL;
	if (m_hfont) {
		oldfnt=(HFONT)dc.SelectObject(m_hfont); 
	}else {
		CWnd *parent;
		parent=GetParent();
		HFONT font1=(HFONT)::SendMessage(parent->m_hWnd,WM_GETFONT,0,0);
		oldfnt=(HFONT)dc.SelectObject(font1); 
	}

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(stitle,rec,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	if (oldfnt){
		dc.SelectObject(oldfnt); 
	}

//	if (!stitle.IsEmpty()) {
//		LOGFONT fnt;
//		CFont cfnt;
//		memset(&fnt,0,sizeof(fnt));
//		CStrFunc::FillFont(&fnt,mfont);
//		cfnt.CreateFontIndirect(&fnt);
//		CFont *poldfnt;
//		poldfnt=(CFont*)dc.SelectObject(&cfnt);
//		dc.SetBkMode(TRANSPARENT); 
//		dc.DrawText(stitle,rec,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
//		dc.SelectObject(poldfnt);
//		cfnt.DeleteObject();
//	}

}
