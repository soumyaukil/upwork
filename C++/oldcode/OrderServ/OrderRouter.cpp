// OrderRouter.cpp: implementation of the OrderRouter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "orderserv.h"
#include "OrderRouter.h"
#include "OrderConfig.h"
#include "Account.h"
#include "Functions\Perf.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "OrderRouteCfg.h"
extern OrderConfig theCfgFile ;

#include "Functions\strfuncs.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OrderRouter::OrderRouter()
{
	mInBuf=NULL;
	mOutBuf=NULL;
	mbShutdowAfterSend=FALSE;

	mServerIP =0;
	mServerPort=0;
	memset(mUserID,0,sizeof(mUserID));
	memset(mUserPWD,0,sizeof(mUserPWD));
	memset(mRouteName,0,sizeof(mRouteName));
	mServerEnable=FALSE;
	mServerOrderTypes=0;
	mlastbeat=0;
	mbTestRoute=FALSE;
	mInCloseCount=0;


	mtmconnect=0;		// the time to start connect;
	mtmdisconnect=0;	// the time to disconnect;
	mlsttmconnect=0;  // the time for last time to connect;
	mautoconnect=0;   // automatic connect to server if disconnect;
	mconnectfailnum=0;
	mAuthLst.init(1000);
	mdeftradable=TRUE; 
	mCxlReplaceEnable=TRUE;
	mServerVersion=0;
	mlastmessagetime=0;
	mNewOrderEnable=TRUE;
	mDisableExch=0;
	m_bHideRoute=FALSE;
	m_bDisableCustomerOrder=FALSE;
}

OrderRouter::~OrderRouter()
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

int OrderRouter::onReading(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject!=NULL) {
		ASSERT(mInBuf!=NULL);
		ASSERT(mInBuf==BuffObject); 
		if (nErrno!=0) {
			LPVOID lpMsgBuf;
			FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				nErrno,
				0, // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			);
			theCfgFile.mlog.AddLogStrFmt("Socket Error:%s",lpMsgBuf); 
			LocalFree( lpMsgBuf );

		}
		if (bflen==0){ //Close
	//		mstate = mstate & (~enReading);
			DoClose(TRUE);
			return 0;
		}
		mInBuf->bufend+=(WORD)bflen;
		if (mInBuf->bufend>BF_SZ){
			theCfgFile.mlog.AddLogStr("OrderRouter:Pkg Len Error!");
			DoClose(TRUE);
			return 0;
		}; 
		char * currBuff;
		while((currBuff=mInBuf->GetPkgBuff())!=NULL) {
			ProcessPkg(currBuff);
		}
		mInBuf->TrimPkgBuff(); 
	}		
	if (mSocket==INVALID_SOCKET) return 0;
	
	PutPendingRead();
	return 0;
}

int OrderRouter::onClose()
{
	char str[100];
	mServerEnable=FALSE;
	if (InterlockedIncrement(&mInCloseCount)==1) {
		sprintf(str,"Lost Connection to %s !",mRouteName);
		theCfgFile.mlog.AddLogStr(str);  
		UpdateStatus();
	}
	InterlockedDecrement(&mInCloseCount); 

//	CWnd *pmainwnd=AfxGetMainWnd(); 

//	if (pmainwnd!=NULL) {
//		pmainwnd->SendMessage(WM_USER+101,0,(LPARAM)mRouteName);
//	}
	return 0;
}

int OrderRouter::SendPkg(char *pbuf)
{
/*	dabuff * pdapkg;
	pdapkg=dabuff::GetVoidDaBuff();
	pdapkg->AddPkgBuff(pbuf); 
	mChainOutBuf.PutDabuff(pdapkg); 
*/
	mChainOutBuf.AddPkgBuff(pbuf);
	return 0;
}

int OrderRouter::PutPendingRead()
{
	int ret;
	if (mInBuf==NULL) {
		mInBuf=dabuff::GetVoidDaBuff(); 
	};
	if(mSocket==INVALID_SOCKET) return 0;
	ret=Recv(mInBuf,&(mInBuf->DataBuff[mInBuf->bufend]) ,BF_SZ - mInBuf->bufend); 
	if(ret==-1) {	//read error, close the 
		DoClose(TRUE);
	}
	return 0;

}

int OrderRouter::PutPendingWrite()
{
	if (mSocket==INVALID_SOCKET) return -1;

	if(mOutBuf==NULL) mOutBuf=mChainOutBuf.GetDabuff();
	if (mOutBuf==NULL) {
		if(mbShutdowAfterSend==TRUE) {
			DoClose(FALSE);
		}
		wov.mOVstate =stVoid;
		return 0;
	}

	/*
	WORD bflen;
	int firstwaittingbflen;
	dabuff * ptmpdabuff;
	bflen=mOutBuf->bufend -mOutBuf->bufbegin ;
	ASSERT(bflen<BF_SZ);
	ASSERT(bflen!=0);
	while (1) {
		firstwaittingbflen=mChainOutBuf.PeekFirstPkgLen();
		if ((firstwaittingbflen>=0 )&&(firstwaittingbflen+bflen <BF_SZ)) {
			ptmpdabuff=mChainOutBuf.GetDabuff();
			ASSERT(ptmpdabuff);
			if (mOutBuf->bufbegin!=0) {
				memmove(mOutBuf->DataBuff,mOutBuf->DataBuff+mOutBuf->bufbegin,bflen);
				mOutBuf ->bufbegin =0;
				mOutBuf->bufend =bflen;
			};
			memcpy (mOutBuf->DataBuff + mOutBuf->bufend,ptmpdabuff->DataBuff + ptmpdabuff->bufbegin, firstwaittingbflen);
			bflen=mOutBuf->bufend=(WORD)firstwaittingbflen+ bflen;
			dabuff::PutVoidDaBuff(ptmpdabuff); 
		} else {
			break;
		}
	}
	*/
	mChainOutBuf.FillPkg(mOutBuf); 
	int ret;
	ret=Send(mOutBuf, mOutBuf->GetBuffAddr(),mOutBuf->GetPkgLen());
	if (ret==-1){
		DoClose(TRUE);
	};
	return 0;
}

int OrderRouter::ProcessPkg(char *pbuff)
{
	DWORD pkgtype;
	int burrlen;
	burrlen=*(WORD*)pbuff;
	ASSERT(burrlen<BF_SZ);
	pkgtype=*(WORD*)(pbuff+2);
	mlastmessagetime=GetTickCount(); 
	switch (pkgtype){
	case Order_Login_Reply::ID:
		PkgLoginReply(pbuff);
		break;
	case Order_Text_info::ID:
		PkgTextInfo(pbuff);
		break;
	case Order_Status_Info::ID:
		PkgStatusIfo(pbuff);
		break;
	case Order_Process_action::ID:
		Perf_Start(60); 
		Perf_SetName(60,"Action ");
		PkgOrderAction(pbuff);
		Perf_Stop(60); 
		break;
	case Order_Execute::ID:
		Perf_Start(50); 
		Perf_SetName(50,"Execution");
		PkgOrderExecute(pbuff);
		Perf_Stop(50);
		break;
	case Order_Execute_Cancel::ID:
		PkgOrderExecute_Cancel(pbuff);
		break;
	case Order_Broken::ID:
		theCfgFile.mlog.AddLogStr("Get Order Broken Message!, this function is not ready yet!");
		break;
	case Order_HeartBeat::ID:
		PkgHeartBeat(pbuff);
		break;
	case Order_HeartBeatRet::ID:
		break;
	case Order_SMT_NewOrder::ID:
		Pkg_SMT_NewOrder(pbuff);
		break;
	case Order_Replace_Action::ID:
		PkgReplaceAction(pbuff);
		break;
	case Order_SMT_TrailerPriceUpdate::ID:
		PkgTrailerPriceUpdate(pbuff);
		break;
	case Order_DirectUDP_Order::ID:
		PkgDirectOrder(pbuff);
		break;
	case Order_Account_Balance::ID:
		PkgAccountBalance(pbuff);
		break;
	case Order_Account_Position::ID:
		PkgAccountPosition(pbuff);
		break;
	default:
		CString st;
		st.Format("Get Unknow Messages!  len %d ,Type %d ",burrlen,pkgtype );
		theCfgFile.mlog.AddLogStr(st);
		break;
	}
	return 0;
}

