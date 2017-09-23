#ifndef __CDLGFUNCS__H
#define __CDLGFUNCS__H
void cdlgchkboxset(CDialog *pdlg,int cid,BOOL bset);
BOOL cdlgchkboxget(CDialog *pdlg,int cid);
void cdlgedboxset(CDialog *pdlg,int cid,LPCSTR pstr);
char *cdlgedboxget(CDialog *pdlg,int cid,char *pstr,int nsize);

#endif __CDLGFUNCS__H