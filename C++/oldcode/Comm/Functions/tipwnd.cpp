// tipwnd.cpp : implementation file
//

#include "stdafx.h"
//#include "..\..\Client\client.h"
#include "tipwnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// TipWnd

TipWnd::TipWnd()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, "InfoWnd", &wndcls)))
	{	// otherwise we need to register a new class
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= LoadCursor( hInst, IDC_ARROW );
		wndcls.hbrBackground	= (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= "InfoWnd";
		if (!AfxRegisterClass(&wndcls))
			AfxThrowResourceException();
	}

}

TipWnd::~TipWnd()
{
}


BEGIN_MESSAGE_MAP(TipWnd, CWnd)
	//{{AFX_MSG_MAP(TipWnd)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// TipWnd message handlers

BOOL TipWnd::Create(CWnd *pParentWnd)
{
	ASSERT_VALID(pParentWnd);
	DWORD dwStyle = WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST ;
	m_pParentWnd = pParentWnd;
	return CreateEx(dwExStyle,"InfoWnd" , NULL, dwStyle, 
                    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		            NULL, NULL, NULL );
}

void TipWnd::Hide()
{
	if (!::IsWindow(GetSafeHwnd()))
        return;

    if (GetCapture()->GetSafeHwnd() == GetSafeHwnd())
        ReleaseCapture();
	ShowWindow( SW_HIDE );
}

void TipWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
//    if (!m_rectHover.PtInRect(point)) 
    {
       // Hide();
        
        // Forward the message
        ClientToScreen( &point );
        CWnd *pWnd = WindowFromPoint( point );
        if ( pWnd == this ) 
            pWnd = m_pParentWnd;
        
        int hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));
        
        if (hittest == HTCLIENT) {
            pWnd->ScreenToClient( &point );
            pWnd->PostMessage( WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y) );
        } else {
            pWnd->PostMessage( WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y) );
        }
    }

//	CWnd::OnMouseMove(nFlags, point);
}

