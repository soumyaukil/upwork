// Client.cpp: implementation of the CClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderServ.h"
#include "Client.h"
//#include "Functions\PkgBuff.h Functions\PkgToken.h"
//#include "RstTrader.h"
#include "account.h"
#include "OrderDB.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//extern CPkgBuff thePkgbuf;
extern COrderServApp theApp;
extern OrderConfig theCfgFile;
extern OrderDB theOrderDB;

//#include "recordset\rstbranch.h"
#include "MGMT\itemsDef.h"	// Added by ClassView
#include "MGMT\itemsSRst.h"
#include "defines\PkgIDDef.h"
#include "Functions\encrypt.h"
#include "Functions\Perf.h"
#include "Functions\StrFuncs.h"

#include "OrdScript.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CClient::CClient()
{
	mInBuf=NULL;
	mOutBuf=NULL;
	mbShutdowAfterSend=FALSE;
	mptrifo=NULL;
	mWatchFlag=0; 
	mtrid=0;
	mlogintime=0;
	mbLogon=FALSE;
	mClientAddress=0;
	mClientSysID=0;
	mLoginFlag=0;
}

CClient::~CClient()
{	
	if(mInBuf!=NULL){
		dabuff::PutVoidDaBuff(mInBuf);
		mInBuf=NULL;
	}
	if(mOutBuf!=NULL){
		dabuff::PutVoidDaBuff(mOutBuf);
		mOutBuf=NULL;
	}
	dabuff * tmpbuf;
	while ((tmpbuf=mChainOutBuf.GetDabuff())!=NULL) {
		dabuff::PutVoidDaBuff(tmpbuf); 
	}
	while ((tmpbuf=mChainInBuf.GetDabuff())!=NULL) {
		dabuff::PutVoidDaBuff (tmpbuf); 
	}
}


int CClient::PutPendingRead()
{
	int ret;
	if (mInBuf==NULL) {
		mInBuf=dabuff::GetVoidDaBuff(); 
	};
	ret=Recv(mInBuf,&(mInBuf->DataBuff[mInBuf->bufend]) ,BF_SZ - mInBuf->bufend); 
/*	int ret,errno;
		DWORD socflag;
		socflag=0;

//		ret=WSARecv(mSocket,&(mInBuf->DataBuff[mInBuf->bufend]) ,BF_SZ - mInBuf->bufend,&rov.mIObytes,&socflag,&rov,NULL);
		ret=ReadFile((HANDLE)mSocket,&(mInBuf->DataBuff[mInBuf->bufend]) ,BF_SZ - mInBuf->bufend,&rov.mIObytes,&rov);
//		if(ret==SOCKET_ERROR){
		if(ret==0){
			errno=WSAGetLastError();
			if(errno==WSA_IO_PENDING){
			}else{
				DoClose(FALSE);
			};
		}else{
//			onReading(&rov);
		};

	};
	mCritical.Unlock();
*/
	if(ret==-1) {//read error, close the 
		DoClose(TRUE);
	}
	return 0;
}


int CClient::SendPkg(dabuff *pdapkg)
{
	// this place can be more efficient
	mChainOutBuf.PutDabuff(pdapkg); 
//	PutPendingWrite();
	return 0;
}

int CClient::PutPendingWrite()
{
	if(mOutBuf==NULL) mOutBuf=mChainOutBuf.GetDabuff();
	if (mOutBuf==NULL) {
		if(mbShutdowAfterSend==TRUE) {
			DoClose(FALSE);
		}
		wov.mOVstate =stVoid;
		return 0;
	}

	mChainOutBuf.FillPkg(mOutBuf);
	int ret;
	ret=Send(mOutBuf, mOutBuf->GetBuffAddr(),mOutBuf->GetPkgLen());
	if (ret==-1){
		DoClose(TRUE);
	};
	return 0;
}

int CClient::PkgLogin(char *currbuff)
{

	if (mptrifo!=NULL) return 0; 
	mbLogon=FALSE;

	char LLogst[1000];
	CTime CurrTm;
	CurrTm=CTime::GetCurrentTime(); 


	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1 the login pkage
	CString CompanyID,Passwd;
	CString tmpstr;
	tmpstr=tok.getCString();
	Passwd=tok.getCString();
	mHeartBeatInt=tok.getBYTE();
	mClientAddress =tok.getDWORD();	
	m_Version=tok.getWORD();
	//+42 Lange Setting;
	//+43 Operating System Setting;
	//+44 NetWork Setting;
	//*(int *) loadbuf=55;
	if (m_Version>=(256+35)){
		mClientSysID=tok.getDWORD(); 
	}
	if (m_Version>=(256+69)) {
		mLoginFlag=tok.getlong();
	}


	Trader *ptrader;
	Trader::sMapTrid.LockMap();
	char logMsgs[1000];
//	int ret;
	memset(logMsgs,0,sizeof(logMsgs));
	
	CString UserID;
	int dotpos;
	tmpstr.Replace(" ",NULL);
	dotpos=tmpstr.Find('.',0);
	if (dotpos<=-1) {
		CompanyID="";
		UserID=tmpstr;
	}else {
		CompanyID=tmpstr.Left(dotpos);
		UserID=tmpstr.Mid(dotpos+1);
	}
//	AfxExtractSubString(CompanyID,tmpstr,0,'.');
//	AfxExtractSubString(UserID,tmpstr,1,'.');
//	CompanyID.TrimLeft();CompanyID.TrimRight();  
//	UserID.TrimLeft();UserID.TrimRight();
	if ((theCfgFile.mCompanyIDCheck)&&(CompanyID.Compare(theCfgFile.mCompanyID)!=0)) {
		strcpy(logMsgs,"Err Company ID!");
		sprintf(LLogst,"LoginErr,%s,%s,%s,ErrCompany,%s",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString(),CompanyID); 
		theCfgFile.mLoginLog.AddLogStr(LLogst);
	}else if (m_Version<=theCfgFile.m_VersionBlock){
		strcpy(logMsgs,theCfgFile.m_msgBlock);
		sprintf(LLogst,"LoginErr,%s,%s,%s,WrongVersion,%d.%d",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString(),HIBYTE(m_Version),LOBYTE(m_Version)); 
		theCfgFile.mLoginLog.AddLogStr(LLogst);
	}else if (UserID.IsEmpty()) {
		strcpy(logMsgs,"Err User ID!");
		sprintf(LLogst,"LoginErr,%s,%s,%s,No USER",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
		theCfgFile.mLoginLog.AddLogStr(LLogst);
	} else {
		ptrader=Trader::FindTrader(UserID);
		if (ptrader==NULL){
			strcpy(logMsgs,"User Does not exist ID!");
			sprintf(LLogst,"LoginErr,%s,%s,%s,No Exist USER",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
			theCfgFile.mLoginLog.AddLogStr(LLogst);
		}else {
			if (!ptrader->IsOpen()) {
					strcpy(logMsgs,"User is closed");
					sprintf(LLogst,"LoginErr,%s,%s,%s,Closed USER",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
					theCfgFile.mLoginLog.AddLogStr(LLogst);
			}else {
				if (strcmp(Passwd,ptrader->minfo.mPWD)==0) {
					mbLogon=TRUE;
				} else {
					char pwdst[20];
					char pw[20];
					char pwa[20];
					encrypt en;
					strcpy(pwdst,Passwd);
					en.DecryptPWD((BYTE*)ptrader->minfo.mPWD,(BYTE*)pw);
					en.DecryptPWD((BYTE*)pwdst,(BYTE*)pwa);
					if ((strcmp(pw,pwa)==0)){
						mbLogon=TRUE;
					}else {
						strcpy(logMsgs,"Wrong Password!");
						sprintf(LLogst,"LoginErr,%s,%s,%s,Wrong PWD",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
						theCfgFile.mLoginLog.AddLogStr(LLogst);
					}
				}
			}
		}

		if (mbLogon) {// check if enable the frontend logon.
			if (ptrader->minfo.mstatus1&(1<<9)) { // No frontend;
				mbLogon=FALSE;
				strcpy(logMsgs,"Limited FrentEnd!");
				sprintf(LLogst,"LoginErr,%s,%s,%s,Limited FrontEnd",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
				theCfgFile.mLoginLog.AddLogStr(LLogst);
			}
		}
		if (mbLogon) {// check the ip block etc.
			DWORD ip1=mrmoteSockAddr.sin_addr.S_un.S_addr;

			int nlimit;
			nlimit=theCfgFile.GetLoginCtrl(UserID,ip1);
			if (nlimit<0) {
				mbLogon=FALSE;
				strcpy(logMsgs,"Limited IP!");
				sprintf(LLogst,"LoginErr,%s,%s,%s,Limited Ip",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
				theCfgFile.mLoginLog.AddLogStr(LLogst);
			}else {
				nlimit=theCfgFile.GetLoginCtrl("*",ip1);
				if (nlimit<0) {
					mbLogon=FALSE;
					strcpy(logMsgs,"Limited IP!");
					sprintf(LLogst,"LoginErr,%s,%s,%s,Limited Ip",UserID,CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString()); 
					theCfgFile.mLoginLog.AddLogStr(LLogst);
				}
			}
			intitem *pitem;
			pitem=theCfgFile.mLogonSysidBlock.FindItem(mClientSysID);
			if (pitem) {
				if (pitem->val==0) {
					mbLogon=FALSE;
					strcpy(logMsgs,"Limited ID!");
					sprintf(LLogst,"LoginErr,%s,%s,%s,id:%u,Limited ID",UserID,
						CurrTm.Format("%Y/%m/%d %H:%M:%S"),GetClientIPString(),mClientSysID); 
					theCfgFile.mLoginLog.AddLogStr(LLogst);
				}
			}
		}
	}

	char  retbuff[2000];
	tok.InitToSet(retbuff); 
	tok.setWORD(2); //the return package for login

	if (mbLogon) { // login successed
		tok.setBYTE(1); // successful to login;
		tok.setDWORD(ptrader->minfo.mtrid); 
		tok.setDWORD(ptrader->minfo.mstatus);// user status;
		tok.setDWORD(ptrader->minfo.mbrid);
		
		time_t mkttime;
		time(&mkttime);
		tm tm1;
		mlogintime=mkttime;
		tm1=*localtime(&mkttime);
		tok.setlong(mkttime);//set market time;
		tok.setWORD(tm1.tm_year+1900);
		tok.setBYTE(tm1.tm_mon+1);
		tok.setBYTE(tm1.tm_mday);
		tok.setBYTE(tm1.tm_hour);
		tok.setBYTE(tm1.tm_min);
		tok.setBYTE(tm1.tm_sec);


		if (m_Version<=theCfgFile.m_VersionUpGrade){
			tok.setVarStr(theCfgFile.m_msgUpGrade.Left(255));  	
		}else {
			tok.setVarStr(theCfgFile.mWelcomeMsg.Left(255)); //The WelComeMsg 
		}
		tok.setBYTE(theCfgFile.mOrderServerID); 
		SOCKADDR_IN locadr;
		int socadrlen= sizeof(SOCKADDR_IN);
		getsockname(mSocket ,(SOCKADDR *)&locadr,&socadrlen);
		tok.setDWORD(locadr.sin_addr.S_un.S_addr);
		tok.setBYTE(theCfgFile.mServerNumber);
		for (int i=0;i<theCfgFile.mServerNumber;i++){
			tok.setBYTE(theCfgFile.mServerIDlst[i]);
			tok.setDWORD(theCfgFile.mServerIPlst[i]);
			tok.setWORD(theCfgFile.mServerPortlst[i]);
		}
		tok.setWORD(theCfgFile.mDayMargBPRate);
		tok.setWORD(theCfgFile.mNightMargBPRate);
		tok.setDouble(theCfgFile.mMKTOrderBPHoldRate);
		tok.setDouble(theCfgFile.mMKTHoldSWCHPrice);
		tok.setDouble(theCfgFile.mCommBPPreHold);
		tok.setVarStr(theCfgFile.mCompanyID);
		tok.setVarStr(theCfgFile.mQSVToken); 
		tok.setWORD(theCfgFile.m_Version);
		tok.setlong(ptrader->minfo.mstatus1);  
		tok.setlong(ptrader->minfo.mstatus2);
		tok.setlong(ptrader->minfo.mstatus3);
	}else { //logon error;
		tok.setBYTE(0);
		tok.setVarStr(logMsgs);
	} 

//---Return the package.
/*	dabuff *ppkgloginret;
	ppkgloginret=dabuff::GetVoidDaBuff();
	ppkgloginret->AddPkgBuff(tok.finalizeSet());
	SendPkg(ppkgloginret);
*/	SendPkg(tok.finalizeSet()); 
	AsyncSelect(FD_WRITE);
	if (!mbLogon){
		Shutdown();
	}else {
		mptrifo=ptrader;
		if (ptrader->mpClient!=NULL) {
			ptrader->mServerId=0;
			ptrader->mpClient->mptrifo=NULL;
			char  retbuff[200];
			tok.InitToSet(retbuff); 
			tok.setWORD(4); //the return package for login
			tok.setVarStr("Someone has logged on with your ID, you have been logged off!");
			//dabuff *ppkgloginret;
			//ppkgloginret=dabuff::GetVoidDaBuff();
			//ppkgloginret->AddPkgBuff(tok.finalizeSet());
			ptrader->mpClient->SendPkg(tok.finalizeSet());
//			ptrader->mpClient->mtrid=0;
			ptrader->mpClient->Shutdown();
			sprintf(LLogst,"Replaced,%s,%s",ptrader->minfo.mtracno,CurrTm.Format("%Y/%m/%d %H:%M:%S")); 
			theCfgFile.mLoginLog.AddLogStr(LLogst);
			ptrader->mpClient->AsyncSelect(FD_WRITE);
		}
		mptrifo->mpClient=this;
		mtrid=mptrifo->minfo.mtrid;
		if (mptrifo->IsTrader()) {
			Trader::mOnlineTrader.Add(mptrifo->minfo.mtrid,(long) this);
		}else {
			Trader::mOnlineAdmin.Add(mptrifo->minfo.mtrid,(long) this);
		}
		RefInc();
		sprintf(LLogst,"Login,%s,%s,%s,Ver:%d.%d sysid:%u",mptrifo->minfo.mtracno,CurrTm.Format("%Y/%m/%d %H:%M:%S"),
			GetClientIPString(),HIBYTE(m_Version),LOBYTE(m_Version),mClientSysID); 
		theCfgFile.mLoginLog.AddLogStr(LLogst);
	};

	Trader::sMapTrid.UnlockMap();
	if (mbLogon) OnLogonSuccess();
	return 0;
}

int CClient::Shutdown()
{
/*	mCritical.Lock();
	mstate =mstate | enShutdown;
	mCritical.Unlock();
*/
	mbShutdowAfterSend=TRUE;
	return 0;
}

int CClient::onClose()
{
	Trader *ptr;
	Trader::sMapTrid.LockMap();
	int i=0;
	if (mtrid!=0) {
		ptr=Trader::FindTrader(mtrid);
		if (ptr!=NULL){
			if (ptr->IsTrader()) {
				i=Trader::mOnlineTrader.RemoveVal(mtrid,(long)this); 
				if (i==0) {
					i=Trader::mOnlineAdmin.RemoveVal(mtrid,(long)this); 
				}
			}else {
				i=Trader::mOnlineAdmin.RemoveVal(mtrid,(long)this); 
				if (i==0) {// incase that this trader has been moved to admin;
					i=Trader::mOnlineTrader.RemoveVal(mtrid,(long)this); 
				}
			}
			if (ptr->mpClient==this) {
				if (mptrifo!=NULL) {
					char str[500];
					char tmspan[100];
					CTime t1;
					t1=CTime::GetCurrentTime(); 
					if (mlogintime) {
						sprintf(tmspan,"%d Min %d Sec",(t1.GetTime()-mlogintime)/60,(t1.GetTime()-mlogintime)%60);
					}else {
						tmspan[0]=0;
					}
					sprintf(str,"LogOut,%s,%s,%s",mptrifo->minfo.mtracno,t1.Format("%Y/%m/%d %H:%M:%S"),tmspan); 
					theCfgFile.mLoginLog.AddLogStr(str);
					mptrifo->mServerId=0;
					mptrifo=NULL;
				}
				ptr->mpClient=NULL;
			}

		}
		mtrid=0;

	}
	if (mptrifo!=NULL) {
		if (mptrifo->mpClient==this) {
			mptrifo->mpClient=NULL;
		}
		mptrifo=NULL;
	}

	if (i>0) RefDec();
	Trader::sMapTrid.UnlockMap();
	return 0;
}

int CClient::PkgLogout(char *currbuff)
{
	PkgToken tok;
	//+42 Lange Setting;
	//+43 Operating System Setting;
	//+44 NetWork Setting;
	//*(int *) loadbuf=55;

	char  retbuff[200];
	tok.InitToSet(retbuff); 
	tok.setWORD(4); //the return package for login
	tok.setVarStr("Bye bye");

/*	dabuff *ppkgloginret;
	ppkgloginret=dabuff::GetVoidDaBuff();
	ppkgloginret->AddPkgBuff(tok.finalizeSet());
	SendPkg(ppkgloginret);
*/
	SendPkg(tok.finalizeSet()); 
	Shutdown();

	Trader::sMapTrid.LockMap();
	char str[500];
	CTime t1;
	t1=CTime::GetCurrentTime(); 
	if (mptrifo) {
		sprintf(str,"LogOut+,%s,%s",mptrifo->minfo.mtracno,t1.Format("%Y/%m/%d %H:%M:%S")); 
	}else {
		sprintf(str,"LogOut+,%s,%s","????",t1.Format("%Y/%m/%d %H:%M:%S")); 
	}
	Trader::sMapTrid.UnlockMap();
	theCfgFile.mLoginLog.AddLogStr(str);

	AsyncSelect(FD_WRITE);

	return 0;

}

int CClient::onConnecting(M_OVBUF *pOvl, SOCKADDR_IN *localaddr, SOCKADDR_IN *rmoteaddr)
{
	Msocket::onConnecting(pOvl,localaddr,rmoteaddr); 

	BOOL no_delay=TRUE;
	setsockopt(mSocket,IPPROTO_TCP,TCP_NODELAY,(char FAR*)&no_delay,sizeof(no_delay));

	PutPendingRead();
	return 0;
}


int CClient::onReading(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject!=NULL) {
		ASSERT(mInBuf!=NULL);
		ASSERT(mInBuf==BuffObject); 
		if (nErrno) {
			char str[100];
			sprintf(str,"SocketReadErr:trid:%d,err:%d,ref:%d",mtrid,nErrno,GetReferenceNum());
			theCfgFile.mlog.AddLogStr(str);
			DoClose(TRUE);
			return 0;

		}
		if (bflen==0){ //Close
			DoClose(TRUE);
			return 0;
		}
		mInBuf->bufend+=(WORD)bflen;
		ASSERT(mInBuf->bufend<=BF_SZ); 
		char * currBuff;
		int ret;
		while((currBuff=mInBuf->GetPkgBuff())!=NULL) {
			RefInc();
			ret=ProcessPkg(currBuff);
			RefDec();
			if (ret==0) {
			}
		}
		mInBuf->TrimPkgBuff(); 
	}		

	if (mSocket==INVALID_SOCKET) return -1;
	PutPendingRead();
	return 0;

}

int CClient::onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject!=NULL){ //buffobject is null is put pending write
		if (nErrno) {
			char str[100];
			sprintf(str,"SocketWriteErr:%d",nErrno);
			theCfgFile.mlog.AddLogStr(str);
			DoClose(TRUE);
			return 0;
		}

		ASSERT(mOutBuf!=NULL);
		ASSERT(mOutBuf==BuffObject);
		if (bflen==0){ //Close
			DoClose(TRUE);
			return 0;
		}
		mOutBuf->bufbegin+=(WORD)bflen;
		ASSERT(mOutBuf->bufbegin <=mOutBuf->bufend); 
		if (mOutBuf->bufbegin==mOutBuf->bufend){
			dabuff::PutVoidDaBuff(mOutBuf);
			mOutBuf=NULL;
		}
	}
	if (mSocket==INVALID_SOCKET) return -1;
	PutPendingWrite();
	return 0;

}

int CClient::PkgBrIfo(char *currbuff)
{
	PkgToken tok;
	char retbuf[1000];
	char infomsg[1000];
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1104 the login pkage
	itemBranch * pbitem=NULL;
	int brid;
	switch(tok.getBYTE()) {//brifoquery type
	case 1://query
		switch(tok.getBYTE()) {
		case 1://branch
			if (mptrifo!=NULL) {
				brid=mptrifo->minfo.mbrid;
				pbitem=theCfgFile.mpItemPool->GetBranch(brid);
				if (pbitem) {
					pbitem->MakeInfoPkg(retbuf,0xffff); 
					SendPkg(retbuf);
				}
				for (int i=0;i<ADM_MAX_BRANCH;i++) {
					brid=mptrifo->mBranchList[i];
					if (brid==0) break;
					pbitem=theCfgFile.mpItemPool->GetBranch(brid);
					if (pbitem) {
						pbitem->MakeInfoPkg(retbuf,0xffff); 
						SendPkg(retbuf);
					}
				}

			}
			   
			/*
			brid=tok.getlong();
			pbitem=theCfgFile.mpItemPool->GetBranch(brid);
			if (pbitem) {
				pbitem->MakeInfoPkg(retbuf,0xffff); 
				SendPkg(retbuf);
			}
			*/
			break;
		case 2://all the branch modify data is later
			intitem *PIBr;
			time_t modidate;
			modidate=tok.getlong(); 
			PIBr=theCfgFile.mpItemPool->MAP_Branch.EnumFirst();
			while(PIBr!=NULL){
				pbitem=(itemBranch*)PIBr->val;
				if (pbitem->mm_date>modidate) {
					pbitem->MakeInfoPkg(retbuf,0xffff); 
					SendPkg(retbuf);				
				}
				PIBr=theCfgFile.mpItemPool->MAP_Branch.EnumNext(PIBr);
			}
			break;
		}
		break;
	case 2://modify
		itemBranch itembr;
		itemPerson itemper;
		itemAddress itemadd;
		itemper.Clear();
		itemadd.Clear();
		itembr.Clear();
		//itemper.modifyflag=itemadd.modifyflag =itembr.modifyflag=0;
		itembr.GetPkgInfo(&tok,&itemper,&itemadd);
		if (itembr.mbrid!=0) {
			itemper.mbrid=itembr.mbrid;
			itemadd.mbrid=itembr.mbrid;
		}

		if (itemper.modifyflag !=0) {
//			rstperson rst;
//			rst.UpdateData(&itemper);
			itemper.modifyflag|=1<<1;
			itemper.UpdateData();
			itemper.MakeInfoPkg(retbuf,itemper.modifyflag);
			SendPkg(retbuf);
			if (itembr.mper_id!=itemper.mperid){
				itembr.mper_id=itemper.mperid;
				itembr.modifyflag|=1<<4;
			}
		}

		if (itemadd.modifyflag !=0) {
//			rstaddress rst;
//			rst.UpdateData(&itemadd);
			itemadd.modifyflag|=1<<2;
			itemadd.UpdateData();
			itemadd.MakeInfoPkg(retbuf,itemadd.modifyflag);
			SendPkg(retbuf);
			if (itembr.madd_id !=itemadd.maddid){
				itembr.madd_id =itemadd.maddid;
				itembr.modifyflag|=1<<5;
			}
		}

		if (itembr.modifyflag!=0) {
//			rstbranch rbranch;//(&theOrderDB.mdb);
//			rbranch.UpdateData(&itembr);
			if (itembr.UpdateData()==TRUE){
				itembr.MakeInfoPkg(retbuf,itembr.modifyflag);
				SendPkg(retbuf);
				pbitem=theCfgFile.mpItemPool->GetBranch(itembr.mbrid);
				if (pbitem!=NULL) {
					pbitem->CopyItem(&itembr,itembr.modifyflag);
					tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"branch modified!");
					SendPkg(infomsg);
				}else {
					itemBranch * pbitem=theCfgFile.mpItemPool->FM_GetBranchItem();
					pbitem->CopyItem(&itembr); 
					theCfgFile.mpItemPool->AddBranchToMap(pbitem);
					tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_NEW,0,"add branch successed!");
					SendPkg(infomsg);
				}
			}else {
				tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"branch update failed !");
				SendPkg(infomsg);
			}
		}else {
			tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"branch information modified!");
		}
		break;
	}
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::SendPkg(char *pbuf)
{
/*	dabuff * pdapkg;
	pdapkg=dabuff::GetVoidDaBuff();
	int ret=pdapkg->AddPkgBuff(pbuf); 
	if (ret==0)mChainOutBuf.PutDabuff(pdapkg); 
*/	mChainOutBuf.AddPkgBuff(pbuf); 
	return 0;
}

