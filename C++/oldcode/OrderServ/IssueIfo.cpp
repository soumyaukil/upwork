// IssueIfo.cpp: implementation of the IssueIfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "orderserv.h"
#include "IssueIfo.h"
#include "OrderDB.h"
#include "Functions\strfuncs.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern OrderDB theOrderDB;
extern OrderConfig theCfgFile;


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

srstSecIfo::srstSecIfo()
{

}

srstSecIfo::~srstSecIfo()
{

}


IssueSet::IssueSet()
{
	mbDefShortable=((theCfgFile.mbDefShortAble&1)!=0);
	mbDefShortable1=((theCfgFile.mbDefShortAble&2)!=0);

	mIssueBuff.init(sizeof(IssueIfo),2048);
	mSecMap.init(2000);
	int i;
	for (i=0;i<MAXSYMBLIST;i++) {
		mSymbLst[i].init(500);
	}
	mFutureInfo.init(200);// the futures symbol is not too much; 
	LoadIssueInfo();
	LoadUnTradableLst();
	char filename[300];
	theCfgFile.GetPath("FuturesInfo.txt",filename);
	LoadFuturesInfo(filename);
}	

IssueSet::~IssueSet()
{
	ClearIssueMap();
	mSecMap.RemoveAll(); 
	for (int i=0;i<MAXSYMBLIST;i++) {
		mSymbLst[i].RemoveAll(); 
	}
	
}



BOOL IssueSet::LoadIssueInfo()
{
//	mlock.Lock();
	mSecMap.LockMap();
	ClearIssueMap();

	srstSecIfo rst;
	rst.Open();
	IssueIfo *pifo;
	while(rst.IsEOF()==FALSE) {
		pifo=GetNewBuff();
		ASSERT(pifo!=NULL);
		memcpy(pifo,&rst.mitem,sizeof(IssueIfo));
		mSecMap.Add(pifo->msecsym,(long)pifo);
		rst.MoveNext();  
	}
	mSecMap.UnlockMap();
	return TRUE;
}

LPCSTR srstSecIfo::GetColLst()
{
	return "select secsym,flag,issuemkt,tiresize,shortsize"
		" from mSecIfo";

}

