#if !defined(AFX_COLORPICKSTATIC_H__F1307625_E89A_11D4_BD29_0050041B8FEC__INCLUDED_)
#define AFX_COLORPICKSTATIC_H__F1307625_E89A_11D4_BD29_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorPickStatic.h : header file
//
#include "strfuncs.h"
/////////////////////////////////////////////////////////////////////////////
// ColorPickStatic window

class ColorPickStatic : public CStatic
{
// Construction
public:
	ColorPickStatic();
	
// Attributes
public:
	COLORREF mcolor;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ColorPickStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF ncolor);
	inline COLORREF GetColor(){return mcolor;};
	virtual ~ColorPickStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(ColorPickStatic)
	afx_msg void OnPaint();
	afx_msg void OnClicked();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// PowerStatic window

class PowerStatic : public CStatic
{
// Construction
	int mclickaction;//0 nothing
					 //1 pickfont
	char mfont[50];
	HFONT m_hfont;
//	CFont mcfont;
private:
	COLORREF mcolFground;
	COLORREF mcolBground;
//	CPen mpenFground;
//	CBrush mbrushBground;
public:
	PowerStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PowerStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	DWORD GetColForgound();
	DWORD GetColBkground();
	BOOL GetFontStr(char *stfnt, int nsize);
	void SetFontStr(LPCSTR stfnt);
	// 0: do nothing; 1: pcik font; 2: pcik color;
	void SetType(int i);
	void SetColBkground(COLORREF col);
	inline void SetColForgound(COLORREF col){ mcolFground=col;};
	virtual ~PowerStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(PowerStatic)
	afx_msg void OnClicked();
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORPICKSTATIC_H__F1307625_E89A_11D4_BD29_0050041B8FEC__INCLUDED_)
