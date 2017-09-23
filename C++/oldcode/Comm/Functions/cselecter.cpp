// columeselecter.cpp : implementation file
//

#include "stdafx.h"
#include "..\..\Client\client.h"
#include "cselecter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCSelecter

CCSelecter::CCSelecter()
{
}

CCSelecter::~CCSelecter()
{
}


BEGIN_MESSAGE_MAP(CCSelecter, CWnd)
	//{{AFX_MSG_MAP(CCSelecter)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(105, OnAdd)
	ON_BN_CLICKED(106, OnRemove)
	ON_BN_CLICKED(107, OnUp)
	ON_BN_CLICKED(108, OnDown)

END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCSelecter message handlers

BOOL CCSelecter::Create(RECT &rc, CWnd *pParent, UINT nid)
{
	if (!CWnd::Create(NULL,NULL,WS_CHILD|WS_VISIBLE, rc, pParent, nid))
		return FALSE;
	CRect rc1(0,0,100,100);
	mlist1.Create(WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_EXTENDEDSEL,rc1,this,100);
	mlist2.Create(WS_BORDER|WS_CHILD|WS_VISIBLE|WS_VSCROLL|LBS_EXTENDEDSEL,rc1,this,101);
	mbutadd.Create(">>",WS_CHILD|WS_VISIBLE,rc1,this,105);
	mbutrem.Create("<<",WS_CHILD|WS_VISIBLE,rc1,this,106);
	mbutup.Create("Up",WS_CHILD|WS_VISIBLE,rc1,this,107);
	mbutdn.Create("Down",WS_CHILD|WS_VISIBLE,rc1,this,108);
	SendMessage(WM_SIZE,0,0);
	return 0;
}

void CCSelecter::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (!IsWindow(mlist1.GetSafeHwnd())) return;
	CRect rc1;
	GetClientRect(rc1);
	CSize s1;
	CDC *pdc;
	pdc=GetDC();
	s1=pdc->GetTextExtent(">>",2);
	ReleaseDC(pdc);

	CRect reclist1,reclist2, recadd,recrem,recup,recdown;
	reclist1=reclist2=recup=recdown=rc1;
	s1.cx += 8;
	s1.cy += 4;
	recadd.left=(rc1.right+rc1.left)/2-s1.cx/2;
	recadd.right=recadd.left+s1.cx;
	recadd.top=rc1.top+(rc1.Height()-s1.cy*2)/3;
	recadd.bottom=recadd.top +s1.cy;
	recrem=recadd;
	recrem+=CPoint(0,(rc1.Height()-s1.cy*2)/3);

	reclist1.right= recadd.left-3;
	reclist2.left = recadd.right+3;
	recup.left=reclist2.left;
	recup.right=recup.left+recup.Width()/2-1;
	recup.top=recup.bottom-s1.cy;
	recdown.left=recdown.right-recup.Width();
	recdown.top=recup.top;
	reclist2.bottom=recup.top-1;   

	mlist1.MoveWindow(reclist1);
	mlist2.MoveWindow(reclist2);
	mbutadd.MoveWindow(recadd);
	mbutrem.MoveWindow(recrem);
	mbutup.MoveWindow(recup);
	mbutdn.MoveWindow(recdown);

/*

	int x0,x1,x2,xe;
	int y0,y1,y2,y3,ye;

	x0=rc1.left,y0=rc1.top; xe=rc1.right, ye=rc1.bottom;
	x1= (rc1.Width()-s1.cx)/2+x0;
	x2=	(rc1.Width()+s1.cx)/2+x0;
	y1= (rc1.Height()- s1.cy *2) /3+y0;
	y2=  (rc1.Height()- s1.cy *2) /3 *2 + s1.cy +y0;
	y3= ye -s1.cy;

	mlist1.MoveWindow(x0,y0,x1-x0,ye); 
	mlist2.MoveWindow(x2,y0,x1-x0,y3-y0);
	mbutadd.MoveWindow(x1,y1,x2-x1,s1.cy);
	mbutrem.MoveWindow(x1,y2,x2-x1,s1.cy);
	mbutup.MoveWindow(x2,y3,x1/2,s1.cy);
	mbutdn.MoveWindow(x2+((x1-x0)/2),y3,(x1/2),s1.cy); 
*/
}




