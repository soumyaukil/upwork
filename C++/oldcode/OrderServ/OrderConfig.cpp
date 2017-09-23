// OrderConfig.cpp: implementation of the OrderConfig class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderServ.h"
#include "OrderConfig.h"
#include "Functions\encrypt.h"
#include "Functions\strfuncs.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

OrderConfig theCfgFile;
#include "InfoPort.h"
extern InfoPort theInfoPort; 
#include "Account.h"
#include "OrdScript.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OrderConfig::OrderConfig()
{
	memset(mCompanyID,0,sizeof(mCompanyID));
	memset(mCompanyIDEx,0,sizeof(mCompanyIDEx));
	mCompanyIDCheck=TRUE;
	mOrderServerID =0;
	int i;
	for (i=0;i<MaxServerNumber;i++) {
		mServerIDlst[i]=0;
		mServerIPlst[i]=0;
		mServerPortlst[i]=0;
		mServerNumber=0;
	}

	mCurrOrderID=0; 
	mCurrTradeID=0;
	mCurrTicketID=0;
/*	strcpy(mRouteISLD.mRouteName,"ISLD");
	strcpy(mRouteSOES.mRouteName,"SOES");
	strcpy(mRouteDOT.mRouteName,"DOT");
	strcpy(mRouteARCA.mRouteName,"ARCA");
	strcpy(mRouteBRUT.mRouteName,"BRUT");
	strcpy(mRouteNITE.mRouteName,"NITE");
	strcpy(mRouteHRZG.mRouteName,"HRZG");
	strcpy(mRouteREDI.mRouteName,"REDI");
	strcpy(mRouteTEST.mRouteName,"TEST");
	strcpy(mRouteGNET.mRouteName,"GNET");
	strcpy(mRouteBAMM.mRouteName."BAMM");
*/
	mpIssueData=NULL;
	mpItemPool=NULL;

//Trading:	
	mMKTOrderBPHoldRate =1.1;
	mMKTHoldSWCHPrice	=6.0;
	mCommBPPreHold=100;

	mtmQuit=0;

	mbDefShortAble =3;
	mECNLST.init(40);
	mETFLST.init(50);

	mDayMargBPRate=4;
	mNightMargBPRate=4;

	mbRecvBPForCash=TRUE;
	mFullRequireSwitch=5.0;
	m_VersionBlock=0;
	m_VersionUpGrade=0;
	mShortPriceSwitch=0;
	mShortPriceSwitch1=0;
	mRoutes.init(30); 
	memset(mlicense,0,sizeof(mlicense));  
	memset(mlicenseDeCode,0,sizeof(mlicenseDeCode));
	memset(mMMRoute,0,sizeof(mMMRoute));
	mMMRouteEnable=TRUE;
	msqlInitTimeOut=120;
	msqlTimeOut=15;
	memset(mQSVToken,0,sizeof(mQSVToken));
	mbEnableOption=FALSE;
	mAutoConnectTest=FALSE;
	mtmFirstSecond=GetTodayFirstSec();
	mtmMktOpen=mtmFirstSecond+(9*60+30)*60;
	mtmMktClose=mtmFirstSecond+(16*60+1)*60; //4:01;

	mtmClose=0;
	mIsClosed=0;

	mRealTimeDBWrite=0; 
	mOrderBpCtrlRatio=1;

	mbEcnFeeInCap=TRUE;
	mbTradeEcnFee=TRUE;
	mbDVPKeepPosition=FALSE; 
	mbARCAUNoSpecialCharge=FALSE;
	mTSXByBasisPoint=0;


	mbRecOrderActiveToLog=TRUE;

	mrouteboxlstidx=0;
	for (i=0;i<5;i++) {
		mrouteboxlst[i][0]=0;
	}

	mbResetOrderID=TRUE;

}

OrderConfig::~OrderConfig()
{
	mECNLST.RemoveAll(); 
	if (mpIssueData!=NULL){
		delete mpIssueData;
		mpIssueData=NULL;
	}
}

