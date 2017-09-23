// viewlv1.cpp : implementation file
//

#include "stdafx.h"
#include "viewlv1.h"
//#include "..\Functions\comfunc.h"
#include "..\Functions\strfuncs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ViewLv1

IMPLEMENT_DYNCREATE(ViewLv1, CView)

ViewLv1::ViewLv1()
{
	mpGrid=NULL;
}

ViewLv1::~ViewLv1()
{
	if (mpGrid) {
		delete mpGrid;
	}
}


BEGIN_MESSAGE_MAP(ViewLv1, CView)
	//{{AFX_MSG_MAP(ViewLv1)
	ON_WM_SIZE()
	ON_NOTIFY( GVN_ENDLABELEDIT, 1000, labelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ViewLv1 drawing

void ViewLv1::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// ViewLv1 diagnostics

#ifdef _DEBUG
void ViewLv1::AssertValid() const
{
	CView::AssertValid();
}

void ViewLv1::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// ViewLv1 message handlers

void ViewLv1::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	CRect rc;
	GetClientRect(rc);
	if (mpGrid==NULL) {
		mpGrid= new L1Grid;
	} else {
		AfxMessageBox("Create L1Grid is wrong");
		return ;
	}

	mpGrid->Create(rc,this,1000,WS_CHILD|WS_TABSTOP|WS_VISIBLE);
	mpGrid->init();
/*	mpGrid->SetColumnCount(6);
	mpGrid->SetFixedRowCount(1);
	mpGrid->SetItemText(0,0,"SecSym");
	mpGrid->SetItemText(0,1,"yClose");
	mpGrid->SetItemText(0,2,"AskPrice");
	mpGrid->SetItemText(0,3,"AskSize");
	mpGrid->SetItemText(0,4,"BidPrice");
	mpGrid->SetItemText(0,5,"BidSize");
//	CGridCell * pcell;
//	pcell=mpGrid->GetCell(1,0);
//	pcell->SetState();
	//mpGrid->SetEditable(FALSE);
	//mpGrid->SetC
	mpGrid->SetRowCount(2);
	// TODO: Add your specialized code here and/or call the base class
*/
}

void ViewLv1::OnSize(UINT nType, int cx, int cy) 
{
//	CView::OnSize(nType, cx, cy);
	CRect rect;
	GetClientRect(rect);
	if (mpGrid){
			if (mpGrid->GetSafeHwnd())
			{
				mpGrid->MoveWindow(rect);
			}
	}

	// TODO: Add your message handler code here
	
}


afx_msg void ViewLv1::labelchange( NMHDR * pNotifyStruct, LRESULT * result )
{
	NM_GRIDVIEW * pnstview;
	pnstview=(NM_GRIDVIEW*)pNotifyStruct;
	int row,col;
	int i;
	row=pnstview->iRow;
	col=pnstview->iColumn;
	CString str;
	str=mpGrid->GetItemText(row,col); 
	str.TrimLeft();
	str.TrimRight();
	if (str.IsEmpty()){
		if (row!=(mpGrid->GetRowCount()-1)){
			mpGrid->DeleteRow(row); 
//			SetFocusCell(nRow,0);
		}else {
			for (i=0;i<mpGrid->GetRowCount();i++) {
				mpGrid->SetItemText(row,i,"");
			}
		}
	}else {
		str.MakeUpper();
		mpGrid->SetItemText(row,0,str);
		mlv1watchlist.AddItem(str,NULL);
/*		IssueIfo * pIssue; 
		if(theIssueIfo.Lookup(str,(void*&)pIssue )==FALSE) { // does exist 
			AfxMessageBox("Secsym Doesn't exist"); 
		}else {
			ShowL1Info(str,pIssue->lv1AskPrice,
					pIssue->lv1AskSize,pIssue->lv1Bidprice,
					pIssue->lv1BidSize,' ');
		
		/char tmpstr[100];
			priceD2Str(pIssue->lv1AskPrice,tmpstr);
			mpGrid->SetItemText(row,2,tmpstr); 
			sprintf(tmpstr,"%d",pIssue->lv1AskSize);
			mpGrid->SetItemText(row,3,tmpstr); 
			
			priceD2Str(pIssue->lv1Bidprice,tmpstr);
			mpGrid->SetItemText(row,4,tmpstr); 
			sprintf(tmpstr,"%d",pIssue->lv1BidSize);
			mpGrid->SetItemText(row,5,tmpstr); 
	
		}
*/
		if (row==(mpGrid->GetRowCount()-1)) {
			mpGrid->SetRowCount(row+2); 
		}
		row++;
	}
//	mpGrid->SetFocusCell(row, 0);  	
	mpGrid->Refresh();
}

int ViewLv1::ShowL1Info(LPCSTR secsym, double askprice, int asksize, double bidprice, int bidsize,BYTE Condition)
{
//	mpGrid->mCritical.Lock(); 
	CString st1;
	char tmpstr[100];
	int row;int ret;
	ret=0;
	for (row=1;row<mpGrid->GetRowCount();row++){
		st1=mpGrid->GetItemText(row,0);
		if (st1.Compare(secsym)==0) {
			CStrFunc::priceD2Str(bidprice,tmpstr);
			mpGrid->SetItemText(row,2,tmpstr); 
			sprintf(tmpstr,"%d",bidsize);
			mpGrid->SetItemText(row,3,tmpstr); 
			CStrFunc::priceD2Str(askprice,tmpstr);
			mpGrid->SetItemText(row,4,tmpstr); 
			sprintf(tmpstr,"%d",asksize);
			mpGrid->SetItemText(row,5,tmpstr); 

			sprintf(tmpstr,"%c",Condition);
			mpGrid->SetItemText(row,6,tmpstr); 
			mpGrid->RedrawRow(row); 

			ret++;
		}
	}
//	mpGrid->mCritical.Unlock();
	return ret;
}

int ViewLv1::AddSecSem(LPCSTR secsym)
{
	if (strlen(secsym)==0) return 0;
	int row;
	row=mpGrid->GetRowCount()-1;
	mpGrid->SetItemText(row,0,secsym);
	mlv1watchlist.AddItem(secsym,NULL);
/*	IssueIfo * pIssue; 
	if(theIssueIfo.Lookup(secsym,(void*&)pIssue )==FALSE) { // does exist 
	}else {
		ShowL1Info(secsym,pIssue->lv1AskPrice,
				pIssue->lv1AskSize,pIssue->lv1Bidprice,
				pIssue->lv1BidSize,' ');
	}
//	if (row==(mpGrid->GetRowCount()-1)) {
*/
	mpGrid->SetRowCount(row+2); 
//	}
//	row++;
	return 0;
}