int OrderRouter::onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject!=NULL){ //buffobject is null is put pending write
		ASSERT(mOutBuf!=NULL);
		ASSERT(mOutBuf==BuffObject);
		if (bflen==0){ //Close
			theCfgFile.mlog.AddLogStrFmtTm(0,"Route %s write error: will be close",mRouteName); 
	//		pOvl->mOVstate=stVoid;
	//		mstate = mstate & (~enWritting);
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

//	pOvl->mOVstate=stVoid;
//	mstate = mstate & (~enWritting);
	PutPendingWrite();
	return 0;
}

int OrderRouter::onConnecting(M_OVBUF *pOvl, SOCKADDR_IN *localaddr, SOCKADDR_IN *rmoteaddr)
{

	char str[100];
	sprintf(str,"Connect to %s Successed!",mRouteName);
	theCfgFile.mlog.AddLogStr(str);  
	CWnd *pmainwnd=AfxGetMainWnd(); 
	if (pmainwnd!=NULL) {
		pmainwnd->PostMessage(WM_USER+101,1,(LPARAM)mRouteName);
	}
	time(&mlastbeat);
	pkgLogin(); 
	PutPendingRead();
	return 0;
}

static UINT st_connect_to_server(LPVOID lparam) 
{
	OrderRouter *proute;
	proute=(OrderRouter *)lparam;
	proute->ConnectToServer(0); 
	Sleep(10000);
	return 0;
}


BOOL OrderRouter::ConnectToServer(int mnewthread)
{
	if (mnewthread) {
		AfxBeginThread(st_connect_to_server,(LPVOID)this);
		return TRUE;

	}else {
		if (mSocket!=INVALID_SOCKET){
			return FALSE;
		}
		ResetStatus();
		mlsttmconnect=time(NULL);
		mlastmessagetime=GetTickCount(); 
		char tmbuff[20];
		theCfgFile.FormatTimeHMS(mlsttmconnect,tmbuff);

		theCfgFile.mlog.AddLogStrFmt("Try to connect to Route %s , %d times! %s.",
				mRouteName,mconnectfailnum+1,tmbuff);
		if (ConnectTo(mServerIP,mServerPort)!=0){
			mlsttmconnect=time(NULL);
			mconnectfailnum++;
			theCfgFile.mlog.AddLogStrFmt("Connect to %s Server Failure, %d times!",mRouteName,mconnectfailnum);
			return FALSE;
		}; 
		BOOL val;
		val=TRUE;
		setsockopt(mSocket,SOL_SOCKET,SO_KEEPALIVE,(char*)&val,sizeof(val));
		return TRUE;
	}
}

BOOL OrderRouter::DisconnectServer()
{
	if (mSocket==INVALID_SOCKET){
		return FALSE;
	}
	DoClose(TRUE);
	RemoveFromList(FALSE);
//	DoShutdown();
	return TRUE;
}

BOOL OrderRouter::pkgLogin()
{
	char str[100];
	PkgToken tok;
	Order_Login pkglogin;
	tok.InitToSet(str);
	tok.setWORD(Order_Login::ID);
	memset((char*)&pkglogin,0,sizeof(pkglogin));
	strcpy(pkglogin.UserID,mUserID);
	strcpy(pkglogin.PWD,mUserPWD);
	tok.setFixData((LPCSTR)&pkglogin,sizeof(pkglogin));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	return TRUE;
}

int OrderRouter::PkgLoginReply(char *pkgbuff)
{
	Order_Login_Reply *pkg;
//	char str[100];
	pkg=(Order_Login_Reply*)(pkgbuff+4);

	theCfgFile.mlog.AddLogStrFmtTm(0,"Login to route %s %s msg:%s", mRouteName,
		pkg->ret?"Successed":"Failed",pkg->InfoText);

	if (pkg->ret==0) { //if login failure, disconnect from the socket.
//		DisconnectServer(); 
	}else { // if login successed, query status;
		int pkglen;
		pkglen=*(WORD*)pkgbuff;
		if (pkglen>=32) {
			mServerVersion=pkg->fixversion; 
		}else {
			mServerVersion=0;
		}

		pkgStatusReq(1); //query if the server is ok.
		pkgStatusReq(2); //query what kinds of order it takes.
		mconnectfailnum=0;//successed login, clean connect fail number;
	}

	return TRUE;
}

BOOL OrderRouter::PkgTextInfo(char *pkgbuff)
{
	Order_Text_info *pkg;
	char str[500];
	pkg=(Order_Text_info*)(pkgbuff+4);
	pkg->InfoText[sizeof(pkg->InfoText)-1]=0; 
	sprintf(str,"%s GotMsg:Type %c,%s",mRouteName,pkg->InfoType,pkg->InfoText);
	theCfgFile.mlog.AddLogStr(str);
	return TRUE;
}


BOOL OrderRouter::pkgStatusReq(int type)
{
	char str[100];
	PkgToken tok;
	Order_Status_Require pkgstatusreq;
	tok.InitToSet(str);
	tok.setWORD(Order_Status_Require::ID);
	pkgstatusreq.InfoType=type; 
	tok.setFixData((LPCSTR)&pkgstatusreq,sizeof(pkgstatusreq));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	return TRUE;
}

BOOL OrderRouter::PkgStatusIfo(char *pkgbuff)
{
	Order_Status_Info *pkg;
	char str[100];
	pkg=(Order_Status_Info *)(pkgbuff+4);
	sprintf(str,"%s GotStatus:Type %d,Val %d",mRouteName,pkg->InfoType,pkg->InfoVal);
	theCfgFile.mlog.AddLogStr(str);
	switch(pkg->InfoType){
	case 1:// can send order or not
		if(pkg->InfoVal==1) {
			if (mServerEnable!=TRUE) {
				mServerEnable=TRUE;
				UpdateStatus();
			}
		}else {
			if (mServerEnable!=FALSE) {
				mServerEnable=FALSE;
				UpdateStatus();
			}
		}
/*		if (mServerEnable) {
			if(pkg->InfoVal!=1) {
				mServerEnable=FALSE;
				UpdateStatus();
			}
		}
*/
		break;
	case 2:// enabled order type;
		if (mServerOrderTypes!=pkg->InfoVal) {
			mServerOrderTypes=pkg->InfoVal; 
			UpdateStatus();
		}; 
		if (mServerOrderTypes & (1<<16)) {
			char buff[BF_SZ];
			int len=4;

			stritem *PIitem;
			theCfgFile.mRoutes.LockMap();
			theCfgFile.mRoutes.EnumFirst(&PIitem);
			OrderRouter *proute;
			while (PIitem) {
				proute=(OrderRouter*)PIitem->val;  
				if ((proute)&&(proute!=this)) {
					strcpy(buff+len,proute->mRouteName);
					len+=strlen(proute->mRouteName)+1;
					*(int*)(buff+len)=proute->GetRouteStatusInt() ;
					len+=4;
			
				}
				theCfgFile.mRoutes.EnumNext(&PIitem); 
			}
			theCfgFile.mRoutes.UnlockMap();

			buff[len]=0;len++;
			*(WORD*)(buff)=len;
			*(WORD*)(buff+2)=Order_SMT_RouteStat::ID; 
			SendPkg(buff);
			AsyncSelect(FD_WRITE);
		}
//		mServerOrderTypes=pkg->InfoVal; 
		break;
	}
	return TRUE;

}

BOOL OrderRouter::SendNewOrder(itemOrder *porder,char *porderpkg)
{
	ASSERT(porder!=NULL);
	if (mServerEnable==FALSE) {
		if (porder->IsGTCOrder()) return TRUE;
		sprintf(porder->mconftxt,"%s is disabled",mRouteName);
		porder->mstatus&=~1L;
		return FALSE;
	}
	if (mNewOrderEnable==FALSE) {
		if (porder->IsGTCOrder()) return TRUE;
		sprintf(porder->mconftxt,"%s no new order",mRouteName);
		porder->mstatus&=~1L;
		return FALSE;
	}

	Order_New_order neworder;
	memset(&neworder,0,sizeof(neworder));
	
	neworder.OrderID=porder->morderid;
	memccpy(neworder.Firm,theCfgFile.mCompanyID,0,sizeof(neworder.Firm)-1); 
	strcpy(neworder.Secsym,porder->msecsym);
	strcpy(neworder.Route,porder->mroute);
	strcpy(neworder.Bkrsym,porder->mbkrsym);
/*	Trader *ptr;
	ptr=Trader::FindTrader(porder->mtrid);
	if (ptr) {
		strcpy(neworder.UserID,ptr->minfo.mtracno); 
	}else{
		itemBranch *pbr;
		pbr=theCfgFile.mpItemPool->GetBranch(porder->morderid);
		if (pbr) {
			strcpy(neworder.UserID,pbr->mshort);  
		}else {
		}
	}
*/
	Account *pacc;
	pacc=Account::FindAcc(porder->maccid);
	if (pacc) {
		strcpy(neworder.UserID,pacc->mitemifo.mname); 
	}else{
		itemBranch *pbr;
		pbr=theCfgFile.mpItemPool->GetBranch(porder->morderid);
		if (pbr) {
			strcpy(neworder.UserID,pbr->mshort);  
		}else {
		}
	}

	neworder.Status=porder->mstatus; 
	neworder.status1=porder->ms1;
	neworder.QTY=porder->mqty;
	neworder.Exchange=porder->mexchange;
	neworder.Price=porder->mprice;
	neworder.StopPrice=porder->mstopprice;
	neworder.Trailer=porder->mtrailprice; 
	neworder.TimeForce=porder->mtmforce;
	neworder.QTYSH=porder->mf1;
	neworder.PriceSH=((double)porder->mf2)/10000;
	if (neworder.Secsym[0]=='+') {
		itemOptsym *popt;
		popt=itemOptsym::FindInfo(neworder.Secsym);
		if (popt==NULL) {
			sprintf(porder->mconftxt,"No Optinfo");
			porder->mstatus&=~1L;
			return FALSE;
		}else {
			neworder.OptExpDay=popt->expday;
			neworder.OptStrikeprice=popt->strikepice; 
		}
	}
	porder->ms1|=(1<<2);
	
//	char str[500];
	PkgToken tok;
	tok.InitToSet(porderpkg);
	tok.setWORD(Order_New_order::ID);
	tok.setFixData((LPCSTR)&neworder ,sizeof(neworder));
	tok.finalizeSet(); 
	return TRUE;
}

BOOL OrderRouter::SendCancelOrder(itemOrder *porder, int lvsqty,char * msg)
{

	ASSERT(porder!=NULL);
	msg[0]=0;
	if (mServerEnable==FALSE) {
		sprintf(msg,"%s is disabled",mRouteName);
		return FALSE;
	}
	porder->ms1|=1;
	Order_Cancel_order cancelorder;
	memset(&cancelorder,0,sizeof(cancelorder));
	
	cancelorder.OrderID =porder->morderid;
	cancelorder.Shares	=lvsqty;
	char str[100];
	PkgToken tok;
	tok.InitToSet(str);
	tok.setWORD(Order_Cancel_order::ID);
	tok.setFixData((LPCSTR)&cancelorder,sizeof(cancelorder));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	return TRUE;

}

int OrderRouter::PkgOrderAction(char *pkgbuff)
{

	Order_Process_action *pkg;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Process_action*)(pkgbuff+4);

	pkg->InfoText[sizeof(pkg->InfoText)-1]=0;
	CStrFunc::Replace(pkg->InfoText,'\r',' ');
	CStrFunc::Replace(pkg->InfoText,'\n',' ');
	CStrFunc::Replace(pkg->InfoText,',',' ');

	pkg->token[sizeof(pkg->token)-1]=0; 

	theCfgFile.mlog.AddLogStrFmtTm(0,"%s GotAction:order %d,action%c, change %d,remain %d,token %s, reason %20s",
		mRouteName,pkg->OrderID,pkg->ActionType,pkg->change,pkg->remain,pkg->token,pkg->InfoText);

	{
		char str[500];
		int len;
		len=sprintf(str,"%%A,ACT,%d,%c,%d,%d,%s,%s,%d",
			pkg->OrderID,pkg->ActionType,pkg->change,pkg->remain,pkg->token,pkg->InfoText,pkg->ntime);
		if (pkglen>=(sizeof(Order_Process_action)+4)) {
			sprintf(str+len,",%s,%s",pkg->EcnOrderID,pkg->RoutedOrderID);
		}

		theCfgFile.mlog.AddLogStr(str);
	}

	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(pkg->OrderID);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"ERROR:Can't find order %d in active order list!",pkg->OrderID);