int OrderConfig::OnGetCfg(LPCSTR id, LPCSTR subid, LPCSTR value)
{
	CString st;
	CString tmpst;
	if (strcmp(id,"ConnectString")==0) {
		mdbConnectString=value;
	}else if(strcmp(id,"Server")==0) {
		if (mServerNumber<MaxServerNumber) {
			mServerIDlst[mServerNumber]=atoi(subid);
			AfxExtractSubString(tmpst,value,0,',');
			mServerIPlst[mServerNumber]=inet_addr(tmpst);
			AfxExtractSubString(tmpst,value,1,',');
			mServerPortlst[mServerNumber]=atoi(tmpst);
			mServerNumber++;
		}
	}else if (strcmp(id,"WelcomeMsg")==0) {
		mWelcomeMsg=value;
	} else if (strcmp(id,"OrderServerID")==0) {
		mOrderServerID=atoi(value);
	} else if(strcmp(id,"ServerPort1")==0) {
		mServerPort1=atoi(value);
		if (mServerPort1==0) mServerPort1=3000; 
	} else if(strcmp(id,"ServerPort2")==0) {
		mServerPort2=atoi(value);
		if (mServerPort2==0) mServerPort2=3000; 
	} else if(strcmp(id,"ServerPort3")==0) {
		mServerPort3=atoi(value);
		if (mServerPort3==0) mServerPort3=3000; 
	} else if (strcmp(id,"HarshNumber")==0) { // the Harsh number;
		if (strcmp(subid,"Branch")==0) {
			mBranchNumber=atoi(value);
			if (mBranchNumber<=10) mBranchNumber=10;
		} else if(strcmp(subid,"Trader")==0) {
			mTraderNumber=atoi(value);
			if(mTraderNumber<=100) mTraderNumber=100;
		}else if (strcmp(subid,"Account")==0) {
			mAccountNumber =atoi(value);
			if(mAccountNumber<=100) mAccountNumber=100;
		}
	}else if (strcmp(id,"CompanyID")==0) {
		if ((subid==NULL)||(subid[0]==0)) {
			if (value!=NULL){
				memccpy(mCompanyID,value,0,10);
			}
		}else {
			if (strcmp(subid,"Check")==0) {
				if ((value[0]=='N')||(value[0]=='n')){
					mCompanyIDCheck=FALSE; 
				}
			}else if (stricmp(subid,"EX")==0) {
				memccpy(mCompanyIDEx,value,0,10); 
			}
		}
	}else if (strcmp(id,"OrderRouterIP")==0){
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr==NULL){
			pr=new(OrderRouter);
			mRoutes.Add(subid,(long)pr); 
		}
		GetIpPortVal(value,&pr->mServerIP ,&pr->mServerPort);
		strncpy(pr->mRouteName,subid,9);
	}else if (strcmp(id,"OrderRouterUser")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr){
			memccpy(pr->mUserID,value,0,sizeof(pr->mUserID));
		}
	}else if (strcmp(id,"OrderRouterPWD")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr){
			memccpy(pr->mUserPWD,value,0,sizeof(pr->mUserPWD));
		}
	}else if (strcmp(id,"Trading")==0){
		if (strcmp(subid,"MKTOrderBPHoldRate")==0) {
			theCfgFile.mMKTOrderBPHoldRate=atof(value); 
		}else if (strcmp(subid,"MKTHoldSWCHPrice")==0) {
			theCfgFile.mMKTHoldSWCHPrice=atof(value); 
		}else if (strcmp(subid,"CommBPPreHold")==0) {
			theCfgFile.mCommBPPreHold=atof(value);
		}else if (strcmp(subid,"RecvBPForCash")==0) {
			if (value[0]=='Y') {
				mbRecvBPForCash=TRUE; 
			}else {
				mbRecvBPForCash=FALSE;
			}
		}else if (strcmp(subid,"FullRequireSwitch")==0) {
			theCfgFile.mFullRequireSwitch=atof(value);
		}else if (strcmp(subid,"OrderBPCtrlRatio")==0) {
			theCfgFile.mOrderBpCtrlRatio=atoi(value); 
		}else if (strcmp(subid,"ECNFeeInCap")==0) {
			mbEcnFeeInCap=GetBoolVal(value);
		}else if (strcmp(subid,"TradeEcnFee")==0) {
			mbTradeEcnFee=GetBoolVal(value);
		}

	}else if (strcmp(id,"TIME")==0) {
		if (strcmp(subid,"QUIT")==0) {
			int secnum;;
			int hour,min;
			hour=min=0;
			sscanf(value,"%d:%d",&hour,&min);
			secnum=(hour*60+min)*60;
			mtmQuit=GetTodayFirstSec()+secnum;
		}else if (strcmp(subid,"CLOSE")==0) {
			int minutes=GetTimeVal(value);
			if (minutes>0) mtmClose=mtmFirstSecond+minutes*60; 
		}else if (strcmp(subid,"MktOpen")==0) {
			int minutes=GetTimeVal(value);
			if (minutes>0) mtmMktOpen=mtmFirstSecond+minutes*60; 
		}else if (strcmp(subid,"MktClose")==0) {
			int minutes=GetTimeVal(value);
			if (minutes>0) mtmMktClose=mtmFirstSecond+minutes*60; 

		}
	}else if (strcmp(id,"ShortList")==0) {
		if (strcmp(subid,"DefaultShortAble")==0) {
			if (value[0]=='Y') {
				mbDefShortAble|=1; 
			}else {
				mbDefShortAble&=~1;
			}
		}else if (strcmp(subid,"DefaultShortAble1")==0) {
			if (value[0]=='Y') {
				mbDefShortAble|=2; 
			}else {
				mbDefShortAble&=~2;
			}
		}else if (strcmp(subid,"ShortAbleSwitch")==0) {
			mShortPriceSwitch=atof(value);
		}else if (strcmp(subid,"ShortAbleSwitch1")==0) {
			mShortPriceSwitch1=atof(value);
		}
	}else if (strcmp(id,"ECNSYMBOL")==0) {
		if(strlen(value)>0) {
			mECNLST.Add(value,1);
		}
	}else if (strcmp(id,"ETFSYMBOL")==0) {
		if(strlen(value)>0) {
			mETFLST.Add(value,1);
		}
	}else if (strcmp(id,"MargBPRate")==0) {
		if (strlen(value)>=0) {
			if (strcmp(subid,"Day")==0){
				mDayMargBPRate=atoi(value); 
			}else if (strcmp(subid,"Night")==0) {
				mNightMargBPRate=atoi(value);
			}
		}
	}else if (strcmp(id,"VERSION")==0){
		WORD hval,lval;
		sscanf(value,"%d.%d",&hval,&lval);
		CString tmpst;
		tmpst=CString(value);
		int k;
		k=tmpst.Find(',');
		if ((k<=0)||(k==(tmpst.GetLength()-1))){
			tmpst="";
		}else {
			tmpst=tmpst.Mid(k+1);
		}
		if (strcmp(subid,"BLOCK")==0) {
			m_VersionBlock=hval*255+lval;
			m_msgBlock=tmpst; 
		}else if (strcmp(subid,"UPGRADE")==0){
			m_VersionUpGrade=hval*255+lval;
			m_msgUpGrade=tmpst; 
		}
	}else if (strcmp(id,"FILEPATH")==0) {
		if (strcmp(subid,"LOG")==0) {
			mPathLog=value;  
		}
	}else if (strcmp(id,"Info")==0) {
		if (strcmp(subid,"ID")==0) {
			theInfoPort.mInfoId=atoi(value);
			if (theInfoPort.mInfoId==0) {
				AfxMessageBox("the Info Server Id should not be 0");
			}
		}else if (strcmp(subid,"OutPut")==0) {
			GetIpPortVal(value,&theInfoPort.mOutIp,&theInfoPort.mOutPort);
		}else if (strcmp(subid,"InPutPort")==0) {
			theInfoPort.mInPort=atoi(value);
		}
	}else if (strcmp(id,"License")==0) {
		strncpy(mlicense,value,100); 
		encrypt en;
		en.DecryptLicenseSt(mlicense,mlicenseDeCode,en.GetSysID());
	}else if (strcmp(id,"MMRoute")==0) {
		if (strcmp(subid,"Route")==0) {
			strcpy(mMMRoute,value);
		}else if (strcmp(subid,"Enable")==0) {
			mMMRouteEnable=((value[0]=='Y')||(value[0]=='T'))?TRUE:FALSE;
		}
	}else if (strcmp(id,"QSVToken")==0) {
		strncpy(mQSVToken,value,15);
	}else if (strcmp(id,"Option")==0) {
		if (strcmp(subid,"Enable")==0) {
			mbEnableOption=((value[0]=='Y')||(value[0]=='T'))?TRUE:FALSE;
		}
	}else if (strcmp(id,"AutoConnectTest")==0) {
		mAutoConnectTest=((value[0]=='Y')||(value[0]=='T'))?TRUE:FALSE;
	}else if (strcmp(id,"OrderRouteStart")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr!=NULL){
			pr->mtmconnect=GetTimeVal(value);
		}
	}else if (strcmp(id,"OrderRouteClose")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr!=NULL){
			pr->mtmdisconnect=GetTimeVal(value);
		}
	}else if (strcmp(id,"OrderRouteAutoConnect")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr!=NULL){
			pr->mautoconnect=CConfig::GetBoolVal(value);
		}
	}else if (strcmp(id,"OrderRouteCxlReplace")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr!=NULL){
			pr->mCxlReplaceEnable=CConfig::GetBoolVal(value);
		}
	}else if (stricmp(id,"OrderRouteDisableExch")==0) {
		OrderRouter *pr;
		pr=GetRoute(subid);
		if (pr!=NULL){
			pr->SetDisableExch(value);
		}
	}else if (stricmp(id,"OrderRouteHide")==0) {
		OrderRouter*pr;
		pr=GetRoute(subid);
		if (pr!=NULL) {
			pr->m_bHideRoute=GetBoolVal(value);
		}
	}else if (stricmp(id,"OrderRouteDisableCustomerOrder")==0) {
		OrderRouter*pr;
		pr=GetRoute(subid);
		if (pr!=NULL) {
			pr->m_bDisableCustomerOrder=GetBoolVal(value);
		}
	}else if (strncmp(id,"ScriptTask",10)==0) {
		if (value ==NULL) return 0;
		if (subid==NULL) return 0;
		int h,m,t;
		h=m=0;
		sscanf(subid,"%d-%d",&h,&m);
		if (h<0) h=0;
		if (m<0) m=0;
		t=GetTodayFirstSec()+(h*60+m)*60;
		CScriptItem *pitem=new(CScriptItem);
		pitem->SetTime(t);
		pitem->SetCmd(value);
		mScriptTask.AddTail(0,(int)pitem); 
		if (strcmp(id+10,"_Workday")==0) {
			pitem->mbworkdayonly=TRUE; 
		}

	}else if (strcmp(id,"Holiday")==0) {
		if (IsToday(value)) {
			mIsWorkDay=0;
		}
	}else if (strcmp(id,"RealTimeDBWrite")==0){
		mRealTimeDBWrite=GetBoolVal(value);
	}else if (stricmp(id,"DVPKeepPosition")==0) {
		mbDVPKeepPosition=GetBoolVal(value);  
	}else if (strcmp(id,"RecOrderActiveToLog")==0) {
		mbRecOrderActiveToLog=GetBoolVal(value);
	}else if (strcmp(id,"ScriptLogon")==0) {
		mScriptLogon.Add(value,0);
	}else if (strcmp(id,"RouteBox")==0){
		if (mrouteboxlstidx<((sizeof(mrouteboxlst)/sizeof(mrouteboxlst[0])))) {
			mrouteboxlst[mrouteboxlstidx][0]=subid[0];
			strncpy(mrouteboxlst[mrouteboxlstidx]+1,value,sizeof(mrouteboxlst[0]));
			mrouteboxlstidx++;
		}
	}else if (stricmp(id,"ARCAUNoSpecialCharge")==0) {
		mbARCAUNoSpecialCharge=GetBoolVal(value); 
	}else if (stricmp(id,"TSXByBasisPoint")==0) {
		mTSXByBasisPoint=atoi(value);
	}else if (stricmp(id,"ResetOrderID")==0) {
		mbResetOrderID=GetBoolVal(value);
	}
	return 0;
}

