// L1Grid.h: interface for the L1Grid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_L1GRID_H__165E5F13_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
#define AFX_L1GRID_H__165E5F13_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\GRIDCTRL\GridCtrl.h"

class L1Grid : public CGridCtrl  
{
public:
	void OnEndEditCell(int nRow, int nCol, CString str);
	int init();
	L1Grid();
	virtual ~L1Grid();

};

#endif // !defined(AFX_L1GRID_H__165E5F13_37C9_11D4_BCBF_00105A82D2D1__INCLUDED_)