//		theCfgFile.mlog.AddLogStr(str);
//		porder=itemOrder::F
//		ASSERT(FALSE);
		return 0;
	}



	if (mbTestRoute) {
		if (!porder->IsTestOrder()) {
			theCfgFile.mlog.AddLogStr("ERROR!!!:Test Route Return life trades!");
			return 0;
		}
	}

	Account *pacc; 
	pacc=Account::FindAcc(porder->maccid);
	if (pacc==NULL){
		theCfgFile.mlog.AddLogStrFmtTm(0,"ERROR:Can't find Account for Order %d!",porder->morderid);
//		theCfgFile.mlog.AddLogStr(str);
	}
	
	BOOL bCheckAutoCover=FALSE;
	pacc->Lock();
	
	itemOrderActive pactive;
	int orderchangeflag;
	pactive.Clear(); 
	switch(pkg->ActionType){
	case 'A':// order Accept;
		porder->SetOrderOpenQty(pkg->remain);
		porder->mstatus|=1<<4;
		strncpy(porder->mconftxt,pkg->InfoText,sizeof(pkg->InfoText));
		strncpy(porder->mtoken,pkg->token,sizeof(pkg->token));  
		time(&porder->mm_date);
		orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|
			itemOrder::deflvsqty|itemOrder::defconftxt|
			itemOrder::deftoken|itemOrder::defm_date;
		porder->SendChange(orderchangeflag);
		porder->ModifyDB(orderchangeflag);
//		porder->SendAction(5,"order Accepted");
		pactive.FillInfo(porder); 
		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.SetType(LOAAccept);
		pactive.mqty=porder->mlvsqty;
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 
		break;
	case 'J':// order Rejected;
		if (porder->IsGTCOrder()) {
			if (((porder->ms1&1)==0)&&((porder->mstatus&(1<<4))!=0)) {//not canceling but accept. we just widthdraw the order.
				porder->ms1&=~(1<<2);
//				porder->mstatus|=1<<2;
				time(&porder->mm_date);
				orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|itemOrder::defmexts1|
						itemOrder::deftoken|itemOrder::defm_date;
					
				porder->SendChange(orderchangeflag);
				porder->ModifyDB(orderchangeflag);
				break;


			}
		}

		porder->SetOrderOpenQty(pkg->remain);
		porder->mstatus|=1<<3;
		porder->mstatus&=~1L;
		strncpy(porder->mconftxt,pkg->InfoText,sizeof(pkg->InfoText));
		strncpy(porder->mtoken,pkg->token,sizeof(pkg->token));  
		time(&porder->mm_date);
		porder->SendChange(itemOrder::deforderid|itemOrder::defstatus|
			itemOrder::deflvsqty|itemOrder::defconftxt|
			itemOrder::deftoken|itemOrder::defm_date);
		porder->ModifyDB(itemOrder::deforderid|itemOrder::defstatus|
			itemOrder::deflvsqty|itemOrder::defconftxt|
			itemOrder::deftoken|itemOrder::defm_date);
//		porder->SendAction(6,pkg->InfoText);
		pactive.FillInfo(porder); 
		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.mqty=pkg->change;
		pactive.SetType(LOASendRej);
		strcpy(pactive.mnote,pkg->InfoText);
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 
		break;
	case 'K':// order Cancel Reject;
		pactive.FillInfo(porder); 
		porder->ms1|=(1<<1);// cancel reject;
		porder->SendChange(itemOrder::defmexts1);
		porder->ModifyDB(itemOrder::defmexts1);
		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.SetType(LOACancelRej);
		pactive.mqty=pkg->remain;
		strncpy(pactive.mnote,pkg->InfoText,sizeof(pkg->InfoText));
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 
		//porder->SendAction(7,pkg->InfoText);
		break;
	case 'D':// order replace reject;
		pactive.FillInfo(porder); 
		porder->ms1|=(1<<1);// cancel reject;
		porder->SendChange(itemOrder::defmexts1);
		porder->ModifyDB(itemOrder::defmexts1);
		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.SetType(LOAReplaceRej);
		pactive.mqty=pkg->change;
		strncpy(pactive.mnote,pkg->InfoText,sizeof(pkg->InfoText));
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 
		//porder->SendAction(7,pkg->InfoText);
		break;
	case 'C':// order canceled;
//		ASSERT(porder->mlvsqty-pkg->change==pkg->remain);  
		if (porder->IsGTCOrder()) {
			if ((porder->ms1&1)==0) {//not canceling
				porder->ms1&=~(1<<2);
//				porder->mstatus|=1<<2;
				time(&porder->mm_date);
				orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|itemOrder::defmexts1|
						itemOrder::deftoken|itemOrder::defm_date;
					
				porder->SendChange(orderchangeflag);
				porder->ModifyDB(orderchangeflag);
				break;

 
			}
		}
		porder->SetOrderOpenQty(pkg->remain);
		porder->mcxlqty+=pkg->change; 
		porder->mstatus|=1<<2;
		if (pkg->remain==0)	{
			porder->mstatus&=~1L;
			bCheckAutoCover=TRUE;
		}
		strncpy(porder->mconftxt,pkg->InfoText,sizeof(pkg->InfoText));
		time(&porder->mm_date);
		orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|
			itemOrder::deflvsqty|itemOrder::defconftxt|
			itemOrder::deftoken|itemOrder::defm_date|
			itemOrder::defcxlqty;
		
		porder->SendChange(orderchangeflag);
		porder->ModifyDB(orderchangeflag);
//		porder->SendAction(4,pkg->InfoText);

		pactive.FillInfo(porder); 

		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.SetType(LOACanceled);
		pactive.mqty=pkg->change; 
		strcpy(pactive.mnote,pkg->InfoText);
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 

		break;
	case 'S'://Order sent. 
		pacc->UnLock();
		return 0;
	}
	double hldpwr=pacc->ReCalOrderHoldBP();
	if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
		pacc->mitemEquity.mCurrHoldBP=hldpwr;
		char pkg[200];
		pacc->MakePkgByEquity(pkg,1<<8);
		Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
	}
	pacc->UnLock();

	if (bCheckAutoCover){
		pacc->AutoCoverPosition(porder->msecsym,porder->mtrid,porder->mprice,porder->mexchange);
	}
	return 0;
}

