#include <stdafx.h>
#include "cdlgfuncs.h"
void cdlgchkboxset(CDialog *pdlg,int nid,BOOL bset) {
	CButton *pbut;
	pbut=(CButton*)pdlg->GetDlgItem(nid);
	if (pbut) {
		if (bset) pbut->SetCheck(1);
		else pbut->SetCheck(0);
	}
}

BOOL cdlgchkboxget(CDialog *pdlg,int nid) {
	CButton *pbut;
	pbut=(CButton*)pdlg->GetDlgItem(nid);
	if (pbut) {
		if (pbut->GetCheck()==1) return TRUE;
	}
	return FALSE;
}


void cdlgedboxset(CDialog *pdlg,int nid,LPCSTR pstr) {
	CEdit *ped;
	ped=(CEdit*)pdlg->GetDlgItem(nid);
	if (ped) {
		ped->SetWindowText(pstr); 
	}
}

//assume the pstr at least 100 length;
char *cdlgedboxget(CDialog *pdlg,int nid,char *pstr,int nsize) {
	CEdit *ped;
	ped=(CEdit*)pdlg->GetDlgItem(nid);
	if (ped) {
		ped->GetWindowText(pstr,nsize); 
	}else {
		pstr[0]=0;
	}
	return pstr;
}