BOOL srstSecIfo::Open()
{

	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	try{
		ExecuteSql(GetColLst());
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}

BOOL srstSecIfo::OnBind(int flag)
{

	UnBindAll(); 
	BindText(flag,1,mitem.msecsym,sizeof(mitem.msecsym));
	BindLong(flag,2,&mitem.mflag);
	BindBYTE(flag,3,&mitem.mIssueMarket);
	BindLong(flag,4,&mitem.mtiresize);
	BindLong(flag,5,&mitem.mshortsize);
	return TRUE;

}

void IssueSet::ClearIssueMap()
{
	mSecMap.LockMap();
	stritem *PIitem;
	IssueIfo *pissue;
	PIitem=mSecMap.EnumFirst();
	while(PIitem) {
		pissue=(IssueIfo *)PIitem->val;
		ReleaseBuff(pissue);
		PIitem=mSecMap.EnumNext(PIitem);
	}
	mSecMap.UnlockMap(); 
	mSecMap.RemoveAll();
}

IssueIfo * IssueSet::GetIssueIfo(LPCSTR secsym)
{
	stritem *pitem;
	pitem=mSecMap.FindItem(secsym);
	if (pitem==NULL) return NULL;
	return (IssueIfo *)pitem->val; 
}


int srstSecIfo::OpenToModify(char *pstr)
{
	if (m_pDatabase==NULL)
		if (init(&theOrderDB.mdb)==FALSE) return FALSE;
	SetConCurrency(SQL_CONCUR_VALUES);
	SetCursorType (SQL_CURSOR_KEYSET_DRIVEN);
	char str[200];
	if (strlen(pstr)==0) {
		sprintf(str,"%s",GetColLst());
	}else {
		sprintf(str,"%s where secsym='%s'",GetColLst(),pstr);
	}
	try{
		ExecuteSql(str);
	}catch (SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
		delete perr;
		return FALSE;
	}
	MoveNext();
	return TRUE;

}


void IssueIfo::Clear()
{
	memset(this,0,sizeof(IssueIfo));
	mflag|=(1<<1);//default is margain able.

}

double IssueSet::GetYestDayClose(LPCSTR secsym)
{
	IssueIfo *pifo;
	stritem *pitem;
	pitem=mSecMap.FindItem(secsym);
	if (pitem==NULL) return 0;
	pifo=(IssueIfo *)pitem->val;
	if (pifo==0) return 0;
	return pifo->mYClose; 
//	return 

}

BOOL IssueSet::AddUnTradable(int listid,LPCSTR secsym)
{
	if (mSymbLst[listid].FindItem(secsym)!=NULL) return TRUE;
	if (strlen(secsym)>15) return FALSE;
	char str[1000];
	sprintf(str,"Insert into mSymbLst (ListId,SecSymb) values (%d,'%s')",listid,secsym);
	theOrderDB.mdb.ExecuteSQL(str);
	mSymbLst[listid].AddNoDup(secsym,0);
	return TRUE;
}

BOOL IssueSet::RMUnTradable(int listid, char *secsym)
{
	if (mSymbLst[listid].FindItem(secsym)==NULL) return TRUE;
	if (strlen(secsym)>15) return FALSE;
	char str[1000];
	sprintf(str,"delete mSymbLst where ListId=%d and SecSymb='%s'",listid,secsym);
	theOrderDB.mdb.ExecuteSQL(str);
	mSymbLst[listid].Remove(secsym);
	return TRUE;
}

BOOL IssueSet::RMAllUnTradable(int listid)
{
	char str[1000];
	sprintf(str,"delete mSymbLst where ListId=%d",listid);
	theOrderDB.mdb.ExecuteSQL(str);
	mSymbLst[listid].RemoveAll();
	return 0;
}

BOOL IssueSet::LoadUnTradableLst()
{
	for (int i=0;i<MAXSYMBLIST;i++) {
		mSymbLst[i].RemoveAll();
	}

	CRecordset rst;
	rst.m_pDatabase=&theOrderDB.mdb;
	rst.Open(AFX_DB_USE_DEFAULT_TYPE,"Select ListId,SecSymb from mSymbLst");
	CString st1;
	CDBVariant var1;
	int listid;
	
	while(rst.IsEOF()==FALSE) {
		rst.GetFieldValue("ListId",var1);
		rst.GetFieldValue("SecSymb",st1);
		listid=var1.m_lVal;
		if ((listid>=0)&&(listid<MAXSYMBLIST)&&
			(st1.GetLength()>0)){
			mSymbLst[listid].AddNoDup(st1,0);	
		}
		rst.MoveNext();  
	}
	rst.Close();
	return TRUE;

}

BOOL IssueSet::LoadShortListFromFile(LPCSTR filename, int nid,BOOL bShortAbleLst)
{
	CStdioFile fp;
	BOOL bret=TRUE;
	int counter=0;
	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"LoadShortList Error: can not open file %s",filename);
		return FALSE;
	}; 
	char str[1000];
	while(fp.ReadString(str,sizeof(str))){
		if (str[0]=='#') continue;
		char *psymbol;
		char *psize;
		int  nsize;
		
		psymbol=strtok(str,", \t\n\r");
		if (psymbol==NULL) continue;
		psize=strtok(NULL,", \t\n\r");
		if (psize) {
			nsize=atoi(psize);
		}else {
			nsize=1000000000;
		}
		
		if ((strlen(psymbol)>11)){
			bret=FALSE;
			break;
		}

		IssueIfo *pifo;
		pifo=GetIssueIfo(psymbol);
		if ((pifo==NULL)){
			pifo=GetNewBuff();
			pifo->Clear();
			ResetItemToDeault(pifo); 
			int flag=0;
			if (nid==0) {
				if (bShortAbleLst) {
					flag|=3; 
					pifo->mshortsize=nsize;
				}
				if (theCfgFile.mbDefShortAble&2) flag |=8; 
			}else if (nid==1) {
				if (bShortAbleLst) flag|=8; 
				if (theCfgFile.mbDefShortAble&1) flag |=1; 
			}
			pifo->mflag=flag; 
			strcpy(pifo->msecsym,psymbol);
			mSecMap.Add(psymbol,(long)pifo); 
		}else {
			int flag=pifo->mflag;;
			if (nid==0) {
				if (bShortAbleLst) {
					flag|=1; 
					pifo->mshortsize=nsize;
				}else {
					flag &=~1;
				}
			}else if (nid==1) {
				if (bShortAbleLst){
					flag|=8; 
				}else {
					flag  &=~8;
				}
			}
			pifo->mflag=flag;
		}
		counter++;

	}
	fp.Close();
	theCfgFile.mlog.AddLogStrFmtTm(0,"Load %d short list items form %s",counter,filename);

	return bret;
}

void IssueSet::ClearnShortList(int nid)
{
	stritem *PI=NULL;
	while(mSecMap.Enum(PI)) {
		IssueIfo *pifo=(IssueIfo *)PI->val;
		if (nid==0) {
			if (theCfgFile.mbDefShortAble&1) pifo->mflag |=1;
			else pifo->mflag&=~1;
		}else if(nid==1) {
			if (theCfgFile.mbDefShortAble&3) pifo->mflag|=8;
			else pifo->mflag&=~8;
		}
	}
}

void IssueSet::ExportIssueInfo(LPCSTR filename)
{
	CStdioFile fp;
	BOOL bret=TRUE;
	if (fp.Open(filename,CFile::modeCreate|CFile::modeWrite)==FALSE) {
		return ;
	}; 
	char str[1000];
	stritem *PI=NULL;
	while(mSecMap.Enum(PI)) {
		IssueIfo *pifo=(IssueIfo *)PI->val;
		char mkt='?';
		if (pifo->mIssueMarket) mkt=pifo->mIssueMarket;

		sprintf(str,"%s,%c,%d,%d,%d,%g\n",
			pifo->msecsym,mkt,pifo->mflag,pifo->mshortsize,pifo->mtiresize,pifo->mYClose);
		fp.WriteString(str);
	}
	fp.Close();
}

