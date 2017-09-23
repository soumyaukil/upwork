// InfoPort.cpp: implementation of the InfoPort class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "orderserv.h"
#include "InfoPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "..\Define\UDPPkgs.h"
#include "OrderConfig.h"

InfoPort theInfoPort;
extern OrderConfig theCfgFile;

#include "account.h"
#include "MGMT\itemsDef.h"	// Added by ClassView

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

InfoPort::InfoPort()
{
	mInfoId=0;
	mOutIp=0;
	mOutPort=0;
	mInPort=0;


	mServerStatus=0;
	mCPUPrecent=0;
	mMEMPrecent=0;
	mLastTkCounter=0;

}

InfoPort::~InfoPort()
{

}

int InfoPort::Init()
{
	if (mSocket!=INVALID_SOCKET) {
		DoClose(TRUE);
		RemoveFromList(FALSE);
		Create(mInPort,SOCK_DGRAM,TRUE);
	}else {
		Create(mInPort,SOCK_DGRAM,TRUE);
	}

	AsyncSelect(FD_READ);

	mServerStatus=1;
	GetStatus();

	return 0;
}

int InfoPort::onReading(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject) { // there is some information readed;
		dabuff * currbuff;
		currbuff=(dabuff *) BuffObject;
		currbuff->bufbegin=0;
		currbuff->bufend=bflen; 
		char * pkgbuff;
		while((pkgbuff=currbuff->GetPkgBuff())!=NULL){
			WORD SrcId =*(WORD*) (pkgbuff+2);
			WORD pkgtype= *(WORD*)(pkgbuff+4);
			switch(pkgtype) {
			case 2:// balance 
				PkgBalance(pkgbuff);
				break;
			case 10://SendOrder
				PkgUdpOrder(pkgbuff);
				break;
			case 11://test package;
				theCfgFile.mlog.AddLogStr("Get an test message for the info port");
				break;
			}
/*			switch (pkgtype){
			case pkg_Itch_Add::PKGID://quote
				pkgAddOrder(pkgbuff);
				break;
			case pkg_Itch_Modify::PKGID://trade
				pkgModifyOrder(pkgbuff);
				break;
			}
*/
		} 
		dabuff::PutVoidDaBuff((dabuff*)BuffObject);	
	}
	dabuff *pbuff;
	pbuff=dabuff::GetVoidDaBuff();

	int i=RecvFrom((void*)pbuff,pbuff->DataBuff,BF_SZ); 
	if (i<0) {
		theCfgFile.mlog.AddLogStr("Info UDP port read Data Error!");
		i=RecvFrom((void*)pbuff,pbuff->DataBuff,BF_SZ); 
		if (i<0) {
			theCfgFile.mlog.AddLogStr("Read Info UDP Data Error again!-- please manual reset it at server-Feed UDP--Async");
		}

	}

//	theCfgFile.mOutputUdp.AsyncSelect(FD_WRITE);
	return 0;
}

int InfoPort::onWritting(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject) {
		dabuff::PutVoidDaBuff((dabuff*)BuffObject);
	}
	
	dabuff *mOutBuf;
	mCritical.Lock();
	if (wov.mOVstate==stVoid) {
		mOutBuf=mChainOutBuf.GetDabuff();
		if (mOutBuf==NULL) {
			mCritical.Unlock();
			return 0 ;
		}
		/*
		WORD bflen;
		int firstwaittingbflen;
		dabuff * ptmpdabuff;
		bflen=mOutBuf->bufend - mOutBuf->bufbegin ;
		ASSERT(bflen<BF_SZ);
		ASSERT(bflen>=0);
		while (1) {
			firstwaittingbflen=mChainOutBuf.PeekFirstPkgLen();
			if (firstwaittingbflen<0 ) break;
			if (firstwaittingbflen+bflen <BF_SZ) {
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
			}else {
				break;
			}
		}
		*/
		mChainOutBuf.FillPkg(mOutBuf);
		SendTo(mOutBuf,mOutBuf->GetBuffAddr() ,mOutBuf->GetPkgLen(),mOutIp, mOutPort);
		mOutBuf=NULL;
	}
	mCritical.Unlock();
	return 0;
}

int InfoPort::SendPkg(WORD ntype, LPCSTR pbuf, int nlen)
{
	if (mSocket==INVALID_SOCKET) {
		return 0;
	}
	if ((mChainOutBuf.GetBuffCount()<20)&&(mOutIp!=0))  { //don't block the messages;
		dabuff *pbuff;
		pbuff=dabuff::GetVoidDaBuff();
		*(WORD*)(pbuff->DataBuff+2)=(WORD)mInfoId;
		*(WORD*)(pbuff->DataBuff+4)=(WORD)ntype;
		memcpy(pbuff->DataBuff+6,pbuf,nlen);	
		*(WORD*)(pbuff->DataBuff)=nlen+6;
		pbuff->bufbegin=0;
		pbuff->bufend=nlen+6; 
		mChainOutBuf.PutDabuff(pbuff);
	}
	return 0;

}