OrderRouter * OrderConfig::FindRouter(itemOrder *porder)//LPCSTR route)
{	char *route;
	route=porder->mroute;
	OrderRouter *pret;

	if (porder->IsTestOrder()) {
		OrderRouter *proute;
		proute=GetRoute("TEST");
		if (proute==NULL) return NULL;
		if (proute->mbTestRoute==TRUE) return proute;
		else return NULL;
	}
//  if (strcmp(route,"SNET")==0) return GetRoute("SOES");
//  if (strcmp(route,"SMTG")==0) return GetRoute("SOES");
//	if (strcmp(route,"BAMM")==0) return GetRoute("SOES");
	if  (strcmp(route,"ATTN")==0) {
		pret=GetRoute("EDGA");
		if (pret==NULL) {
			pret=GetRoute("ATTN");
		}
		return pret;

	}
	return GetRoute(route);

/*
	if (strcmp(route,"ISLD")==0) return &mRouteISLD;
	if (strcmp(route,"SOES")==0) return &mRouteSOES;
	if (strcmp(route,"SNET")==0) return &mRouteSOES;
	if (strcmp(route,"DOT")==0) return &mRouteDOT;
	if (strcmp(route,"REDI")==0) return &mRouteREDI;
	if (strcmp(route,"ARCA")==0) return &mRouteARCA;
	if (strcmp(route,"HRZG")==0) return &mRouteHRZG;
	if (strcmp(route,"BRUT")==0) return &mRouteBRUT;
	if (strcmp(route,"NITE")==0) return &mRouteNITE;
	else return NULL;
*/

}