int CClient::ProcessPkg(char *pbuff)
{
	DWORD pkgtype;
	int burrlen;
	burrlen=*(WORD*)pbuff;
	ASSERT(burrlen<BF_SZ);
	pkgtype=*(WORD*)(pbuff+2);
	int ret;
	ret=1;
	if (!mbLogon){
		if (pkgtype!=100) {
			Shutdown();
			return 0;
		}
	}
	switch (pkgtype){
	case 100:
		ret=PkgLogin(pbuff);
		break;
	case 3:
		ret=PkgLogout(pbuff);
		break;
	case 7:
		ret=PkgHeartBeat(pbuff);
		break;
	case 9:
		ret=PkgServersReq(pbuff);
		break;
	case 11:
		ret=PkgOnlineTraders(pbuff);
		break;

	case 500:
		ret=PkgOMCmd(pbuff);
		break;
	case 600:
		ret=PkgRouteBoxLst(pbuff);
		break;
	case 1100:
		ret=PkgTrader(pbuff);
		break;
	case 1101:
		ret=PkgAccount(pbuff);
		break;
	case 1102:
		ret=PkgPosition(pbuff);
		break;
	case 1003:
		ret=PkgAcc_Access(pbuff);
		break;
	case 1104:
		ret=PkgBrIfo(pbuff);
		break;
	case 1110:
		ret=PkgPerIfo(pbuff);
		break;
	case 1111:
		ret=PkgAddIfo(pbuff);
		break;
	case 1020:
		ret=PkgOrders(pbuff);
		break;
	case 1021:
		ret=PkgTrades(pbuff);
		break;
	case 1022:
		ret=PkgTickets(pbuff);
		break;
	case 1026:
		ret=PkgOptInfo(pbuff);
		break;
	case 1029:
		ret=PkgManualOrder(pbuff,burrlen);
		break;
	case 1031:
//		ret=PkgDeal(pbuff);
		break;
	case 1032:
		ret=PkgDealNew(pbuff,1);
		break;
	case 1033:
		if (m_Version>=(1<<8)+57) {
			ret=PkgDealNew(pbuff,5);
		}else if (m_Version>=(1<<8)+42){
			ret=PkgDealNew(pbuff,4);
		}else if (m_Version>=(1<<8)+28){
			ret=PkgDealNew(pbuff,3);
		}else {
			ret=PkgDealNew(pbuff,2);
		}
		break;
	case 1040:
		ret=PkgAccCtrl(pbuff);
		break;
	case 1050:
		ret=PkgLOARequest(pbuff);
		break;
	case 2002:
		ret=PkgSecIfoQuery(pbuff);
		break;
	case 2004:
		ret=PkgSecIfoChange(pbuff);
		break;
	case 2005:
		ret=PkgUntradable(pbuff);
		break;
	case 2006:
		ret=PkgFuturesInfo(pbuff);
		break;
	case 2007://exchange aggrements;
		ret=PkgExchAgree(pbuff);
		break;


	case 2011:
		ret=PkgNewMsg(pbuff);
		break;
	case 2012:
		ret=PkgMsgQuery(pbuff);
		break;

	
	
	default:
		CString st;
		st.Format("Get Unknow Messages!  len %d ,Type %d ",burrlen,pkgtype );
		theCfgFile.mlog.AddLogStr(st);
		break;
		ret=0;
	}
	return ret;
}