void CCSelecter::OnAdd()
{
   for (int i=0;i < mlist1.GetCount();i++) {
   
	   if (mlist1.GetSel( i ) > 0) {
			char text[100];
			mlist1.GetText(i,text);
			if (mlist2.FindStringExact(-1,text)<0){
				int idx=mlist2.AddString(text);
				mlist2.SetSel(idx);
			}
	   }
   }
/*
   
   int sel=mlist1.GetCurSel(); 
	if (sel<0) return;
	char text[100];
	mlist1.GetText(sel,text);
	if (mlist2.FindStringExact(-1,text)<0){ 
		mlist2.AddString(text);
	}
*/
}

void CCSelecter::OnRemove()
{
   for (int i=0;i < mlist2.GetCount();i++) {
	   if (mlist2.GetSel( i ) > 0) {
		   mlist2.DeleteString(i);
		   i--;
	   }
   }



}

void CCSelecter::OnUp()
{

	if (mlist2.GetCount()<=0) return;
	if (mlist2.GetSel(0)>0) return;

	for (int i=1;i<mlist2.GetCount();i++) {
		if (mlist2.GetSel(i)>0) {
			char text[100];
			mlist2.GetText(i,text);
			mlist2.DeleteString(i);
			mlist2.InsertString(i-1,text);
			mlist2.SetSel(i-1);
		}
	}
}

void CCSelecter::OnDown()
{
	int count=mlist2.GetCount();
	if (count<=2) return;
	if (mlist2.GetSel(count-1)>0) return;

	for (int i=count-2;i>=0;i--) {
		if (mlist2.GetSel(i)>0) {
			char text[100];
			mlist2.GetText(i,text);
			mlist2.DeleteString(i);
			mlist2.InsertString(i+1,text);
			mlist2.SetSel(i+1);
		}
	}
}



//this function set items.
//the colset is the whole list of the columns. if not avaiable, -1 will be used.
//colset will be the same size if items.
//DEL void CCSelecter::SetAviableItems(char **items,int *colset)
//DEL {
//DEL 	int i;
//DEL 	char cset[100];
//DEL 	mlist1.ResetContent();
//DEL 	mlist2.ResetContent();
//DEL 	for (i=0;!(items[i]==NULL);i++) {
//DEL 		mlist1.AddString(items[i]);
//DEL 		cset[colset[i]]=i;
//DEL 	}
//DEL 	
//DEL 	for (i=0;cset[i]>=0;i++) {
//DEL 		mlist2.AddString(items[cset[i]]); 
//DEL 	}
//DEL }
//this function get items.
//the colset is the whole list of the columns. if not abaible, -1 will be used.
//DEL void CCSelecter::GetColumnSet(int *pcols)
//DEL {
//DEL 	int i;
//DEL 	int max=mlist1.GetCount(); 
//DEL 	if (max<=0) return;
//DEL 	char text[100];
//DEL 	for (i=0;i<max;i++){
//DEL 		int sel2;
//DEL 		mlist1.GetText(i,text);
//DEL 		sel2=mlist2.FindStringExact(-1,text);
//DEL 		if (sel2>=0) pcols[i]=sel2;
//DEL 		else pcols[i]=-1;
//DEL 	}
//DEL 	pcols[max]=-1;
//DEL }

//this function set the items.
//the sel is current selected items. end with 0;
//sels values begin with 1
void CCSelecter::SetItems(char **items, int *sel)
{
	int i;
	mlist1.ResetContent();
	mlist2.ResetContent();
	int total=0;
	if (items==NULL) return;
	if (sel==NULL) return;
	for (i=0;!(items[i]==NULL);i++) {
		mlist1.AddString(items[i]);
		total++;
	}
	
	for (i=0;sel[i]>0;i++) {
		if (sel[i]>total) continue;
		if (sel[i]<=0) continue;
		mlist2.AddString(items[sel[i]-1]); 
	}
}

int CCSelecter::GetItems(int *sel)
{
	int i;
	int max=mlist2.GetCount(); 
	if (max<0) return 0;
	if (sel==NULL) return 0;
	char text[100];
	for (i=0;i<max;i++){
		int sel2;
		mlist2.GetText(i,text);
		sel2=mlist1.FindStringExact(-1,text);
		if (sel2>=0) sel[i]=sel2+1;
		else sel[i]=0;
	}
	sel[max]=0;
	return max;

}




BOOL CCSelecter::CreateFromStatic(UINT nID, CWnd *pParent)
{
	CStatic wndStatic;
	if (!wndStatic.SubclassDlgItem(nID, pParent))
		return FALSE;
	CRect rc;
	wndStatic.GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);
	wndStatic.DestroyWindow();
	return Create(rc, pParent, nID);

}
