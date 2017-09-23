// OrdScript.cpp: implementation of the COrdScript class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "orderserv.h"
#include "OrdScript.h"
#include "OrderConfig.h"
#include "Account.h"
#include "Infoport.h"
#include "OrderDB.h"
#include "Functions\encrypt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern InfoPort theInfoPort;
extern OrderConfig theCfgFile; 
extern OrderDB theOrderDB;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COrdScript::COrdScript()
{
	msg[0]=0;

}

COrdScript::~COrdScript()
{

}

void COrdScript::OnErrorMsg(char *errmsg)
{
	theCfgFile.mlog.AddLogStrFmt("CmdScript:%s",errmsg);
}

BOOL COrdScript::OnRunCmd(int argc, char **argv)
{
	char tmbuff[20];
	theCfgFile.FormatTimeYMDHMS(time(NULL),tmbuff);
	CString cmd;
	cmd.Format( "RunCmd at %s  :",tmbuff);
	for (int i=0;i<argc;i++) {
		cmd+=argv[i];
		cmd+=" ";
	}
	theCfgFile.mlog.AddLogStr(cmd);

	if (_stricmp(argv[0],"Help")==0) {
		doHelp();
		return TRUE;
	}else if (_stricmp(argv[0],"LoadShortList")==0) {
//-----------------------------
		if (argc!=4) {
			OnErrorMsg("Error:LoadShortList command need 4 Args");
			return FALSE;
		}
		char filename[300];
		int  nid;
		BOOL shortable;
		nid=atoi(argv[1]);
		if ((nid<0)||(nid>2)) {
			OnErrorMsg("Error:nid:0,1");
			return FALSE;
		}
		//sprintf(filename,"%s/%s",theCfgFile.mAppPath,argv[2]);
		theCfgFile.GetPath(argv[2],filename);
		if (strcmp(argv[3],"shortable")==0) {
			shortable=TRUE;
		}else {
			shortable=FALSE;
		}
		theCfgFile.mpIssueData->LoadShortListFromFile(filename,nid,shortable);
		return TRUE;
	}else if (_stricmp(argv[0],"ClearShortList")==0) {
//-----------------------------
		if (argc!=2) {
			OnErrorMsg("Error:ClearShortList command need 4 Args");
			return FALSE;

		}
		int  nid;
		nid=atoi(argv[1]);
		if ((nid<0)||(nid>2)) {
			OnErrorMsg("Error:nid:0,1");
			return FALSE;
		}
		theCfgFile.mpIssueData->ClearnShortList(nid);
		return TRUE;

	}else if (_stricmp(argv[0],"ClearIssueInfo")==0) {
//-----------------------------
		theCfgFile.mpIssueData->ClearIssueMap();
		return TRUE;
//-----------------------------
	}else if (_stricmp(argv[0],"LoadPenonExt539")==0) {
		if (argc<2) return FALSE;
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		theCfgFile.mpIssueData->LoadPensonExt539(filename);
		return TRUE;
	}else if (_stricmp(argv[0],"ClearNotMargainList")==0) {
		theCfgFile.mpIssueData->ClearnNotMarginList();
		return TRUE;
//-----------------------------
	}else if (_stricmp(argv[0],"ExportIssueInfo")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:ExportIssueInfo command need 2 Args");
			return FALSE;
		}
		char filename[300];
//		sprintf(filename,"%s/%s",theCfgFile.mAppPath,argv[1]);
		theCfgFile.GetPath(argv[1],filename);
		theCfgFile.mpIssueData->ExportIssueInfo(filename);
		return TRUE;

	}else if (_stricmp(argv[0],"SaveIssueInfo")==0) {
//-----------------------------
		theCfgFile.mpIssueData->SaveIssueInfo();
		return TRUE;

		
	}else if (_stricmp(argv[0],"LoadIssueInfo")==0) {
//-----------------------------
		theCfgFile.mpIssueData->LoadIssueInfo();
		return TRUE;

		
	}else if (_stricmp(argv[0],"LogStr")==0) {
//-----------------------------
		int i;
		time_t t1;
		char str[1000];
		time(&t1);
		theCfgFile.FormatTimeHMS(t1,str);
		strcat(str,">");
		for (i=1;i<argc;i++){
			strcat(str,argv[i]);
		};
		theCfgFile.mlog.AddLogStr(str);
		return TRUE;
	}else if (_stricmp(argv[0],"LoadFuturesInfo")==0) {
//-----------------------------
		char filename[300];
		if (argc>=2) {
			theCfgFile.GetPath(argv[1],filename);
		}else {
			theCfgFile.GetPath("FuturesInfo.txt",filename);
		}
		theCfgFile.mpIssueData->LoadFuturesInfo(filename);
		return TRUE;
		
	}else if (_stricmp(argv[0],"SaveGTCOrder")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:SaveGTCOrder filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		SaveGTCOrder(filename);
		return TRUE;
	}else if (_stricmp(argv[0],"LoadGTCOrder")==0) {
		if (argc!=3) {
			OnErrorMsg("Error:LoadGTCOrder filename errorfile");
			return FALSE;
		}
		char filename[300],errfilename[300];
		theCfgFile.GetPath(argv[1],filename);
		theCfgFile.GetPath(argv[2],errfilename);
		LoadGTCOrder(filename,errfilename);
		return TRUE;
	}else if (_stricmp(argv[0],"SendGTCOrder")==0) {
		SendGTCOrder();
	}else if (_stricmp(argv[0],"LoadPenson044")==0) {
//	this is the penson buying power and equity file.
		if (argc!=2) {
			OnErrorMsg("Error:LoadPenson044 filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		Account::LoadPenson044(filename);	
		return TRUE;
		
	}else if (_stricmp(argv[0],"LoadNorthAmerica002")==0) {
//	this is the penson buying power and equity file.
		if (argc!=2) {
			OnErrorMsg("Error:LoadNorthAmerica filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		Account::LoadNorthAmerica002(filename);	
		return TRUE;
		
	}else if (_stricmp(argv[0],"LoadCCSBPFile")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:LoadCCSBPFile filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		Account::LoadCCSBPFile(filename);	
		return TRUE;
	}else if (_stricmp(argv[0],"LoadGAONBPFile")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:LoadGAONBPFile filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		Account::LoadBP_NBLT_GAON(filename);	
		return TRUE;
	}else if (_stricmp(argv[0],"LoadQuestSOD")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:LoadQuestSOD filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		Account::LoadSODQuest(filename);	
		return TRUE;
	}else if (_stricmp(argv[0],"SaveRptFile")==0) {
		if (argc!=2) {
			OnErrorMsg("Error:SaveRptFile filename");
			return FALSE;
		}
		char filename[300];
		theCfgFile.GetPath(argv[1],filename);
		BOOL SaveReportFile(LPCSTR fname);
		SaveReportFile(filename);	
		return TRUE;
	}else if (_stricmp(argv[0],"CancelAllOpenOrder")==0){
		Account::CancelAllOpenOrders(argv[1]);
		return TRUE;
	}else if (_stricmp(argv[0],"ResetInfoPort")==0) {
		theInfoPort.ResetPort();
		return TRUE;
	}else if (_stricmp(argv[0],"OrderBpCtrlRatio")==0) {
		if (argc>=2){
			theCfgFile.mOrderBpCtrlRatio=atoi(argv[1]);
		}
		theCfgFile.mlog.AddLogStrFmt("OrderBpCtrlRatio:%d", theCfgFile.mOrderBpCtrlRatio);
	}else if (_stricmp(argv[0],"ConnectDB")==0) {
		BOOL dbret;
		dbret =theOrderDB.ConnectOrderSV(theCfgFile.mdbConnectString );
		if (dbret==FALSE) {
			theCfgFile.mlog.AddLogStrFmtTm(0,"Connect to database fail!"); 
		}else {
			theCfgFile.mlog.AddLogStrFmtTm(0,"Connect to database successed!"); 
		}
	}else if (_stricmp(argv[0],"DisConnectDB")==0) {
		theOrderDB.CloseConnection();
		theCfgFile.mlog.AddLogStrFmtTm(0,"Disconnected to database"); 

	}else if (_stricmp(argv[0],"ConnectRoute")==0) {
		if (argc<2) return TRUE;
		OrderRouter *proute=theCfgFile.GetRoute(argv[1]); 
		if (proute) proute->ConnectToServer(); 
	}else if (_stricmp(argv[0],"DisConnectRoute")==0) {
		if (argc<2) return TRUE;
		OrderRouter *proute=theCfgFile.GetRoute(argv[1]); 
		if (proute)	proute->DisconnectServer(); 
	}else if (stricmp(argv[0],"RouteNewOrder")==0) {
		if (argc<2) return TRUE;
		OrderRouter *proute=theCfgFile.GetRoute(argv[1]);
		if (proute==NULL) {
			strcpy(msg,"Invlid Route");
			return TRUE;
		}
		if (argc>=3) {
			if (argv[2][0]=='Y') {
				proute->mNewOrderEnable=TRUE;
			}else {
				proute->mNewOrderEnable=FALSE;
			}
		}
		sprintf(msg,"Route %s neworder %s",proute->mRouteName,proute->mNewOrderEnable?"Enable":"Disable");
		theCfgFile.mlog.AddLogStrFmtTm(0,msg); 
	}else if (stricmp(argv[0],"RecalculateEcnFee")==0) {
		//		Account::Reca
		Account::sMapAccid.LockMap();
		intitem *PIaccitem=NULL;
		while(Account::sMapAccid.Enum(PIaccitem)) {
			Account *pacc=(Account*)PIaccitem->val;
			intitem *PItrade;
			pacc->mLstTrades.LockLst();
			pacc->mLstTrades.EnumFirst(&PItrade);
			while(PItrade!=NULL) {
				itemTrade *pTrade=(itemTrade*)PItrade->val; 
				pTrade->mEcnFee=pTrade->GetEcnFee(NULL);
				pTrade->SendChange(itemTrade::defecnfee);
				pacc->mLstTrades.EnumNext(&PItrade); 
			}
			pacc->mLstTrades.UnlockLst();
		}
		Account::sMapAccid.UnlockMap();
	}else if (stricmp(argv[0],"RecalculateHoldBP")==0) {
		if (argc<2) {
			theCfgFile.mlog.AddLogStr("Account missing");
			return TRUE;
		}
		Account *pacc=Account::FindAccByName(argv[1]);
		if (pacc==NULL) {
			theCfgFile.mlog.AddLogStr("Invalid account"); 
			return TRUE;
		}
		double hldpwr=pacc->ReCalOrderHoldBP();
		if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
			pacc->mitemEquity.mCurrHoldBP=hldpwr;
			char pkg[200];
			pacc->MakePkgByEquity(pkg,1<<8);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
	}else if (stricmp(argv[0],"enpwd")==0) {//this script is just for testing the password encrpyt 
		encrypt en;
		unsigned char result[100];
		en.EncryptPWD((unsigned char* )argv[1],result);
		theCfgFile.mLoginLog.AddLogStrFmt("result>%s<",result);
	}else {
		return CCmdScript::OnRunCmd(argc,argv);
	}
	return TRUE;
}

void COrdScript::doHelp()
{
	CString st;
	st+="This command script can execute some command to control order server's task.\n";
	st+="For detail command list, refre to the document.\n";
	
	theCfgFile.mlog.AddLogStr(st);
		
}

CScriptItem::CScriptItem()
{
	mpcmd=NULL;
	mtime=0;
	mbtrigged=FALSE;
	mbworkdayonly=FALSE;
}

CScriptItem::~CScriptItem()
{	
	SetCmd(NULL);
}

void CScriptItem::SetCmd(LPCSTR pcmd)
{
	if (mpcmd) {
		free(mpcmd);
		mpcmd=NULL;
	}
	if (pcmd==NULL) return;
	if (strlen(pcmd)==0) return;
	mpcmd=(char*)malloc(strlen(pcmd)+1);
	strcpy(mpcmd,pcmd);
}

void CScriptItem::SetTime(int ntime)
{
	time_t t1;
	time(&t1);
	if (ntime<t1){
		mtime=0;
	}else{
		mtime=ntime;
		mbtrigged=FALSE;
	}
}

//false can be removed.
BOOL CScriptItem::CheckTrigger(int ntime)
{
	if ((mbtrigged)||(mtime==0)) return FALSE;
	if (ntime>mtime) {
		mbtrigged=TRUE;
		if (mpcmd==NULL) return TRUE;
		COrdScript script;
		script.RunCmds(mpcmd); 
		return FALSE;
	}
	return TRUE;
}

BOOL COrdScript::SaveGTCOrder(LPCSTR fname)
{
	char sttm[30];
	theCfgFile.FormatTimeYMDHMS(0,sttm);
	
	CFile pfile;
	BOOL bret;
	bret=pfile.Open(fname,CFile::modeWrite |CFile::modeCreate |CFile::shareDenyNone);
	if (bret==FALSE) {
		theCfgFile.mlog.AddLogStrFmt("Error: %s Save The GTC Order to file:%s",sttm,fname);
		return FALSE;
	}
	
	theCfgFile.mlog.AddLogStrFmt("%s Save The GTC Order to file:%s",sttm,fname);
	
	char buff[1000];int len;
	itemOrder::sMapOrderID.LockMap();
	intitem *PIitem;
	itemOrder *porder;
	itemOrder::sMapOrderID.EnumFirst(&PIitem);
	while(PIitem)  {
		porder=(itemOrder*)PIitem->val;
		if (porder->IsOpenOrder()&&(porder->IsGTCOrder())){
			if (porder->msecsym[0]=='+') {
				itemOptsym *poptsym;
				poptsym=itemOptsym::FindInfo(porder->msecsym);
				if (poptsym) {
					poptsym->SaveStr(buff,sizeof(buff));
					len=strlen(buff);
					strcpy(buff+len,"\r\n");len+=2;
					pfile.Write(buff,len);
				}

			}
			porder->SaveStr(buff,sizeof(buff),0xffffffff);
			len=strlen(buff);
			strcpy(buff+len,"\r\n");len+=2;
			pfile.Write(buff,len); 
		};

		itemOrder::sMapOrderID.EnumNext(&PIitem);
	}

	itemOrder::sMapOrderID.UnlockMap();
	pfile.Close();
	return TRUE;
}

BOOL COrdScript::LoadGTCOrder(LPCSTR fname,LPCSTR errfile)
{
	CStdioFile pfile;
	CStdioFile perrfile;
	BOOL bret;
	bret=pfile.Open(fname,CFile::modeRead);
	if (bret==FALSE) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"Error: Can not open GTC Order File:%s",fname);
		return FALSE;
	}
	
	bret=perrfile.Open(errfile,CFile::modeWrite |CFile::modeCreate |CFile::shareDenyNone);
	if (bret==FALSE) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"Error: Can not open GTC Order Error File:%s",errfile);
		pfile.Close();
		return FALSE;
	}

	theCfgFile.mlog.AddLogStrFmtTm(0,"Load The GTC Order to file:%s,errfile:%s",fname,errfile);

	char buff[1000];
	char buffsave[1000];

	while(pfile.ReadString(buff,sizeof(buff))) {
		strncpy(buffsave,buff,sizeof(buffsave));
		if (buffsave[0]!='%') continue;
		switch(buffsave[1]) {
		case 'P':// the option info;
			{ 
				itemOptsym item;
				itemOptsym *pos;
				item.LoadStr(buff);
				pos=item.FindAddInfo(&item,TRUE,TRUE);
				//if ((pos!=NULL)&&(pr>0)) {
				//	pos->mcurrundprice=pr;
				//}/
			}
			break;
		case 'O':// Order'
			{
				itemOrder ord;
				ord.Clear();
				ord.LoadStr(buff);
				int origorderid;
				int origorderCdate;
				if (ord.morigoid){
					origorderid=ord.morigoid;	
				}else {
					origorderid=ord.morderid; 
				}
				origorderCdate=ord.mc_date;  
				ord.mc_date=origorderCdate;

				ord.morderid=0;
				ord.mqty=ord.mlvsqty;
				ord.morigoid=origorderid;
					
				if (ord.IsOpenOrder()==0) continue;
				char msg[200];
				if (ord.NewOrder(0xffffffff,NULL,msg)==FALSE) {
					char errstr[1000];
					perrfile.WriteString(buffsave);
					sprintf(errstr,"Error:load gtc order:%s %s %d %g %s msg:%s\n",ord.IsBuyOrder()?"Buy":"Sell", 
						ord.msecsym, ord.mqty ,ord.mprice, ord.mroute,msg); 
					perrfile.WriteString(errstr); 

					theCfgFile.mlog.AddLogStrFmt("Erro in Load GTC Order:%s %s %d %g %s, msg:%s",
						ord.IsBuyOrder()?"Buy":"Sell", ord.msecsym, ord.mqty ,ord.mprice, ord.mroute,msg); 
					
				}else {
					theCfgFile.mlog.AddLogStrFmt("%%E,T,%d,%d,%d,%d,%d,%d",
						ord.morderid,ord.mm_date,origorderid,origorderCdate,0,0); 
				}
			}
			break;
		}
	}
	pfile.Close();
	perrfile.Close();
	return TRUE;

}

BOOL COrdScript::SendGTCOrder()
{

	int counter;
	int counterfail;
	counter=counterfail=0;
	itemOrder::sMapOrderID.LockMap();
	intitem *PIitem;
	itemOrder *porder;
	itemOrder::sMapOrderID.EnumFirst(&PIitem);
	while(PIitem)  {
		porder=(itemOrder*)PIitem->val;
		if (porder->IsOpenOrder()&&(porder->IsGTCOrder())){
			if ((porder->ms1&(1<<2))==0) {
				BOOL bret;
				bret=porder->SendOrder();
				if (bret) counter++;
				else counterfail++;
			}
		};

		itemOrder::sMapOrderID.EnumNext(&PIitem);
	}

	itemOrder::sMapOrderID.UnlockMap();
	theCfgFile.mlog.AddLogStrFmtTm(0,"Send GTC Order %d successed, %d fail",counter,counterfail);

	return TRUE;
}