int CClient::PkgTrader(char *currbuff)
{

	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1104 the login pkage
	switch(tok.getBYTE()) {//
	case 1://query
		PkgTrader_Query(&tok);
		break;
	case 2:// modify 
		PkgTrader_Modify(&tok);
		break;
	} 
//	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgPerIfo(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1104 the login pkage
	switch(tok.getBYTE()) {//brifoquery type
	case 1://query
		PkgPerIfo_Query(&tok); 
		break;
	}
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgAddIfo(char *currbuff)
{
	PkgToken tok;
//	char retbuf[1000];
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1104 the login pkage
	switch(tok.getBYTE()) {//brifoquery type
	case 1://query
		PkgAddIfo_Query(&tok); 
	}
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgAccount(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype 
	switch(tok.getBYTE()) {//
	case 1://query
		PkgAccount_Query(&tok);
		break;
	case 2:// modify 
		PkgAccount_Modify(&tok);
		break;
	} 
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgAcc_Access(char *currbuff)
{
	PkgToken tok;
	char retbuf[200];

	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1003 the login pkage
	BYTE actionflag;
	actionflag=tok.getBYTE();
	if (actionflag==0) return 0;
	AC_TR_relation actritem;
//	AC_TR_relation *ptmp;
	Account *pacc;
	switch(actionflag) {//brifoquery type
	case 1://query
		PkgAcc_Access_Query(&tok); 
		break;
	case 2://modify
		actritem.GetPkgNew(&tok);
		pacc=Account::FindAcc(actritem.maccid);
		if (pacc==NULL) return 0;
		theCfgFile.mlog.AddLogStr("Modify or new Acct_Access"); 
		if (actritem.ModifyDB()==TRUE) {
			AC_TR_relation *pitem;
			pitem=AC_TR_relation::Finditem(actritem.mtrid,actritem.maccid);
			if (pitem==NULL){
				pitem=AC_TR_relation::AddItem(&actritem);
			}else {
				pitem->CopyItem(&actritem);
			}
			pitem->MakeInfoPkgNew(retbuf,2);
			Trader::SPKGAcc(retbuf,actritem.maccid,pacc->mitemifo.mbrid,0);
		};
		break;
	case 3://delete;
		actritem.GetPkgNew(&tok); 
		pacc=Account::FindAcc(actritem.maccid);
		if (pacc==NULL) return 0;
		
		theCfgFile.mlog.AddLogStr("Delete Acct_Access"); 
		actritem.DeleteDB();
		AC_TR_relation::DeleteItem(actritem.mtrid,actritem.maccid);
		actritem.MakeInfoPkgNew(retbuf,3);

		Trader::SPKGAcc(retbuf,actritem.maccid,pacc->mitemifo.mbrid,0);
		break;
	}

	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgPosition(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1102 
	switch(tok.getBYTE()) {//query type
	case 1://query
		PkgPosition_Query(&tok); 
		break;
	case 2://modify position 
		{
			itemPosition item;
			itemPosition *pitem;
			WORD flag;
			//itemPosition *pitempos;
			item.Clear();
			item.GetPkg(&tok,&flag);
			Account *pacc;
			pacc=Account::FindAcc(item.maccid);
			if (pacc!=NULL) {
				pitem=pacc->FindPosition(item.msecsym,item.mptype);
				if (pitem==NULL) {

					time(&item.mc_date);
					item.mm_date=item.mc_date;
					//flag|=(1<<10) | (1<<9);
					flag=0xffff;

				}else {
					if(pitem->mqty==0) {
						time(&item.mc_date);
						item.mm_date=item.mc_date;
						flag|=iPosCdat|iPosMdat;//(1<<10) | (1<<9);
					}else {
						time(&item.mm_date);
						flag|=iPosMdat;//(1<<10);
					}
					if (flag & iPosInitQty) {//(1<<5)) {
						int incr;
						incr=pitem->mInitQty-item.mInitQty;
//						item.mqty=pitem->mqty-incr;
//						item.mHoldqty=pitem->mHoldqty-incr; 
//						flag|=(1<<7);
//						flag |= iPosQty|iPosHoldQty;
					}
				}
				pacc->ModifyPosition(&item,flag);
				item.ModifyDB(flag);
				pacc->TktRecaluate(TRUE,TRUE); 
//				item.SendChange(flag);
			}
//			Trader::SendPositionChange(&item,flag);

		}
		break;
	}
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgOrders(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1020 the Order information package
	BYTE action;
	action=tok.getBYTE();
	
	if (mptrifo==NULL) {
		theCfgFile.mlog.AddLogStr("Error!!!!, may crash here");
		return 0;
	}

	switch(action) {
	case 1://Order modify
		break;
	case 2://order Query;
		PkgOrders_Query(&tok); 
		break;
			
	case 3://New Orders;
		{
			Perf_Start(0);
			itemOrder item;
			DWORD flag;
			item.Clear(); 
			item.GetPkg(&tok,&flag);
			BOOL bret;
			Perf_Start(1);
			Perf_SetName(1,"NewOrder");
			bret=item.NewOrder(flag,(char*)this,NULL,(1<<0)); 
			Perf_Stop(1);
			Perf_Start(2);
			if (mptrifo==NULL) {//fund crash at next code. one more check!!!!, user the critical section!
				theCfgFile.mlog.AddLogStr("Error!!!!, may crash here");
				return 0;
			}

			char buffipo[20];
			char buffipi[20];
			CStrFunc::FormatIPAddr(mrmoteSockAddr.sin_addr.S_un.S_addr,buffipo);
			CStrFunc::FormatIPAddr(mClientAddress,buffipi);
			theCfgFile.mlog.AddLogStrFmtTm(0,"NewOrder:%s--%s %d %s,oid:%d,ip:%s:%s",mptrifo->minfo.mtracno,
				item.IsBuyOrder()?"Buy":"Sell", item.mqty,
				item.msecsym,item.morderid,
				buffipo,buffipi); 
			if (item.morigtkn==0) {
				item.morigtkn=mrmoteSockAddr.sin_addr.S_un.S_addr;
			}else {
				item.ms1|=(1<<13);
			}
			if (!bret) {
				SendOrderTknRply(0,item.morigtkn,item.morderid);
				Account *pacc;
				pacc=Account::FindAcc(item.maccid);
				if (pacc){
					char pkg[300];
					pacc->MakePkgByEquity(pkg,itemAccEquity::defCurrHoldBP|itemAccEquity::defCurrBP);
					SendPkg(pkg);
				};
			}else {
				theCfgFile.mlog.AddLogStrFmt("%%E,R,%d,%d",item.morderid,item.mc_date); 
			}
			Perf_Stop(2);
			Perf_Stop(0);
		}
		break;
	case 4://cancel Order
		PkgOrders_Cancel(&tok);
		break;
	case 5: //replace order;
		PkgOrders_Replace(&tok);
		break;
	case 9://Close Order
		PkgOrders_Close(&tok); 
		break;
	}
	return 0;
}

int CClient::PkgOrders_Query(PkgToken *ptok)
{
	DWORD modifydate;
	BYTE  queryttype;
	queryttype=ptok->getBYTE();
	modifydate=ptok->getlong(); 
	intitem *porderitem;
	itemOrder *porder;
	long accid;
	long brid;
	long trid;
	char retbuff[300];
	Account *pac;
	BOOL bOpenOrderOnly=mLoginFlag&1;
	switch(queryttype) {
	case 1://listed Account and modify date
		while((accid=ptok->getlong())!=0){
			pac=Account::FindAcc(accid);
			if(pac!=0) {
				pac->mLstOrders.LockLst(); 
				pac->mLstOrders.EnumFirst(&porderitem);
				while(porderitem!=NULL) {
					porder=(itemOrder*)porderitem->val; 
					if ((DWORD)porder->mm_date >(DWORD)modifydate){
						if (m_Version<38){
							porder->SetInfoPkg(retbuff,0x07ffffff);
						}else {
							porder->SetInfoPkg(retbuff,0x7fffffff);
						}
						
						SendPkg(retbuff);
					}
					pac->mLstOrders.EnumNext(&porderitem); 
				}
				pac->mLstOrders.UnlockLst() ;
			}
		}
		break;
	case 2://listed branch and modify date
		brid=ptok->getlong();
		itemOrder::sMapOrderID.LockMap();
		porderitem=itemOrder::sMapOrderID.EnumFirst();
		while(porderitem!=NULL) {
			porder=(itemOrder*)porderitem->val; 
			if ((DWORD)porder->mm_date >(DWORD)modifydate)
				if ((IsWatching(porder->mbrid,0))||(brid==0)) { // this is queryed account
					if ((!bOpenOrderOnly)||porder->IsOpenOrder()) { 
						if (m_Version<38){
							porder->SetInfoPkg(retbuff,0x07ffffff);
						}else {
							porder->SetInfoPkg(retbuff,0x7fffffff);
						}
		//					porder->SetInfoPkg(retbuff);

						SendPkg(retbuff);
					}
				}
			porderitem=itemOrder::sMapOrderID.EnumNext(porderitem); 
		}
		itemOrder::sMapOrderID.UnlockMap();
		break;
	case 3:// the trader
		long accbuff[100];
		int sz;
		sz=100;
		trid=ptok->getlong();
		Trader::FindTraderAccount(trid,accbuff,&sz);
		int i;
		for (i=0;i<sz;i++) {
			accid=accbuff[i];
			pac=Account::FindAcc(accid);
			if(pac!=0) {
				pac->mLstOrders.LockLst();
				pac->mLstOrders.EnumFirst(&porderitem);
				while(porderitem!=NULL) {
					porder=(itemOrder*)porderitem->val; 
					if ((DWORD)porder->mm_date >(DWORD)modifydate){
						if ((!bOpenOrderOnly)||porder->IsOpenOrder()) { 
							if (m_Version<38){
								porder->SetInfoPkg(retbuff,0x07ffffff);
							}else {
								porder->SetInfoPkg(retbuff,0x7fffffff);
							}
							SendPkg(retbuff);
						}
					}
					pac->mLstOrders.EnumNext(&porderitem); 
				}
				pac->mLstOrders.UnlockLst() ;
			}
		}
		break;

	}
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgOrders_Cancel(PkgToken *ptok)
{
	int orderid;
	int lvsqty;
	//itemOrder *pitem;
	orderid=ptok->getlong();
	lvsqty=ptok->getlong();
//	char pkgmessage[200];
	theCfgFile.mlog.AddLogStrFmtTm(0,"Try Cancel Order %d",orderid);  
	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(orderid);
	OrderRouter *prouter;
	itemOrderActive oac;
	oac.Clear(); 
	if (porder==NULL){
		oac.mtrid=mtrid;
		oac.morderid=orderid;
		oac.SetType(LOACancelRej); 
		strcpy(oac.mnote,"Order not exist");
		oac.SendActive((char*)this);
		return -1;
	}
	
	if (porder->mlvsqty<lvsqty) {
		oac.FillInfo(porder);
		oac.SetType(LOACancelRej); 
		strcpy(oac.mnote,"qty error");
		oac.SendActive((char*)this);
		return -1;
	}
	if (porder->IsGTCOrder()) {
		if ((porder->ms1&(1<<2))==0) {//not send to sever;
			int change;
			Account *pacc; 

			pacc=Account::FindAcc(porder->maccid);
			if (pacc==NULL){
				theCfgFile.mlog.AddLogStr("ERROR:Can't find Account for Order!");
			}
			
			pacc->Lock();

			change=porder->mlvsqty-lvsqty;
			porder->SetOrderOpenQty(lvsqty);
			porder->mcxlqty+=change; 
			porder->mstatus|=1<<2;
			if (lvsqty==0)	porder->mstatus&=~1L;
			strncpy(porder->mconftxt,"Canceled",sizeof(porder->mconftxt));
			time(&porder->mm_date);
			DWORD orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|
				itemOrder::deflvsqty|itemOrder::defconftxt|
				itemOrder::deftoken|itemOrder::defm_date|
				itemOrder::defcxlqty;
			
			porder->SendChange(orderchangeflag);
			porder->ModifyDB(orderchangeflag);

			itemOrderActive pactive;
			pactive.Clear(); 
			
			pactive.FillInfo(porder); 

			pactive.SetType(LOACanceled);
			pactive.mqty=change; 
			strcpy(pactive.mnote,"Canceled");
			pactive.SendActive(porder->mbrid);
			pactive.SaveToDB(); 

			double hldpwr=pacc->ReCalOrderHoldBP();
			if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
				pacc->mitemEquity.mCurrHoldBP=hldpwr;
				char pkg[200];
				pacc->MakePkgByEquity(pkg,1<<8);
				Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			}
			pacc->UnLock(); 
			theCfgFile.mlog.AddLogStrFmt("%%E,CR,%d,%d",porder->morderid,porder->mm_date); 
			return 0;
		}
	}

	prouter=theCfgFile.FindRouter(porder);//->mroute);
	if (prouter==NULL){
		oac.FillInfo(porder);
		oac.SetType(LOACancelRej); 
		strcpy(oac.mnote,"Wrong Route");
		oac.SendActive((char*)this);
		return -1;
	}
	if (prouter->IsEnabled()==FALSE) {
		oac.FillInfo(porder);
		oac.SetType(LOACancelRej); 
		strcpy(oac.mnote,"Route Disabled");
		oac.SendActive((char*)this);
		return -1;
	}
	BOOL ret;
	char msgstr[50];
	memset(msgstr,0,sizeof(msgstr));
	ret=prouter->SendCancelOrder(porder,lvsqty,msgstr); 
	if (ret==FALSE) {
		oac.FillInfo(porder);
		oac.SetType(LOACancelRej); 
		strcpy(oac.mnote,msgstr);
		oac.SendActive((char*)this);
		return -1;
	}

	oac.FillInfo(porder);
	oac.SetType(LOACanceling); 
	strcpy(oac.mnote,msgstr);

	if (oac.mlogtime==0) oac.mlogtime=time(NULL); 
	theCfgFile.mlog.AddLogStrFmt("%%E,CR,%d,%d",porder->morderid,oac.mlogtime); 
	oac.SendActive(porder->mbrid);
	return 0;
}

int CClient::PkgTrades(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1021 the trades information package
	BYTE action;
	action=tok.getBYTE();

	switch(action) {
	case 1://Trades modify
		PkgTrades_Modify(&tok);
		break;
	case 2://Trades Query;
		PkgTrades_Query(&tok); 
		break;
	}
	return 0;

}

int CClient::PkgTrades_Query(PkgToken *ptok)
{
	DWORD modifydate;
	BYTE  queryttype;
	queryttype=ptok->getBYTE();
	modifydate=ptok->getlong(); 
	intitem *ptradeitem;
	itemTrade  *pTrade;
	long accid;
	long brid;
	char retbuff[300];
	switch(queryttype) {
	case 1://listed Account and modify date
		while ((accid=ptok->getlong())!=0) {
			Account *pacc;
			pacc=Account::FindAcc(accid);
			if (pacc==NULL) continue;
			pacc->mLstTrades.LockLst();
			pacc->mLstTrades.EnumFirst(&ptradeitem);
			while(ptradeitem!=NULL) {
				pTrade=(itemTrade*)ptradeitem->val; 
				if ((DWORD)pTrade->mm_date >(DWORD)modifydate) {
						pTrade->SetInfoPkg(retbuff);
						SendPkg(retbuff);
					}
				pacc->mLstTrades.EnumNext(&ptradeitem); 
			}
			pacc->mLstTrades.UnlockLst();
		}
		break;
	case 2://Branch and modify date
		brid=ptok->getlong();
		itemTrade::sMapTradeID.LockMap();
		itemTrade::sMapTradeID.EnumFirst(&ptradeitem);
		while(ptradeitem!=NULL) {
			pTrade=(itemTrade*)ptradeitem->val; 
			if ((DWORD)pTrade->mm_date >(DWORD)modifydate)
				if ((IsWatching(pTrade->mbrid,0))||(brid==0)) { // this is queryed account
					pTrade->SetInfoPkg(retbuff);
					SendPkg(retbuff);
				}
			itemTrade::sMapTradeID.EnumNext(&ptradeitem); 
		}
		itemTrade::sMapTradeID.UnlockMap();
		break;
	case 3:
		long accbuff[100];
		int sz;
		int trid;
		sz=100;
		trid=ptok->getlong();
		Trader::FindTraderAccount(trid,accbuff,&sz);
		int i;
		for (i=0;i<sz;i++) {
			Account *pacc;
			pacc=Account::FindAcc(accbuff[i]);
			if (pacc==NULL) continue;
			pacc->mLstTrades.LockLst();
			pacc->mLstTrades.EnumFirst(&ptradeitem);
			while(ptradeitem!=NULL) {
				pTrade=(itemTrade*)ptradeitem->val; 
				if ((DWORD)pTrade->mm_date >(DWORD)modifydate) {
						pTrade->SetInfoPkg(retbuff);
						SendPkg(retbuff);
					}
				pacc->mLstTrades.EnumNext(&ptradeitem); 
			}
			pacc->mLstTrades.UnlockLst();
		}
		break;

	default:
		return 0;
	}
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgTrades_Modify(PkgToken *ptok)
{
	itemTrade item;
	DWORD flag;
	item.Clear();
	item.GetPkg(ptok,&flag);
	itemTrade *ptrade;
	intitem *ptraditem;
	Account *pacc;

	if (item.mtradeid==0) {// new trade.
		item.mtradeid=theCfgFile.GetNewTradeID();//(DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrTradeID);
		flag|=1;//changed traderid;
		time(&item.mc_date);
		item.mm_date=item.mc_date;
		flag|= (itemTrade::defm_date|itemTrade::defc_date);
		item.ModifyDB(flag);
		pacc=Account::FindAcc(item.maccid);
		if (pacc!=NULL){
			itemAccEquity oldequity;
			oldequity.CopyFrom(&pacc->mitemEquity); 
			ptrade=itemTrade::GetVoidBuff();
			ptrade->CopyFrom(&item); 
			pacc->mLstTrades.AddAscent(ptrade->me_date,(long)ptrade);
			itemTrade::sMapTradeID.Add(ptrade->mtradeid,(long)ptrade);
			ptrade->SendChange(flag);
			pacc->TktRecaluate(TRUE,TRUE);
		}
	}else {// modify exist trade.
		ptraditem=itemTrade::sMapTradeID.FindItem(item.mtradeid);
		if (ptraditem==NULL) return 0;
		ptrade=(itemTrade*)ptraditem->val;
		if (ptrade==NULL) return 0;

		time(&item.mm_date);
		flag|= (itemTrade::defm_date);
		if (flag&itemTrade::defaccid) {// change the account;
			Account *pacc1;
			if (ptraditem!=NULL) {
				//remove the old trade
				ASSERT(ptrade!=NULL);
				pacc=Account::FindAcc(ptrade->maccid);
				pacc1=Account::FindAcc(item.maccid);

				itemTrade *ptrade1;
				ptrade1=itemTrade::GetVoidBuff(); 
				ptrade1->CopyFrom(ptrade); 

				if (pacc!=NULL) {
//					pacc->mLstTrades.RemoveVal((long)ptrade);
					ptrade->SetOpenFlag(FALSE); 
					ptrade->ModifyDB(itemTrade::defstatus|itemTrade::defm_date);
					ptrade->SendChange(itemTrade::defstatus|itemTrade::defm_date);
					pacc->TktRecaluate(TRUE,TRUE);
				}
				if (pacc1==NULL) {
					item.maccid=0;
				}
				

				ptrade1->CopyFrom(&item,flag);
//				item.ModifyDB(flag);
//				item.SendChange(flag);

				if (pacc1!=NULL) { //find account which the trade switch to.
					ptrade1->mtradeid=theCfgFile.GetNewTradeID();// (DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrTradeID);
					ptrade1->ModifyDB  (0xffffffff);
					ptrade1->SendChange(0xffffffff);
					//Add New Trade to new account;
					pacc1->mLstTrades.AddAscent(ptrade1->me_date,(long)ptrade1);
					itemTrade::sMapTradeID.Add(ptrade1->mtradeid,(long)ptrade1);
					pacc1->TktRecaluate(TRUE,TRUE); 
				}else {// invalid trade, free the buff.
					ptrade1->PutToBuff();
				}
			}
		} else {//just change some other information;
			ptrade->CopyFrom(&item,flag);
			ptrade->ModifyDB(flag);
			ptrade->SendChange(flag);
			pacc=Account::FindAcc(ptrade->maccid);
			if (pacc!=0) {
				pacc->TktRecaluate(TRUE,TRUE);
			}
		}
	}
	return 0;
}

int CClient::PkgAccount_Query(PkgToken *ptok)
{
	char buff[3000];
	Account * item;
	intitem *PIaccount;
	time_t modidate;
	long tmpid;
	int accid;
	switch(ptok->getBYTE()) {
	case 1://the spcecific accounts.
		modidate=ptok->getlong();
		tmpid=ptok->getlong();
		item=Account::FindAcc(tmpid);
		if (item!=NULL){
			if (item->MakePkgByTime(buff,modidate)) {
				SendPkg(buff);
			};
			accid=item->mitemifo.maccid ;
			if ((item->mitemCtrl.maccid!=0)&&(item->mitemCtrl.MakePkg(buff))) {
				SendPkg(buff);
			};
			if ((item->mpitemCtrl_Symb!=NULL)&&(itemAccCtrl_Symb::MakePkg_Sym(accid,item->mpitemCtrl_Symb,buff))){
				SendPkg(buff);
			};
			if ((item->mpitemCtrl_Route!=NULL)&&(itemAccCtrl_Route::MakePkg_Route(accid,item->mpitemCtrl_Route,buff))){
				SendPkg(buff);
			};

		}
		break;
	case 2://all the Accounts modify data is later
		modidate=ptok->getlong(); 
		tmpid=ptok->getlong();
		Account::sMapAccid.LockMap();
		Account::sMapAccid.EnumFirst(&PIaccount);
		while(PIaccount!=NULL){
			item=(Account*)PIaccount->val ;
			ASSERT(item!=NULL);
			if ((tmpid==0)||(IsWatching(item->mitemifo.mbrid,0))) {//(item->mitemifo.mbrid==tmpid)) {
				if (item->MakePkgByTime(buff,modidate)) {
					SendPkg(buff);
				}
				accid=item->mitemifo.maccid ;
				if ((item->mitemCtrl.maccid!=0)&&(item->mitemCtrl.MakePkg(buff))) {
					SendPkg(buff);
				};
				if ((item->mpitemCtrl_Symb!=NULL)&&(itemAccCtrl_Symb::MakePkg_Sym(accid,item->mpitemCtrl_Symb,buff))){
					SendPkg(buff);
				};
				if ((item->mpitemCtrl_Route!=NULL)&&(itemAccCtrl_Route::MakePkg_Route(accid,item->mpitemCtrl_Route,buff))){
					SendPkg(buff);
				};
			}
			Account::sMapAccid.EnumNext(&PIaccount);

		}
		Account::sMapAccid.UnlockMap();
		break;
	case 3://all the Accounts belong the traderid
		modidate=ptok->getlong();
		tmpid=ptok->getlong();
		Trader *ptr;
		AC_TR_relation *pAC_TR;
		ptr=Trader::FindTrader(tmpid);
		if (ptr!=NULL) {
			pAC_TR=ptr->pFistTrader;
			while(pAC_TR!=NULL) {
				item=Account::FindAcc(pAC_TR->maccid);
				if (item!=NULL){
					if (item->MakePkgByTime(buff,modidate)) {
						SendPkg(buff);
					}; 
					accid=item->mitemifo.maccid ;
					if ((item->mitemCtrl.maccid!=0)&&(item->mitemCtrl.MakePkg(buff))) {
						SendPkg(buff);
					};
					if ((item->mpitemCtrl_Symb!=NULL)&&(itemAccCtrl_Symb::MakePkg_Sym(accid,item->mpitemCtrl_Symb,buff))){
						SendPkg(buff);
					};
					if ((item->mpitemCtrl_Route!=NULL)&&(itemAccCtrl_Route::MakePkg_Route(accid,item->mpitemCtrl_Route,buff))){
						SendPkg(buff);
					};
			}
				pAC_TR=pAC_TR->pnexttr;
			}
		}
		break;
	}  
	return 0;
}

int CClient::PkgAccount_Modify(PkgToken *ptok)
{
	char *retpkgIfo;
	char *retpkgPer;
	char *retpkgAdd1;
	char *retpkgAdd2;

	itemAccIfo itemacc;
	itemPerson itemper;
	itemAddress itemadd1;
	itemAddress itemadd2;
	itemAccEquity itemequity;
	itemper.Clear();
	itemadd1.Clear();
	itemadd2.Clear();
	itemacc.Clear();
	retpkgIfo=retpkgPer=retpkgAdd1=retpkgAdd2=NULL; 
	
	long accid=0;
	long brid=0;
	//itemper.modifyflag=itemadd.modifyflag =itembr.modifyflag=0;
	BYTE flag;			//the item list flag;
	WORD ifoflag=0;		// the info change mask flag;
	WORD Equityflag=0;// the equity changed mask flag;


	flag=ptok->getBYTE();
	if (flag & 1) {
		ifoflag=itemacc.GetPkg(ptok);
			accid=itemacc.maccid;
			brid=itemacc.mbrid;
	}
	if (flag&(1<<1)) {
		itemper.GetPkgInfo(ptok);
	}
	if (flag&(1<<2)) {
		itemadd1.GetPkgInfo(ptok);
	}
	if (flag&(1<<3)) {
		itemadd2.GetPkgInfo(ptok);
	}
	if (flag&(1<<4)) {
		Equityflag=itemequity.GetPkg(ptok);
		if (accid==0){
			accid=itemequity.maccid;
		}
	}
	
	
	Account *pacc;
	pacc=Account::FindAcc(accid);
	if (pacc!=NULL) {
		if ((ifoflag & (1<<2))==0) {
			brid=pacc->mitemifo.mbrid;
		}
	}
	
	if (itemper.modifyflag !=0) {
		if((flag&1)&&(itemacc.modifyflag & (1<<2))) {//brid;
			itemper.mbrid= brid;
			itemper.modifyflag |=(1<<1);
		}
		itemper.UpdateData();
		retpkgPer=(char*)malloc(1000);
		ASSERT(retpkgPer!=NULL); 
		itemper.MakeInfoPkg(retpkgPer,itemper.modifyflag);
//		SendPkg(retpkg);
		if (itemacc.mper_id !=itemper.mperid){
			itemacc.mper_id =itemper.mperid;
			ifoflag|=1<<7;
		}else {
			ifoflag&=~(1<<7);
		}
	}

	
	if (itemadd1.modifyflag !=0) {
		if((flag&1)&&(itemacc.modifyflag & (1<<2))) {//brid;
			itemadd1.mbrid= brid;
			itemadd1.modifyflag |=(1<<2);
		}
		itemadd1.UpdateData();
		retpkgAdd1=(char*)malloc(1000); 
		itemadd1.MakeInfoPkg(retpkgAdd1,itemadd1.modifyflag);
//		SendPkg(retpkg);
		if (itemacc.maddr1_id !=itemadd1.maddid){
			itemacc.maddr1_id=itemadd1.maddid;
			ifoflag|=1<<8;
		}else {
			ifoflag&=~(1<<8);
		} 
	}
	if (itemadd2.modifyflag !=0) {
		if((flag&1)&&(itemacc.modifyflag & (1<<2))) {//brid;
			itemadd2.mbrid= brid;
			itemadd2.modifyflag |=(1<<2);
		}
		itemadd2.UpdateData();
		retpkgAdd2=(char*)malloc(1000);
		itemadd2.MakeInfoPkg(retpkgAdd2,itemadd2.modifyflag);
//		SendPkg(retpkg);
		if (itemacc.maddr2_id !=itemadd2.maddid){
			itemacc.maddr2_id =itemadd2.maddid;
			ifoflag|=1<<9;
		}else {
			ifoflag&=~(1<<9);
		}
	}

	
	
	if (pacc!=NULL) {
		if (ifoflag) {
			itemacc.modifyflag=ifoflag; 
			itemacc.UpdateData();
			ifoflag|=1<<11;
			pacc->mitemifo.CopyItem(&itemacc,itemacc.modifyflag);
			retpkgIfo=(char*)malloc(1000);
			PkgToken tok;
			tok.InitToSet(retpkgIfo);
			tok.setWORD(1001);
			tok.setBYTE(1);
			ifoflag|=1;
			pacc->mitemifo.SetPkg(&tok,ifoflag); 
			tok.setBYTE(0);
			tok.finalizeSet();
//			Trader::SPKGAcc(retpkg,pacc->mitemifo.maccid,
//				pacc->mitemifo.mbrid,0);  
		}
		if (Equityflag!=0) {
			itemAccEquity newequity;
			newequity.CopyFrom(&pacc->mitemEquity);
			time(&newequity.mm_time);
			Equityflag|=(1<<13);
			double incr;
			if (Equityflag&(1<<1)){ //chang open buypower;
				incr=itemequity.mOpenBP-newequity.mOpenBP;
				newequity.mOpenBP=itemequity.mOpenBP;
				newequity.mCurrBP+=incr; 
			};	
			if (Equityflag&(1<<2)) {//changed open overnight buypower;
				incr=itemequity.mOpenNBP-newequity.mOpenNBP;
				newequity.mOpenNBP=itemequity.mOpenNBP;
				newequity.mCurrNBP+=incr;//incr; 
			}

			if(Equityflag&(1<<3)) {// change the Open euqity
				incr=itemequity.mOpenEQ-newequity.mOpenEQ;
				newequity.mOpenEQ=itemequity.mOpenEQ;
				newequity.mCurrEQ+=incr;
				newequity.mOpenMGEQ+=incr;
				newequity.mCurrMGEQ+=incr;
				newequity.mOpenEQRQ-=incr;
				newequity.mCurrEQRQ-=incr; 
			}
			pacc->TktEquityUpdate(&newequity,TRUE);
			pacc->mitemEquity.UpdateDB();
		}

	}else {
		if (ifoflag) {
			pacc=new Account;
			pacc->mitemEquity.Clear();
			pacc->mitemifo.Clear();
			itemacc.modifyflag=ifoflag;
			itemacc.UpdateData();
			ASSERT(itemacc.maccid!=0);
			ifoflag|=(1<<0)|(1<<11)|(1<<10);
			pacc->mitemifo.CopyItem(&itemacc); 

			ASSERT(pacc->mitemifo.maccid!=0);
			pacc->mitemEquity.maccid=pacc->mitemifo.maccid;
			Account::sMapAccid.AddNoDup(itemacc.maccid,long(pacc));
			accid=itemacc.maccid;
			PkgToken tok;
			retpkgIfo=(char*)malloc(1000);
			tok.InitToSet(retpkgIfo);
			tok.setWORD(1001);
			tok.setBYTE(1);
			ifoflag|=1;
			pacc->mitemifo.SetPkg(&tok,ifoflag); 
			tok.setBYTE(0);
			tok.finalizeSet();
		}
	}

	if (retpkgPer!=NULL) {
		Trader::SPKGAcc(retpkgPer,accid,brid,0);  
		delete retpkgPer;
	}
	if (retpkgAdd1!=NULL){
		Trader::SPKGAcc(retpkgAdd1,accid,brid,0);  
		delete retpkgAdd1;
	}
	if (retpkgAdd2!=NULL){
		Trader::SPKGAcc(retpkgAdd2,accid,brid,0);  
		delete retpkgAdd2;
	}
	if (retpkgIfo!=NULL){
		Trader::SPKGAcc(retpkgIfo,accid,brid,0);  
		delete retpkgIfo;
	}

	return 0;
}

int CClient::PkgOrders_Close(PkgToken *ptok)
{
	int orderid;
	//itemOrder *pitem;
	orderid=ptok->getlong();
	char pkgmessage[200];
	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(orderid);
	OrderRouter *prouter;

	if (porder==NULL){
		PkgToken tok;
		tok.InitToSet(pkgmessage); 
		tok.setWORD(1020);
		tok.setBYTE(7);// reject cancel;
		tok.setlong(orderid);
		tok.setVarStr("order doesn't exist");
		tok.finalizeSet();
		SendPkg(pkgmessage);
		return -1;
	};
	prouter=theCfgFile.FindRouter(porder);//->mroute);
	if (porder->mlvsqty<=0) prouter=NULL;
	if (prouter!=NULL) 
		if (prouter->mServerEnable==FALSE) prouter=NULL;
	if (prouter!=NULL) {
		BOOL ret;
		char msgstr[50];
		memset(msgstr,0,sizeof(msgstr));
		ret=prouter->SendCancelOrder(porder,0,msgstr); 
	}
	
	porder->mstatus&=~1L;
	porder->SetOrderOpenQty(0);
	porder->mstatus|=1<<2;
	strcpy(porder->mconftxt,"USR CLOSE");
	porder->SendChange(1|(1<<7)|(1<11)|(1<<9));
	porder->ModifyDB(1|(1<<7)|(1<11)|(1<<9));
	

	itemOrderActive oac;
	oac.Clear(); 
	oac.FillInfo(porder);
	oac.SetType(LOAClose); 

	Trader::sMapTrid.LockMap();
	strcpy(oac.mnote,mptrifo->minfo.mtracno);
	Trader::sMapTrid.UnlockMap();
	
	oac.SendActive(porder->mbrid);

	Account *pacc; 
	pacc=Account::FindAcc(porder->maccid);
	if (pacc!=NULL){
		double hldpwr=pacc->ReCalOrderHoldBP();
		if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
			pacc->mitemEquity.mCurrHoldBP=hldpwr;
			char pkg[200];
			pacc->MakePkgByEquity(pkg,1<<8);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
	}

	return 0;

}

//DEL int CClient::PkgDeal(char *currbuff)
//DEL {
//DEL 	PkgToken tok;
//DEL 	tok.InitToGet(currbuff);
//DEL 	tok.getWORD();   // pkgtype =1031
//DEL 	BYTE action;
//DEL 	action=tok.getBYTE();
//DEL 	char buff[2048];
//DEL 	itemDeal *pdeal;
//DEL 	PkgToken newtok;
//DEL 	itemDeal::sMapDealID.LockMap();
//DEL 
//DEL 	switch(action) {
//DEL 	case 1://deal modify
//DEL 		pdeal=itemDeal::GetPkgInfo(&tok);			
//DEL 		BOOL ret;
//DEL 		newtok.InitToSet(buff,2048);
//DEL 		ret=pdeal->SetPkgInfo(&newtok);
//DEL 		if (ret==TRUE) {
//DEL 			SendPkg(buff);
//DEL 		}
//DEL 		break;
//DEL 	case 2:{//Deal Query;
//DEL 		DWORD mmodifytm;
//DEL 		mmodifytm=tok.getDWORD(); 
//DEL 		intitem *pintitem;
//DEL 		itemDeal::sMapDealID.EnumFirst(&pintitem);
//DEL 		while (pintitem!=NULL) {
//DEL 			pdeal=(itemDeal*)pintitem->val;
//DEL 			if ((unsigned long)(pdeal->mm_date)>=mmodifytm){
//DEL 				BOOL ret;
//DEL 				newtok.InitToSet(buff,2048);
//DEL 				ret=pdeal->SetPkgInfo(&newtok);
//DEL 				if (ret==TRUE) {
//DEL 					SendPkg(buff);
//DEL 				}
//DEL 				
//DEL 			}
//DEL 			itemDeal::sMapDealID.EnumNext(&pintitem); 
//DEL 		}
//DEL 		break;
//DEL 		   } 
//DEL 	case 3:// delete deal
//DEL 		int dealid;
//DEL 		dealid=tok.getlong();
//DEL 		pdeal=itemDeal::FindDeal(dealid);
//DEL 		if (pdeal!=NULL) {
//DEL 			itemPassthrufee *p1,*p2;
//DEL 			
//DEL 			p1=pdeal->ppathfee;
//DEL 			while(p1!=NULL) {
//DEL 				p2=p1;
//DEL 				p1=p1->pnext;
//DEL 				itemPassthrufee::sitemPassTFbuff.PutVoidBuff((char*)p2); 
//DEL 			}
//DEL 
//DEL 			itemDeal::sMapDealID.RemoveVal(dealid,(long)pdeal);
//DEL 			itemDeal::sitemDealbuff.PutVoidBuff((char*)pdeal); 
//DEL 
//DEL 			try{
//DEL 				srstDeal rstdeal;
//DEL 				rstdeal.OpenToUpdate(dealid); 
//DEL 				if (!rstdeal.IsEOF()) {
//DEL 					rstdeal.Delete();
//DEL 				}
//DEL 				rstdeal.Close();
//DEL 			}catch (SRSTErr *perr) {
//DEL 				char str[100];
//DEL 				sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
//DEL 				theCfgFile.mlog.AddLogStr(str);
//DEL 				theCfgFile.mlog.AddLogStr(perr->merrmsg);
//DEL 				delete perr;
//DEL 			}
//DEL 
//DEL 			try{
//DEL 				srstPassthruFee rstpfee;
//DEL 				rstpfee.OpenToUpdate(dealid);
//DEL 				while(!rstpfee.IsEOF())
//DEL 				{
//DEL 					rstpfee.Delete();
//DEL 					rstpfee.MoveNext();
//DEL 				}
//DEL 				rstpfee.Close();
//DEL 			}catch (SRSTErr *perr) {
//DEL 				char str[100];
//DEL 				sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
//DEL 				theCfgFile.mlog.AddLogStr(str);
//DEL 				theCfgFile.mlog.AddLogStr(perr->merrmsg);
//DEL 				delete perr;
//DEL 
//DEL 			}
//DEL 			newtok.InitToSet(buff,2048);
//DEL 			newtok.setWORD(1031); // the package id is 1031 
//DEL 			newtok.setBYTE(3);	// information package;
//DEL 			newtok.setlong(dealid);
//DEL 			newtok.finalizeSet(); 
//DEL 			SendPkg(buff);
//DEL 
//DEL 		}
//DEL 		break;
//DEL 	}
//DEL 
//DEL 	itemDeal::sMapDealID.UnlockMap();
//DEL 	AsyncSelect(FD_WRITE);
//DEL 	return 0;
//DEL 
//DEL }

int CClient::PkgTickets(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1022 the ticket query package
	BYTE action;
	action=tok.getBYTE();
	switch(action) {
	case 1://Trades modify, not used by server, becaure ticket is modify by trades.
//		PkgTrades_Modify(&tok);
		break;
	case 2://Trades Query;
		PkgTickets_Query(&tok); 
		break;
	}
	return 0;


}

int CClient::PkgTickets_Query(PkgToken *ptok)
{
	DWORD modifydate;
	BYTE  queryttype;
	queryttype=ptok->getBYTE();
	modifydate=ptok->getlong(); 
	intitem *PIticketitem;
	intitem *PIaccitem;
	itemTicket *pTicket;
	Account *pacc;
	long accid;
	long brid;
	char retbuff[300];
	switch(queryttype) {
	case 1://listed Account and modify date
		while ((accid=ptok->getlong())!=0) {
			pacc=Account::FindAcc(accid);
			if (pacc==NULL) continue;
			pacc->TktRecaluate(FALSE,FALSE); 
			pacc->mLstTickets.LockLst();
			pacc->mLstTickets.EnumFirst(&PIticketitem);
			while(PIticketitem!=NULL) {
				pTicket=(itemTicket*)PIticketitem->val; 
				if ((DWORD)pTicket->mm_date >(DWORD)modifydate) {
						pTicket->SetInfoPkg(retbuff);
						SendPkg(retbuff);
					}
				pacc->mLstTickets.EnumNext(&PIticketitem); 
			}
			pacc->mLstTickets.UnlockLst();
		}
		break;
	case 2://Branch and modify date
		brid=ptok->getlong();
//		if (brid!
		Account::sMapAccid.LockMap();
		Account::sMapAccid.EnumFirst(&PIaccitem);
		while(PIaccitem) {
			pacc=(Account*)PIaccitem->val;
			if ((brid==0)||(brid==pacc->mitemifo.mbrid)) {
				pacc->TktRecaluate(FALSE,FALSE); 
				pacc->mLstTickets.LockLst();
				pacc->mLstTickets.EnumFirst(&PIticketitem);
				while(PIticketitem!=NULL) {
					pTicket=(itemTicket*)PIticketitem->val; 
					if ((DWORD)pTicket->mm_date >(DWORD)modifydate) {
							pTicket->SetInfoPkg(retbuff);
							SendPkg(retbuff);
						}
					pacc->mLstTickets.EnumNext(&PIticketitem); 
				}
				pacc->mLstTickets.UnlockLst();
			}
			Account::sMapAccid.EnumNext(&PIaccitem);
		}
		Account::sMapAccid.UnlockMap();
		break;
	
	default:
		return 0;
	}
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgSecIfoQuery(char *pbuff)
{
	PkgToken tok;
	tok.InitToGet(pbuff,4);
	BYTE items,retitems;
	items=tok.getBYTE();
	if (items==0) return 0;
	char retbuff[BF_SZ];
	PkgToken tokret;
	WORD pos1, pos2;
	tokret.InitToSet(retbuff); 
	tokret.setWORD(2003);//return package;
	pos1=tokret.GetPos();
	tokret.setBYTE(0); 
	int i;
	char secsym[20];
	retitems=0; 
	IssueIfo *piIssue;
	int nlist=0;
	if ((mptrifo)&&(mptrifo->minfo.mstatus&(1<<31))){
		nlist=1;
	}else {
		nlist=0;
	}

	for (i=0;i<items;i++) {
		tok.getString(secsym,sizeof(secsym));
		piIssue=theCfgFile.mpIssueData->GetIssueIfo(secsym);
		if (piIssue!=NULL) {
			tokret.setVarStr(piIssue->msecsym);
			tokret.setlong(piIssue->mflag);
			tokret.setBYTE(piIssue->mIssueMarket);
//			tokret.setlong(piIssue->mtiresize);
			tokret.setlong(theCfgFile.GetShortAbleSize(piIssue->msecsym,piIssue->mIssueMarket,nlist));
			retitems++;
		}else {
			tokret.setVarStr(secsym);
			int flag=0;
			if (theCfgFile.mbDefShortAble&1) {
				flag|=3;
			}
			if (theCfgFile.mbDefShortAble&2) {
				flag|=8;
			}
			flag|=(1<<1);//deafult is maraginable.
			tokret.setlong(flag);// default shortable;
			tokret.setBYTE('?');
			tokret.setlong(theCfgFile.GetShortAbleSize(secsym,'?',nlist));
			retitems++;
		}
	}
	pos2=tokret.SetPos(pos1); 
	tokret.setBYTE(retitems);
	tokret.SetPos(pos2);
	SendPkg(tokret.finalizeSet());
	AsyncSelect(FD_WRITE); 
	return 0;	
}

int CClient::PkgAcc_Access_Query(PkgToken *ptok)
{
	char retbuf[200];
	BYTE qtype;
	long qmodify;
	long qid;
	AC_TR_relation *ptmp;

	qtype=ptok->getBYTE(); 
	qmodify=ptok->getlong();  
	qid=ptok->getlong();
	Trader *ptr;
	Account *pac;
	intitem *PItrader;
	switch(qtype) {
	case 3:// by Traderid;
		ptr=Trader::FindTrader(qid);
		if (ptr!=NULL) {
			ptmp=ptr->pFistTrader;
			while(ptmp!=NULL) {
				if (ptmp->mm_date>=qmodify) {
					ptmp->MakeInfoPkgNew(retbuf,2);
					SendPkg(retbuf);
				}
				ptmp=ptmp->pnexttr;
			}
		}
		break;
	case 1://by accountid;
		pac=Account::FindAcc(qid);
		if (pac!=NULL) {
			ptmp=pac->pFistAcc;
			while(ptmp!=NULL) {
				if (ptmp->mm_date>=qmodify) {
					ptmp->MakeInfoPkgNew(retbuf,2);
					SendPkg(retbuf);
				}
				ptmp=ptmp->pnextac;
			}
		}
	case 2://by branchid;
		Trader::sMapTrid.LockMap();
		Trader::sMapTrid.EnumFirst(&PItrader);
		while(PItrader!=NULL){
			ptr=(Trader*)PItrader->val;
			if ((qid==0) ||(IsWatching(ptr->minfo.mbrid,0))) { //ptr->minfo.mbrid==qid)) { 
				ptmp=ptr->pFistTrader;
				while(ptmp!=NULL) {
					if (ptmp->mm_date>=qmodify) {
						ptmp->MakeInfoPkgNew(retbuf,2);
						SendPkg(retbuf);
					}
					ptmp=ptmp->pnexttr;
				}
			}
			Trader::sMapTrid.EnumNext(&PItrader);
		}

		Trader::sMapTrid.UnlockMap();
		break;
	}
	return 0;

}

int CClient::PkgPosition_Query(PkgToken *tok)
{
	char retbuf[200];
	long modidate;
	long curracc;
	long currbrid;
	intitem *pintitem;
	intitem *pintitemacc;
	itemPosition *pitem;
	Account *pacc;

	switch(tok->getBYTE()) {
	case 1://listed Account and modify date
		modidate=tok->getlong(); 
		while ((curracc=tok->getlong())!=0) {
			pacc=Account::FindAcc(curracc);
			if (pacc==NULL) continue;
			pacc->mLstPositions.LockLst();
			pacc->mLstPositions.EnumFirst(&pintitem);
			while(pintitem!=NULL) {
				pitem=(itemPosition*)pintitem->val;
				if ((pitem->mm_date>=modidate)) {
					pitem->MakeInfoPkg(retbuf,0xffff);
					SendPkg(retbuf); 
				}
				pacc->mLstPositions.EnumNext(&pintitem);
			}
			pacc->mLstPositions.UnlockLst();
		}
		break;
	case 2://listed branch and modify date
		modidate=tok->getlong(); 
		currbrid=tok->getlong();
		Account::sMapAccid.LockMap();
		Account::sMapAccid.EnumFirst(&pintitemacc);
		while(pintitemacc!=NULL) {
			pacc=(Account*)pintitemacc->val;
			if ((currbrid==0)||(IsWatching(pacc->mitemifo.mbrid,0))){//currbrid==pacc->mitemifo.mbrid)){
				pacc->mLstPositions.LockLst();
				pacc->mLstPositions.EnumFirst(&pintitem);
				while(pintitem!=NULL) {
					pitem=(itemPosition*)pintitem->val;
					if ((pitem->mm_date>=modidate)) {
						pitem->MakeInfoPkg(retbuf,0xffff);
						SendPkg(retbuf); 
					}
					pacc->mLstPositions.EnumNext(&pintitem);
				}
				pacc->mLstPositions.UnlockLst();
			}
			Account::sMapAccid.EnumNext(&pintitemacc);
		}
		Account::sMapAccid.UnlockMap();
		break;
	case 3:// the account and modify data;
		int trid;
		modidate=tok->getlong();
		trid=tok->getlong();
		long accbuff[100];
		int sz;
		sz=100;
		Trader::FindTraderAccount(trid,accbuff,&sz);
		int i;
		for (i=0;i<sz;i++) {
			pacc=Account::FindAcc(accbuff[i]);
			if (pacc==NULL) continue;
			pacc->mLstPositions.LockLst();
			pacc->mLstPositions.EnumFirst(&pintitem);
			while(pintitem!=NULL) {
				pitem=(itemPosition*)pintitem->val;
				if ((pitem->mm_date>=modidate)) {
					pitem->MakeInfoPkg(retbuf,0xffff);
					SendPkg(retbuf); 
				}
				pacc->mLstPositions.EnumNext(&pintitem);
			}
			pacc->mLstPositions.UnlockLst();
		}
		break;
	}
	return 0;

}

int CClient::PkgPerIfo_Query(PkgToken *ptok)
{
	BYTE qtype;
	long qmodify;
	long qid;
	char retbuf[1000];
	qtype=ptok->getBYTE();
	qmodify=ptok->getlong();
	qid=ptok->getlong();

	int *pbrlst=NULL;
	if (mptrifo) pbrlst=mptrifo->mBranchList;

	srstPerson srst;
	if(srst.OpenToRead(qtype,qmodify,qid,pbrlst)==TRUE) {;
		while(srst.IsEOF()==FALSE) {
			srst.mitem.MakeInfoPkg(retbuf,0xffff);
			SendPkg(retbuf);
			srst.MoveNext(); 
		}
		srst.Close(); 
	}
	return 0;
/*

	switch(tok.getBYTE()) {
	case 1://listed person
		break;
	case 2://person in certin branch:
		
		break;
	case 3://all the person modify data is later
		rstperson rst;
		rst.m_pDatabase =&theOrderDB.mdb;
		rst.Open();
		while(rst.IsEOF()==FALSE) {
//				rst.mData.modifyflag=0xffffffff; 
			rst.mData.MakeInfoPkg(retbuf,0xffff);
			SendPkg(retbuf);
			rst.MoveNext(); 
		}
		rst.Close();
	break;
	} 
*/

}

int CClient::PkgAddIfo_Query(PkgToken *ptok)
{
	BYTE qtype;
	long qmodify;
	long qid;
	char retbuf[1000];
	qtype=ptok->getBYTE();
	qmodify=ptok->getlong();
	qid=ptok->getlong();
	srstAddress srst;
	int *pbrlst=NULL;
	if (mptrifo) pbrlst=mptrifo->mBranchList;

	if(srst.OpenToRead(qtype,qmodify,qid,pbrlst)==TRUE) {;
		while(srst.IsEOF()==FALSE) {
			srst.mitem.MakeInfoPkg(retbuf,0xffff);
			SendPkg(retbuf);
			srst.MoveNext(); 
		}
		srst.Close(); 
	}
	return 0;
}

int CClient::PkgNewMsg(char *currbuff)
{
	itemMessage item;
	PkgToken tok;
	tok.InitToGet(currbuff,4);
	item.Clear();
	item.GetPkgInfo(&tok); 
	Trader::sMapTrid.LockMap();
	if (mptrifo==NULL) {
	}else if (strcmp(item.mFr,mptrifo->minfo.mtracno)!=0) {
	}else {
		time(&item.ms_date);
		Trader::DispatchMessage(&item,&mptrifo->minfo);
	}
	Trader::sMapTrid.UnlockMap();
	return 0;
}

int CClient::SendMsg(itemMessage *pitem)
{
	PkgToken tok;
	char buff[BF_SZ];
	tok.InitToSet(buff); 
	tok.setWORD(2010); 
	pitem->SetPkgInfo(&tok);
	tok.finalizeSet();
	SendPkg(buff);
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgMsgQuery(char *pbuff)
{
	PkgToken tok;
	tok.InitToGet(pbuff,4);
	BYTE qtype=tok.getBYTE(); 
	long mid=tok.getlong();
	long mdate=tok.getlong();
	srstMessage srst;
	if(srst.OpenToRead(mid,mdate)==FALSE)
		return 0;
	while(srst.IsEOF()==FALSE) {
		SendMsg(&srst.mitem);
		srst.MoveNext(); 
	}
	srst.Close();
	AsyncSelect(FD_WRITE); 
	return 0;

}

int CClient::PkgServersReq(char *currbuff)
{
//	if (mptrifo!=NULL) return 0; 

	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =9
	BYTE servernumber;
	servernumber=tok.getBYTE(); 
	BYTE i;
	char  retbuff[2000];
	PkgToken rettok;
	rettok.InitToSet(retbuff); 
	rettok.setWORD(10); //the return package for login
	int lastpos;
	lastpos=rettok.GetPos();
	rettok.setBYTE(0);
	BYTE serid;
	BYTE retnum=0;
	for (i=0;i<servernumber;i++) {
		serid=tok.getBYTE();
		BOOL bfind=FALSE;
		for (int k=0;k<theCfgFile.mServerNumber;k++){
			if (theCfgFile.mServerIDlst[k]==serid){
				rettok.setBYTE(theCfgFile.mServerIDlst[k]);
				rettok.setDWORD(theCfgFile.mServerIPlst[k]);
				rettok.setWORD(theCfgFile.mServerPortlst[k]);
				retnum++;
				bfind=TRUE;
			}
		}
		if(!bfind) {
				rettok.setBYTE(serid);
				rettok.setDWORD(0);
				rettok.setWORD(0);
				retnum++;
		}
	}
	rettok.finalizeSet();
	rettok.SetPos(lastpos);
	rettok.setBYTE(retnum); 
	SendPkg(retbuff);
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::PkgTrader_Query(PkgToken *ptok)
{
	char retbuf[1000];
	time_t tmpmodidate;
	long  tmpid;			
	intitem *PItrader;
	Trader * tritem;
			
	switch(ptok->getBYTE()) {
	case 1://Speicfic Traders
		tmpmodidate=ptok->getlong(); 
		tmpid=ptok->getlong();
		PItrader=Trader::sMapTrid.FindItem(tmpid);
		if (PItrader!=NULL) {
			tritem=(Trader*)PItrader->val;
			if (tritem==NULL) return 0;
			tritem->minfo.MakeInfoPkg(retbuf,0xffff);
			SendPkg(retbuf);
		}
		break;
	case 2://all the Traders modify data is later
		tmpmodidate=ptok->getlong(); 
		tmpid=ptok->getlong();
		Trader::sMapTrid.LockMap();
		Trader::sMapTrid.EnumFirst(&PItrader);
		while(PItrader!=NULL){
			tritem=(Trader*)PItrader->val;
			if ((tmpid==0) ||(IsWatching(tritem->minfo.mbrid,0))) { 
				if (tritem->minfo.mm_date>tmpmodidate) {
					tritem->minfo.MakeInfoPkg(retbuf,0xffff); 
					SendPkg(retbuf);				
				}
			}
			Trader::sMapTrid.EnumNext(&PItrader);
		}

		Trader::sMapTrid.UnlockMap();
		break;
	}
	return 0;
}

int CClient::PkgTrader_Modify(PkgToken *ptok)
{
	char *retpkgtr;
	char *retpkgper;
	char *retpkgadd1;
	char *retpkgadd2;
	retpkgtr=retpkgper=retpkgadd1=retpkgadd2=0;
	
	itemTrader itemtr;
	itemPerson itemper;
	itemAddress itemadd1;
	itemAddress itemadd2;
	itemper.Clear();
	itemadd1.Clear();
	itemadd2.Clear();
	itemtr.Clear();
	int trid;
	int brid;
	
	int oldstatus=0;

	//0; no change, 1: new trader, 2: modify trader;
	int nTraderChangeFlag=0;

	//itemper.modifyflag=itemadd.modifyflag =itembr.modifyflag=0;
	itemtr.GetPkgInfo(ptok,&itemper,&itemadd1,&itemadd2);
	if (itemtr.mtrid!=0) {
		trid=itemtr.mtrid;
		Trader *ptrader;
		ptrader=Trader::FindTrader(itemtr.mtrid);
		if (((itemtr.modifyflag &(1<<3))==0)&&((ptrader!=NULL))){
			brid=ptrader->minfo.mbrid;
		}else {
			brid=itemtr.mbrid;
		}
		if (ptrader!=NULL) {
			oldstatus=ptrader->minfo.mstatus;
		}
	}else {
		brid=itemtr.mbrid;
	}

	if (itemtr.modifyflag & (1<<4)) {// status
		if (!(mptrifo->minfo.IsAdmin()&&(mptrifo->minfo.mbrid==0))) {
			if ((oldstatus&(1<<24))!=(itemtr.mstatus&(1<<24))){
				SendEventTextMsg("Can't change the DealCtrl flag!");
				return 0;
			}
		}
	}



	if (itemper.modifyflag !=0) {
		if(itemtr.modifyflag & (1<<3)) {//brid;
			itemper.mbrid = brid;
			itemper.modifyflag |=(1<<1);
		}
		itemper.UpdateData();
		retpkgper=(char*)malloc(1000);
		itemper.MakeInfoPkg(retpkgper,itemper.modifyflag);
		if (itemtr.mPerid !=itemper.mperid){
			itemtr.mPerid=itemper.mperid;
			itemtr.modifyflag|=1<<5;
		}else {
			itemtr.modifyflag&=~(1<<5);
		}
	}

	if (itemadd1.modifyflag !=0) {
		/*rstaddress rst;
		rst.UpdateData(&itemadd1);
		*/
		if(itemtr.modifyflag & (1<<3)) {//brid;
			itemadd1.mbrid = brid;
			itemadd1.modifyflag |=(1<<2);
		}
		itemadd1.UpdateData();
		retpkgadd1=(char*)malloc(1000);
		itemadd1.MakeInfoPkg(retpkgadd1,itemadd1.modifyflag);
		if (itemtr.mAdd1ID !=itemadd1.maddid){
			itemtr.mAdd1ID=itemadd1.maddid;
			itemtr.modifyflag|=1<<6;
		}else {
			itemtr.modifyflag &=~(1<<6);
		}
	}
	if (itemadd2.modifyflag !=0) {
/*			rstaddress rst;
		rst.UpdateData(&itemadd2);
*/
		if(itemtr.modifyflag & (1<<3)) {//brid;
			itemadd2.mbrid = itemtr.mbrid;
			itemadd2.modifyflag |=(1<<2);
		}
		itemadd2.UpdateData();
		retpkgadd2=(char*)malloc(1000);
		itemadd2.MakeInfoPkg(retpkgadd2,itemadd2.modifyflag);
		if (itemtr.mAdd2ID !=itemadd2.maddid){
			itemtr.mAdd2ID=itemadd2.maddid;
			itemtr.modifyflag|=1<<7;
		}else {
			itemtr.modifyflag &=~(1<<7);
		}
	}

	Trader *ptrader=NULL;
	if (itemtr.modifyflag!=0) {
		//rsttrader rtrader;//(&theOrderDB.mdb);
		//rtrader.UpdateData(&itemtr);
		if (itemtr.UpdateData()==TRUE){
			retpkgtr=(char*)malloc(1000);
			trid=itemtr.mtrid;
			itemtr.MakeInfoPkg(retpkgtr,itemtr.modifyflag);
			ptrader=Trader::FindTrader(itemtr.mtrid);
			if (ptrader!=NULL) {
				ptrader->minfo.CopyItem(&itemtr,itemtr.modifyflag);
//				tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"Trader modified!");
//				SendPkg(infomsg);
				nTraderChangeFlag=2;
			}else {
				ptrader =new Trader;
				ptrader->minfo.CopyItem(&itemtr,0xffffffff); 
				Trader::sMapTrid.Add(itemtr.mtrid,(long)ptrader); 
//				tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_NEW,0,"add Trader successed!");
//				SendPkg(infomsg);
				nTraderChangeFlag=1;
			}
		}else {
//			tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"Trader update failed!");
//			SendPkg(infomsg);
		}
	}else {
//		tok.setmsgpkg(infomsg,DP_TEXT,DT_BR_MODIFY,0,"Trader information modified!");
	}

	if (retpkgper!=NULL) {
		Trader::SPKGTrader(retpkgper,trid,brid,0);
		delete retpkgper;
	}
	if (retpkgadd1!=NULL) {
		Trader::SPKGTrader(retpkgadd1,trid,brid,0);
		delete retpkgadd1;
	}
	if (retpkgadd2!=NULL) {
		Trader::SPKGTrader(retpkgadd2,trid,brid,0);
		delete retpkgadd2;
	}
	if (retpkgtr!=NULL) {
		Trader::SPKGTrader(retpkgtr,trid,brid,0);
		delete retpkgtr;
	}

	if ((nTraderChangeFlag!=0)&&(ptrader)){
		theCfgFile.mlog.AddLogStrFmtTm(0,"%s %s Trader %d: %s %s %s",mptrifo->minfo.mtracno, (nTraderChangeFlag==1)?"Create":"Change",
			ptrader->minfo.mtrid, ptrader->minfo.mtracno, ptrader->minfo.IsAdmin()?"Admin":ptrader->minfo.IsManager()?"Manager":"Trader",
			ptrader->minfo.IsOpen()?"Open":"Closed");
			


	}
	return 0;

}

BOOL CClient::IsWatching(int brid, int watchflag)
{
	if (mptrifo==NULL) return FALSE;
	if (mptrifo->minfo.mbrid==0) {
		if (!mptrifo->IsTrader()){
			return TRUE;
		}
	}
	if (!mptrifo->IsTrader()) {
		if (mptrifo->minfo.mbrid==brid) return TRUE;
		for (int i=0;i<ADM_MAX_BRANCH;i++) {
			if (mptrifo->mBranchList[i]==0) break; 
			if (mptrifo->mBranchList[i]==brid) return TRUE;
		}
	}
/*	if (mptrifo->minfo.mbrid!=brid) {
		return FALSE;
	}else if (!mptrifo->IsTrader()) {
		return TRUE;
	}else {
		return FALSE;
	}
*/
	return FALSE;
}

int CClient::PkgSecIfoChange(char *pbuff)
{
	PkgToken tok;
	tok.InitToGet(pbuff,4);
	IssueIfo ifo;
	ifo.Clear();
	tok.getString(ifo.msecsym,sizeof(ifo.msecsym));
	ifo.mflag=tok.getlong();
	ifo.mshortsize=tok.getlong();
	
	IssueIfo *piIssue;
	piIssue=theCfgFile.mpIssueData->GetIssueIfo(ifo.msecsym);
	if (piIssue==NULL) {
		piIssue=theCfgFile.mpIssueData->GetNewBuff();
		ASSERT(piIssue!=NULL);
		memcpy(piIssue,&ifo,sizeof(IssueIfo));
		theCfgFile.mpIssueData->mSecMap.Add(piIssue->msecsym,(long)piIssue);
	}else {
		piIssue->mflag=ifo.mflag;
		piIssue->mshortsize=ifo.mshortsize;  
	}

 	srstSecIfo rst;
	try{
		rst.OpenToModify(piIssue->msecsym);
		if (rst.IsEOF()) {
			memcpy(&rst.mitem,piIssue,sizeof(IssueIfo));
			rst.AddNew();
		}else {
			if (strcmp(rst.mitem.msecsym,piIssue->msecsym)==0) {
				rst.mitem.mflag=piIssue->mflag;
				rst.mitem.mshortsize=piIssue->mshortsize;
				rst.Update(); 
			}else {
				theCfgFile.mlog.AddLogStrFmt("Error: update shortlist symbol mismatch. pkg:%s,db:%s",piIssue->msecsym, rst.mitem.msecsym);
			}
		}
		rst.Close();
	}catch(SRSTErr *perr) {
		char str[100];
		sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
		theCfgFile.mlog.AddLogStr(str);
		theCfgFile.mlog.AddLogStr(perr->merrmsg);
 		delete perr;
 	}

	char retbuff[BF_SZ];
	PkgToken tokret;
	tokret.InitToSet(retbuff); 
	tokret.setWORD(2003);//return package;
	tokret.setBYTE(1); //just one symbol
	if (piIssue!=NULL) {
		tokret.setVarStr(piIssue->msecsym);
		tokret.setlong(piIssue->mflag);
		tokret.setBYTE(piIssue->mIssueMarket);
//		int nlist==0;
//		if ((mptrifo)&&(mptrifo->minfo){
//			if (mptrifo->minfo.mstatus&(1<<31)) nlist=1;
//		}
		tokret.setlong(theCfgFile.GetShortAbleSize(piIssue->msecsym,piIssue->mIssueMarket,0));
	}
	SendPkg(tokret.finalizeSet());
	AsyncSelect(FD_WRITE); 
	return 0;	

}

int CClient::PkgHeartBeat(char *currbuff)
{

	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =7
	DWORD tickernu=tok.getDWORD();
	
	char  retbuff[2000];
	PkgToken rettok;
	rettok.InitToSet(retbuff); 
	rettok.setWORD(8); //the return package for login
	rettok.setDWORD(tickernu);

	rettok.finalizeSet();
	SendPkg(retbuff);
	AsyncSelect(FD_WRITE);
	return 0;

}

int CClient::GetWriteBuffSize()
{
	return mChainOutBuf.GetBuffCount();
}

CString CClient::GetClientIPString()
{
	CString st1;
	DWORD ip,ip1;;
	ip=mrmoteSockAddr.sin_addr.S_un.S_addr;
	ip1=mClientAddress;
	if (ip==ip1){
		st1.Format("%d.%d.%d.%d",ip&0xff,(ip>>8)&0xff,(ip>>16)&0xff,(ip>>24)&0xff);
	}else{
		st1.Format("%d.%d.%d.%d/%d.%d.%d.%d",ip&0xff,(ip>>8)&0xff,(ip>>16)&0xff,(ip>>24)&0xff,
			ip1&0xff,(ip1>>8)&0xff,(ip1>>16)&0xff,(ip1>>24)&0xff
		);
	};
	return st1;
}

BOOL CClient::SendOrderTknRply(BYTE stat, long origtkn, long orderid)
{
	if (m_Version<36) return TRUE; 

	char buff[100];
	PkgToken tok;
	tok.InitToSet(buff);
	tok.setWORD(1024);// token reply messge
	tok.setBYTE(stat);
	tok.setlong(origtkn);
	tok.setlong(orderid);
	SendPkg(tok.finalizeSet());
	return TRUE;

}

int CClient::onFinish()
{
	delete this;
	return 0;

}

int CClient::PkgUntradable(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =2005
	BYTE type;
	type=tok.getBYTE();
	BYTE num;
	char secsym[20];
	char retbuff[2048];
	int i;

	num=0;
	stritem *PIitem;
	PkgToken tretok;
	int listid;
	listid=(type>>4)&0xf;
	type&=0xf;

	if (listid>MAXSYMBLIST) return 0;

	switch (type){
	case 1: // request all pkg;
		theCfgFile.mpIssueData->mSymbLst[listid].LockMap();
		theCfgFile.mpIssueData->mSymbLst[listid].EnumFirst(&PIitem);  
		while(PIitem) {
			if (num==0){
				tretok.InitToSet(retbuff);
				tretok.setWORD(2005);
				tretok.setBYTE(((listid<<4)&0xf0)+2);
				tretok.setBYTE(0);
			}
			num++;
			tretok.setVarStr(PIitem->key);
			if (num>=250){
				*(BYTE*)(retbuff+5)=num;
				tretok.finalizeSet();
				SendPkg(retbuff);
				num=0;
			}
			theCfgFile.mpIssueData->mSymbLst[listid].EnumNext(&PIitem);  
		}
		if (num){
			*(BYTE*)(retbuff+5)=num;
			tretok.finalizeSet();
			SendPkg(retbuff);
			num=0;
		}
		theCfgFile.mpIssueData->mSymbLst[listid].UnlockMap();
		AsyncSelect(FD_WRITE);
		break;
	case 2: //add tradable
		num=tok.getBYTE();
		for (i=0;i<num;i++) {
			tok.getString(secsym,sizeof(secsym));
			theCfgFile.mpIssueData->AddUnTradable(listid,secsym);
		}
		break;
	case 3: // remove tradable;
		num=tok.getBYTE();
		for (i=0;i<num;i++) {
			tok.getString(secsym,sizeof(secsym));
			theCfgFile.mpIssueData->RMUnTradable(listid,secsym);
		}
		break;
	case 4: // clear all Tradable list;
		theCfgFile.mpIssueData->RMAllUnTradable(listid);   
		break;
	}
	return 0;
}

int CClient::PkgOptInfo(char *pbuff)
{
	PkgToken tok;
	tok.InitToGet(pbuff,4);
	BYTE num;
	num=tok.getBYTE();
	BYTE i;
	itemOptsym ios; 
	itemOptsym *pos;
	long pr;
	for (i=0;i<num;i++) {
		tok.getString(ios.optsymb,sizeof(ios.optsymb));
		ios.expday=tok.getlong(); 
		ios.strikepice=tok.getlong();
		tok.getString(ios.undsymb,sizeof(ios.optsymb));
		ios.pmundlyshr=tok.getlong();
		ios.m_date=tok.getlong();
		pr=tok.getlong();
		if (pr>0) {
			ios.mcurrundprice=pr;
		}
		pos=ios.FindAddInfo(&ios,TRUE,TRUE);
		if ((pos!=NULL)&&(pr>0)) {
			pos->mcurrundprice=pr;
		}
	}
	return 0;
}

int CClient::PkgDealNew(char *currbuff,int ver)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =1032
	BYTE action;
	action=tok.getBYTE();
	char buff[2048];
	itemDeal *pdeal;
	PkgToken newtok;

	if (mptrifo->IsAdmin()==FALSE) {
		char str[1000];
		sprintf(str,"Error: regular user %s try to change or view deal",mptrifo->minfo.mtracno); 
		theCfgFile.mlog.AddLogStr(str);
		SendEventTextMsg("Can't change or view deal!");
		return 0;
	}
	if ((mptrifo->minfo.mstatus&(1<<24))==0) {
		SendEventTextMsg("Can't change or view deal!");
		return 0;
	}


	itemDeal::sMapDealID.LockMap();

	itemDeal *pdeal1; 
	int dealid;
	switch(action) {
	case 1://deal modify
		pdeal=itemDeal::GetVoidDeal();
		pdeal->GetPkgInfoNew(&tok,ver);
		if ((pdeal->mbrid!=mptrifo->minfo.mbrid)&&
			(mptrifo->minfo.mbrid!=0)) {
			SendEventTextMsg("No authurity to change the deal!");
			break;
		}

		dealid=pdeal->mdealid; 
		if (dealid>=0){
			pdeal1=itemDeal::FindDeal(pdeal->mdealid);
			if (pdeal1!=NULL) {
				if ((pdeal1->mbrid!=mptrifo->minfo.mbrid)&&
					(mptrifo->minfo.mbrid!=0)) {
					SendEventTextMsg("No authurity to change the deal!");
					break;
				}
				pdeal1->CopyDeal(pdeal);
				pdeal1->SaveDealToDB();
				itemDeal::PutVoidDeal(pdeal); 
				pdeal=pdeal1;
			}else {
				itemDeal::sMapDealID.Add(dealid,(long)pdeal);
				pdeal->SaveDealToDB();

			}
		};
		theCfgFile.mlog.AddLogStrFmtTm(0,"Deal %d changed by %s",dealid,mptrifo->minfo.mtracno); 
		BOOL ret;
		newtok.InitToSet(buff,2048);
		if (pdeal!=NULL)  {
			ret=pdeal->SetPkgInfoNew(&newtok,ver);
			if (ret==TRUE) {
				SendPkg(buff);
			}
		}
		break;
	case 2:{//Deal Query;
		int brid;
		DWORD mmodifytm;
		brid=tok.getlong(); //don't use right now.
		mmodifytm=tok.getDWORD(); 
		intitem *pintitem;
		itemDeal::sMapDealID.EnumFirst(&pintitem);
		brid=mptrifo->minfo.mbrid;
		while (pintitem!=NULL) {
			pdeal=(itemDeal*)pintitem->val;
			if ((unsigned long)(pdeal->mm_date)>=mmodifytm){
				if ((brid==pdeal->mbrid)||(brid==0)||(pdeal->mbrid==0)){
					BOOL ret;
					newtok.InitToSet(buff,2048);
					ret=pdeal->SetPkgInfoNew(&newtok,ver);
					if (ret==TRUE) {
						SendPkg(buff);
					}
				}
				
			}
			itemDeal::sMapDealID.EnumNext(&pintitem); 
		}
		break;
		   } 
	case 3:// delete deal
		int dealid;
		dealid=tok.getlong();
		pdeal=itemDeal::FindDeal(dealid);
		if (pdeal!=NULL) {
			if ((pdeal->mbrid!=mptrifo->minfo.mbrid)&&
				(mptrifo->minfo.mbrid!=0)) {
				SendEventTextMsg("No authurity to delete the deal!");
				break;
			}
		
			itemPassthrufee *p1,*p2;
			
			p1=pdeal->ppathfee;
			while(p1!=NULL) {
				p2=p1;
				p1=p1->pnext;
				itemPassthrufee::sitemPassTFbuff.PutVoidBuff((char*)p2); 
			}

			itemDeal::sMapDealID.RemoveVal(dealid,(long)pdeal);
			itemDeal::sitemDealbuff.PutVoidBuff((char*)pdeal); 

			try{
				srstDeal rstdeal;
				rstdeal.OpenToUpdate(dealid); 
				if (!rstdeal.IsEOF()) {
					rstdeal.Delete();
				}
				rstdeal.Close();
			}catch (SRSTErr *perr) {
				char str[100];
				sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
				theCfgFile.mlog.AddLogStr(str);
				theCfgFile.mlog.AddLogStr(perr->merrmsg);
				delete perr;
			}

			try{
				srstPassthruFee rstpfee;
				rstpfee.OpenToUpdate(dealid);
				while(!rstpfee.IsEOF())
				{
					rstpfee.Delete();
					rstpfee.MoveNext();
				}
				rstpfee.Close();
			}catch (SRSTErr *perr) {
				char str[100];
				sprintf(str,"ERROR:ODBC  %s,%d",__FILE__,__LINE__);
				theCfgFile.mlog.AddLogStr(str);
				theCfgFile.mlog.AddLogStr(perr->merrmsg);
				delete perr;

			}
			theCfgFile.mlog.AddLogStrFmtTm(0,"Deal %d deleted by %s",dealid,mptrifo->minfo.mtracno); 

			newtok.InitToSet(buff,2048);
			newtok.setWORD(1032); // the package id is 1031 
			newtok.setBYTE(3);	// information package;
			newtok.setlong(dealid);
			newtok.finalizeSet(); 
			SendPkg(buff);

		}
		break;
	}

	itemDeal::sMapDealID.UnlockMap();

	AsyncSelect(FD_WRITE);
	return TRUE;
}


//nid==100, will popup a message box in the client frontend;

BOOL CClient::SendEventTextMsg(LPCSTR pstr,WORD nid,WORD nval)
{
	PkgToken tok; 
	char buff[2048];
	tok.InitToSet(buff);
	tok.setWORD(2001);
	tok.setWORD(nid);
	tok.setWORD(nval);
	tok.setVarStr(pstr);
	tok.finalizeSet();
	SendPkg(buff);
	AsyncSelect(FD_WRITE);
	return TRUE;
}

int CClient::OnLogonSuccess()
{
	// send the route information;
	char buff[BF_SZ];
	int item;
	PkgToken tok;
	tok.InitToSet(buff,0);
	item=0;
	tok.setWORD(2000);
	tok.setBYTE(1);
	stritem *PI;
	theCfgFile.mRoutes.EnumFirst(&PI); 
	while(PI) {
		OrderRouter *proute;
		proute=(OrderRouter *)PI->val;
		if (!proute->m_bHideRoute){
			tok.setVarStr(proute->mRouteName);
			tok.setWORD((WORD)proute->GetRouteStatus()); 
			item++;
		}
		theCfgFile.mRoutes.EnumNext(&PI); 
	}
	
	//check if the linked account is currency account, if it is, query the position and equity;
	AC_TR_relation *prel=mptrifo->pFistTrader; 
	while (prel) {
		if (prel->mstatus&(1<<0)){ //enable
			Account *item=Account::FindAcc(prel->maccid);
			if ((item!=NULL)&&(item->mitemifo.mstatus &(1<<24))) {//currency account;
				OrderRouter *proute= theCfgFile.GetRoute("GAIN");//GainCaptial
				if (proute) {
					Order_Account_Request req;
					req.QueryID=0;
					strncpy(req.UserID,item->mitemifo.mname,sizeof(req.UserID));
					req.QueryTime=time(NULL);
					req.QueryType=1+2;// both account equity and positions; 
					proute->SendPkg(req.ID,(LPCSTR)&req,sizeof(req));
					proute->DoSend();
				}
			}
		}
		prel=prel->pnexttr;
		
	}

		

	if (item) {
		tok.setVarStr("");
		tok.setWORD(0);
		tok.finalizeSet();
		SendPkg(buff);
		AsyncSelect(FD_WRITE);
	}

	return 0;
}

int CClient::PkgFuturesInfo(char *currbuff)
{
	PkgToken tok;
	tok.InitToGet(currbuff);
	tok.getWORD();   // pkgtype =2006
	BYTE type;
	type=tok.getBYTE();
	BYTE num;
	char secsym[20];
	char retbuff[2048];
//	int i;

	num=0;
//	stritem *PIitem;
	PkgToken tretok;

	switch (type){
	case 1: // 
		tok.getString(secsym,sizeof(secsym));
		FutureIfo *pifo;
		pifo=theCfgFile.mpIssueData->GetFutureInfo(secsym);
		if (pifo==NULL) {
			return 0;
		}
		tretok.InitToSet(retbuff);
		tretok.setWORD(2006);
		tretok.setBYTE(2);
		tretok.setVarStr(pifo->msecsym);
		tretok.setVarStr(pifo->mname);
		tretok.setVarStr(pifo->mExch);
		tretok.setDouble(pifo->m_tickersize);
		tretok.setDouble(pifo->m_multiplier);
		tretok.setDouble(pifo->m_DayInit);
		tretok.setDouble(pifo->m_InitReq);
		tretok.setDouble(pifo->m_Maintain);
		tretok.finalizeSet();
		SendPkg(retbuff);
		AsyncSelect(FD_WRITE);
		break;
	}
	return 0;
}

int CClient::PkgOrders_Replace(PkgToken *ptok)
{
	itemOrder item;
	DWORD flag;
	item.Clear(); 
	item.GetPkg(ptok,&flag);

	theCfgFile.mlog.AddLogStrFmtTm(0,"Try Replace Order %d",item.morderid);  

	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(item.morderid);
	OrderRouter *prouter;
	itemOrderActive oac;
	oac.Clear(); 
	if (porder==NULL){
		oac.mtrid=mtrid;
		oac.morderid=item.morderid;
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,"Order not exist");
		oac.SendActive((char*)this);
		return -1;
	}else {
		prouter=theCfgFile.FindRouter(porder);//->mroute);
		if (prouter==NULL){
			oac.FillInfo(porder);
			oac.SetType(LOAReplaceRej); 
			strcpy(oac.mnote,"Wrong Route");
			oac.SendActive((char*)this);
			return -1;
		}
	}

	if ((mptrifo->minfo.mstatus1&(1<<5))==0) {
			oac.FillInfo(porder);
			oac.SetType(LOAReplaceRej); 
			strcpy(oac.mnote,"No Permission");
			oac.SendActive((char*)this);
			return -1;
	}

	if (prouter->mCxlReplaceEnable==0) {
		oac.FillInfo(porder);
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,"Route Not Supported");
		oac.SendActive((char*)this);
		return -1;
	}

	if ((porder->mlvsqty<=0)) {
		oac.FillInfo(porder);
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,"qty error");
		oac.SendActive((char*)this);
		return -1;
	}
	if (prouter->IsEnabled()==FALSE) {
		oac.FillInfo(porder);
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,"Route Disabled");
		oac.SendActive((char*)this);
		return -1;
	}
	if (prouter->mNewOrderEnable==FALSE) {
		oac.FillInfo(porder);
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,"Route no Neworder");
		oac.SendActive((char*)this);
		return -1;
	}

	BOOL ret;
	char msgstr[50];
	memset(msgstr,0,sizeof(msgstr));
	if (item.mqty==0) { // if new order's mqty is 0, then we will send a cancel.
		ret=prouter->SendCancelOrder(porder,0,msgstr); 
	}else {
		ret=prouter->SendOrderReplace(porder,&item,msgstr);
	}
	if (ret==FALSE) {
		oac.FillInfo(&item);
		oac.SetType(LOAReplaceRej); 
		strcpy(oac.mnote,msgstr);
		oac.SendActive((char*)this);
		return -1;
	}

	oac.FillInfo(&item);
	oac.SetType(LOAReplacing); 
	strcpy(oac.mnote,msgstr);
	if (oac.mlogtime==0) oac.mlogtime=time(NULL); 
	theCfgFile.mlog.AddLogStrFmt("%%E,PR,%d,%d,%d,%g",item.morderid,oac.mlogtime,item.mqty,item.mprice); 
	oac.SendActive(porder->mbrid);

	return 0;
}


static int AddTrPkg(PkgToken *ptok,CClient *pclient) {
	
	if (ptok->GetPos()==2) {
		ptok->setWORD(12); 
		ptok->setlong(0);
	};
	ptok->setlong(pclient->mptrifo->minfo.mtrid);
	ptok->setlong(pclient->mlogintime);
	ptok->setDWORD(pclient->m_Version);
	ptok->setDWORD(pclient->GetPeerAddr());
	ptok->setDWORD(pclient->mClientAddress);
	ptok->IncrCounter(4);

	if (ptok->GetBuffLeftSize()<150) {
		return 1;
	}
	else return 0;
}

int CClient::PkgOnlineTraders(char *pbuff)
{
	if (!(mptrifo->minfo.IsAdmin())) {
		SendEventTextMsg("No permission to query Traders");
	}else {
		PkgToken tokin;
		char buffer[BF_SZ];
		PkgToken tok;
		tok.InitToSet(buffer,sizeof(buffer)-50);
		tokin.InitToGet(pbuff,4); 
		int brid;
		brid=tokin.getlong();
		intitem *pitem;
		Trader::sMapTrid.LockMap();
		Trader::mOnlineAdmin.LockMap();
		pitem=Trader::mOnlineAdmin.EnumFirst();
		while(pitem!=NULL) {
			CClient *pclient;
			pclient=(CClient*)pitem->val;
			if (pclient->mptrifo){
				if ((brid==0)||(brid==pclient->mptrifo->minfo.mbrid)) {
					if (AddTrPkg(&tok,pclient)){
						SendPkg(tok.finalizeSet()); 
						tok.SetPos(2);
					};
				}
			}
		
			pitem=Trader::mOnlineAdmin.EnumNext(pitem);
		}
		Trader::mOnlineAdmin.UnlockMap();

		Trader::mOnlineTrader.LockMap();
		pitem=Trader::mOnlineTrader.EnumFirst();
		while(pitem!=NULL) {
			CClient *pclient;
			pclient=(CClient*)pitem->val;
			if (pclient->mptrifo){
				if ((brid==0)||(brid==pclient->mptrifo->minfo.mbrid)) {
					if (AddTrPkg(&tok,pclient)) {;
						SendPkg(tok.finalizeSet());
						tok.SetPos(2);
					}
				}
			}
			pitem=Trader::mOnlineTrader.EnumNext(pitem);
		}
		Trader::mOnlineTrader.UnlockMap();
		Trader::sMapTrid.UnlockMap();
		if (tok.GetPos()>2) {
			SendPkg(tok.finalizeSet()); 
		}
		AsyncSelect(FD_WRITE);
	}
	return 0;
}

int CClient::PkgAccCtrl(char *pbuff)
{
	PkgToken tok;
	tok.InitToGet(pbuff);
	tok.getWORD();   // pkgtype =1040 the login pkage
	BYTE ntype=tok.getBYTE();
	int accid;
	char retpkg[3000];
	PkgToken tokret;
	switch(ntype) {
	case 1: //query
		accid=tok.getlong();
		Account *pacc;
		pacc=Account::FindAcc(accid);
		if (pacc==NULL) {
			return 0;
		}
		pacc->mitemCtrl.MakePkg(retpkg);
		SendPkg(retpkg);

		pacc->Lock();
		itemAccCtrl_Symb::MakePkg_Sym(accid, pacc->mpitemCtrl_Symb,retpkg);
		pacc->UnLock();
		SendPkg(retpkg);

		pacc->Lock();
		itemAccCtrl_Route::MakePkg_Route(accid, pacc->mpitemCtrl_Route,retpkg);
		pacc->UnLock();
		SendPkg(retpkg);

		AsyncSelect(FD_WRITE); 
		break;
	case 2: //update;
		if (mptrifo->IsAdmin()==FALSE) {
			char str[1000];
			sprintf(str,"Error: regular user %s try to change Account Settings.",mptrifo->minfo.mtracno); 
			theCfgFile.mlog.AddLogStr(str);
			SendEventTextMsg("No permission to change Account settings");
			return 0;
		}
		
		{
			accid=tok.getlong();
			double maxloss;
			Account *pacc;
			pacc=Account::FindAcc(accid);
			if (pacc==NULL) {
				return 0;
			}
			if (pacc->mitemCtrl.maccid!=accid) {
				pacc->mitemCtrl.maccid=accid;
			}
			pacc->mitemCtrl.mflag=tok.getlong();
			pacc->mitemCtrl.mflagex=tok.getlong();
			maxloss=tok.getDouble();
			pacc->mitemCtrl.mMaxLoss =maxloss;
			if (tok.GetLeftLen()>0) {
				pacc->mitemCtrl.maxshare=tok.getlong();
				pacc->mitemCtrl.maxtotalshare=tok.getlong();
				pacc->mitemCtrl.maxsymbshare=tok.getlong();
			}
			
			if (tok.GetLeftLen()>0) {
				pacc->mitemCtrl.maxsymbs=tok.getlong();
				pacc->mitemCtrl.maxUnrealizedLoss=tok.getDouble();
				pacc->mitemCtrl.mstarttradetime=tok.getlong();
				pacc->mitemCtrl.mstoptradetime=tok.getlong();
			}

			srstAccCtrl rstaccctrl;
			rstaccctrl.OpenToUpdate(accid);
			memcpy(&rstaccctrl.mitem,&pacc->mitemCtrl ,sizeof(itemAccCtrl));
			rstaccctrl.mitem.maccid=accid;
			if (rstaccctrl.IsEOF()) { 
				rstaccctrl.AddNew();
			}else {
				rstaccctrl.Update();
			}
			rstaccctrl.Close();
			char retpkg[3000];
			pacc->mitemCtrl.MakePkg(retpkg);
			Trader::SPKGAcc(retpkg,accid,pacc->mitemifo.mbrid,0);  


			return TRUE;

		}


		break;

	case 3:
		if (mptrifo->IsAdmin()==FALSE) {
			char str[1000];
			sprintf(str,"Error: regular user %s try to change Account Settings.",mptrifo->minfo.mtracno); 
			theCfgFile.mlog.AddLogStr(str);
			SendEventTextMsg("No permission to change Account settings");
			return 0;
		}
		
		{
			accid=tok.getlong();

			Account *pacc;
			pacc=Account::FindAcc(accid);
			if (pacc==NULL) {
				return 0;
			}
			pacc->Lock();
			itemAccCtrl_Symb *psym=pacc->mpitemCtrl_Symb; 	
			while (psym) {
				itemAccCtrl_Symb *p1=psym;
				psym=psym->pnext;
				delete p1;
			}
			pacc->mpitemCtrl_Symb=NULL;
			
			while(tok.GetLeftLen()>0){
				itemAccCtrl_Symb st_symbol;
				tok.getString(st_symbol.msymbol,sizeof(st_symbol.msymbol));
				st_symbol.maxpos=tok.getlong();
				st_symbol.maxloss=tok.getDouble();
				st_symbol.maxorder=tok.getlong();
				itemAccCtrl_Symb *p1=new itemAccCtrl_Symb;
				memcpy(p1,&st_symbol,sizeof(st_symbol));
				p1->pnext=pacc->mpitemCtrl_Symb;
				pacc->mpitemCtrl_Symb=p1;
			}
			char sqlstr[500];
			sprintf(sqlstr,"delete mAccCtrlEx_Symb where accid=%d",accid);
			theOrderDB.mdb.ExecuteSQL(sqlstr);
			
			psym=pacc->mpitemCtrl_Symb; 	
			while (psym) {
				sprintf(sqlstr,"insert into mAccCtrlEx_Symb (accid,symbol,maxpos,maxloss,maxorder)"
					" values (%d,'%s',%d,%f,%d) ",
					accid,psym->msymbol,psym->maxpos,psym->maxloss,psym->maxorder);   
				theOrderDB.mdb.ExecuteSQL(sqlstr);
				psym=psym->pnext;
			}
			char retpkg[3000];
			itemAccCtrl_Symb::MakePkg_Sym(accid, pacc->mpitemCtrl_Symb,retpkg);
			pacc->UnLock();
			Trader::SPKGAcc(retpkg,accid,pacc->mitemifo.mbrid,0);  
		}
		
		break;
	case 5:
		if (mptrifo->IsAdmin()==FALSE) {
			char str[1000];
			sprintf(str,"Error: regular user %s try to change Account Settings.",mptrifo->minfo.mtracno); 
			theCfgFile.mlog.AddLogStr(str);
			SendEventTextMsg("No permission to change Account settings");
			return 0;
		}
		
		{
			accid=tok.getlong();

			Account *pacc;
			pacc=Account::FindAcc(accid);
			if (pacc==NULL) {
				return 0;
			}
			pacc->Lock();
			itemAccCtrl_Route *proute=pacc->mpitemCtrl_Route; 	
			while (proute) {
				itemAccCtrl_Route *p1=proute;
				proute=proute->pnext;
				delete p1;
			}
			pacc->mpitemCtrl_Route=NULL;
			
			while(tok.GetLeftLen()>0){
				itemAccCtrl_Route st_route;
				tok.getString(st_route.mroute ,sizeof(st_route.mroute ));
				st_route.mflag =tok.getlong();
				itemAccCtrl_Route *p1=new itemAccCtrl_Route;
				memcpy(p1,&st_route,sizeof(st_route));
				p1->pnext=pacc->mpitemCtrl_Route;
				pacc->mpitemCtrl_Route =p1;
			}
			char sqlstr[500];
			sprintf(sqlstr,"delete mAccCtrlEx_Route where accid=%d",accid);
			theOrderDB.mdb.ExecuteSQL(sqlstr);
			
			proute=pacc->mpitemCtrl_Route; 	
			while (proute) {
				sprintf(sqlstr,"insert into mAccCtrlEx_Route (accid,mroute,mflag)"
					" values (%d,'%s',%d) ",
					accid,proute->mroute,proute->mflag);   
				theOrderDB.mdb.ExecuteSQL(sqlstr);
				proute=proute->pnext;
			}
			char retpkg[3000];
			itemAccCtrl_Route::MakePkg_Route(accid, pacc->mpitemCtrl_Route,retpkg);
			pacc->UnLock();
			Trader::SPKGAcc(retpkg,accid,pacc->mitemifo.mbrid,0);  
		}
		
		break;

	case 11://query trader branlist;
		{
			int trid;
			trid=tok.getlong();
			Trader *ptr=Trader::FindTrader(trid);
			if (ptr!=NULL) {
				char retbuff[1000];
				PkgToken tokret;
				tokret.InitToSet(retbuff);
				tokret.setWORD(1040);
				tokret.setBYTE(12);// trader branch list update;
				tokret.setlong(trid);
				for (int i=0;i<ADM_MAX_BRANCH;i++) {
					if (ptr->mBranchList[i]==0) break;
					tokret.setlong(ptr->mBranchList[i]);
					tokret.setlong(ptr->mBranchPerm[i]); 
				}
				tokret.setlong(0);
				SendPkg(tokret.finalizeSet());
				AsyncSelect(FD_WRITE); 

			}
		}
		break;
	case 12://update the admin branch list
		if ((mptrifo->IsAdmin()==FALSE)||(mptrifo->minfo.mbrid!=0)) {
			char str[1000];
			sprintf(str,"Error: regular user %s try to change Trader Branch List.",mptrifo->minfo.mtracno); 
			theCfgFile.mlog.AddLogStr(str);
			SendEventTextMsg("No permission to change Trader settings");
			return 0;
		}

		{
			int trid;
			trid=tok.getlong();
			Trader *ptr=Trader::FindTrader(trid);
			if (ptr!=NULL) {
				char str[1000];
				int len=0;

				len+=sprintf(str+len,"Admin %s change trader %s for exchange ",mptrifo->minfo.mtracno,ptr->minfo.mtracno);

				memset(&ptr->mBranchList,0,sizeof(ptr->mBranchList)); 
				for (int i=0;i<ADM_MAX_BRANCH;i++){
					
					ptr->mBranchList[i]=tok.getlong();
					if(ptr->mBranchList[i]==0) break;
					itemBranch *pbr=theCfgFile.mpItemPool->GetBranch(ptr->mBranchList[i]);
					if (pbr) len+=sprintf(str+len, " %s",pbr->mshort);
					else     len+=sprintf(str+len, " %d",ptr->mBranchList[i]);
					ptr->mBranchPerm[i]=tok.getlong(); 
				}
				theCfgFile.mlog.AddLogStrFmtTm(0,str);  
				ptr->UpdateExBranchLst();
			}
		}

	}
	return 0;
}

int CClient::PkgLOARequest(char *pbuff)
{
	
	CStdioFile f1;
	char pathname[1000];
	CTime t1=CTime::GetCurrentTime();
	sprintf(pathname,"%sOSVLOG%04d%02d%02d.txt",theCfgFile.mPathLog,
		t1.GetYear(),t1.GetMonth(),t1.GetDay());
	if (f1.Open(pathname,CFile::modeRead|CFile::shareDenyNone)==FALSE) return TRUE;

	char pkg[BF_SZ];
	PkgToken tok;
	tok.InitToSet(pkg);
	tok.setWORD(1050);
	int pos=tok.GetPos(); 
	tok.setBYTE(1);
	tok.setBYTE(1);
	BYTE itemnu=0;
	char st[1000];
	while(f1.ReadString(st,sizeof(st))) {
		if (st[0]=='%') {
			if (st[1]=='a') {
				itemOrderActive ac;
				ac.LoadStr(st);
				ac.SetPkg(&tok);
				itemnu++;
				if ((tok.GetBuffLeftSize()<3*sizeof(itemOrderActive))||(itemnu>250)) {
					int newpos=tok.SetPos(pos+1);
					tok.setBYTE(itemnu); 
					tok.SetPos(newpos);
					SendPkg(tok.finalizeSet());
					itemnu=0;
					tok.SetPos(pos);
					tok.setBYTE(2);
					tok.setBYTE(0);
				}
			}
		}
	}
	f1.Close();
	
	int newpos=tok.SetPos(pos);
	tok.setBYTE(3);
	tok.setBYTE(itemnu); 
	tok.SetPos(newpos);
	SendPkg(tok.finalizeSet());
	AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgOMCmd(char *currbuff)
{
	PkgToken tok;
	char str[2000];
	if (mptrifo==NULL) return 0;
	tok.InitToGet(currbuff,4);
	tok.getString(str,sizeof(str)); 
	LPCSTR userlogon=mptrifo->minfo.mtracno;
	theCfgFile.mlog.AddLogStrFmtTm(0,"Got OMCmd from:%s:%s",userlogon,str);

	if (theCfgFile.mScriptLogon.FindItem(userlogon)==NULL) {
		char *pmsg="NoPermission to run OMCmd";
		theCfgFile.mlog.AddLogStrFmtTm(0,pmsg);
		SendEventTextMsg(pmsg,100,0);
	}else {
		COrdScript scpt;
		scpt.RunCmds(str);
		if (strlen(scpt.msg)>0) SendEventTextMsg(scpt.msg,100,0);
	}
	return 0;
}

int CClient::PkgExchAgree(char * pbuff)
{
	PkgToken tok;
	char str[2000];
	tok.InitToGet(pbuff,4);
	int uid=tok.getlong();
	int type=tok.getlong();
	int event=tok.getlong();
	char notestr[300];
	tok.getString(notestr,sizeof(notestr));
	char timebuffer[50];

	sprintf(str,"INSERT INTO mExchAgree (trid,type,event,note,logdate) values(%d,%d,%d,'%s','%s') ",
		uid,type,event,notestr,theOrderDB.FormatSQLTime(0,timebuffer));

	theOrderDB.mdb.ExecuteSQL(str);
	return 0;
}	

int CClient::PkgRouteBoxLst(char *pbuff)
{

	char retpkg[BF_SZ];
	PkgToken tok;
	int i;
	BOOL bSet=FALSE;
	char exch[10];
	for (i=0;i<theCfgFile.mrouteboxlstidx;i++){
		tok.InitToSet(retpkg); 
		tok.setWORD(601);
		exch[0]=theCfgFile.mrouteboxlst[i][0];
		exch[1]=0;
		tok.setVarStr(exch);
		tok.setVarStr(theCfgFile.mrouteboxlst[i]+1);
		SendPkg(tok.finalizeSet());
		bSet=TRUE;
	}
	if (bSet) AsyncSelect(FD_WRITE);
	return 0;
}

int CClient::PkgManualOrder(char *pbuff, int len)
{
	PkgToken tok;
	tok.InitToGet(pbuff);
	tok.getWORD();   // pkgtype =1020 the Order information package
	BYTE action;
	action=tok.getBYTE();
	
	if (mptrifo==NULL) {
		theCfgFile.mlog.AddLogStr("Error!!!!, may crash here");
		return 0;
	}
	Account *pacc;

	switch(action) {
	case 1://Order modify
		break;
	case 2://order Query;
		PkgManualOrders_Query(&tok); 
		break;
			
	case 3://New Orders;
		{
			itemManualOrder item;
			DWORD flag;
			memset(&item,0,sizeof(itemManualOrder)); 
			item.GetPkg(&tok,&flag);
//			BOOL bret;
			if (mptrifo==NULL) {//fund crash at next code. one more check!!!!, user the critical section!
				theCfgFile.mlog.AddLogStr("Error!!!!, may crash here");
				return 0;
			}

			pacc=Account::FindAcc(item.accid);
			if (pacc==NULL) return 0;

			item.id=theCfgFile.GetNewOrderID();//(DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrOrderID);  
			itemManualOrder *pord=(itemManualOrder *)itemManualOrder::sOrderBuffer.GetVoidBuff();  
			memcpy(pord,&item,sizeof(itemManualOrder));
			pord->c_time=time(NULL);
			pord->m_time=pord->c_time;
			itemManualOrder::smapOrderid.Add(item.id,(long)pord);
			char retbuf[2000];
			PkgToken tok;
			tok.InitToSet(retbuf);
			tok.setWORD(1029);
			tok.setBYTE(1);
			pord->SetPkg(&tok,flag);
			pord->Save();
			return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
		break;
	case 4://cancel Order
		{
			int orderid;
			orderid=tok.getlong();
			itemManualOrder *pord=itemManualOrder::Find(orderid);
			if (pord==NULL) return 0;
			
			pacc=Account::FindAcc(pord->accid);
			if (pacc==NULL) return 0;

			if (!pord->IsAccepted()) { //NOT ACCEPTED, JUST CANCEL IT.
				pord->status|=(1<<2);
				pord->status&=~(1<<0);
				pord->m_time=time(NULL);
				char retbuf[2000];
				PkgToken tok;
				tok.InitToSet(retbuf);
				tok.setWORD(1029);
				tok.setBYTE(1);
				pord->SetPkg(&tok,0xffff);
				pord->Save();
				return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			}else {//accepted, post a cancel replace order;
				itemManualOrder *pordcxl=(itemManualOrder *)itemManualOrder::sOrderBuffer.GetVoidBuff();  
				memcpy(pordcxl,pord,sizeof(itemManualOrder));
				pordcxl->ntype=3;
				pordcxl->origoid=pord->id;
				pordcxl->id=theCfgFile.GetNewOrderID();//(DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrOrderID); 
				pordcxl->status&=~(0x3f);
				pordcxl->status|=1;
				pordcxl->c_time=time(NULL);
				pordcxl->m_time=pordcxl->c_time;
				itemManualOrder::smapOrderid.Add(pordcxl->id,(long)pordcxl);
				char retbuf[2000];
				PkgToken tok;
				tok.InitToSet(retbuf);
				tok.setWORD(1029);
				tok.setBYTE(1);
				pordcxl->SetPkg(&tok,0xffff);
				pordcxl->Save();
				return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			}
		}
		break;
	case 5: //replace order;
//		PkgOrders_Replace(&tok);
		break;
	case 9://Close Order
//		PkgOrders_Close(&tok); 
		break;
	case 10://accept manual order;
		{
			int orderid;
			orderid=tok.getlong();
			itemManualOrder *pord=itemManualOrder::Find(orderid);
			if (pord==NULL) return 0;
			
			pacc=Account::FindAcc(pord->accid);
			if (pacc==NULL) return 0;

			switch(pord->ntype) {
			case enMOOrder: //order;
				if (!pord->IsAccepted()) { 
					pord->status|=(1<<4);
					pord->acceptby=mtrid;// the one who did the accept; 
					pord->left_share=pord->share;
					pord->m_time=time(NULL);
					char retbuf[2000];
					PkgToken tok;
					tok.InitToSet(retbuf);
					tok.setWORD(1029);
					tok.setBYTE(1);
					pord->SetPkg(&tok,0xffff);
					pord->Save();
					return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
				}
			case enMOCancel:
				if (!pord->IsAccepted()) { 
					pord->status|=(1<<4);
					pord->status&=~(1<<0); //cancel request been accept means closed.
					pord->acceptby=mtrid;// the one who did the accept; 
					pord->m_time=time(NULL);
					char retbuf[2000];
					PkgToken tok;
					tok.InitToSet(retbuf);
					tok.setWORD(1029);
					tok.setBYTE(1);
					pord->SetPkg(&tok,0xffff);
					pord->Save();
					Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
					
					itemManualOrder *porigord=itemManualOrder::Find(pord->origoid);
					if (porigord) {//canceled;
						porigord->status|=(1<<2);
						porigord->status&=~(1<<0);
						pord->m_time=time(NULL);
						char retbuf[2000];
						PkgToken tok;
						tok.InitToSet(retbuf);
						tok.setWORD(1029);
						tok.setBYTE(1);
						porigord->SetPkg(&tok,0xffff);
						porigord->Save();
						return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  

					}

				}
			}
		}

		break;
	case 11:// Administrator  reject the order;
		{
			int orderid;
			orderid=tok.getlong();
			itemManualOrder *pord=itemManualOrder::Find(orderid);
			if (pord==NULL) return 0;
			if (pord->ntype==enMOExecution)	break;

			pacc=Account::FindAcc(pord->accid);
			if (pacc==NULL) return 0;

			if (!pord->IsAccepted()) { 
				pord->status|=(1<<3);
				pord->status&=~(1<<0);
				pord->acceptby=mtrid;// the one who did the accept; 
				pord->m_time=time(NULL);
				char retbuf[2000];
				PkgToken tok;
				tok.InitToSet(retbuf);
				tok.setWORD(1029);
				tok.setBYTE(1);
				pord->SetPkg(&tok,0xffff);
				pord->Save();
				return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			}
		}
		break;
	case 12: //Administrator Cancel Order;
		{
			int orderid;
			orderid=tok.getlong();
			itemManualOrder *pord=itemManualOrder::Find(orderid);
			if (pord==NULL) return 0;
			
			pacc=Account::FindAcc(pord->accid);
			if (pacc==NULL) return 0;

			if ((pord->IsAccepted()) &&( pord->ntype ==enMOOrder)) { 
				pord->status|=(1<<2);
				pord->status&=~(1<<0);
				pord->acceptby=mtrid;// the one who did the accept; 
				pord->m_time=time(NULL);
				char retbuf[2000];
				PkgToken tok;
				tok.InitToSet(retbuf);
				tok.setWORD(1029);
				tok.setBYTE(1);
				pord->SetPkg(&tok,0xffff);
				pord->Save();
				return Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			}
		}
		break;
	case 13:// Administrator Add Executon;
		{
			int orderid;
			orderid=tok.getlong();
			itemManualOrder *pord=itemManualOrder::Find(orderid);
			if (pord==NULL) return 0;
			
			if (pord->ntype!=enMOOrder) return 0;
			
			pacc=Account::FindAcc(pord->accid);
			if (pacc==NULL) return 0;
			int share;
			double price;
			share=tok.getlong();
			price=tok.getDouble(); 
			
			pord->status|=3;
			pord->left_share-=share;
			pord->m_time=time(NULL);
			char retbuf[2000];
			PkgToken tok;
			tok.InitToSet(retbuf);
			tok.setWORD(1029);
			tok.setBYTE(1);
			pord->SetPkg(&tok,0xffff);
			pord->Save();
			Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
			

			itemManualOrder *pexe=(itemManualOrder *)itemManualOrder::sOrderBuffer.GetVoidBuff();  
			memcpy(pexe,pord,sizeof(itemManualOrder));
			pexe->ntype=enMOExecution;
			pexe->origoid=pord->id;
			pexe->id=theCfgFile.GetNewOrderID();//(DWORD)InterlockedIncrement((long *)&theCfgFile.mCurrOrderID); 
			pexe->status&=~(0x3f);
			pexe->status|=3;
			pexe->share=share;
			pexe->left_share=share;
			pexe->c_time=time(NULL);
			pexe->m_time=pexe->c_time;
			sprintf(pexe->price,"%g",price); 
			itemManualOrder::smapOrderid.Add(pexe->id,(long)pexe);
			pexe->Save();


			tok.InitToSet(retbuf);
			tok.setWORD(1029);
			tok.setBYTE(1);
			pexe->SetPkg(&tok,0xffff);
			Trader::SPKGAcc(tok.finalizeSet(),pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
		break;

	}
	return 0;
}

int CClient::PkgManualOrders_Query(PkgToken *ptok)
{
	DWORD modifydate;
	BYTE  queryttype;
	queryttype=ptok->getBYTE();
	modifydate=ptok->getlong(); 
	long accid;
	long brid;
	long trid;
	char retbuff[100];

	itemManualOrder *pord;
	intitem *PI=NULL;
	switch(queryttype) {
	case 1://listed Account and modify date
		while((accid=ptok->getlong())!=0){
			itemManualOrder::smapOrderid.LockMap();
			while(itemManualOrder::smapOrderid.Enum(PI)) {
				pord=(itemManualOrder*)PI->val;
				if (pord->accid==accid) {
					pord->SetInfoPkg(retbuff,0xffffff);
					SendPkg(retbuff);
				}
			}
			itemManualOrder::smapOrderid.UnlockMap();
		}
		break;
	case 2://listed branch and modify date
		brid=ptok->getlong();
		itemManualOrder::smapOrderid.LockMap();
		while(itemManualOrder::smapOrderid.Enum(PI)) {
			pord=(itemManualOrder*)PI->val;
			Account *pac=Account::FindAcc(pord->id);
			if (pac) {
				if ((DWORD)pord->m_time>(DWORD)modifydate){
					if ((pac->mitemifo.mbrid==brid)||(brid==0)) { // this is queryed account
						pord->SetInfoPkg(retbuff,0xffffff);
						SendPkg(retbuff);
					}
				}
			}
		}
		itemManualOrder::smapOrderid.UnlockMap();
		break;
	case 3:// the trader
		long accbuff[100];
		int sz;
		sz=100;
		trid=ptok->getlong();
		Trader::FindTraderAccount(trid,accbuff,&sz);
		int i;
		for (i=0;i<sz;i++) {
			accid=accbuff[i];
			itemManualOrder::smapOrderid.LockMap();
			while(itemManualOrder::smapOrderid.Enum(PI)) {
				pord=(itemManualOrder*)PI->val;
				if ((DWORD)pord->m_time>(DWORD)modifydate){
					pord->SetInfoPkg(retbuff,0xffffff);
					SendPkg(retbuff);
				}
			}
			itemManualOrder::smapOrderid.UnlockMap();
		}
		break;

	}
	DoSend();
	return 0;

}
