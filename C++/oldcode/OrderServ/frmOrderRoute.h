#if !defined(AFX_FRMORDERROUTE_H__9926A1A4_7E3A_4C5F_8ECA_419632A01D98__INCLUDED_)
#define AFX_FRMORDERROUTE_H__9926A1A4_7E3A_4C5F_8ECA_419632A01D98__INCLUDED_

#include "GRIDCTRL\GridCtrl.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// frmOrderRoute.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// frmOrderRoute frame

class frmOrderRoute : public CMDIChildWnd
{
	DECLARE_DYNCREATE(frmOrderRoute)
protected:
	frmOrderRoute();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	LRESULT OnRouteMSG(WPARAM wparam, LPARAM lparam);
	static frmOrderRoute * mfrm;
	afx_msg void OnRMouseDown( NMHDR * pNotifyStruct, LRESULT * result );
	BOOL showroute(int row, OrderRouter *por);
	void InitGrid();
	CGridCtrl mGrid;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(frmOrderRoute)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~frmOrderRoute();

	// Generated message map functions
	//{{AFX_MSG(frmOrderRoute)
	afx_msg void OnRouteConfig();
	afx_msg void OnRouteConnectall();
	afx_msg void OnRouteDisconnect();
	afx_msg void OnRouteDisconnectall();
	afx_msg void OnRouteConnect();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRMORDERROUTE_H__9926A1A4_7E3A_4C5F_8ECA_419632A01D98__INCLUDED_)
