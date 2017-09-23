// frmmgmt.cpp : implementation file
//

#include "stdafx.h"
#include "..\orderserv.h"
#include "frmmgmt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

frmSecIfo * frmSecIfo::pfrm=NULL;
frmEquity * frmEquity::pfrm=NULL;
frmTraders * frmTraders::pfrm=NULL;
#include "..\OrderConfig.h"
extern OrderConfig theCfgFile; 

/////////////////////////////////////////////////////////////////////////////
// viewSecIfo

IMPLEMENT_DYNCREATE(viewSecIfo, CFormView)

viewSecIfo::viewSecIfo()
	: CFormView(viewSecIfo::IDD)
{
	//{{AFX_DATA_INIT(viewSecIfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

viewSecIfo::~viewSecIfo()
{
}

void viewSecIfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(viewSecIfo)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(viewSecIfo, CFormView)
	//{{AFX_MSG_MAP(viewSecIfo)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BLoadShortLst, OnBLoadShortLst)
	ON_BN_CLICKED(IDC_BDEL, OnBdel)
	ON_BN_CLICKED(IDC_BClear, OnBClear)
	ON_BN_CLICKED(IDC_BUndel, OnBUndel)
	ON_BN_CLICKED(IDC_BChange, OnBChange)
	ON_BN_CLICKED(IDC_BFind, OnBFind)
	ON_BN_CLICKED(IDC_BComitChange, OnBComitChange)
	ON_BN_CLICKED(IDC_BSort, OnBSort)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_BN_CLICKED(IDC_BLoadUnShortLst, OnBLoadUnShortLst)
	ON_NOTIFY( NM_CLICK, IDC_STGRID, OnGridLMouseClick)
	ON_BN_CLICKED(IDC_BRemoveAll, OnBRemoveAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// viewSecIfo diagnostics

#ifdef _DEBUG
void viewSecIfo::AssertValid() const
{
	CFormView::AssertValid();
}

void viewSecIfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// viewSecIfo message handlers
/////////////////////////////////////////////////////////////////////////////
// frmSecIfo

IMPLEMENT_DYNCREATE(frmSecIfo, CMDIChildWnd)

frmSecIfo::frmSecIfo()
{
}

frmSecIfo::~frmSecIfo()
{
}


BEGIN_MESSAGE_MAP(frmSecIfo, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmSecIfo)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmSecIfo message handlers


BOOL frmSecIfo::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	pContext->m_pNewViewClass=RUNTIME_CLASS(viewSecIfo);
	pview=(viewSecIfo*)CreateView(pContext,AFX_IDW_PANE_FIRST);
	return TRUE;
	//return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void viewSecIfo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
		CStatic *pstatic;
	CRect rc;
	pstatic=(CStatic *) GetDlgItem(IDC_STGRID);
	pstatic->GetWindowRect(&rc);
	ScreenToClient(&rc);
	pstatic->DestroyWindow();
	mGrid.Create(rc,this,IDC_STGRID); 
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnCount(8);
	mGrid.SetFixedColumnCount(1);
	mGrid.SetEditable(FALSE);

	mGrid.SetItemText(0,1,"SecSym");
	mGrid.SetItemText(0,2,"MKT");
	mGrid.SetItemText(0,3,"Shrt");
	mGrid.SetItemText(0,4,"Marg");
	mGrid.SetItemText(0,5,"Inet");
	mGrid.SetItemText(0,6,"Tiresize");
	mGrid.SetItemText(0,7,"ShortSize");
	mGrid.SetHeaderSort();

	InitGrid();
	mGrid.AutoSizeColumns(); 
	CSize actsize;
	actsize=GetTotalSize();
	mPoshelper.SetParent(this,actsize.cx,actsize.cy);
	mPoshelper.AddControl(IDC_STGRID, CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	HWND pwndgrid;
	GetDlgItem(IDC_STGRID,&pwndgrid);

	HWND pwnd;
	pwnd=::FindWindowEx(this->m_hWnd,NULL,NULL,NULL);
	while (pwnd!=NULL) {
		if (pwnd!=pwndgrid){
			CWnd wnd;
			wnd.Attach(pwnd);
			//::GetMenu(pwnd);
			mPoshelper.AddControl(&wnd);
			wnd.Detach();
		}
		pwnd=::FindWindowEx(this->m_hWnd,pwnd,NULL,NULL);
	}
	mPoshelper.SetNegativeMoves(TRUE);
	mPoshelper.MoveControls();

	// TODO: Add your specialized code here and/or call the base class
	
}

void viewSecIfo::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	EnableScrollBarCtrl(SB_BOTH,FALSE);
	mPoshelper.MoveControls();	
}

void viewSecIfo::InitGrid()
{
	IssueSet *pisset; 
	IssueIfo *pifo;
	pisset=theCfgFile.mpIssueData;
	if (pisset==NULL) return;
	stritem *PIISS;
	mGrid.SetRowCount(1);
	pisset->mSecMap.EnumFirst(&PIISS);
	while(PIISS){
		pifo=(IssueIfo *)PIISS->val;
		AddIfo(pifo);
		pisset->mSecMap.EnumNext(&PIISS);
	}

}

void viewSecIfo::AddIfo(IssueIfo *pifo)
{
	int newrow;
	newrow=mGrid.InsertRow(" ");
	mGrid.SetItemData(newrow,0,0);
	ShowIfo(newrow,pifo);
}

void frmSecIfo::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (pfrm==this) pfrm=NULL;

	CMDIChildWnd::OnClose();
}

void viewSecIfo::OnBLoadShortLst() 
{
	// TODO: Add your control notification handler code here
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Load Short List";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return ;
	}; 
	CProgressCtrl *pr;
	pr=(CProgressCtrl*)GetDlgItem(IDC_PROGCOMIT); 
	pr->ShowWindow(SW_SHOW); 
	pr->SetRange(0,1000);
	int i=0;
	CArchive ar(&fp,CArchive::load);
	char st[400];
	CString strsym;
	CString strsrtsz;
	int newline;