int OrderRouter::PkgOrderExecute(char *pkgbuff)
{
	Order_Execute *pkg;
	char str[100];
	int orderid;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Execute*)(pkgbuff+4);

	orderid=pkg->OrderID;
	sprintf(str,"%s OrderExecute:order %d,Share %d",
		mRouteName,pkg->OrderID,pkg->ExeShares );
	if (((BYTE)pkg->ExeSeq)>126){
		theCfgFile.mlog.AddLogStrFmtTm(0,"Warning:exeseq:oid:%d, seq:%d,%u\n",pkg->OrderID, pkg->ExeSeq,pkg->ExeSeq);   
		theCfgFile.mlog.Flush();

	}
	theCfgFile.mlog.AddLogStr(str);
	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(orderid);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStr("ERROR:can't find this order in Active Order;");
//		porder=itemOrder::FindClos(pkg->OrderID);
//		should find the order in the Closed order list
		return 0;
	}
	int accid;
	accid=porder->maccid;
	itemTrade *ptrade;
	Account *pacc;
	pacc=Account::FindAcc(accid);
	if (pacc==NULL){
		theCfgFile.mlog.AddLogStr("Error:can't find the Account!");
		return 0;
	}
	Trader *ptrader;
	ptrader=Trader::FindTrader(porder->mtrid);
	char trname[20];
	trname[0]=0;
	if (ptrader!=NULL) {
		strcpy(trname,ptrader->minfo.mtracno); 
	}
	char strlog[20000];
	sprintf(strlog,"#EXE,%d,%s,%s,%s,%s,%d,%g,%s",
		pkg->OrderID, 
		trname,
		pacc->mitemifo.mname,
		(porder->IsBuyOrder()?"Buy":"Sell"),
		porder->msecsym,pkg->ExeShares, 
		pkg->Exeprice,
		mRouteName 
		); 
	theCfgFile.mlog.AddLogStr(strlog); 
	
	{
		int ofset=sprintf(strlog,"%%A,EXE,%d,%d,%f,%d,%d,%s,%d,%d",
			pkg->OrderID,pkg->ExeSeq,pkg->Exeprice,pkg->ExeShares,pkg->RemShares,pkg->Bkrsym,
			pkg->Status,pkg->ntime);
		if (pkglen>=(sizeof(Order_Execute)+4)){
			strlog[ofset]=',';
			strncpy(strlog+ofset+1,pkg->ExecID,sizeof(pkg->ExecID));
		}
		theCfgFile.mlog.AddLogStr(strlog);

	}
	if (mbTestRoute) {
		if (!porder->IsTestOrder()) {
			theCfgFile.mlog.AddLogStr("ERROR!!!:Test Route Return life trades!");
			return 0;
		}
	}

	pacc->Lock(); 
	ptrade=itemTrade::FindTradeByExe(accid,orderid,pkg->ExeSeq);
	if (ptrade!=NULL) { //this execute already exist;
	}else {//new execute;
		itemAccEquity oldequity;
		oldequity.CopyFrom(&pacc->mitemEquity);
		DWORD orderchangeflag;
		ptrade=itemTrade::GetVoidBuff();
		ptrade->Clear();
		ptrade->CopyFromOrder(porder);
		ptrade->mtradeid=theCfgFile.GetNewTradeID();//InterlockedIncrement((long*)&(theCfgFile.mCurrTradeID));  
		ptrade->mexecnum=pkg->ExeSeq;
		strncpy(ptrade->mbkrsym,pkg->Bkrsym,sizeof(pkg->Bkrsym));
		ptrade->mqty=pkg->ExeShares;
		ptrade->mprice=pkg->Exeprice;
		ptrade->me_date=pkg->ntime;
		time(&ptrade->mc_date);
		ptrade->mm_date=ptrade->mc_date;

		if (pkglen>=(sizeof(Order_Execute)+4)){
			ptrade->mtrtype=pkg->trtype;
		}


		if (pkg->Status&(1<<20)){
			ptrade->mstatus|=(1<<20);
		}else {
			ptrade->mstatus&=~(1<<20);
		}
		if (pkg->Status&(1<<27)) {
			ptrade->mstatus|=(1<<27);
		}else {
			ptrade->mstatus&=~(1<<27);
		}

		if (theCfgFile.mbTradeEcnFee) {
			ptrade->mEcnFee=ptrade->GetEcnFee(NULL);
		}

//		ptrade->mstatus=pkg->Status;
		double lastpl;
		lastpl=pacc->mitemEquity.mRealized;  
		intitem *pintitem;
		pintitem=pacc->mLstTrades.AddAscent(ptrade->me_date,(long)ptrade);  
		itemTrade::sMapTradeID.Add(ptrade->mtradeid,(long)ptrade); 

		if (pacc->TktNeedRecalcuate(ptrade)){
			pacc->TktRecaluate(TRUE,FALSE);
		}else {
			pacc->TktPatchAddTrade(ptrade,TRUE,FALSE);
		}

		ptrade->mTradePL=pacc->mitemEquity.mRealized-lastpl; 
			
		porder->SetOrderOpenQty(pkg->RemShares); 
		porder->mexecnum=pkg->ExeSeq; 
		ptrade->ModifyDB  (0x7fffffff);
		ptrade->SendChange(0x7fffffff);
		
		time(&porder->mm_date);
		orderchangeflag=itemOrder::defstatus|itemOrder::defexecnum|
			itemOrder::deflvsqty|itemOrder::defm_date;
		porder->mstatus|= 1<<1;
		if(pkg->RemShares==0) {
			porder->mstatus &= ~1L;
		}
		porder->ModifyDB(orderchangeflag);
		porder->SendChange(orderchangeflag|1);
//		char st[100];
//		sprintf(st,"%d share been executed");

		itemOrderActive active; 
		active.Clear();				
 		active.FillInfo(porder); 
		if (pkg->ntime!=0) active.mlogtime=pkg->ntime; 

		active.mqty=ptrade->mqty;
		active.mprice=ptrade->mprice;
		active.SetType(LOAExecuted);
//		strcpy(pactive.mnote,pkg->InfoText);
		active.SendActive(porder->mbrid);
		active.SaveToDB(); 
//		porder->SendAction(8,"Order Executed!");

		pacc->mitemEquity.mCurrHoldBP=pacc->ReCalOrderHoldBP(); 

		WORD eqdiff;
		eqdiff=oldequity.CompareItem(&pacc->mitemEquity);
		if (eqdiff) {
			char pkg[200];
			pacc->MakePkgByEquity(pkg,eqdiff);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}
		if (porder->ms1 & (1<<11)) {//auto cancel order.
			pacc->CancelAllUnwindOrder(porder); 
		}
	}
	pacc->UnLock();
	if (porder->mlvsqty==0) {
		pacc->AutoCoverPosition(porder->msecsym,porder->mtrid,porder->mprice,porder->mexchange);
	}
	return 0;
}