BOOL OrderConfig::init()
{

	if (mpIssueData==NULL) {
		mpIssueData=new IssueSet;
		ASSERT(mpIssueData); 
	}
	if (mpItemPool==NULL) {
		mpItemPool= new ItemPool;
		ASSERT(mpItemPool);
	}

	char pathname[1000];
	CTime t1;
	t1=CTime::GetCurrentTime();
	sprintf(pathname,"%sLOGINLOG%04d%02d%02d.txt",mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay());
	mLoginLog.OpenLogFile(pathname);

	sprintf(pathname,"%sOSVLOG%04d%02d%02d.txt",mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay());
	mlog.OpenLogFile(pathname);
	
/*	sprintf(pathname,"%sOSVLOG%04d%02d%02d.txt",mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay());
	mActlog.OpenLogFile(pathname);
*/

	time_t tt;
	tt=t1.GetTime();
	char tmst[20];
	FormatTimeYMDHMS(tt,tmst);  
	char st[40];
	sprintf(st,"msg,Startlog,%s",tmst);
	mLoginLog.AddLogStr(st);
	mlog.AddLogStr(st);

	mLogonAllow.init(1000); 
	mLogonReject.init(1000);
	mLogonSysidBlock.init(1000);
	LoadLogonCtrl();
	

	if (mIsWorkDay) mlog.AddLogStr("Today is workday"); 
	else mlog.AddLogStr("Today is not workday"); 



	mCurrTradeID	=mOrderServerID<<24;  
	mCurrOrderID	=mOrderServerID<<24;  
	mCurrTicketID   =mOrderServerID<<24;

	return TRUE;
}

