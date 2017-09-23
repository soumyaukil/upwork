#include "stdafx.h"
#include "Msocket_Tcp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Msocket_tcp::Msocket_tcp()
{
	mbShutdowAfterSend=FALSE;
	mInBuf=NULL;
	mOutBuf=NULL;
	mbAutoConnect=FALSE;
	m_LastConnectTicker=0; 
	m_total_bytes_recv=0;
	m_total_bytes_send=0;
	m_bNoBlock=TRUE;
}

Msocket_tcp::~Msocket_tcp()
{
	ResetBuffer();
}

int Msocket_tcp::onConnect(DWORD nErrno)
{
	if (nErrno==0) {
		AsyncSelect(FD_READ);
	}
	return 0;

}

int Msocket_tcp::onReading(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	dabuff *pbuff;
	if (BuffObject!=NULL) {
		dabuff *pbuff=(dabuff *)BuffObject; 
		if (nErrno) {
//			theApp.mlog.AddLogStrFmt("SocketReadErr:err:%d,ref:%d",nErrno,GetReferenceNum());;
			DoClose(TRUE);
			return 0;

		}
		if (bflen==0){ //Close
			DoClose(TRUE);
			return 0;
		}
		m_total_bytes_recv+=bflen;
		pbuff->bufend+=(WORD)bflen;
		ASSERT(pbuff->bufend<=BF_SZ); 
		mChainInBuf.PutDabuff(pbuff);
	}		
	if (mSocket==INVALID_SOCKET) return -1;

//*****************************
//	Read All the socket before issue the io pending 
	pbuff=NULL;
	while (1){
		int ret;
		pbuff=dabuff::GetVoidDaBuff(); 
		DWORD nbytes=0;
		DWORD socflag=0;
		mInWSABUF.buf=pbuff->DataBuff;
		mInWSABUF.len= BF_SZ;
		ret=WSARecv(mSocket,&mInWSABUF,1,&nbytes,&socflag, NULL,NULL);
		if (ret<0) break;
		if (nbytes==0) break;
		m_total_bytes_recv+=nbytes;
		pbuff->bufend+=(WORD)nbytes;
		ASSERT(pbuff->bufend<=BF_SZ); 
		mChainInBuf.PutDabuff(pbuff);
		pbuff=NULL;
	}

//*****************************/
// the pbuff already in the buffer;
	int ret=Recv(pbuff,pbuff->DataBuff ,BF_SZ); 
	if(ret==-1) {//read error, close the 
		DoClose(TRUE);
	}else {
	}
	return 0;
}

int Msocket_tcp::ProcessPkg(char *pbuff)
{
/*
	switch(*(WORD*)(pbuff+2)){
	case 1://logon;
//		pkgLogon(pbuff);
		break;

	}
*/
	return 0;
}

int Msocket_tcp::OnNoMoreDataToRead()
{
	return 0;
}