int OrderRouter::PkgHeartBeat(char *pkgbuff)
{
	time(&mlastbeat);
	char str[100];
	PkgToken tok;
	Order_HeartBeatRet retpkg; 
	tok.InitToSet(str);
	tok.setWORD(Order_HeartBeatRet::ID);
	retpkg.ntime=mlastbeat; 
	tok.setFixData((LPCSTR)&retpkg,sizeof(Order_HeartBeatRet));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	theCfgFile.mlog.AddLogStrFmt("Route %s got heart beat!",mRouteName); 
	return TRUE;

	
}

BOOL OrderRouter::IsEnabled()
{
	return (mServerEnable==TRUE); //do not check heartbeat;

/*	time_t t1;
	time(&t1);
	if ((t1-mlastbeat>21)){
		if (mSocket!=INVALID_SOCKET){
			DoClose(TRUE); 
		}
		return FALSE;
	}else {
		return (mServerEnable==TRUE);
	}
	return FALSE;
*/

}

BOOL OrderRouter::IsConnected()
{
	return (mSocket!=INVALID_SOCKET);
}

BOOL OrderRouter::DoConfig()
{
	OrderRouteCfg1 dlg;
	dlg.mproute=this;
	strcpy(dlg.mRouteName,mRouteName);
	dlg.mIP=mServerIP;
	dlg.mPort=mServerPort;
	strncpy(dlg.mUserID,mUserID,sizeof(mUserID));
	strncpy(dlg.mUserPWD,mUserPWD,sizeof(mUserPWD));
	dlg.mtmstart=mtmconnect;
	dlg.mtmstop=mtmdisconnect;
	dlg.mautoconnect=mautoconnect;
	if (dlg.DoModal()==IDOK) {
			mServerIP=dlg.mIP;
			mServerPort=dlg.mPort;
			strncpy(mUserID,dlg.mUserID,sizeof(mUserID));
			strncpy(mUserPWD,dlg.mUserPWD,sizeof(mUserPWD));
			mtmconnect=dlg.mtmstart;
			mtmdisconnect=dlg.mtmstop;
			mautoconnect=dlg.mautoconnect;
			CWnd *pmainwnd=AfxGetMainWnd(); 
			if (pmainwnd!=NULL) {
				pmainwnd->PostMessage(WM_USER+101,1,(LPARAM)mRouteName);
			}

	}
	return TRUE;
}

BOOL OrderRouter::ResetStatus()
{
	mCritical.Lock(); 
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
	mCritical.Unlock();
	return TRUE;
}

BOOL OrderRouter::UpdateStatus()
{
	CWnd *pmainwnd=AfxGetMainWnd(); 
	if (pmainwnd!=NULL) {
		pmainwnd->PostMessage(WM_USER+101,1,(LPARAM)mRouteName);
	}

	char pkg[500];
	if (!m_bHideRoute) {
		PkgToken tok;
		tok.InitToSet(pkg);
		tok.setWORD(2000);
		tok.setBYTE(1);
		tok.setVarStr(mRouteName);
		tok.setWORD((WORD)GetRouteStatus()); 
		tok.setVarStr(""); 
		tok.finalizeSet(); 

		Trader::SPKGTraderAll(pkg);
	}

	int len=4;
	strcpy(pkg+len,mRouteName);
	len+=strlen(mRouteName)+1;
		
	*(int*)(pkg+len)=GetRouteStatusInt();
	len+=4;
	pkg[len]=0;len++;
	*(WORD*)(pkg)=len;
	*(WORD*)(pkg+2)=Order_SMT_RouteStat::ID; 
	stritem *PIitem;
	theCfgFile.mRoutes.EnumFirst(&PIitem);
	OrderRouter *proute;
	while (PIitem) {
		proute=(OrderRouter*)PIitem->val;  
		if ((proute)){//&&(proute!=this)
			if ((proute->mServerOrderTypes&(1<<16))&&
				proute->IsConnected()&&
				proute->IsEnabled()){

				proute->SendPkg(pkg);
				proute->AsyncSelect(FD_WRITE);

			}
		}
		theCfgFile.mRoutes.EnumNext(&PIitem); 
	}
	return TRUE;
}

