// spinbutton.cpp : implementation file
//

#include "stdafx.h"
#include "spinbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SpinButton

SpinButton::SpinButton()
{
	mstatus=0;
	mreptstat=0; 
	moldHWND=NULL;
}

SpinButton::~SpinButton()
{
}


BEGIN_MESSAGE_MAP(SpinButton, CWnd)
	//{{AFX_MSG_MAP(SpinButton)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// SpinButton message handlers

void SpinButton::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rec;
	GetClientRect(rec);
	CRect recup,recdown;
	recup=rec;
	recdown=rec;
	recup.bottom =rec.top+(rec.bottom-rec.top)/2;
	//recup.InflateRect(1,1);
	recdown.top= rec.top+(rec.bottom-rec.top)/2;
	//recdown.InflateRect(1,1); 
	int stat;

	stat=0;
	if (mstatus==1) stat|=1;
	if ((mreptstat>0)&&(mrepval==1)) stat|=(1<<1);
	stat |=(1<<2);
	DrawButton(&dc,recup,stat);

	stat=0;
	if (mstatus==2) stat|=1;
	if ((mreptstat>0)&&(mrepval==0)) stat|=(1<<1);
	DrawButton(&dc,recdown,stat);


	//DrawUpArrow(&dc,recup);
	//DrawDnArrow(&dc,recdown);
	// Do not call CWnd::OnPaint() for painting messages
}

BOOL SpinButton::CreateFromStatic(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	DWORD oldstyle;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	oldstyle=wndStatic.GetStyle();
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
//	rc.bottom = rc.top + GetDesiredHeight();
//	return Create(WS_CHILD|WS_VISIBLE, rc, pParent, nID);

	static LPCTSTR lpClassName = _T("LIUSPINBUTTON");
	static BOOL bRegistered = FALSE; // registered?
	if (!bRegistered) {
		WNDCLASS wc;
		memset(&wc, 0, sizeof(wc));
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
		wc.lpfnWndProc = (WNDPROC)::DefWindowProc; // will get hooked by MFC
		wc.hInstance = AfxGetInstanceHandle();
      wc.hCursor = LoadCursor(NULL, IDC_ARROW);
      wc.hbrBackground = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
      wc.lpszMenuName = NULL;
      wc.lpszClassName = lpClassName;
		if (!AfxRegisterClass(&wc)) {
			TRACE("*** SPINBUTTON::AfxRegisterClass failed!\n");
			return FALSE;
		}
		bRegistered = TRUE;
	}

	if (!CWnd::CreateEx(0, lpClassName, NULL, oldstyle, rc, pParent, nID))
		return FALSE;

/*	// initialize fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	lf.lfHeight = GetSystemMetrics(SM_CYHSCROLL)-CYMARGIN;
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = DEFAULT_CHARSET;
	_tcscpy(lf.lfFaceName, _T("Arial"));
	m_fontNormal.CreateFontIndirect(&lf);
	//lf.lfHeight = 1;
	m_fontSelected.CreateFontIndirect(&lf);
*/
	return TRUE;



}

void SpinButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rec;
	GetClientRect(rec);
	CRect recup,recdown;
	recup=rec;
	recdown=rec;
	recup.bottom =rec.top+(rec.bottom-rec.top)/2;
	recdown.top= rec.top+(rec.bottom-rec.top)/2+1;
	CWnd *parent;
	parent=GetParent();
	if (parent==NULL) return;
	//ScreenToClient(&point);
	if (recup.PtInRect(point)) {
		parent->SendMessage(WM_USER+123,1,GetDlgCtrlID());
		mreptstat=1;
		mrepval=1;
		SetTimer(200,1000,NULL);
		SetCapture();
		Invalidate();
		
	}else if (recdown.PtInRect(point)) {
		parent->SendMessage(WM_USER+123,0,GetDlgCtrlID());
		mreptstat=1;
		mrepval=0;
		SetTimer(200,1000,NULL);
		SetCapture();
		Invalidate();
	}
		
	CWnd::OnLButtonDown(nFlags, point);
}