//	InitGrid();
	for (newline=1;newline<mGrid.GetRowCount();newline++){
		mGrid.SetItemText(newline,7,"");
		mGrid.SetItemBkColour(newline,7,RGB(255,128,128));
	}

	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		i++;
		if (i>=1000) i=0;
		pr->SetPos(i);
		if (st[0]=='#') continue;
		if (AfxExtractSubString(strsym,st,0,',')==FALSE) continue;
		if (AfxExtractSubString(strsrtsz,st,1,',')==FALSE) continue;
		strsym.TrimLeft();
		strsym.TrimRight(); 
		if ((strsym.GetLength()<1)||(strsym.GetLength()>10)){
			AfxMessageBox("Wrong shortlist File Format!");
			break;
		}

		IssueIfo *pifo;
		pifo=theCfgFile.mpIssueData->GetIssueIfo(strsym);
		newline=0;
		if (pifo!=NULL) {
			newline=FindRowInGrid(strsym);
		};
		if ((pifo==NULL)||(newline==-1)){
			newline=mGrid.InsertRow(" ",-1);
			mGrid.SetItemText(newline,1,strsym);
			mGrid.SetItemText(newline,7,strsrtsz);
			mGrid.SetItemData(newline,7,0);
			mGrid.SetItemData(newline,2,0);
			mGrid.SetItemData(newline,3,1);
			mGrid.SetItemText(newline,3,"Y");
			mGrid.SetItemBkColour(newline,7,RGB(128,128,255));
			SetGridRowStatus(newline,2);
		}else {
			int shortsize;
			shortsize=atoi(strsrtsz);
			mGrid.SetItemText(newline,7,strsrtsz);
			if (shortsize==mGrid.GetItemData(newline,7)){
				mGrid.SetItemBkColour(newline,7,RGB(255,255,255));
			}else {
				mGrid.SetItemBkColour(newline,7,RGB(255,128,128));
				SetGridRowStatus(newline,4);
			}
			int flag;
			flag=mGrid.GetItemData(newline,3);
			flag|=1;
			mGrid.SetItemData(newline,3,flag);
			mGrid.SetItemText(newline,3,"Y");


		}
	}
	ar.Close();
	fp.Close();
	pr->ShowWindow(SW_HIDE);
	mGrid.Refresh();
	return ;
}

void viewSecIfo::OnBUndel() 
{
	CCellID cell=mGrid.GetFocusCell();
	if (cell.row>1) {
		int flag;
		flag=mGrid.GetItemData(cell.row,0);
		flag&=~1;
		SetGridRowStatus(cell.row,flag);
	}
	
}

void viewSecIfo::OnBdel() 
{
	// TODO: Add your control notification handler code here
	CCellID cell=mGrid.GetFocusCell();
	if (cell.row>1) {
		int flag;
		flag=mGrid.GetItemData(cell.row,0);
		flag|=1;
		SetGridRowStatus(cell.row,flag);
	}
/*	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDSECSYM);
	char str[15];
	ped->GetWindowText(str,sizeof(str));
	if (strlen(str)<=0) return;

	theCfgFile.mpIssueData->mSecMap.LockMap();
	stritem *PIstritem;
	PIstritem=theCfgFile.mpIssueData->mSecMap.FindItem(str);
	if (PIstritem!=NULL){
		srstSecIfo rst;
		rst.OpenToModify(str);
		if (!rst.IsEOF()){
			rst.Delete(); 
		}
		rst.Close();
		
		IssueIfo *pifo;
		pifo=(IssueIfo *)PIstritem->val;
		theCfgFile.mpIssueData->mSecMap.Remove(str);
		theCfgFile.mpIssueData->ReleaseBuff(pifo);
	} 
	theCfgFile.mpIssueData->mSecMap.UnlockMap();
	int i;
	CString str1;
	for (i=1;i<mGrid.GetRowCount();i++) {
		str1=mGrid.GetItemText(i,1);
		if (str1.Compare(str)==0) {
			mGrid.DeleteRow(i);
			mGrid.Refresh();
			break;
		}
	};
*/
	return ;
}

void viewSecIfo::OnGridLMouseClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	NM_GRIDVIEW *pifo;
	pifo=(NM_GRIDVIEW *)pNotifyStruct;
	if (pifo->iRow<=0) 
		return ;
	int newrow;
	newrow=pifo->iRow;
	IssueIfo ifo;
	GetRowIfo(newrow,&ifo);
	LoadData(&ifo);
	/*
	if (pifo->iColumn==0) {

		CString str;
		str=mGrid.GetItemText(pifo->iRow,1);
		if (str.IsEmpty()) return 0;
		IssueIfo ifo;
		if (str.GetLength()>=sizeof(ifo.msecsym)) return 0;
		ifo.Clear();
		strcpy(ifo.msecsym,str);
		
		str=mGrid.GetItemText(pifo->iRow,2);
		if (str.IsEmpty()){
			ifo.mIssueMarket=0; 
		}else {
			ifo.mIssueMarket=str.GetAt(0);
		}

		str=mGrid.GetItemText(pifo->iRow,3);
		if (!str.IsEmpty()){
			if (str.GetAt(0)=='Y') ifo.mflag|=1;
		}
		str=mGrid.GetItemText(pifo->iRow,4);
		if (!str.IsEmpty()){
			if (str.GetAt(0)=='Y') ifo.mflag|=2;
		}
		str=mGrid.GetItemText(pifo->iRow,5);
		if (!str.IsEmpty()){
			if (str.GetAt(0)=='Y') ifo.mflag|=4;
		}
		str=mGrid.GetItemText(pifo->iRow,6);
		ifo.mtiresize=atoi(str);
		str=mGrid.GetItemText(pifo->iRow,7);
		ifo.mshortsize=atoi(str);
		LoadData(&ifo);
	}
	*/
}

int viewSecIfo::LoadData(IssueIfo *pifo)
{
	if (pifo==NULL) return 0;
	CEdit *ped;
	ped=(CEdit *) GetDlgItem(IDC_EDSECSYM); 
	ped->SetWindowText(pifo->msecsym);

	char str[10];
	sprintf(str,"%c",pifo->mIssueMarket);
	CComboBox *pcbo;
	pcbo=(CComboBox*)GetDlgItem(IDC_COMBMKT);
	pcbo->SetCurSel(-1);
	pcbo->SelectString(-1,str);

	CButton *pbr;
	pbr=(CButton*)GetDlgItem(IDC_CHKSHORT);
	if (pifo->IsShortAble()){
		pbr->SetCheck(1);
	}else {
		pbr->SetCheck(0); 
	}

	pbr=(CButton*)GetDlgItem(IDC_CHKMARG);
	if (pifo->IsMarginAble()){
		pbr->SetCheck(1);
	}else {
		pbr->SetCheck(0); 
	}

	pbr=(CButton*)GetDlgItem(IDC_CHKHIRISK);
	if (pifo->IsHiRisk()){
		pbr->SetCheck(1);
	}else {
		pbr->SetCheck(0); 
	}

	ped=(CEdit*)GetDlgItem(IDC_EDSHORTSIZE);
	sprintf(str,"%d",pifo->mshortsize);
	ped->SetWindowText(str);

	ped=(CEdit*)GetDlgItem(IDC_EDSOESTIRE);
	sprintf(str,"%d",pifo->mtiresize);
	ped->SetWindowText(str);
	return 0;
}