//this function is the status value send to the client.
int OrderRouter::GetRouteStatus()
{
	int ret=1;//always set the first bit 1;
	if (IsConnected()) ret|=2;
	if (IsEnabled())   ret|=4;
	if (mbTestRoute) ret|=8;
	return ret;
}


//This function is for the route status for the smart.
int OrderRouter::GetRouteStatusInt()
{
	int ret=mServerOrderTypes&0xFFFFFF;
	if (IsConnected()) ret|=(1<<24);
	if (IsEnabled())   ret|=(1<<25);
	if (mbTestRoute) ret|=(1<<26);
	return ret;
}

// send new order according by the auto routed order.
int OrderRouter::Pkg_SMT_NewOrder(char *pkgbuff)
{

	Order_SMT_NewOrder *pkg;
	pkg=(Order_SMT_NewOrder*)(pkgbuff+4);

	theCfgFile.mlog.AddLogStrFmt("%s Send_NewOrder %d",mRouteName,pkg->OrderID);
	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(pkg->OrderID);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStr("ERROR:Can't find Smart order in active order list!");
		return 0;
	}

	porder->mlvsqty=pkg->liveqty;
	porder->mstatus=pkg->status;
	int aflag;
	porder->mm_date=time(NULL);
	aflag=itemOrder::deflvsqty|itemOrder::defstatus|itemOrder::defmexts1|itemOrder::defm_date;
	porder->ModifyDB(aflag);

// send new order;
	itemOrder item;
	DWORD flag;
	item.Clear(); 
	item.CopyFrom(porder);
	item.morigtkn=pkg->newTkn;
	if (porder->morigoid) { 
		item.morigoid=porder->morigoid;
	}else {
		item.morigoid=porder->morderid;
	}
	item.mc_date=porder->mc_date;
	strcpy(item.mroute,pkg->newRoute);  
	strcpy(item.mbkrsym,pkg->newbkrsym);
	item.mstatus=pkg->newstatus;
	item.ms1=pkg->newstatusex;
	item.mqty=pkg->newqty;
	item.mlvsqty=item.mqty; 
	item.mprice=pkg->newprice;
	item.mstopprice=pkg->newstopprice;	
	item.mtrailprice=pkg->newtrailer;
	item.mtmforce=pkg->newtimeforce;
	item.mf1=pkg->newqtysh;
	item.mf2=(int)(pkg->newpricesh*1000);

	item.morderid=0;
	
	flag=0xffffffff;;

	Account *pacc;
	pacc=Account::FindAcc(item.maccid);
	double hldpwrt=0;
	double oldhold;
	if (pacc) {
		hldpwrt=pacc->ReCalOrderHoldBP();
		oldhold=pacc->mitemEquity.mCurrHoldBP;
		pacc->mitemEquity.mCurrHoldBP=hldpwrt;

	}
	
	CClient *pclient;
	pclient=Trader::FindRefClient(item.mentrid); 

	BOOL bret;
	bret=item.NewOrder(flag,(char*)pclient,NULL); 


	if (pclient) pclient->RefDec();

	
//	char str1[500];
//	sprintf(str1,"NewOrder:%s--%s,id:%d",mptrifo->minfo.mtracno,item.msecsym,item.morderid);  
	theCfgFile.mlog.AddLogStrFmt("AutoSend NewOrder:%s, id:%d qty:%d, price:%g ",item.msecsym,item.morderid,
			item.mqty,item.mprice); 

	if (!bret) {
		// the auto order was triggered, but send new order error! 
		// update the hold order bp;
		if (pacc){
			double hldpwr=pacc->ReCalOrderHoldBP();
//			if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
			pacc->mitemEquity.mCurrHoldBP=hldpwr;
			char pkg[200];
			pacc->MakePkgByEquity(pkg,1<<8);
			Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
		}

	/*	SendOrderTknRply(0,item.morigtkn,item.morderid);
		Account *pacc;
		pacc=Account::FindAcc(item.maccid);
		if (pacc){
			char pkg[300];
			pacc->MakePkgByEquity(pkg,itemAccEquity::defCurrHoldBP|itemAccEquity::defCurrBP);
			SendPkg(pkg);
		};
*/
	}else {
		theCfgFile.mlog.AddLogStrFmt("%%E,T,%d,%d,%d,%d,%d,%d",
				item.morderid,item.mc_date,item.morigoid,porder->mc_date,porder->morderid,porder->mc_date); 

	}

	porder->SendChange(aflag);
	return 0;

}

int OrderRouter::PkgOrderExecute_Cancel(char *pkgbuff)
{
	Order_Execute_Cancel *pkg;
	char str[100];
	int orderid;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Execute_Cancel*)(pkgbuff+4);

	orderid=pkg->OrderID;
	sprintf(str,"%s OrderExecute_cancel:order %d,Share %d",
		mRouteName,pkg->OrderID,pkg->ExeShares );
	theCfgFile.mlog.AddLogStr(str);
	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(orderid);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStr("ERROR:can't find this order in Active Order;");
//		porder=itemOrder::FindClos(pkg->OrderID);
//		should find the order in the Closed order list
		return 0;
	}
	int accid;
	accid=porder->maccid;
	itemTrade *ptrade;
	Account *pacc;
	pacc=Account::FindAcc(accid);
	if (pacc==NULL){
		theCfgFile.mlog.AddLogStr("Error:can't find the Account!");
		return 0;
	}
	Trader *ptrader;
	ptrader=Trader::FindTrader(porder->mtrid);
	char trname[20];
	trname[0]=0;
	if (ptrader!=NULL) {
		strcpy(trname,ptrader->minfo.mtracno); 
	}
	char strlog[20000];
	sprintf(strlog,"#CXLEXE,%d,%s,%s,%s,%s,%d,%g,%s",
		pkg->OrderID, 
		trname,
		pacc->mitemifo.mname,
		(porder->IsBuyOrder()?"Buy":"Sell"),
		porder->msecsym,pkg->ExeShares, 
		pkg->Exeprice,
		mRouteName 
		); 
	theCfgFile.mlog.AddLogStr(strlog); 
	
	{
		int ofset=sprintf(strlog,"%%A,CXLEXE,%d,%d,%f,%d,%d,%s,%d,%d,%s",
			pkg->OrderID,pkg->ExeSeq,pkg->Exeprice,pkg->ExeShares,pkg->RemShares,pkg->Bkrsym,
			pkg->Status,pkg->ntime,pkg->ExecID);
		theCfgFile.mlog.AddLogStr(strlog);

	}
	if (mbTestRoute) {
		if (!porder->IsTestOrder()) {
			theCfgFile.mlog.AddLogStr("ERROR!!!:Test Route Return life trades!");
			return 0;
		}
	}

	pacc->Lock(); 
	ptrade=itemTrade::FindTradeByExe(accid,orderid,pkg->ExeSeq);
	if (ptrade!=NULL) { //this execute already exist;
		//Should chang the order also,
		//but we just ignore it right now. we may do it later.
		//don't know the process about Brust trades.
		if (pkg->ExeShares==0){
			ptrade->SetOpenFlag(FALSE); 
		}else {
			ptrade->mqty=pkg->ExeShares;
		}
		ptrade->ModifyDB(itemTrade::defqty|itemTrade::defstatus|itemTrade::defm_date);
		ptrade->SendChange(itemTrade::defqty|itemTrade::defstatus|itemTrade::defm_date);
		pacc->TktRecaluate(TRUE,TRUE);

	}else {//new execute;
	}
	pacc->UnLock();
	return 0;
}