void SpinButton::DrawUpArrow(CDC *pdc, LPRECT uprect)
{
	CPen uppen,*poldpen;;
	CBrush brush,*poldbush;
	if (mstatus==1) {
		brush.CreateSolidBrush(RGB(255,100,100));
	}else {
		brush.CreateSolidBrush(RGB(225,225,225));
//		pdc->Draw3dRect(prec,RGB(100,100,100),RGB(255,255,255));
	}
	poldbush=(CBrush*)pdc->SelectObject(&brush);
	poldpen=(CPen*)pdc->SelectStockObject(NULL_PEN);
	pdc->Rectangle(uprect);
	uppen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	poldpen=pdc->SelectObject(&uppen);
	int ilm,itm;
	ilm=(uprect->right-uprect->left)/5;
	itm=(uprect->bottom-uprect->top)/4; 
	CPoint p1,p2,p3;
	p1.x= uprect->left+ilm; p1.y=uprect->bottom-itm;
	p2.x=((uprect->left+uprect->right)/2);p2.y=uprect->top+itm;
	p3.x=uprect->right-ilm;p3.y=uprect->bottom-itm;

	pdc->MoveTo(p1);
	pdc->LineTo(p2);
	pdc->LineTo(p3);
	pdc->LineTo(p1); 

	pdc->SelectObject(poldbush);
	pdc->SelectObject(poldpen);
}

void SpinButton::DrawDnArrow(CDC *pdc, LPRECT dwnrec)
{
	CPen uppen,*poldpen;;
	CBrush brush;//,*poldbush;
	if (mstatus==2) {
		brush.CreateSolidBrush(RGB(255,100,100));
		pdc->FillRect(dwnrec,&brush);
		pdc->Draw3dRect(dwnrec,RGB(255,255,255),RGB(100,100,100)); 
//		pdc->Draw3dRect(dwnrec,RGB(100,100,100),RGB(255,255,255));
	}else {
		brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
		pdc->FillRect(dwnrec,&brush);
		pdc->Draw3dRect(dwnrec,RGB(255,255,255),RGB(100,100,100)); 
	}
//	poldbush=(CBrush*)pdc->SelectObject(&brush);
	poldpen=(CPen*)pdc->SelectStockObject(NULL_PEN);
//	pdc->Rectangle(dwnrec);
	uppen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	poldpen=pdc->SelectObject(&uppen);
	int ilm,itm;
	ilm=(dwnrec->right-dwnrec->left)/5;
	itm=(dwnrec->bottom-dwnrec->top)/4; 
	CPoint p1,p2,p3;
	p1.x= dwnrec->left+ilm; p1.y=dwnrec->top+itm;
	p2.x=((dwnrec->left+dwnrec->right)/2);p2.y=dwnrec->bottom-itm;
	p3.x=dwnrec->right-ilm;p3.y=dwnrec->top+itm;

	pdc->MoveTo(p1);
	pdc->LineTo(p2);
	pdc->LineTo(p3);
	pdc->LineTo(p1); 

//	pdc->SelectObject(poldbush);
	pdc->SelectObject(poldpen);
}

void SpinButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rec;
	GetClientRect(rec);
	CRect recup,recdown;
	recup=rec;
	recdown=rec;
	recup.bottom =rec.top+(rec.bottom-rec.top)/2;
	recdown.top= rec.top+(rec.bottom-rec.top)/2+1;
	CWnd *parent;
	parent=GetParent();
	if (parent==NULL) return;
	//ScreenToClient(&point);
	if (recup.PtInRect(point)) {
		if (mstatus==0) {
			SetTimer(100,100,NULL); 
			//SetCapture(); 
		}
		if (mstatus!=1) {
			mstatus=1;
			Invalidate();
		}
	}else if (recdown.PtInRect(point)) {
		if (mstatus==0) {
			SetTimer(100,100,NULL); 
			//SetCapture(); 
		}
		if (mstatus!=2) {
			mstatus=2;
			Invalidate();
		}
	}else {
		if (mstatus!=0){
			mstatus=0;
			KillTimer(100);
			Invalidate(); 
			//ReleaseCapture(); 
		}
	}
	if (moldHWND==NULL) {
		moldHWND=::GetFocus();
		::SetFocus(GetSafeHwnd()); 
	}
	CWnd::OnMouseMove(nFlags, point);
}

void SpinButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent==100) {
		CPoint pt; 
		GetCursorPos(&pt);
		CRect rec;
		GetClientRect(rec);
		ScreenToClient(&pt);
		if(rec.PtInRect(pt)==FALSE) {
			KillTimer(100);
			mstatus=0;
			if (moldHWND!=NULL) {
			//	::DestroyWindow(moldHWND); 
			//	::ShowWindow(moldHWND,SW_HIDE);
				::SetFocus(moldHWND);
				moldHWND=NULL;
			};
			Invalidate();
		}else {
			HWND foc;
			foc=::GetFocus();
			if (foc!=GetSafeHwnd()){
				moldHWND=::SetFocus(GetSafeHwnd());
			}
		}

	}else if (nIDEvent=200) {
		KillTimer(200);
		mreptstat=2;
		CWnd *parent;
		parent=GetParent();
		if (parent!=NULL){
			parent->SendMessage(WM_USER+123,mrepval,GetDlgCtrlID());
		}
		SetTimer(201,200,NULL);
	}else if (nIDEvent=201) {
		CWnd *parent;
		parent=GetParent();
		if (parent!=NULL){
			parent->SendMessage(WM_USER+123,mrepval,GetDlgCtrlID());
		}
	}
	CWnd::OnTimer(nIDEvent);
}

void SpinButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (mreptstat==1){ 
		KillTimer(200);
		mreptstat=0;
	}		
	if (mreptstat==2) {
		KillTimer(201);
		mreptstat=0; 
	}
	ReleaseCapture(); 
	Invalidate();
	CWnd::OnLButtonUp(nFlags, point);
}

BOOL SpinButton::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
//	CPoint pt1; 
//	GetCursorPos(&pt1);
	CRect rec;
	GetClientRect(rec);
	ScreenToClient(&pt);
	if (!rec.PtInRect(pt)) {
		return FALSE;
	}

	CWnd *parent;
	parent=GetParent();
	if (zDelta>0) {
		parent->SendMessage(WM_USER+123,1,GetDlgCtrlID());
	}else {
		parent->SendMessage(WM_USER+123,0,GetDlgCtrlID());
	}

 
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void SpinButton::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd *parent;
	parent=GetParent();
	parent->SendMessage(WM_USER+123,2,GetDlgCtrlID());
	CWnd::OnRButtonDown(nFlags, point);
}

//stat: bit 0: mouse in button, bit 1: butclicked; bit 2 up or down;
//
void SpinButton::DrawButton(CDC *pdc, LPRECT prec, int stat)
{



	CPen linepen,*poldpen;;
	CBrush brush;//*poldbush;
	if (stat &1) {
		brush.CreateSolidBrush(RGB(255,150,150));
	}else {
		brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	}
	pdc->FillRect(prec,&brush);
	if (stat&2) {
		pdc->Draw3dRect(prec,RGB(100,100,100),RGB(255,255,255));
	}else {
		pdc->Draw3dRect(prec,RGB(255,255,255),RGB(100,100,100)); 
	}

	poldpen=(CPen*)pdc->SelectStockObject(NULL_PEN);
//	pdc->Rectangle(dwnrec);

	linepen.CreatePen(PS_SOLID,1,RGB(0,0,0));
	poldpen=pdc->SelectObject(&linepen);
	int ilm,itm;
	CPoint p1,p2,p3;
	if (stat&(1<<2)) {// up
		ilm=(prec->right-prec->left)/5;
		itm=(prec->bottom-prec->top)/4; 
		p1.x= prec->left+ilm; p1.y=prec->bottom-itm;
		p2.x=((prec->left+prec->right)/2);p2.y=prec->top+itm;
		p3.x=prec->right-ilm;p3.y=prec->bottom-itm;
	}else { //down
		ilm=(prec->right-prec->left)/5;
		itm=(prec->bottom-prec->top)/4; 
		p1.x= prec->left+ilm; p1.y=prec->top+itm;
		p2.x=((prec->left+prec->right)/2);p2.y=prec->bottom-itm;
		p3.x=prec->right-ilm;p3.y=prec->top+itm;
	}
	pdc->MoveTo(p1);
	pdc->LineTo(p2);
	pdc->LineTo(p3);
	pdc->LineTo(p1); 

//	pdc->SelectObject(poldbush);
	pdc->SelectObject(poldpen);
}