BOOL viewSecIfo::SaveData(IssueIfo *pifo)
{
	if (pifo==NULL) return FALSE;
	CEdit *ped;
	CString strsym;
	ped=(CEdit *) GetDlgItem(IDC_EDSECSYM); 
	//ped->GetWindowText(pifo->msecsym,sizeof(pifo->msecsym));
	ped->GetWindowText(strsym);
	strsym.TrimLeft();strsym.TrimRight();
	if (strsym.GetLength()<1) return FALSE;
	if (strsym.GetLength()>(sizeof(pifo->msecsym)-1)) return FALSE;
	strcpy(pifo->msecsym,strsym);
	char str[20];
	CComboBox *pcbo;
	pcbo=(CComboBox*)GetDlgItem(IDC_COMBMKT);
	pcbo->GetWindowText(str,sizeof(str)); 
	pifo->mIssueMarket=str[0];

	CButton *pbr;
	pbr=(CButton*)GetDlgItem(IDC_CHKSHORT);
	if (pbr->GetCheck()) {
		pifo->mflag |=1;
	}else {
		pifo->mflag &= ~1;
	}

	pbr=(CButton*)GetDlgItem(IDC_CHKMARG);
	if (pbr->GetCheck()) {
		pifo->mflag|=2;
	}else {
		pifo->mflag&=~2;
	}

	pbr=(CButton*)GetDlgItem(IDC_CHKHIRISK);
	if (pbr->GetCheck()) {
		pifo->mflag|=4;
	}else {
		pifo->mflag&=~4;
	}

	ped=(CEdit*)GetDlgItem(IDC_EDSHORTSIZE);
	ped->GetWindowText(str,sizeof(str));
	pifo->mshortsize=atoi(str);

	ped=(CEdit*)GetDlgItem(IDC_EDSOESTIRE);
	ped->GetWindowText(str,sizeof(str));
	pifo->mtiresize=atoi(str);
	return TRUE;

}


void viewSecIfo::ShowIfo(int row,IssueIfo *pifo)
{
	mGrid.SetItemText(row,1,pifo->msecsym);
	mGrid.SetItemTextFmt(row,2,"%c",pifo->mIssueMarket);
	mGrid.SetItemData(row,2,pifo->mIssueMarket);
	mGrid.SetItemData(row,3,pifo->mflag);
	if (pifo->mflag&1) {//shortable
		mGrid.SetItemText(row,3,"Y");
	}else {
		mGrid.SetItemText(row,3,"N");
	}
	if (pifo->mflag&2) {//marginable
		mGrid.SetItemText(row,4,"Y");
	}else {
		mGrid.SetItemText(row,4,"N");
	}
	if (pifo->mflag&4) {//internet
		mGrid.SetItemText(row,5,"Y");
	}else {
		mGrid.SetItemText(row,5,"N");
	}
	mGrid.SetItemTextFmt(row,6,"%d",pifo->mtiresize);
	mGrid.SetItemTextFmt(row,7,"%d",pifo->mshortsize);
	mGrid.SetItemData(row,7,pifo->mshortsize);

}

void viewSecIfo::OnBClear() 
{
	// TODO: Add your control notification handler code here
	IssueIfo ifo;
	memset(&ifo,0,sizeof(IssueIfo));
	LoadData(&ifo);
}

int viewSecIfo::FindRowInGrid(LPCSTR str)
{
	int totalrow;
	int ret;
	totalrow=mGrid.GetRowCount();
	CString str1;
	for(ret=1;ret <totalrow;ret++) {
		str1=mGrid.GetItemText(ret,1);
		if (str1.Compare(str)==0) {
			return ret;
		}
	};
	return -1;
}


int viewSecIfo::SetGridRowStatus(int row, int flag)
{
	if (mGrid.GetRowCount()<(row+1)) return 0;
	if (row<=0) return 0;
	mGrid.SetItemData(row,0,flag);
	if (flag & 1) {
		mGrid.SetItemText(row,0,"D");
	}else if (flag& 2) {
		mGrid.SetItemText(row,0,"N");
	}else if (flag & 4) {
		mGrid.SetItemText(row,0,"C");
	}else {
		mGrid.SetItemText(row,0," ");
	}
	mGrid.RedrawCell(row,0); 
	return 0;
}

void viewSecIfo::OnBChange() 
{
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDSECSYM);

	IssueIfo ifo;
	if (SaveData(&ifo)==FALSE) {
		AfxMessageBox("Data is wrong");
		return;
	}

	int newrow;
	newrow=FindRowInGrid(ifo.msecsym);
	if (newrow>0){
		ShowIfo(newrow,&ifo);
		mGrid.RedrawRow(newrow); 
	}else {
		newrow=mGrid.InsertRow("");
		ShowIfo(newrow,&ifo);
		mGrid.Refresh(); 
	}

	stritem *PIstritem;
	PIstritem=theCfgFile.mpIssueData->mSecMap.FindItem(ifo.msecsym);
	if (PIstritem!=NULL){
		SetGridRowStatus(newrow,4);
	}else {
		SetGridRowStatus(newrow,2); 
	}
	
}

void viewSecIfo::OnBFind() 
{
	// TODO: Add your control notification handler code here
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDSECSYM);
	char str[20];
	ped->GetWindowText(str,sizeof(str)); 

	int newrow;
	newrow=FindRowInGrid(str);
	if (newrow<=0){
		AfxMessageBox("Can't Find");
	}else {
		mGrid.EnsureVisible(newrow,0);
		mGrid.SetSelectedRange(newrow, 0,
                         newrow, mGrid.GetColumnCount()-1);
		IssueIfo ifo;
		GetRowIfo(newrow,&ifo);
		LoadData(&ifo);
	}
}