BOOL OrderRouter::SendOrderReplace(itemOrder *porder, itemOrder *pnew, char *msg)
{

	ASSERT(porder!=NULL);
	msg[0]=0;
	if (mServerEnable==FALSE) {
		sprintf(msg,"%s is disabled",mRouteName);
		return FALSE;
	}
	if (mNewOrderEnable==FALSE) {
		sprintf(msg,"%s no new order",mRouteName);
		return FALSE;
	}


	porder->ms1|=(1<<3);

	Order_Replace_order st;
	memset(&st,0,sizeof(st));
	st.OrderID=porder->morderid;
	strcpy(st.Route,porder->mroute);
	strcpy(st.Bkrsym,pnew->mbkrsym);
	st.Share=pnew->mqty;
	st.Price=pnew->mprice;
	st.StopPrice=pnew->mstopprice;
	st.Trailer=pnew->mtrailprice;
	st.TimeForce=pnew->mtmforce;
	st.QTYSH=porder->mf1;
	st.PriceSH=((double)porder->mf2)/10000;

	int stmask;
	int s1mask;
	stmask=(1<<9);
	s1mask=(1<<6);


	st.status=(porder->mstatus & ~stmask)|(pnew->mstatus & stmask);
	st.status1=(porder->ms1 & ~s1mask)|(pnew->ms1 & s1mask);


	char str[200];
	PkgToken tok;
	tok.InitToSet(str);
	tok.setWORD(Order_Replace_order::ID);
	tok.setFixData((LPCSTR)&st,sizeof(st));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	return TRUE;
}

int OrderRouter::PkgReplaceAction(char *pkgbuff)
{
	Order_Replace_Action *pkg;
	char str[500];
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Replace_Action*)(pkgbuff+4);

	pkg->InfoText[sizeof(pkg->InfoText)-1]=0;
	CStrFunc::Replace(pkg->InfoText,'\r',' ');
	CStrFunc::Replace(pkg->InfoText,'\n',' ');
	CStrFunc::Replace(pkg->InfoText,',',' ');

	pkg->token[sizeof(pkg->token)-1]=0; 

	sprintf(str,"%s GotRplace:order %d,action%c, change %d,remain %d,token %s, reason %20s",
		mRouteName,pkg->m_repl.OrderID,pkg->ActionType,pkg->change,pkg->remain,pkg->token,pkg->InfoText);

	theCfgFile.mlog.AddLogStr(str);

	{
		int len;
		len=sprintf(str,"%%A,RPL,%d,%c,%d,%d,%s,%s,%d",
			pkg->m_repl.OrderID,pkg->ActionType,pkg->change,pkg->remain,pkg->token,pkg->InfoText,pkg->ntime);
		if (pkglen>=(sizeof(Order_Process_action)+4)) {
			sprintf(str+len,",%s,%s",pkg->EcnOrderID,pkg->RoutedOrderID);
		}

		theCfgFile.mlog.AddLogStr(str);
	}

	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(pkg->m_repl.OrderID);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStr("ERROR:Can't find order in active order list!");
		theCfgFile.mlog.AddLogStr(str);
//		porder=itemOrder::F
//		ASSERT(FALSE);
		return 0;
	}



	if (mbTestRoute) {
		if (!porder->IsTestOrder()) {
			theCfgFile.mlog.AddLogStr("ERROR!!!:Test Route Return life trades!");
			return 0;
		}
	}

	Account *pacc; 
	pacc=Account::FindAcc(porder->maccid);
	if (pacc==NULL){
		theCfgFile.mlog.AddLogStr("ERROR:Can't find Account for Order!");
		theCfgFile.mlog.AddLogStr(str);
	}
	
	pacc->Lock();
	
	itemOrderActive pactive;
	int orderchangeflag;
	pactive.Clear(); 
	switch(pkg->ActionType){
	case 'A':// order Accept;
		porder->mqty=pkg->m_repl.Share; 
		porder->SetOrderOpenQty(pkg->remain);//		porder->mlvsqty=pkg->remain; 
		porder->mstatus|=1<<4;
		strncpy(porder->mconftxt,pkg->InfoText,sizeof(pkg->InfoText));
		//strncpy(porder->mtoken,pkg->token,sizeof(pkg->token));  
		porder->mprice=pkg->m_repl.Price;

		int stmask;
		int s1mask;
		stmask=(1<<9);
		s1mask=(1<<6);
		porder->mstatus=(porder->mstatus & ~stmask) |(pkg->m_repl.status & stmask);
		porder->ms1=(porder->ms1& ~stmask) |(pkg->m_repl.status1 & stmask);
		if (pkg->remain==0)	porder->mstatus&=~1L;
	



		time(&porder->mm_date);
		orderchangeflag=itemOrder::deforderid|itemOrder::defstatus|itemOrder::defqty|
			itemOrder::deflvsqty|itemOrder::defconftxt|
			itemOrder::deftoken|itemOrder::defm_date|itemOrder::defprice;
		porder->SendChange(orderchangeflag);
		porder->ModifyDB(orderchangeflag);

		pactive.FillInfo(porder); 
		if (pkg->ntime!=0) pactive.mlogtime=pkg->ntime; 
		pactive.SetType(LOAReplaced);
		pactive.mqty=porder->mqty;
		pactive.SendActive(porder->mbrid);
		pactive.SaveToDB(); 
		break;

	case 'S'://Replace order sent. don't do anything.

		pacc->UnLock();
		return 0;

	}
	double hldpwr=pacc->ReCalOrderHoldBP();
	if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
		pacc->mitemEquity.mCurrHoldBP=hldpwr;
		char pkg[200];
		pacc->MakePkgByEquity(pkg,1<<8);
		Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
	}
	pacc->UnLock();

	return 0;
}

int OrderRouter::PkgTrailerPriceUpdate(char *pbuff)
{
	Order_SMT_TrailerPriceUpdate *pkg;
	int pkglen=*(WORD*)pbuff;
	pkg=(Order_SMT_TrailerPriceUpdate*)(pbuff+4);

	theCfgFile.mlog.AddLogStrFmtTm(0,"Trailer Price Change %d, Peak:%g Trailer:%g",
		pkg->OrderID,pkg->TrailerPeak,pkg->TrailerPrice);

	itemOrder *porder;
	porder=itemOrder::FindActiveOrder(pkg->OrderID);
	if (porder==NULL) {
		theCfgFile.mlog.AddLogStrFmtTm(0,"ERROR:Can't find order %d in active order list!",pkg->OrderID);
		return 0;
	}
	if (!porder->IsTrailerOrder()){
		theCfgFile.mlog.AddLogStrFmtTm(0,"ERROR:order %d is not Trailer order",pkg->OrderID);
		return 0;
	}
	porder->mstopprice=pkg->TrailerPeak;
	porder->mtrailprice=pkg->TrailerPrice;
	
	DWORD orderchangeflag=itemOrder::deforderid|itemOrder::defstopprice|itemOrder::deftrailprice;
	porder->SendChange(orderchangeflag);
	porder->ModifyDB(orderchangeflag);
	return 0;

}