int Msocket_tcp::onWritting(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	if (BuffObject!=NULL){ //buffobject is null is put pending write
		if (nErrno) {
//			theApp.mlog.AddLogStrFmt("SocketWriteErr:%d",nErrno);
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
		m_total_bytes_send+=bflen; 
		ASSERT(mOutBuf->bufbegin <=mOutBuf->bufend); 
		if (mOutBuf->bufbegin==mOutBuf->bufend){
			dabuff::PutVoidDaBuff(mOutBuf);
			mOutBuf=NULL;
		}
	}
	if (mSocket==INVALID_SOCKET) return -1;
//*****************************
//	Send All the socket before issue the io pending 
	{
		int ret;
		if(mOutBuf==NULL) mOutBuf=mChainOutBuf.GetDabuff();
		while(mOutBuf) {
			DWORD nbytes=0;
			DWORD socflag=0;
			mOutWSABUF.buf=mOutBuf->GetBuffAddr();
			mOutWSABUF.len= mOutBuf->GetPkgLen();
			ret=WSASend (mSocket,&mOutWSABUF,1,&nbytes,socflag,NULL,NULL);
			if (ret<0) break;
			if (nbytes==0) break;
			mOutBuf->bufbegin+=(WORD)nbytes;
			m_total_bytes_send+=bflen; 
			if (mOutBuf->bufbegin==mOutBuf->bufend){
				dabuff::PutVoidDaBuff(mOutBuf);
				mOutBuf=mChainOutBuf.GetDabuff();
			}
		}; 
	}

//*****************************/

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

//DEL int Msocket_tcp::PutPendingRead()
//DEL {
//DEL }

//DEL int Msocket_tcp::PutPendingWrite()
//DEL {
//DEL 
//DEL }

//DEL int Msocket_tcp::onFinish()
//DEL {
//DEL 	delete this;
//DEL 	return 0;
//DEL }

//DEL int Msocket_tcp::pkgLogon(char *pbuff)
//DEL {
//DEL /*	PkgToken tok;
//DEL 	tok.InitToGet(pbuff,4);
//DEL 	char username[12];
//DEL 	char password[20];
//DEL 	int  userid=0;
//DEL 	tok.getString(username,sizeof(username));
//DEL 	tok.getString(password,sizeof(password));
//DEL 
//DEL 
//DEL 	intitem *PI=NULL;
//DEL 	itemLogon *plogon;
//DEL 	while(theApp.m_LogonSet.Enum(PI)) {
//DEL 		plogon=(itemLogon*)PI->val;
//DEL 		if((stricmp(plogon->m_name,username)==0)&&(strcmp(password,plogon->m_pwd)==0)){
//DEL 			userid=plogon->m_id; 
//DEL 			break;
//DEL 		}
//DEL 	}
//DEL 	char retpkg[200];
//DEL 	PkgToken tokret;
//DEL 	tokret.InitToSet(retpkg);
//DEL 	tokret.setWORD(2);//login return;
//DEL 	
//DEL 	if (PI==NULL) {//wrong password;
//DEL 		tokret.setBYTE(0);
//DEL 		tokret.setVarStr("Invalid User/Pwd");
//DEL 		SendPkg(tokret.finalizeSet());
//DEL 		mbShutdowAfterSend=TRUE;
//DEL 		AsyncSelect(FD_WRITE);
//DEL 		return 0;
//DEL 	}
//DEL 
//DEL 	theApp.m_ClientMap.LockMap(); 
//DEL 	intitem *PIOLD;
//DEL 	PIOLD=theApp.m_ClientMap.FindItem(userid);
//DEL 	if (PIOLD) {
//DEL 		Msocket_tcp *pcl=(Msocket_tcp *)PIOLD->val;
//DEL 		pcl->mbShutdowAfterSend=TRUE;
//DEL 		theApp.m_ClientMap.Remove(userid);
//DEL 		pcl->RefDec();
//DEL 		theApp.mlog.AddLogStrFmtTm(0,"the user %d has been replaced!",plogon->m_name);  
//DEL 	}
//DEL 	theApp.m_ClientMap.Add(userid,(long)this);
//DEL 	m_UserID=userid;
//DEL 	RefInc(); 
//DEL 	theApp.m_ClientMap.UnlockMap();
//DEL 	
//DEL 	tokret.setBYTE(1);
//DEL 	tokret.setlong(userid);
//DEL 	tokret.setlong(theApp.m_ServerID); 
//DEL 	SendPkg(tokret.finalizeSet());
//DEL 	AsyncSelect(FD_WRITE);
//DEL */
//DEL 	return 0;
//DEL }

void Msocket_tcp::ResetBuffer()
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
		dabuff::PutVoidDaBuff(tmpbuf); 
	}
	mCritical.Unlock(); 
}

UINT do_connect(LPVOID data)
{
	Msocket_tcp *ptcp=(Msocket_tcp *)data;
	ptcp->ConnectToServer();
	Sleep(2000);
	return 0;
}

BOOL Msocket_tcp::AutoConnect(BOOL bNewThread)
{
	if (mbAutoConnect) {
		if (!IsConnected()) {
			DWORD tr=GetTickCount();
			if ((tr-m_LastConnectTicker)>10000) {
				m_LastConnectTicker=tr;
				if (bNewThread) {
					AfxBeginThread(do_connect,this);
				}else {
					ConnectToServer();
				}
			}
		}
	}
	return TRUE;
}

BOOL Msocket_tcp::ConnectToServer()
{
	return TRUE;
}