BOOL viewSecIfo::GetRowIfo(int row, IssueIfo *pifo)
{
	CString str;
	str=mGrid.GetItemText(row,1);
	if (str.IsEmpty()) return FALSE;
	if (str.GetLength()>=sizeof(pifo->msecsym)) return FALSE;
	pifo->Clear();
	strcpy(pifo->msecsym,str);

	pifo->mIssueMarket=mGrid.GetItemData(row,2) & 0xff;
	pifo->mflag=mGrid.GetItemData(row,3);
	
	str=mGrid.GetItemText(row,6);
	pifo->mtiresize=atoi(str);
	str=mGrid.GetItemText(row,7);
	pifo->mshortsize=atoi(str);
	return TRUE;

}

void viewSecIfo::OnBComitChange() 
{
	// TODO: Add your control notification handler code here
	mGrid.SortTextItems(1,TRUE);
 	srstSecIfo rst;
	CProgressCtrl *pr;
	pr=(CProgressCtrl*)GetDlgItem(IDC_PROGCOMIT); 
	pr->ShowWindow(SW_SHOW); 

	int i;
	try{
		rst.OpenToModify("");
		pr->SetRange32(0,rst.GetRowCount());  
		i=0;
		while(!rst.IsEOF()) {
			rst.Delete();
			pr->SetPos(i);
			i++;
			rst.MoveNext();
		}
		pr->SetRange(0,mGrid.GetRowCount());
		for (i=1;i<mGrid.GetRowCount();i++) {
			int flag;
			pr->SetPos(i);
			flag=mGrid.GetItemData(i,0);
			if ((flag &1)==0) {
				GetRowIfo(i,&rst.mitem);
				try{
					rst.AddNew(); 
				}catch(SRSTErr *perr) {
					
					char str[100];
					sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
					theCfgFile.mlog.AddLogStr(str);
					theCfgFile.mlog.AddLogStr(perr->merrmsg);
			 		delete perr;
				}
			}
		}
 	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
 		delete perr;
 	}
 	rst.Close();
	pr->ShowWindow(SW_HIDE);

	if (theCfgFile.mpIssueData ) {
		theCfgFile.mpIssueData->LoadIssueInfo();
		InitGrid();
	}
}

void viewSecIfo::OnBSort() 
{
	CCellID cid=mGrid.GetFocusCell(); 
	static BOOL bsortflag=TRUE;
	if (bsortflag) bsortflag=FALSE; else bsortflag=TRUE;
	if (cid.col<5){
		mGrid.SortTextItems(cid.col,bsortflag);
	}else {
		mGrid.SortNumItems(cid.col,bsortflag);
	}
	mGrid.Refresh();
}
/////////////////////////////////////////////////////////////////////////////
// frmEquity

IMPLEMENT_DYNCREATE(frmEquity, CMDIChildWnd)

frmEquity::frmEquity()
{
}

frmEquity::~frmEquity()
{
}


BEGIN_MESSAGE_MAP(frmEquity, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmEquity)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmEquity message handlers

void frmEquity::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (pfrm==this) pfrm=NULL;
	CMDIChildWnd::OnClose();
}
/////////////////////////////////////////////////////////////////////////////
// viewEquity

IMPLEMENT_DYNCREATE(viewEquity, CFormView)

viewEquity::viewEquity()
	: CFormView(viewEquity::IDD)
{
	//{{AFX_DATA_INIT(viewEquity)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

viewEquity::~viewEquity()
{
}

void viewEquity::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(viewEquity)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(viewEquity, CFormView)
	//{{AFX_MSG_MAP(viewEquity)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BFind, OnBFind)
	ON_BN_CLICKED(IDC_BSort, OnBSort)
	ON_BN_CLICKED(IDC_BChange, OnBChange)
	ON_BN_CLICKED(IDC_BLoadBuypower, OnBLoadBuypower)
	ON_BN_CLICKED(IDC_BLoadEquity, OnBLoadEquity)
	ON_BN_CLICKED(IDC_BComitChange, OnBComitChange)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_NOTIFY( NM_CLICK, IDC_STGRID, OnGridLMouseClick)
	ON_BN_CLICKED(IDC_BLoadCCSBalance, OnBLoadCCSBalance)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// viewEquity diagnostics

#ifdef _DEBUG
void viewEquity::AssertValid() const
{
	CFormView::AssertValid();
}

void viewEquity::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// viewEquity message handlers

BOOL frmEquity::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	pContext->m_pNewViewClass=RUNTIME_CLASS(viewEquity);
	pview=(viewEquity*)CreateView(pContext,AFX_IDW_PANE_FIRST);
	return TRUE;

//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void viewEquity::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CStatic *pstatic;
	CRect rc;
	pstatic=(CStatic *) GetDlgItem(IDC_STGRID);
	pstatic->GetWindowRect(&rc);
	ScreenToClient(&rc);
	pstatic->DestroyWindow();
	mGrid.Create(rc,this,IDC_STGRID); 
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnCount(6);
	mGrid.SetFixedColumnCount(1);
	mGrid.SetEditable(FALSE);

	mGrid.SetItemText(0,1,"Acct");
	mGrid.SetItemText(0,2,"totalEQ");
	mGrid.SetItemText(0,3,"MarginEQ");
	mGrid.SetItemText(0,4,"DayBP");
	mGrid.SetItemText(0,5,"NightBP");

	InitGrid();
	mGrid.AutoSizeColumns(); 
	CSize actsize;
	actsize=GetTotalSize();
	mPoshelper.SetParent(this,actsize.cx,actsize.cy);
	mPoshelper.AddControl(IDC_STGRID, CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	HWND pwndgrid;
	GetDlgItem(IDC_STGRID,&pwndgrid);

	HWND pwnd;
	pwnd=::FindWindowEx(this->m_hWnd,NULL,NULL,NULL);
	while (pwnd!=NULL) {
		if (pwnd!=pwndgrid){
			CWnd wnd;
			wnd.Attach(pwnd);
			//::GetMenu(pwnd);
			mPoshelper.AddControl(&wnd);
			wnd.Detach();
		}
		pwnd=::FindWindowEx(this->m_hWnd,pwnd,NULL,NULL);
	}
	mPoshelper.SetNegativeMoves(TRUE);
	mPoshelper.MoveControls();
	
}

void viewEquity::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	EnableScrollBarCtrl(SB_BOTH,FALSE);
	mPoshelper.MoveControls();	
}