BOOL OrderConfig::Exit()
{

	if (mpIssueData!=NULL){
		delete mpIssueData;
		mpIssueData=NULL;
	}
	if (mpItemPool!=NULL) {
		delete mpItemPool;
		mpItemPool=NULL;
	}
	OrderRouter *por;
	stritem *PIitem;
	mRoutes.LockMap();
	mRoutes.EnumFirst(&PIitem);
	while(PIitem){
		por=(OrderRouter*)PIitem->val;
		if (por!=NULL){
//			por->Close(TRUE);
			delete por;
		}
		mRoutes.EnumNext(&PIitem);
	}
	mRoutes.RemoveAll(); 
	mRoutes.UnlockMap();

	time_t t1;
	time(&t1);
	char tmst[20];
	FormatTimeYMDHMS(t1,tmst);  
	char st[40];
	sprintf(st,"msg,StopLog,%s",tmst);
		
	mLoginLog.AddLogStr(st);
	mlog.AddLogStr(st);
	mLoginLog.CloseLogFile();
	mlog.CloseLogFile();


	return TRUE;
}

int OrderConfig::GetShortAbleSize(char *secsym,BYTE cIssueMkt,int shortlist)
{
	IssueIfo *pifo=NULL;
	if (mpIssueData!=NULL) {
		pifo=mpIssueData->GetIssueIfo(secsym);
	}
	if (pifo==NULL) {
		switch(cIssueMkt) {// for default 
		case 'u':case 'U':
		case 'S':
			return 0;
		}
		if (shortlist==0){
			if (mbDefShortAble&1) return 1000000;
			else return 0;
		}else if (shortlist==1) {
			if (mbDefShortAble&1) return 1000000;
			else return 0;
		}
	}else {
		if (shortlist==0) {
			if (pifo->IsShortAble()) return pifo->mshortsize;
			else return 0;
		}else if (shortlist==1) {
			if (pifo->IsShortAble1()) return 10000000;
			else return 0;
		}
	}
	return 0;
}