int OrderRouter::PkgDirectOrder(char *pkgbuff)
{
	Order_DirectUDP_Order *pkg;
//	char str[100];
//	int orderid;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_DirectUDP_Order*)(pkgbuff+4);


	Trader *ptrader=Trader::FindTrader(pkg->Trader); 
	if (ptrader==NULL) {
		theCfgFile.mlog.AddLogStr("Invalid Trader");
		return 0;
	}
	Account *pacc=Account::FindAccByName(pkg->Account); 
	if (pacc==0) {
		theCfgFile.mlog.AddLogStr("Invalid Account");
		return 0;
	}

	itemOrder ord;
//	BOOL ret;
	ord.Clear();
	ord.mtrid=ptrader->minfo.mtrid;
	ord.maccid=pacc->mitemifo.maccid;
	ord.maskprice=ord.mbidprice=pkg->Price;
	strncpy(ord.msecsym,pkg->Secsym,sizeof(ord.msecsym)-1);
	strncpy(ord.mroute,mRouteName,sizeof(ord.mroute)-1);
	ord.mprice=pkg->Price;
	ord.mqty=pkg->QTY;

	ord.mentrid=ord.mtrid;
	ord.mbrid=pacc->mitemifo.mbrid;
	ord.mc_date=time(NULL);
	ord.mm_date=ord.mc_date;
	ord.mexchange=pkg->Exchange; 
	ord.mtmforce=0; //send as IOC order;
	ord.mstatus=pkg->Status;
	ord.ms1=pkg->status1;
	ord.morderid=pkg->OrderID;

	itemOrder *porder;
	int newflag;
	pacc->Lock(); 

	newflag=0xffffffff;
	porder=Account::AddNewOrder(&ord);

	theCfgFile.mlog.AddLogStrFmt("%%E,R,%d,%d",ord.morderid,ord.mc_date); 

	porder->ModifyDB(newflag);
	porder->SendChange(newflag);
	double hldpwr=pacc->ReCalOrderHoldBP();

	if (hldpwr!=pacc->mitemEquity.mCurrHoldBP) {
		pacc->mitemEquity.mCurrHoldBP=hldpwr;
		char pkg[200];
		pacc->MakePkgByEquity(pkg,1<<8);
		Trader::SPKGAcc(pkg,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
	}

	pacc->UnLock(); 

	return 0;

}

BOOL OrderRouter::SendHeartBeat()
{
	Order_HeartBeat heartbeat;
	heartbeat.ntime=time(NULL); 
	char str[100];
	PkgToken tok;
	tok.InitToSet(str);
	tok.setWORD(Order_HeartBeat::ID);
	tok.setFixData((LPCSTR)&heartbeat,sizeof(heartbeat));
	SendPkg(tok.finalizeSet());  
	AsyncSelect(FD_WRITE);
	return TRUE;

}

void OrderRouter::SetDisableExch(LPCSTR str)
{
	if (str==NULL) return;
	mDisableExch=0;
	int i=0;
	while(str[i]) {
		switch(str[i]) {
		case 'N' : mDisableExch |=(1<<0); break;
		case 'A' : mDisableExch |=(1<<1); break;
		case 'Q' : mDisableExch |=(1<<2); break;
		case 'U' : mDisableExch |=(1<<3); break;
		case 'u' : mDisableExch |=(1<<4); break;
		case 'O' : mDisableExch |=(1<<5); break;
		case 'F' : mDisableExch |=(1<<6); break;
		}
		i++;
	}
}

BOOL OrderRouter::IsExchDisable(char *symbol, char nexchange)
{
	if (mDisableExch==0) return FALSE;
	switch(nexchange) {
	case 'N' :if (mDisableExch & (1<<0)) return TRUE; break;
	case 'A' :if (mDisableExch & (1<<1)) return TRUE; break;
	case 'Q' :if (mDisableExch & (1<<2)) return TRUE; break;
	case 'U' :if (mDisableExch & (1<<3)) return TRUE; break; 
	case 'u' :if (mDisableExch & (1<<4)) return TRUE; break; 
	default:
		if (symbol[0]=='+'){		// option;
			if (mDisableExch & (1<<5)) return TRUE;
		}else if (symbol[0]=='#') {	//futures;
			if (mDisableExch & (1<<6)) return TRUE;
		}
		break;
	}
	return FALSE;

}

void OrderRouter::GetDisableExch(char *buff)
{
	
	buff[0]=0;
	if (mDisableExch&(1<<0)) strcat(buff,"N");
	if (mDisableExch&(1<<1)) strcat(buff,"A");
	if (mDisableExch&(1<<2)) strcat(buff,"Q");
	if (mDisableExch&(1<<3)) strcat(buff,"U");
	if (mDisableExch&(1<<4)) strcat(buff,"u");
	if (mDisableExch&(1<<5)) strcat(buff,"O");
	if (mDisableExch&(1<<6)) strcat(buff,"F");
	

}

int OrderRouter::SendPkg(int type, const char *pbuf, int len)
{
	char buff[500];
	*(WORD*)buff=len+4;
	*(WORD*)(buff+2)=type;
	memcpy(buff+4,pbuf,len);

	mChainOutBuf.AddPkgBuff(buff);
	return 0;
}

int OrderRouter::PkgAccountBalance(char *pkgbuff)
{
	Order_Account_Balance *pkg;
//	char str[100];
//	int orderid;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Account_Balance*)(pkgbuff+4);


	Account *pacc=Account::FindAccByName(pkg->UserID); 
	if (pacc==0) {
		theCfgFile.mlog.AddLogStrFmt("Error: Account Balance: Invalid Account %s ",pkg->UserID );
		return 0;
	}


	pacc->mitemEquity.mOpenBP=pkg->StartCash*pkg->MarginRatio;
	pacc->mitemEquity.mCurrBP=pkg->CashOutstanding*pkg->MarginRatio;
	pacc->mitemEquity.mOpenEQ=pkg->StartCash;
	pacc->mitemEquity.mCurrEQ=pkg->EndCash;  
	
	theCfgFile.mlog.AddLogStrFmt("equity:%s, openbp:%g currbp:%g",pkg->UserID, pacc->mitemEquity.mOpenBP,pacc->mitemEquity.mCurrBP);


	char pkgret[200];
	pacc->MakePkgByEquity(pkgret,0xffff);
	Trader::SPKGAcc(pkgret,pacc->mitemifo.maccid,pacc->mitemifo.mbrid,0);  
	
	return 0;
}

int OrderRouter::PkgAccountPosition(char *pkgbuff)
{
	Order_Account_Position *pkg;
	int pkglen=*(WORD*)pkgbuff;
	pkg=(Order_Account_Position*)(pkgbuff+4);


	Account *pacc=Account::FindAccByName(pkg->UserID); 
	if (pacc==0) {
		theCfgFile.mlog.AddLogStrFmt("Error: Account Balance: Invalid Account %s ",pkg->UserID );
		return 0;
	}
/*
	if (pkg->PositionIdx==1) {
		intitem *pintitem;
		itemPosition *pitem; 
		intlst *plst;
		plst=&pacc->mLstPositions;
		plst->LockLst();
		plst->EnumFirst(&pintitem);
		while(pintitem!=NULL) {
			pitem=(itemPosition*)pintitem->val;
			pitem->mInitQty=0; 
			plst->EnumNext(&pintitem); 
		}
		plst->UnlockLst();
	}
	
	int ntype;
	int share;
	share=pkg->LongQty-pkg->ShortQty;
	if (share!=0) {
		if (share>0) ntype=2;  
		else ntype=3;
		
		itemPosition *pitem;
		pitem=pacc->FindPosition(pkg->Symbol,ntype);
		if (pitem==NULL) {
			time(&pitem->mc_date);
			pitem->mm_date=item.mc_date;
			pitem->mInitQty=share;
			pitem->mInitPrice=pkg->AvgPrice;
			pitem->m
			flag=0xffff;

		}else {
			pitem->mInitQty= 
			int incr;
			incr=pitem->mInitQty-item.mInitQty;
			pacc->ModifyPosition(&item,flag);
			item.ModifyDB(flag);

	}
*/
	theCfgFile.mlog.AddLogStrFmt("position :%s, %s long:%d, short %d,avgcost:%g",pkg->UserID,pkg->Symbol,pkg->LongQty,pkg->ShortQty,pkg->AvgPrice);

	return 0;

}