void IssueSet::SaveIssueInfo()
{
	srstSecIfo rst;
	try{
		rst.OpenToModify("");
		while(!rst.IsEOF()) {
			rst.Delete();
			rst.MoveNext();
		}
		stritem *PI=NULL;
		while(mSecMap.Enum(PI)) {
			IssueIfo *pifo=(IssueIfo *)PI->val;
			memcpy(&rst.mitem,pifo,sizeof(IssueIfo));
			rst.AddNew(); 
		}

 	}catch(SRSTErr *perr) {
		theCfgFile.mlog.AddLogStrFmt("ERROR:ODBC  %s,%d,%s",__FILE__,__LINE__,perr->merrmsg);
 		delete perr;
 	}
 	rst.Close();
}

BOOL IssueSet::LoadFuturesInfo(LPCSTR filename)
{

	CStdioFile fp;
	BOOL bret=TRUE;
	if (fp.Open(filename,CFile::modeRead)==FALSE) {
		return FALSE;
	}; 
	char buff[2000];
	int count=0;
	while(fp.ReadString(buff,sizeof(buff))){
		if (buff[0]=='#') continue;

		char *agv[10];
		int col=10;
		CStrFunc::BreakStr(buff,',',agv,&col);
		if (col<8) continue;
		stritem *PI;
		FutureIfo *pitem;
		PI=mFutureInfo.FindItem(agv[1]);
		if (PI) {
			pitem=(FutureIfo*)PI->val;
		}else {
			pitem=new FutureIfo;
			memset(pitem,0,sizeof(FutureIfo));
			strcpy(pitem->msecsym,agv[0]); 
			mFutureInfo.Add(pitem->msecsym,(long)pitem);
		}
		strncpy(pitem->mname,agv[1],sizeof(pitem->mname)-1); 
		strncpy(pitem->mExch,agv[2],sizeof(pitem->mExch)-1);  
		pitem->m_tickersize=atof(agv[3]);
		pitem->m_multiplier=atof(agv[4]);
		pitem->m_DayInit=atof(agv[5]);
		pitem->m_InitReq=atof(agv[6]);
		pitem->m_Maintain=atof(agv[7]); 
		count++;

	}
	theCfgFile.mlog.AddLogStrFmtTm(0,"Load %d Futures items.",count);

	fp.Close(); 
	return TRUE;
}

//this function use the full futures symbol to query the futures symbol information;
// for example: #ESU5 is the Emini S&P 500 futures symbol;
FutureIfo * IssueSet::GetFutureInfo(LPCSTR psymbol)
{
	char symbolroot[10];
	if (psymbol[0]!='#') return NULL;
	int len=strlen(psymbol);
	if (len<4) return NULL;
	int i = 0;
	for (i=0;i<len-3;i++) symbolroot[i]=psymbol[i+1];
	symbolroot[i]=0;
	stritem *PI;
	PI=mFutureInfo.FindItem(symbolroot);
	if (PI==NULL) return NULL;
	return (FutureIfo*)PI->val;
}

BOOL IssueSet::LoadPensonExt539(char *pfile)
{
	CStdioFile fp;
	BOOL bret=TRUE;
	int counter=0;
	if (fp.Open(pfile,CFile::modeRead)==FALSE) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"LoadShortList Error: can not open file %s",pfile);
		return FALSE;
	}; 
	char str[1000];
	while(fp.ReadString(str,sizeof(str))){
		if (str[0]=='#') continue;
		char *arr[200];
		int num=200;
		if ((CStrFunc::BreakStr(str,',',arr,&num)==FALSE)) {
			theCfgFile.mlog.AddLogStr("Invalid Penson Ext539 file format for ");	
			break;
		};
		if (num<65) {
			theCfgFile.mlog.AddLogStr("Invalid Penson Ext539 file format for ");	
			break;
		}
		
		theCfgFile.mlog.AddLogStrFmt("margainlist %s",arr[2]); 
		
		IssueIfo *pifo;
		pifo=GetIssueIfo(arr[2]);
		if ((pifo==NULL)){
			pifo=GetNewBuff();
			pifo->Clear();
			ResetItemToDeault(pifo);
			strcpy(pifo->msecsym,arr[2]);
			mSecMap.Add(pifo->msecsym,(long)pifo); 
		}
		pifo->mflag&=~0x2;
	}

	return TRUE;

}


void IssueSet::ResetItemToDeault(IssueIfo *pifo)
{
	if (theCfgFile.mbDefShortAble&1) {
		pifo->mflag |=1;
		pifo->mshortsize=1000000000;
 	}else pifo->mflag&=~1;

	if (theCfgFile.mbDefShortAble&3) pifo->mflag|=8;
	else pifo->mflag&=~8;
	pifo->mflag|=(1<<1);// default is margainable;

}

void IssueSet::ClearnNotMarginList()
{
	stritem *PI=NULL;
	mSecMap.LockMap();
	while(mSecMap.Enum(PI)) {
		IssueIfo *pifo=(IssueIfo *)PI->val;
		pifo->mflag|=(1<<1);
	}
	mSecMap.UnlockMap();
}