BOOL OrderConfig::IsECN(char *pmmid)
{
	if (mECNLST.FindItem(pmmid)!=NULL) return TRUE;
	return FALSE;
}

BOOL OrderConfig::GetMarginAble(char *secsym, BYTE cIssueMkt)
{
	switch(cIssueMkt) {
	case 'u':case 'U':
//	case 'S':		//small caps default MarginAble
	case 'I':case 'F':
		return FALSE;
	}
	
	if (mpIssueData!=NULL){
		IssueIfo *pifo=NULL;
		pifo=mpIssueData->GetIssueIfo(secsym);
		if (pifo!=NULL) {
			if (!pifo->IsMarginAble()) return FALSE;
		}
	}
	return TRUE;
}

BOOL OrderConfig::SetBalance(BYTE ServerID, DWORD ServerIp, WORD ServerPort)
{
	int i;
	int bFund=FALSE;
	for (i=0;i<mServerNumber;i++) {
		if (mServerIDlst[i]==ServerID) {;
			mServerIPlst[i]=ServerIp;
			mServerPortlst[i]=ServerPort;
			bFund=TRUE;
			break;
		}
	}
	return bFund;
}

void OrderConfig::LoadLogonCtrl()
{
	CFile profile;
	char filename[1000];
	char buf[512];
	sprintf(filename,"%sLogonCtrl.txt",mAppPath);
	if (!profile.Open(filename,CFile::modeRead)) return;
	mLogonAllow.RemoveAll();
	mLogonReject.RemoveAll(); 
	CArchive ar( &profile, CArchive::load, 512, buf );
	CString str,id,subid,value;
	int serpert1,serpert2;
	while(ar.ReadString(str)) {
		str.TrimLeft();
		if (str.IsEmpty()==FALSE){
			if (str.GetAt(0)!='#') {
				serpert1 =str.Find(':',0);
				if (serpert1 ==-1) {
					continue;
				}else {
					id=str.Mid(0,serpert1);
					id.TrimRight();
					serpert2=str.Find(':',serpert1+1 );
					if (serpert2==-1) {
						continue;
					}else{
						subid=str.Mid(serpert1+1,serpert2-serpert1-1 );
						subid.TrimLeft();
						subid.TrimRight();
						value=str.Mid(serpert2+1);
						value.TrimLeft();
						value.TrimRight();
						if (value.IsEmpty()) continue;
						//get the ip address;
						GetLogonCtrlItem(id,subid,value);
					}
				}
			}
		}
	};
	ar.Close();
	profile.Close();
}


void OrderConfig::LoadRouteCtrl()
{
	FILE *fp;
	char filename[1000];
	char buff[512];
	sprintf(filename,"%sRouteCtrl.txt",mAppPath);
	fp=fopen(filename,"r");
	if (fp==NULL) {
		mlog.AddLogStr("can't load the RouteCtrl.txt, route control file");
		return;
	}
	OrderRouter *proute; 
	stritem *PI;
	mRoutes.EnumFirst(&PI);
	while(PI) {
		proute=(OrderRouter*)PI->val;
		proute->mAuthLst.RemoveAll();  
		mRoutes.EnumNext(&PI);
	}
	char *pst;
	char *strarr[10];
	int  ret;
	Account *pacc;
	int counter=0;
	while(fgets(buff,sizeof(buff),fp)) {
		int val=0;
		pst=CStrFunc::Trim(buff);
		if (pst[0]=='#') continue;
		ret=3;
		CStrFunc::BreakStr(pst,':',strarr,&ret);
		if (ret!=3) continue;
		proute=GetRoute(strarr[0]);
		if (proute==NULL) continue;
		pacc=Account::FindAccByName(CStrFunc::Trim(strarr[1]));  
		if (pacc==NULL) continue;
		if (strchr(strarr[2],'+')) val|=1;
		if (strchr(strarr[2],'-')) val|=2;
		proute->mAuthLst.Add(pacc->mitemifo.maccid,val);   
		counter++;
	}
	fclose(fp);
	mlog.AddLogStrFmtTm(0,"Load RouteCtrl.txt %d items", counter);

}