BOOL TipWnd::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd;
	int hittest;
	switch (pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		POINTS pts = MAKEPOINTS( pMsg->lParam );
		POINT  point;
		point.x = pts.x;
		point.y = pts.y;
		ClientToScreen( &point );
		pWnd = WindowFromPoint( point );
		if( pWnd == this ) 
			pWnd = m_pParentWnd;

		hittest = (int)pWnd->SendMessage(WM_NCHITTEST,0,MAKELONG(point.x,point.y));

		if (hittest == HTCLIENT) {
			pWnd->ScreenToClient( &point );
			pMsg->lParam = MAKELONG(point.x,point.y);
		} else {
			switch (pMsg->message) {
			case WM_LBUTTONDOWN: 
				pMsg->message = WM_NCLBUTTONDOWN;
				break;
			case WM_RBUTTONDOWN: 
				pMsg->message = WM_NCRBUTTONDOWN;
				break;
			case WM_MBUTTONDOWN: 
				pMsg->message = WM_NCMBUTTONDOWN;
				break;
			}
			pMsg->wParam = hittest;
			pMsg->lParam = MAKELONG(point.x,point.y);
		}

        Hide();
		pWnd->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
		return TRUE;	
		
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
        Hide();
		m_pParentWnd->PostMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
		return TRUE;
	}

	if( GetFocus() == NULL )
	{
        Hide();
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

// Show 		 - Show the titletip if needed
// rectTitle	 - The rectangle within which the original 
//				    title is constrained - in client coordinates
// lpszTitleText - The text to be displayed
// xoffset		 - Number of pixel that the text is offset from
//				   left border of the cell
void TipWnd::Show(CRect rectTitle, LPCSTR lpszTitleText, int xoffset ,
				  LPRECT lpHoverRect, 
				  LPLOGFONT lpLogFont)
{
	ASSERT( ::IsWindow( GetSafeHwnd() ) );

    if (rectTitle.IsRectEmpty())
        return;

	// If titletip is already displayed, don't do anything.
	if( IsWindowVisible() ) 
		return;

    m_rectHover = (lpHoverRect != NULL)? lpHoverRect : rectTitle;
    m_rectHover.right++; m_rectHover.bottom++;

	m_pParentWnd->ClientToScreen( m_rectHover );
    ScreenToClient( m_rectHover );


	// Do not display the titletip is app does not have focus
	if( GetFocus() == NULL )
		return;

	// Define the rectangle outside which the titletip will be hidden.
	// We add a buffer of one pixel around the rectangle
	m_rectTitle.top    = -1;
	m_rectTitle.left   = -xoffset-1;
	m_rectTitle.right  = rectTitle.Width()-xoffset;
	m_rectTitle.bottom = rectTitle.Height()+1;

	// Determine the width of the text
	m_pParentWnd->ClientToScreen( rectTitle );

	CClientDC dc(this);
	CString strTitle = _T("");
    strTitle += _T(" ");
    strTitle += lpszTitleText; 
    strTitle += _T(" ");

	CFont font, *pOldFont = NULL;
    if (lpLogFont)
    {
        font.CreateFontIndirect(lpLogFont);
	    pOldFont = dc.SelectObject( &font );
    }
    else
    {
        // use same font as ctrl
	    pOldFont = dc.SelectObject( m_pParentWnd->GetFont() );
    }

	CSize size = dc.GetTextExtent( strTitle );

    TEXTMETRIC tm;
    dc.GetTextMetrics(&tm);
    size.cx += tm.tmOverhang;

	CRect rectDisplay = rectTitle;
	rectDisplay.left += xoffset;
	rectDisplay.right = rectDisplay.left + size.cx + xoffset;
    
    // Do not display if the text fits within available space
     // Show the titletip
    SetWindowPos( &wndTop, rectDisplay.left, rectDisplay.top, 
        rectDisplay.Width(), rectDisplay.Height(), 
        SWP_SHOWWINDOW|SWP_NOACTIVATE );
    
    dc.SetBkMode( TRANSPARENT );
    dc.TextOut( 0, 0, strTitle );
    SetCapture();
    
    dc.SelectObject( pOldFont );

}

void TipWnd::Show(LPPOINT ppos, LPCSTR strTitle)
{
	ASSERT( ::IsWindow( GetSafeHwnd() ) );
	
	CClientDC dc(this);
	CRect rectneed;
	dc.DrawText(strTitle,strlen(strTitle),&rectneed,DT_CALCRECT);
	//(rectneed+*ppos);
	//m_rectHover =CRect(ppos->x,ppos->y,ppos->x+rectneed.Width()+4,ppos->y+rectneed.Height()+4);
	int scrwidth,scrhigh;
	scrwidth=GetSystemMetrics(SM_CXSCREEN);
	scrhigh=GetSystemMetrics(SM_CYSCREEN);

	m_rectHover =CRect(ppos->x,ppos->y,ppos->x+rectneed.Width()+4,ppos->y+rectneed.Height()+4);
//	m_rectHover =CRect(ppos->x-rectneed.Width()-4,ppos->y-rectneed.Height()-4 ,ppos->x,ppos->y);
	m_rectHover.right++; m_rectHover.bottom++;

	m_pParentWnd->ClientToScreen( m_rectHover );
	if (m_rectHover.right>scrwidth) {
		m_rectHover+=CPoint(scrwidth-m_rectHover.right,0);
	}
	if (m_rectHover.bottom>scrhigh) {
		m_rectHover+=CPoint(0,scrhigh-m_rectHover.bottom);
	}

    m_rectTitle=m_rectHover; 
    ScreenToClient( m_rectHover );
//	int scrwidth,scrhigh;
//	scrwidth=GetSystemMetrics(SM_CXSCREEN);
//	scrhigh=GetSystemMetrics(SM_CYSCREEN);



	// Do not display the titletip is app does not have focus
	if( GetFocus() == NULL )
		return;


    // Do not display if the text fits within available space
     // Show the titletip
	if (IsWindowVisible()==FALSE ) {
		SetWindowPos( &wndTop, m_rectTitle.left, m_rectTitle.top, 
			m_rectTitle.Width(), m_rectTitle.Height(), 
			SWP_SHOWWINDOW|SWP_NOACTIVATE );
	}else {
		MoveWindow(m_rectTitle.left, m_rectTitle.top, 
			m_rectTitle.Width(), m_rectTitle.Height());
	}
    
	CRect clientrec;
	GetClientRect(clientrec);
	dc.Rectangle(clientrec);
    dc.SetBkMode( TRANSPARENT );
	clientrec.top=2; 
    dc.DrawText(strTitle,strlen(strTitle),&clientrec,DT_LEFT);
//SetCapture();
    
//    dc.SelectObject( pOldFont );

}
