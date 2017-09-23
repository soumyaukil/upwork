// L1Grid.cpp: implementation of the L1Grid class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
//#include "riskctrl.h"
#include "L1Grid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

L1Grid::L1Grid()
{

}

L1Grid::~L1Grid()
{

}

int L1Grid::init()
{
	SetColumnCount(7);
	SetFixedRowCount(1);
	SetItemText(0,0,"SecSym");
	SetItemText(0,1,"yClose");
	SetItemText(0,2,"BidPrice");
	SetItemText(0,3,"BidSize");
	SetItemText(0,4,"AskPrice");
	SetItemText(0,5,"AskSize");
	SetItemText(0,6,"flag");
	SetRowCount(2);
	return 0;
}

void L1Grid::OnEndEditCell(int nRow, int nCol, CString str)
{
	CGridCtrl::OnEndEditCell(nRow,nCol,str);
	return;
	int i;
	str.TrimLeft();
	str.TrimRight();
	mCritical.Lock();
	if (str.IsEmpty()){
		if (nRow!=(GetRowCount()-1)){
//			CGridCtrl::OnEndEditCell(nRow,nCol,str);
			CGridCtrl::OnEndEditCell(nRow,nCol,str);
			DeleteRow(nRow); 
//			SetFocusCell(nRow,0);
		
//			SetFocus();
		}else {
			for (i=0;i<GetRowCount();i++) {
				SetItemText(nRow,i,"");
			}
			CGridCtrl::OnEndEditCell(nRow,nCol,str);
		}
	}else {
		str.MakeUpper();
		if (nRow==(GetRowCount()-1)) {
			SetRowCount(nRow+2); 
		}
		CGridCtrl::OnEndEditCell(nRow,nCol,str);
		nRow++;

	}
//	SetFocusCell(nRow, 0);  	
	Refresh();
	mCritical.Unlock();
//	AfxMessageBox(str);
}