void viewEquity::InitGrid()
{
	intitem *pintitem;
	Account *pacc;
	Account::sMapAccid.LockMap();
	Account::sMapAccid.EnumFirst(&pintitem);
	mGrid.SetRowCount(1);
	while(pintitem!=NULL) {
		pacc=(Account*)pintitem->val;
		ASSERT(pacc!=NULL);

		if (!pacc->mitemifo.IsDeleted()){
			AddInfo(pacc);
		}
		
		Account::sMapAccid.EnumNext(&pintitem);
	}
	
	Account::sMapAccid.UnlockMap();

}

int viewEquity::AddInfo(Account *pacc)
{
	int newrow;
	newrow=mGrid.InsertRow(" ");
	mGrid.SetItemData(newrow,0,0);
	ShowIfo(newrow,pacc);
	return 0;
}

int viewEquity::ShowIfo(int row, Account *pacc)
{
	mGrid.SetItemText(row,1,pacc->mitemifo.mname);
	mGrid.SetItemTextFmt(row,2,"%.10g",pacc->mitemEquity.mOpenEQ);
	mGrid.SetItemTextFmt(row,3,"%.10g",pacc->mitemEquity.mOpenMGEQ);
	mGrid.SetItemTextFmt(row,4,"%.10g",pacc->mitemEquity.mOpenBP);
	mGrid.SetItemTextFmt(row,5,"%.10g",pacc->mitemEquity.mOpenNBP);
	mGrid.SetItemData(row,0,0);
	return 0;
}

void viewEquity::OnGridLMouseClick(NMHDR *pNotifyStruct, LRESULT *result)
{
	NM_GRIDVIEW *pifo;
	pifo=(NM_GRIDVIEW *)pNotifyStruct;
	if (pifo->iRow<=0) 
		return ;
	LoadRowInfo(pifo->iRow); 
}

void viewEquity::OnBFind() 
{
	// TODO: Add your control notification handler code here
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_ACC);
	char str[20];
	ped->GetWindowText(str,sizeof(str)); 

	int newrow;
	newrow=FindRowInGrid(str);
	if (newrow<=0){
		AfxMessageBox("Can't Find");
	}else {
		mGrid.EnsureVisible(newrow,0);
		mGrid.SetSelectedRange(newrow, 0,
                         newrow, mGrid.GetColumnCount()-1);
		LoadRowInfo(newrow);
	}
}

BOOL viewEquity::LoadRowInfo(int row)
{
	CEdit *ped;
	ped=(CEdit *)GetDlgItem(IDC_ACC);
	ped->SetWindowText(mGrid.GetItemText(row,1));
	ped=(CEdit *)GetDlgItem(IDC_EQ);
	ped->SetWindowText(mGrid.GetItemText(row,2));
	ped=(CEdit *)GetDlgItem(IDC_MEQ);
	ped->SetWindowText(mGrid.GetItemText(row,3));

	ped=(CEdit *)GetDlgItem(IDC_DBP);
	ped->SetWindowText(mGrid.GetItemText(row,4));
	ped=(CEdit *)GetDlgItem(IDC_NBP);
	ped->SetWindowText(mGrid.GetItemText(row,5));
	return TRUE;
}

int viewEquity::FindRowInGrid(LPCSTR str)
{
	int totalrow;
	int ret;
	totalrow=mGrid.GetRowCount();
	CString str1;
	for(ret=1;ret <totalrow;ret++) {
		str1=mGrid.GetItemText(ret,1);
		if (str1.Compare(str)==0) {
			return ret;
		}
	};
	return -1;

}

void viewEquity::OnBSort() 
{
	// TODO: Add your control notification handler code here
	CCellID cid=mGrid.GetFocusCell(); 
	static BOOL bsortflag=TRUE;
	if (bsortflag) bsortflag=FALSE; else bsortflag=TRUE;
	if (cid.col<2){
		mGrid.SortTextItems(cid.col,bsortflag);
	}else {
		mGrid.SortNumItems(cid.col,bsortflag);
	}
	mGrid.Refresh();

}

void viewEquity::OnBChange() 
{
	// TODO: Add your control notification handler code here
	int row;
	char str[20];
	CEdit *ped;
	ped=(CEdit *)GetDlgItem(IDC_ACC);
	ped->GetWindowText(str,sizeof(str));
	row=FindRowInGrid(str);
	if (row==-1) {
		AfxMessageBox("Wrong Account");
		return;
	}
	double bp,nbp,eq,meq;

	ped=(CEdit *)GetDlgItem(IDC_EQ);
	ped->GetWindowText(str,sizeof(str));
	eq=atof(str);
	mGrid.SetItemTextFmt(row,2,"%.10g",eq);

	ped=(CEdit *)GetDlgItem(IDC_MEQ);
	ped->GetWindowText(str,sizeof(str));
	meq=atof(str);
	mGrid.SetItemTextFmt(row,3,"%.10g",meq);

	ped=(CEdit *)GetDlgItem(IDC_DBP);
	ped->GetWindowText(str,sizeof(str));
	bp=atof(str);
	mGrid.SetItemTextFmt(row,4,"%.10g",bp);

	ped=(CEdit *)GetDlgItem(IDC_NBP);
	ped->GetWindowText(str,sizeof(str));
	nbp=atof(str);
	mGrid.SetItemTextFmt(row,5,"%.10g",nbp);

	mGrid.SetItemText(row,0,"C");
	mGrid.SetItemData(row,0,1); 
	mGrid.RedrawRow(row);
	return;

}

void viewEquity::OnBLoadBuypower() 
{
	// TODO: Add your control notification handler code here
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Buying power file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return ; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return ;
	}; 
	CArchive ar(&fp,CArchive::load);
	char st[400];
	char *starry[6];
//	Account *pacc;
	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		if (st[0]=='#') continue;
		int i;
		starry[0]=strtok(st,",");
		for (i=1;i<4;i++){
			starry[i]=strtok(NULL,",");
			if (starry[i]==NULL) {
				starry[0]=0;break;
			}
		};

		if (starry[0]==NULL) {
			AfxMessageBox("Data formate error!");
			break;
			
		}
	
		int row;
		row=FindRowInGrid(starry[0]);
		if (row==-1) continue;
		double bp,nbp;

		if (starry[2]) {
			bp=atof(starry[2]);
		}
		if (starry[3]) {
			nbp=atof(starry[3]);
		}
		mGrid.SetItemTextFmt(row,4,"%.10g",bp);
		mGrid.SetItemTextFmt(row,5,"%.10g",nbp);
		mGrid.SetItemData(row,0,1);
		mGrid.SetItemBkColour(row,4,RGB(255,200,200)); 
		mGrid.SetItemBkColour(row,5,RGB(255,200,200)); 
	}
	ar.Close();
	fp.Close();
	mGrid.Refresh();
	return ;

}