void OrderConfig::GetLogonCtrlItem(LPCSTR id, LPCSTR subid, LPCSTR value)
{
	if (id==NULL) return;
	if (subid==NULL) return;
	if (value==NULL) return;
	DWORD ip;
	DWORD a1,a2,a3,a4;
	if (sscanf(value,"%d.%d.%d.%d",&a1,&a2,&a3,&a4)!=4) return ;
	ip=((a4&0xff)<<24)+((a3&0xff)<<16)+((a2&0xff)<<8)+(a1&0xff);
	if (subid[0]=='+') {
		mLogonAllow.Add(id,(long)ip); 
	}else if (subid[0]=='-') {
		mLogonReject.Add(id,(long)ip); 
	}

}
//0 not fund
//1 fund in Allow;
//-1 found in reject;
int OrderConfig::GetLoginCtrl(LPCSTR id, DWORD ip)
{
	int ret;
	ret=0;
	stritem *PIitem;
	mLogonReject.LockMap();  
	mLogonReject.EnumFirstKey(id,&PIitem);
	while(PIitem) {
		DWORD lip;
		DWORD mask;
		lip=PIitem->val;
		mask=0;
		if ((lip&0xff      )==0xff      ) mask|=0xff      ;
		if ((lip&0xff00    )==0xff00    ) mask|=0xff00    ;
		if ((lip&0xff0000  )==0xff0000  ) mask|=0xff0000  ;
		if ((lip&0xff000000)==0xff000000) mask|=0xff000000;
		if((ip|mask)==(lip|mask)){
			ret=-1;
		}
		mLogonReject.EnumNextKey(id,&PIitem);  
	}
	mLogonReject.UnlockMap();
	
	mLogonAllow.LockMap();  
	mLogonAllow.EnumFirstKey(id,&PIitem);
	while(PIitem) {
		DWORD lip;
		DWORD mask;
		lip=PIitem->val;
		mask=0;
		if ((lip&0xff      )==0xff      ) mask|=0xff      ;
		if ((lip&0xff00    )==0xff00    ) mask|=0xff00    ;
		if ((lip&0xff0000  )==0xff0000  ) mask|=0xff0000  ;
		if ((lip&0xff000000)==0xff000000) mask|=0xff000000;
		if((ip|mask)==(lip|mask)){
			ret=1;
		}

		mLogonAllow.EnumNextKey(id,&PIitem);  
	}
	mLogonAllow.UnlockMap();
	return ret;	
}

BOOL OrderConfig::IsTestSecurity(char *secsym)
{
	char *pdest;
	pdest=strstr(secsym,"TEST");
	if (pdest==NULL) {
		return FALSE;
	}else {
		return TRUE;
	}

}

OrderRouter * OrderConfig::GetRoute(LPCSTR proute)
{
	stritem *PIitem;
	PIitem=mRoutes.FindItem(proute);
	if (PIitem==NULL) return NULL;
	return (OrderRouter*)PIitem->val;
}

BOOL OrderConfig::DisConnectAllRoute()
{
	OrderRouter *por;
	stritem *PIitem;
//	mRoutes.LockMap();//may cause deadlock; 2003-08-03
	mRoutes.EnumFirst(&PIitem);
	while(PIitem){
		por=(OrderRouter*)PIitem->val;
		if (por!=NULL){
			por->mautoconnect=0; 
			por->DisconnectServer();
		}
		mRoutes.EnumNext(&PIitem);
	}
//	mRoutes.UnlockMap();
	return TRUE;
}

void OrderConfig::SetMaxOrderID(DWORD oid)
{
	if ((BYTE)(oid>>24)==mOrderServerID) {
		if (mCurrOrderID<oid) {
			mCurrOrderID=oid;
		}
	}
}

void OrderConfig::SetMaxTradeID(DWORD tid)
{
	if ((BYTE)(tid>>24)==mOrderServerID) {
		if (mCurrTradeID<tid) {
			mCurrTradeID=tid;
		}
	}

}

time_t OrderConfig::GetExpTime()
{
	tm t1;
	time_t tret;
	WORD exp;
	exp=*(WORD*)mlicenseDeCode; 
	memset(&t1,0,sizeof(t1));
	t1.tm_mday=1;
	t1.tm_mon=exp%12;
	t1.tm_year=(exp/12)+100;
	t1.tm_isdst=-1;

	tret=mktime(&t1);
	return tret;
}