int InfoPort::PkgBalance(LPCSTR buff)
{
	DWORD ip;
	WORD  port;
	ip=*(DWORD*)(buff+8);
	port=*(WORD*)(buff+12);
	theCfgFile.SetBalance(20,ip,port);
	SendPkg(2,buff+6,8);
	AsyncSelect(FD_WRITE);
	return 0;
}

void InfoPort::SendHeartBeat()
{
	UDP_HeartBeat pkg;
	pkg.status =mServerStatus;
	pkg.cpu=mCPUPrecent;
	pkg.mem=mMEMPrecent;
//	admnu=Trader::mOnlineAdmin.GetCounter();
//	tradnu=Trader::mOnlineTrader.GetCounter(); 

	pkg.UserNum= 0;
	SendPkg(UDP_HeartBeat::ID,(char*)&pkg,sizeof(pkg));
	AsyncSelect(FD_WRITE); 
}

void InfoPort::GetStatus()
{
	DWORD thistkcount;
	thistkcount=GetTickCount();
	if ((thistkcount>(mLastTkCounter+5000))||(thistkcount<mLastTkCounter)){
		mCPUPrecent=mCPUIfo.GetCPUPrecent();
		mMEMPrecent=mCPUIfo.GetMemPrecent(); 
		if ((mCPUPrecent>95)||(mMEMPrecent>90)) {
			mServerStatus|=(1<<2);
		}else if ((mCPUPrecent>80)||(mMEMPrecent>80)) {
			mServerStatus&=~(1<<2);
			mServerStatus|=(1<<1);
		}else {
			mServerStatus&=~((1<<1)|(1<<2));
		}
		mLastTkCounter=thistkcount;
		SendHeartBeat(); 
	}

}

void InfoPort::exit()
{
	mServerStatus=0;
	SendHeartBeat();

}

int InfoPort::PkgUdpOrder(LPCSTR pkgbuff)
{

	char symbol[100];
	char primexch;
	int  share;
	double price;
	char side;
	char account[20];
	char trader[20];
	char route[20];

	
	
	PkgToken tok;
	tok.InitToGet((char*)pkgbuff);
	if (tok.getWORD()!=55) {
		theCfgFile.mlog.AddLogStr("Wrong format");
	};
	tok.getWORD();//type 10
	tok.getString(symbol,sizeof(symbol));
	primexch=tok.getBYTE(); 
	side=tok.getBYTE();
	price=tok.getDouble();
	share=tok.getlong();
	tok.getString(trader,sizeof(trader));
	tok.getString(account,sizeof(account));
	tok.getString(route,sizeof(route));

	theCfgFile.mlog.AddLogStrFmtTm(0,"GetUdpOrder: %s %d %g %d %s %s %s",
		symbol,side,price,share,trader,account,route); 

	Trader *ptrader=Trader::FindTrader(trader); 
	if (ptrader==NULL) {
		theCfgFile.mlog.AddLogStr("Invalid Trader");
		return 0;
	}
	Account *pacc=Account::FindAccByName(account); 
	if (pacc==0) {
		theCfgFile.mlog.AddLogStr("Invalid Account");
		return 0;
	}

	itemOrder ord;
	BOOL ret;
	ord.Clear();
	ord.mtrid=ptrader->minfo.mtrid;
	ord.maccid=pacc->mitemifo.maccid;
	ord.maskprice=ord.mbidprice=price;
	strncpy(ord.msecsym,symbol,sizeof(ord.msecsym)-1);
	strncpy(ord.mroute,route,sizeof(ord.mroute)-1);
	ord.mprice=price;
	ord.mqty=share;
	ord.mentrid=ord.mtrid;
	ord.mbrid=pacc->mitemifo.mbrid;
	ord.mc_date=time(NULL);
	ord.mm_date=ord.mc_date;
	ord.mexchange=primexch; 
	ord.mtmforce=0; //send as IOC order;
	

	if (side=='B') ord.mstatus|=(1<<6);
	ord.mstatus|=(1<<14); //send as invisible;
	if(pacc->mitemifo.IsTraining()) ord.mstatus|=(1<<24);
	
	
//	CClient *pclient;
//	pclient=Trader::FindRefClient(ord.mentrid); 

//	ret=ord.NewOrder(0xffffffff,(char*)pclient);
	char msg[200];
	ret=ord.NewOrder(0xffffffff,NULL,msg);
//	if (pclient) pclient->RefDec();



	if (ret){
		theCfgFile.mlog.AddLogStr("Send UdpOrder Successed!");
		theCfgFile.mlog.AddLogStrFmt("%%E,R,%d,%d",ord.morderid,ord.mc_date); 
	}else{
		theCfgFile.mlog.AddLogStrFmt("Send UdpOrder Fail:%s!",msg);
	}
	return 0;
}

//this is auto
void InfoPort::ResetPort()
{
	char str[500];
	sprintf(str,"io:%d,ref:%d\n"
		"Shut:%s,Close:%s\n"
		"Async?",GetIssuedIONum(),GetReferenceNum(),
		((GetState()&enShutdown)?"Y":"N"),
		((GetState()&enCloseing)?"Y":"N"));
	if(AfxMessageBox(str,MB_OKCANCEL)==IDOK){
		AsyncSelect(FD_READ); 
	};
	return ;

}