void viewEquity::OnBLoadEquity() 
{
	// TODO: Add your control notification handler code here
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Buying power file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return ; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return ;
	}; 
	CArchive ar(&fp,CArchive::load);
	char str[400];
	CString tmpstr;
	
//	Account *pacc;
	while (1){
		if(ar.ReadString(str,sizeof(str))==NULL) break; 

		AfxExtractSubString(tmpstr,str,0,';');//account
		if (tmpstr.Compare("ACCOUNT")!=0){
			AfxMessageBox("the File is not Pensen equity File! Format Error");
			break;
		}
		
		if (AfxExtractSubString(tmpstr,str,4,';')==TRUE){ //accid
			int row;
			row=FindRowInGrid((LPCSTR)tmpstr);
			if (row==-1) continue;
			double eq,meq;

			
			if (AfxExtractSubString(tmpstr,str,12,';')==FALSE) {
				AfxMessageBox("the File is not Pensen equity File! Format Error");
				break;
			};
			meq=atof(tmpstr);
			if (AfxExtractSubString(tmpstr,str,16,';')==FALSE) {
				AfxMessageBox("the File is not Pensen equity File! Format Error");
				break;
			};
			eq=atof(tmpstr);

			mGrid.SetItemTextFmt(row,2,"%.10g",eq);
			mGrid.SetItemTextFmt(row,3,"%.10g",meq);
			mGrid.SetItemData(row,0,1);
			mGrid.SetItemBkColour(row,2,RGB(255,200,200)); 
			mGrid.SetItemBkColour(row,3,RGB(255,200,200)); 
			
		}else {
			AfxMessageBox("the File is not Pensen equity File! Format Error");
			break;
		}
	}
	ar.Close();
	fp.Close();
	mGrid.Refresh();
	return ;
	
}

void viewEquity::OnBComitChange() 
{
	// TODO: Add your control notification handler code here
	int i;
	int totalitem;
	CString str;
	Account *pacc;
	int changed;
	changed=0;
	totalitem=mGrid.GetRowCount(); 
	for (i=1;i<totalitem;i++){
		str=mGrid.GetItemText(i,1);
		pacc=Account::FindAccByName(str);
		if (pacc==NULL) continue;
		itemAccEquity eq;
		eq.CopyFrom(&pacc->mitemEquity);
		
		str=mGrid.GetItemText(i,2);
		eq.SetOpenTotalEQ(atof(str));
		str=mGrid.GetItemText(i,3);
		eq.SetOpenMarginEQ(atof(str));
		str=mGrid.GetItemText(i,4);
		eq.SetOpenDayBP(atof(str));
		str=mGrid.GetItemText(i,5);
		eq.SetOpenNightBP(atof(str));

		eq.mCurrHoldBP=pacc->ReCalOrderHoldBP();
		pacc->TktRecaluate(FALSE,FALSE); 
	
//		eq.mCurrBP=eq.mOpenBP-eq.mCurrHoldBP;  
		time(&(eq.mm_time));
		changed++;

		pacc->mitemEquity.CopyFrom(&eq);
		char pkg[200];
		pacc->MakePkgByEquity(pkg,0xffff);
		Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
	}
	if (changed>0) {
		Account::SaveAllEquityInfo(FALSE);
		InitGrid();
	}
}

void viewEquity::OnFilePrint() 
{
	// TODO: Add your command handler code here
	
	mGrid.Print();
}

void viewSecIfo::OnFilePrint() 
{
	// TODO: Add your command handler code here
	mGrid.Print();
}
/////////////////////////////////////////////////////////////////////////////
// frmTraders

IMPLEMENT_DYNCREATE(frmTraders, CMDIChildWnd)

frmTraders::frmTraders()
{
	pview=NULL;
}

frmTraders::~frmTraders()
{
}


BEGIN_MESSAGE_MAP(frmTraders, CMDIChildWnd)
	//{{AFX_MSG_MAP(frmTraders)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// frmTraders message handlers

void frmTraders::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(pfrm==this) pfrm=NULL;
	CMDIChildWnd::OnClose();
}

BOOL frmTraders::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	pContext->m_pNewViewClass=RUNTIME_CLASS(viewTraders);
	pview=(viewTraders*)CreateView(pContext,AFX_IDW_PANE_FIRST);
	return TRUE;

//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

/////////////////////////////////////////////////////////////////////////////
// viewTraders

IMPLEMENT_DYNCREATE(viewTraders, CFormView)

viewTraders::viewTraders()
	: CFormView(viewTraders::IDD)
{
	//{{AFX_DATA_INIT(viewTraders)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

viewTraders::~viewTraders()
{
}

void viewTraders::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(viewTraders)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(viewTraders, CFormView)
	//{{AFX_MSG_MAP(viewTraders)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BRefresh, OnBRefresh)
	ON_BN_CLICKED(IDC_BKickOut, OnBKickOut)
	ON_BN_CLICKED(IDC_BSortAsc, OnBSortAsc)
	ON_BN_CLICKED(IDC_BSortDec, OnBSortDec)
	ON_BN_CLICKED(IDC_BCount, OnBCount)
	ON_BN_CLICKED(IDC_BBlock, OnBBlock)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// viewTraders diagnostics

#ifdef _DEBUG
void viewTraders::AssertValid() const
{
	CFormView::AssertValid();
}

void viewTraders::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// viewTraders message handlers

void viewTraders::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CStatic *pstatic;
	CRect rc;
	pstatic=(CStatic *) GetDlgItem(IDC_STGRID);
	pstatic->GetWindowRect(&rc);
	ScreenToClient(&rc);
	pstatic->DestroyWindow();
	mGrid.Create(rc,this,IDC_STGRID); 
	mGrid.SetRowCount(1);
	mGrid.SetFixedRowCount(1);
	mGrid.SetColumnCount(9);
	mGrid.SetFixedColumnCount(1);
	mGrid.SetEditable(FALSE);

	mGrid.SetItemText(0,1,"Branch");
	mGrid.SetItemText(0,2,"Trader");
	mGrid.SetItemText(0,3,"Type");
	mGrid.SetItemText(0,4,"IP.From");
	mGrid.SetItemText(0,5,"Login");
	mGrid.SetItemText(0,6,"Ver#");
	mGrid.SetItemText(0,7,"PendedW");
	mGrid.SetItemText(0,8,"SysID");
	mGrid.SetHeaderSort();

	InitGrid();
	mGrid.AutoSizeColumns(); 
	CSize actsize;
	actsize=GetTotalSize();
	mPoshelper.SetParent(this,actsize.cx,actsize.cy);
	mPoshelper.AddControl(IDC_STGRID, CP_RESIZE_HORIZONTAL|CP_RESIZE_VERTICAL);
	HWND pwndgrid;
	GetDlgItem(IDC_STGRID,&pwndgrid);

	HWND pwnd;
	pwnd=::FindWindowEx(this->m_hWnd,NULL,NULL,NULL);
	while (pwnd!=NULL) {
		if (pwnd!=pwndgrid){
			CWnd wnd;
			wnd.Attach(pwnd);
			//::GetMenu(pwnd);
			mPoshelper.AddControl(&wnd);
			wnd.Detach();
		}
		pwnd=::FindWindowEx(this->m_hWnd,pwnd,NULL,NULL);
	}
	mPoshelper.SetNegativeMoves(TRUE);
	mPoshelper.MoveControls();

	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDLOGOUTMSG);
	ped->SetWindowText("Been logout by Administrator");
	ped->SetLimitText(255);
}