BOOL OrderConfig::ConnectAllRoute()
{
	OrderRouter *por;
	stritem *PIitem;
//	mRoutes.LockMap(); //may cause deadlock; 2003-08-03
	mRoutes.EnumFirst(&PIitem);
	while(PIitem){
		por=(OrderRouter*)PIitem->val;
		if (por!=NULL){
			por->ConnectToServer();
		}
		mRoutes.EnumNext(&PIitem);
	}
//	mRoutes.UnlockMap();
	return TRUE;

}

//this is for caculate commision,
//return :Q, nasdaq,L:listed,E ETF,
//return :U BBO
int OrderConfig::GetSecType(char *symbol, char nexchange)
{
	switch(nexchange) {
	case 'N': return 'L';
	case 'A': 
		if (mETFLST.FindItem(symbol)!=NULL) return 'E';
		else return 'L';
		break;
	case 'U':
	case 'u':
	case 'v':
	case 'V':
		return 'U';
	default:
		return 'Q';
	}
	return 'Q';
}

BOOL OrderConfig::AutoConnectRoute()
{
	int currsec;
	int currtm;
	currtm=time(NULL);
	currsec=currtm-mtmFirstSecond;

	OrderRouter *por;
	stritem *PIitem;
//	mRoutes.LockMap(); //may cause deadlock; 2003-08-03
	mRoutes.EnumFirst(&PIitem);
	while(PIitem){
		por=(OrderRouter*)PIitem->val;
		mRoutes.EnumNext(&PIitem);
		if (por==NULL) continue;
		if (por->mautoconnect==0) continue;
		if (currsec>por->mtmconnect*60){
			if (currsec<por->mtmdisconnect*60){ // try connect;
				if (por->IsConnected()==FALSE) { 
					if ((por->mlsttmconnect==0)||((currsec-(por->mlsttmconnect-mtmFirstSecond))>60)) {
						if (por->mconnectfailnum<30){
							por->ConnectToServer();
						}
					}
				}else {//is connected, check and send the heatbeat;
					if (por->mServerVersion>=((2<<8)+22)){//start to use the version;
						DWORD thisticker;
						thisticker=GetTickCount();
						if (por->mlastmessagetime==0){
							por->mlastmessagetime=thisticker; 
						}
						if ((thisticker-por->mlastmessagetime)>5*1000){
							if ((thisticker-por->mlastheartbeatSendtime)>5*1000) {
	//							theCfgFile.mlog.AddLogStrFmtTm(0,"send heartbeat %s",por->mRouteName);    
								por->SendHeartBeat();
								por->mlastheartbeatSendtime=thisticker;
							}
						}
						if ((thisticker-por->mlastmessagetime)>15*1000) {
							theCfgFile.mlog.AddLogStrFmtTm(0,"Missing HeartBeat, disconnect route:%s",por->mRouteName);    
							por->DisconnectServer();
						}
					}
				}
			}else { // try disconnect;
				if (por->IsConnected()) { 
					por->DisconnectServer();
				}
			}
		}
	}
//	mRoutes.UnlockMap();
	return TRUE;

}

void OrderConfig::DoDayClose()
{
	if (InterlockedIncrement(&mIsClosed)>1) {
		InterlockedDecrement(&mIsClosed);
		return;
	}; 
	mlog.AddLogStr("%SCLOSESTAT1"); 
	char fname[100];
	sprintf(fname,"%sPositions%s.txt",mPathLog,GetYMDStr());
	Account::SaveAllPositionToFile(fname); 
	Account::SaveAllData();
	InterlockedIncrement(&mIsClosed);  
	mlog.AddLogStr("%SCLOSESTAT2");

}


BOOL OrderConfig::IsMktHours(time_t t1)
{
	if (t1) {
		if ((t1>=mtmMktOpen)&&(t1<=mtmMktClose)) return TRUE;
	}else {
		time_t tt;
		tt=time(NULL);
		if ((tt>=mtmMktOpen)&&(tt<=mtmMktClose)) return TRUE;
	}
	return FALSE;

}

DWORD OrderConfig::GetNewOrderID()
{
	return (DWORD) InterlockedIncrement((long*)&mCurrOrderID); 
}

DWORD OrderConfig::GetNewTradeID()
{
	return (DWORD) InterlockedIncrement((long*)&mCurrTradeID); 
}

DWORD OrderConfig::GetNewTicketID()
{
	return (DWORD) InterlockedIncrement((long*)&mCurrTicketID); 
}