void viewTraders::InitGrid()
{
	intitem *pitem;
//	char str[1000];
	mGrid.SetRowCount(1);
	Trader::sMapTrid.LockMap();
	Trader::mOnlineAdmin.LockMap();
	pitem=Trader::mOnlineAdmin.EnumFirst();
	while(pitem!=NULL) {
		CClient *pclient;
		pclient=(CClient*)pitem->val;
		if (pclient->mptrifo)
			AddIfo(pclient);
		pitem=Trader::mOnlineAdmin.EnumNext(pitem);
	}
	Trader::mOnlineAdmin.UnlockMap();

	Trader::mOnlineTrader.LockMap();
	pitem=Trader::mOnlineTrader.EnumFirst();
	while(pitem!=NULL) {
		CClient *pclient;
		pclient=(CClient*)pitem->val;
		if (pclient->mptrifo)
			AddIfo(pclient);
		pitem=Trader::mOnlineTrader.EnumNext(pitem);
	}
	Trader::mOnlineTrader.UnlockMap();
	Trader::sMapTrid.UnlockMap();
	mGrid.Refresh();

}

int viewTraders::AddIfo(CClient *pclient)
{
	int newrow;
	newrow=mGrid.InsertRow(" ");
	mGrid.SetItemData(newrow,0,0);
	int brid;
	brid=pclient->mptrifo->minfo.mbrid;
	mGrid.SetItemTextFmt(newrow,1,"%d",brid);
	mGrid.SetItemText(newrow,2,pclient->mptrifo->minfo.mtracno);
	mGrid.SetItemData(newrow,2,pclient->mptrifo->minfo.mtrid);
	if (pclient->mptrifo->IsTrader()){
		mGrid.SetItemText(newrow,3,"Trader");
	}else	if (pclient->mptrifo->IsManager()) {
		mGrid.SetItemText(newrow,3,"Manager");
	}else	if (pclient->mptrifo->IsAdmin()) {
		mGrid.SetItemText(newrow,3,"Admin");
	}
	
//	char addstr[20];
//	DWORD ip;
//	ip=pclient->mClientAddress;
//	sprintf(addstr,"%d.%d.%d.%d",ip&0xff,(ip>>8)&0xff,(ip>>16)&0xff,(ip>>24)&0xff);

	mGrid.SetItemText(newrow,4,pclient->GetClientIPString()); 
	
		
	tm *ptm;
	tm tm1;
	ptm=localtime(&(pclient->mlogintime));
	if (ptm!=NULL) {
		memcpy(&tm1,ptm,sizeof(tm));
		char str[20];
		sprintf(str,"%d:%d:%d",tm1.tm_hour,tm1.tm_min,tm1.tm_sec);
		mGrid.SetItemText(newrow,5,str);
	}else {
		mGrid.SetItemText(newrow,5,"N/A");
	}
	mGrid.SetItemTextFmt(newrow,6,"%d.%d",HIBYTE(pclient->m_Version),LOBYTE(pclient->m_Version));

	int pw;
	pw=pclient->GetWriteBuffSize();
	mGrid.SetItemTextFmt(newrow,7,"%d",pw);
	mGrid.SetItemTextFmt(newrow,8,"%u",pclient->mClientSysID);


	return newrow;
}

void viewTraders::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	EnableScrollBarCtrl(SB_BOTH,FALSE);
	mPoshelper.MoveControls();	
	
	// TODO: Add your message handler code here
	
}

void viewTraders::OnBRefresh() 
{
	// TODO: Add your control notification handler code here
	InitGrid();
}

void viewTraders::OnBKickOut() 
{
	// TODO: Add your control notification handler code here
	CCellID cid=mGrid.GetFocusCell();
	char str[500];
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDLOGOUTMSG); 
	ped->GetWindowText(str,sizeof(str));
	if (cid.row<1) {
		return ;
	}
	int tid;
	tid=mGrid.GetItemData(cid.row,2);
	Trader *ptr=Trader::FindTrader(tid);
	if (ptr!=NULL) {
		ptr->LogItOut(str);
	}
	InitGrid();
}

void viewSecIfo::OnBLoadUnShortLst() 
{
	// TODO: Add your control notification handler code here
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Load unShortable List";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return ;
	}; 

	CProgressCtrl *pr;
	pr=(CProgressCtrl*)GetDlgItem(IDC_PROGCOMIT); 
	pr->ShowWindow(SW_SHOW); 
	pr->SetRange(0,1000);
	int i=0;
	CArchive ar(&fp,CArchive::load);
	char st[400];
	CString strsym;
	CString strsrtsz;
	int newline;
	long flag;
//	InitGrid();
	for (newline=1;newline<mGrid.GetRowCount();newline++){
		mGrid.SetItemText(newline,7,"1000000");
		flag=mGrid.GetItemData(newline,3);
		flag|=1;
		mGrid.SetItemData(newline,3,flag); 
		mGrid.SetItemText(newline,3,"Y"); 
		mGrid.SetItemBkColour(newline,7,RGB(255,128,128));
	}

	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		i++;
		if (i>=1000) i=0;
		pr->SetPos(i);
		if (st[0]=='#') continue;
		if (AfxExtractSubString(strsym,st,0,',')==FALSE) continue;
//		if (AfxExtractSubString(strsrtsz,st,1,',')==FALSE) continue;
		strsym.TrimLeft();
		strsym.TrimRight(); 
		if ((strsym.GetLength()<1)||(strsym.GetLength()>10)){
			AfxMessageBox("Wrong shortlist File Format!");
			break;
		}

		IssueIfo *pifo;
		pifo=theCfgFile.mpIssueData->GetIssueIfo(strsym);
		newline=0;
		if (pifo!=NULL) {
			newline=FindRowInGrid(strsym);
		};
		if ((pifo==NULL)||(newline==-1)){
			newline=mGrid.InsertRow(" ",-1);
			mGrid.SetItemText(newline,1,strsym);
			mGrid.SetItemText(newline,7,"0");
			mGrid.SetItemData(newline,7,0);
			mGrid.SetItemData(newline,2,0);
			mGrid.SetItemData(newline,3,0);
			mGrid.SetItemText(newline,3,"N");
			mGrid.SetItemBkColour(newline,7,RGB(128,128,255));
			SetGridRowStatus(newline,2);
		}else {
			int shortsize;
			shortsize=-1;
			mGrid.SetItemText(newline,7,"0");
			if (shortsize==mGrid.GetItemData(newline,7)){
				mGrid.SetItemBkColour(newline,7,RGB(255,255,255));
			}else {
				mGrid.SetItemBkColour(newline,7,RGB(255,128,128));
				SetGridRowStatus(newline,4);
			}
			int flag;
			flag=mGrid.GetItemData(newline,3);
			flag&=(~1);
			mGrid.SetItemData(newline,3,flag);
			mGrid.SetItemText(newline,3,"N");
		}
	}
	ar.Close();
	fp.Close();
	pr->ShowWindow(SW_HIDE);
	mGrid.Refresh();
	return;
}

void viewSecIfo::OnBRemoveAll() 
{
	// TODO: Add your control notification handler code here
	mGrid.SetRowCount(1);	
}

void viewTraders::OnBSortAsc() 
{
	// TODO: Add your control notification handler code here
	int colno;
	CCellID cid;
	cid=mGrid.GetFocusCell();
	colno=cid.col;
	if (colno<0) return;
	mGrid.SortTextItems(colno,TRUE);	
	mGrid.Refresh(); 
}

void viewTraders::OnBSortDec() 
{
	// TODO: Add your control notification handler code here
	int colno;
	CCellID cid;
	cid=mGrid.GetFocusCell();
	colno=cid.col;
	if (colno<0) return;
	mGrid.SortTextItems(colno,FALSE);	
	mGrid.Refresh(); 
}

void viewTraders::OnBCount() 
{
	// TODO: Add your control notification handler code here
	int admnu;
	int tradnu;
	char str[100];
	admnu=Trader::mOnlineAdmin.GetCounter();
	tradnu=Trader::mOnlineTrader.GetCounter(); 
	sprintf(str,"Adm:%d,Trader:%d\xd\xaTotal:%d",admnu,tradnu,admnu+tradnu);
	AfxMessageBox(str);
}

void viewEquity::OnBLoadCCSBalance() 
{
	CFile fp;
	CFileDialog fdlg(TRUE);
	fdlg.m_ofn.lpstrTitle="Open Buying power file";  
	fdlg.m_ofn.lpstrFilter="*.txt\0*.txt\0*.*\0*.*\0\0";
	if (fdlg.DoModal()==IDCANCEL) return ; 
	if (fp.Open(fdlg.GetFileName(),CFile::modeRead)==FALSE) {
		return ;
	}; 
	CArchive ar(&fp,CArchive::load);
	char st[1000];
	char *starry[30];
//	Account *pacc;
	while (1){
		if(ar.ReadString(st,sizeof(st))==NULL) break; 
		if (st[0]=='#') continue;
		int i;
		i=0;
		starry[0]=strtok(st,",");
		for (i=1;i<28;i++){
			starry[i]=strtok(NULL,",");
			if (starry[i]==NULL) {
				break;
			}
		};
		if (i<26) {
			AfxMessageBox("Data formate error!");
			break;
			
		}
	
		int row;
		row=FindRowInGrid(starry[0]);
		if (row==-1) continue;
		double bp,nbp,eq,meq;

		if (starry[18]) {
			bp=atof(starry[18]);
		}
		if (starry[19]) {
			nbp=atof(starry[19]);
		}

		if (starry[25]) {
			eq=atof(starry[25]);
		}
		if (starry[14]) {
			meq=atof(starry[14]);
		}

		Account *pacc;
		CString str;
		str=mGrid.GetItemText(row,1); 
		pacc=Account::FindAccByName(str);
		if (pacc==NULL) continue;
		if (pacc->mitemifo.IsMargin()) { 
			mGrid.SetItemTextFmt(row,4,"%.10g",bp);
			mGrid.SetItemTextFmt(row,5,"%.10g",nbp);
		}else {// cash account:
			mGrid.SetItemTextFmt(row,4,"%.10g",bp*theCfgFile.mDayMargBPRate);
			mGrid.SetItemTextFmt(row,5,"%.10g",nbp*theCfgFile.mDayMargBPRate);

		}

		mGrid.SetItemTextFmt(row,2,"%.10g",eq);
		mGrid.SetItemTextFmt(row,3,"%.10g",meq);
		mGrid.SetItemData(row,0,1);
		mGrid.SetItemBkColour(row,2,RGB(255,200,200)); 
		mGrid.SetItemBkColour(row,3,RGB(255,200,200)); 
		mGrid.SetItemData(row,0,1);
		mGrid.SetItemBkColour(row,4,RGB(255,200,200)); 
		mGrid.SetItemBkColour(row,5,RGB(255,200,200)); 
	}
	ar.Close();
	fp.Close();
	mGrid.Refresh();
	return ;
	
}

void viewTraders::OnBBlock() 
{
	CCellID cid=mGrid.GetFocusCell();
	char str[500];
	CEdit *ped;
	ped=(CEdit*)GetDlgItem(IDC_EDLOGOUTMSG); 
	ped->GetWindowText(str,sizeof(str));
	if (cid.row<1) {
		return ;
	}
	int tid;
	tid=mGrid.GetItemData(cid.row,2);
	Trader *ptr=Trader::FindTrader(tid);
	if (ptr!=NULL) {
		if (ptr->mpClient){
			theCfgFile.mLogonSysidBlock.AddNoDup(ptr->mpClient->mClientSysID,0); 
		}
	}
}
