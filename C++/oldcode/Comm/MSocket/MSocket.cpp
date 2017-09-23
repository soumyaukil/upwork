// Msocket.cpp: implementation of the Msocket class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Msocket.h"
#include <Mswsock.h>
#include <process.h>

#define SocketAddrLen	(sizeof(sockaddr_in)+64)

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
static HANDLE M_hIocps=NULL;
static long M_ThreadNum=0;
static long M_socketInited=0;
static long M_CurrThreadNum=8; //concurrent thread number; don't include threads;

long Msocket::mMaxThreads=10;	// max threads in system we should maintain;
long Msocket::mCurrThreads=0;	//current active threads; include wait thread
intlst Msocket::mSocklst;
long Msocket::mMaintain=0;
BOOL Msocket::mMaintainLoop=TRUE;

static long M_PendingWriteNum=0;
static long M_PendingReadNum=0;
/*
static CCriticalSection M_Critical;
*/
/*int M_SocketStartUp()
{   
	if (InterlockedIncrement(&M_socketInited) !=1) {
		InterlockedDecrement(&M_socketInited); 
		return -1;
	}
	WSADATA wd = { 0 };
	int errno;
	errno = WSAStartup( MAKEWORD( 2, 0 ), &wd );
	if (errno!=0) {
		AfxMessageBox("Start NetWork Service failed");
		InterlockedDecrement(&M_socketInited); 
		exit(-1);
	}
	if ( LOBYTE( wd.wVersion ) < 2) {
		AfxMessageBox("This program need the WinSocket 2.0!");
		InterlockedDecrement(&M_socketInited); 
		exit(-1);
	}
	M_hIocps =0;
	M_hIocps = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, M_CurrThreadNum);
	if (M_hIocps ==0 ) {
		return -1;
	}else {
//		StartTheard(); 
	}
	return 0;

}
*/

int Msocket::M_SocketShutdown()
{
	int waitsec;
	waitsec=1000;
	int i;
	
	intitem *PIitem;
	Msocket *psoc;
	mSocklst.LockLst();
	mSocklst.EnumFirst(&PIitem);
	while(PIitem) {
		psoc=(Msocket *)PIitem->val;
		if (psoc) {
			if (psoc->mSocket!=INVALID_SOCKET) {
				psoc->DoClose(TRUE); 
			}
		}
		mSocklst.EnumNext(&PIitem);
	}
	mSocklst.UnlockLst();

	for (i=0;i<M_ThreadNum;i++) {
		PostQueuedCompletionStatus( M_hIocps, 0, INVALID_SOCKET, 0 );
	}
	mMaintainLoop=FALSE; 
	while( waitsec>0) {
		Sleep(1);
		if (0==M_ThreadNum) break;
		waitsec--;
	}
	waitsec=1000;
	while( waitsec>0) {
		Sleep(1);
		if (0==mMaintain) break;
		waitsec--;
	}
	
	if (M_hIocps!=0) {
		CloseHandle(M_hIocps );
		M_hIocps=0;
	}

	if (InterlockedDecrement(&M_socketInited)==0) { 
		WSACleanup();
	}else {
		InterlockedIncrement(&M_socketInited);
	}
	return 0;
}

UINT Msocket::MaintainProc(LPVOID pParam)
{
	BOOL LoopFlag=TRUE;
	intitem *PIitem;
	Msocket *psoc;
	DWORD thisticker;
	
	if (InterlockedIncrement(&mMaintain)>=2) {// only one thread allowed.
		InterlockedDecrement(&mMaintain);
		return 0;
	};
	while(mMaintainLoop) {
		mSocklst.LockLst();
		mSocklst.EnumFirst(&PIitem);
		while(PIitem) {
			psoc=(Msocket *)PIitem->val;
			if (psoc) {
				if (psoc->mCritical.TryLock()){ 
					thisticker=GetTickCount();
					if(thisticker<psoc->mLastCloseTicker){
						psoc->mLastCloseTicker=thisticker;
					}
					if (psoc->mstate& enCloseing){
						if ((thisticker-psoc->mLastCloseTicker)>60000){
							if (psoc->mSocket!=INVALID_SOCKET) {
								if(closesocket(psoc->mSocket)==0){;
									psoc->mSocket=INVALID_SOCKET;
									psoc->onClose();
									psoc->mLastCloseTicker=thisticker;
								}
								psoc->mCritical.Unlock();
								mSocklst.EnumNext(&PIitem);
								continue;
							}
						};
						if ((thisticker-psoc->mLastCloseTicker)>120000) {
							if (psoc->GetReferenceNum()==0) {
								if (psoc->mIssuedIO==0) {
									psoc->onClose();
									mSocklst.RemoveItem(&PIitem); 
									psoc->mCritical.Unlock();
									psoc->onFinish();
									continue;
								}
							}
						}
					}else if (psoc->mstate & enShutdown){
						if ((thisticker-psoc->mLastCloseTicker)>10000) {
							if (psoc->mSocket!=INVALID_SOCKET) {
								psoc->onClose();
								shutdown(psoc->mSocket,SD_BOTH);
							}
							psoc->mstate &= enCloseing;
							psoc->mLastCloseTicker=thisticker;
						};
					}else if (psoc->OnCheckTimeOut()) {
						psoc->DoClose(FALSE); 
					}
					psoc->mCritical.Unlock();
				}// try lock 
			}
			mSocklst.EnumNext(&PIitem);
		}// end of while loop;
		mSocklst.UnlockLst();
		Sleep(10000);
	}
	InterlockedDecrement(&mMaintain);
	return 0;
}


UINT Msocket::WorkerProc( LPVOID pParam )
{
	InterlockedIncrement(&M_ThreadNum);
	M_OVBUF  *pOvl;
	DWORD nBytes;
	ULONG 	iokey;
	BOOL ret;
	BOOL LoopFlag=TRUE;
	while (LoopFlag==TRUE) {// Do the loop work
		nBytes =0;
		pOvl=NULL;
		InterlockedIncrement(&mCurrThreads);
		ret=GetQueuedCompletionStatus(M_hIocps,&nBytes,&iokey,(OVERLAPPED **)&pOvl,30000);//1 minute we check if it is timeout;
		if ((InterlockedDecrement(&mCurrThreads)==0)&&(M_ThreadNum<mMaxThreads)){ //dynamic add new threads;
//			Msocket::M_StartThread();
		};  
		if (iokey==INVALID_SOCKET) break;
		if (pOvl !=NULL) pOvl->mIObytes =nBytes;
		if ( ret == FALSE ) {// something gone awry?
			DWORD errnumber;
			errnumber=GetLastError();
			if ( pOvl == NULL ){ // the IOCP op failed
				if (errnumber==WAIT_TIMEOUT) {
					if (mCurrThreads>mMaxThreads) {
// don't automatically add new
//						LoopFlag=FALSE;
//						break;
					}
				}else {
/*					LPCSTR lpMsgBuf;
					FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						errnumber,
						0, // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
					);
//					AfxMessageBox(lpMsgBuf);
					LocalFree((LPVOID) lpMsgBuf );
*/				}
			} else{
				// key, byte count, and overlapped are valid.
				// tear down the connection and requeue the socket
				//AfxMessageBox("Error!!!, ERROR IO Completion Status, return false");
				pOvl->mpobj->DoIO(pOvl,errnumber);				
				//DoClose( *pov, true );
			}
		}else{ // GQCS() returned TRUE
			// since we are still here, we have an I/O to process
			// n is redundant -- see pov->InternalHigh
			pOvl->mpobj->DoIO(pOvl,0);
		}
	} // end of loop

	InterlockedDecrement(&M_ThreadNum);
	return 0;
};


int Msocket::M_StartThread()
{

	CWinThread *NewThread =AfxBeginThread(WorkerProc, NULL);
	return 0;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Msocket::Msocket()
{
	mstate=0;
	mPrcessThreadNum=0;
	mSocket=INVALID_SOCKET;
	rov.mpobj= this;
	wov.mpobj= this;
	
	mpAcceptBuff=NULL;
	mAsyncflag=0;
	mLastReadTick=0;
	mLastWriteTick=0;
	mIssuedIO=0;
	mLastCloseTicker=0;
	mReference=0; 
	mIsConnecting=0;
	m_bNoBlock=FALSE;
}

Msocket::~Msocket()
{
	if (mpAcceptBuff!=NULL) {
		free(mpAcceptBuff);
	}

}

int Msocket::Create(UINT nSocketPort, int nSocketType,BOOL reuse)
{

/*	if (mSocket!=INVALID_SOCKET) {
		
	}
*/
	mstate=0;
	ASSERT(mSocket==INVALID_SOCKET);
	mSocket=socket(AF_INET,nSocketType,0);
	SOCKADDR_IN addr;
	int eno;
	ASSERT(mSocket!=INVALID_SOCKET);

	memset(&addr,0,sizeof(SOCKADDR_IN));

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons( (short) nSocketPort);
	if (reuse==TRUE) {
		int bo;
		bo=TRUE;
		setsockopt(mSocket,SOL_SOCKET,SO_REUSEADDR,(char*)&bo,sizeof(bo)); 
	}

	eno = bind( mSocket, (SOCKADDR *) &addr, sizeof addr );
	if ( eno  == SOCKET_ERROR) {
		AfxMessageBox( "bind() to port error ");
		return -1;
	};

	CreateIoCompletionPort( (HANDLE) mSocket, M_hIocps,mSocket, M_CurrThreadNum  );
	AddToList();
//	mSocklst.AddTail(0,(int)this); 
	return 0;
}


BOOL Msocket::Attach(SOCKET hSocket)
{
	ASSERT(mSocket==INVALID_SOCKET);
	ASSERT(hSocket!=INVALID_SOCKET);
	mSocket=hSocket;
	CreateIoCompletionPort( (HANDLE) mSocket, M_hIocps,mSocket, M_CurrThreadNum  );
	AddToList();
//	mSocklst.AddTail(0,(int)this);  
	return 0;

}

int Msocket::SendTo(void *BufObject, char *lpBuf, int bflen, unsigned long nIp,unsigned short nPort)
{
	int fret=0;
	RefInc();
	mCritical.Lock();
	
	if (mSocket==INVALID_SOCKET){
		fret=-1;
	}else if (((mstate & (enCloseing|enShutdown))==0)&&(wov.mOVstate==stVoid)){
		wov.mOVstate=stWritting;
		wov.mbufobj=BufObject;
		wov.mbuf= lpBuf;
		
		int ret, errno;
		DWORD socflag;
		socflag=0;
		
		mOutSockAddr.sin_family =AF_INET;
		mOutSockAddr.sin_addr.S_un.S_addr=nIp;
		mOutSockAddr.sin_port=ntohs (nPort); 
		mOutAddlen= sizeof(mOutSockAddr);
		mOutWSABUF.buf=lpBuf;
		mOutWSABUF.len= bflen;
		RefInc();
		InterlockedIncrement(&M_PendingWriteNum);  
		ret=WSASendTo(mSocket,&mOutWSABUF,1,&wov.mIObytes,socflag,(SOCKADDR*)&mOutSockAddr,mOutAddlen , &wov,NULL);
		InterlockedIncrement(&mIssuedIO);
		if(ret==SOCKET_ERROR){
			errno=WSAGetLastError();
			if(errno==WSA_IO_PENDING){
					mstate|=enWritePending;
					mLastWriteTick=GetTickCount();
			}else{
				wov.mOVstate=stVoid;
				wov.mbufobj=NULL;
				wov.mbuf= NULL;
				if (errno!=WSAEWOULDBLOCK){
					InterlockedDecrement(&mIssuedIO);
					RefDec();
					fret=-1;
				}else {
					InterlockedDecrement(&mIssuedIO);
					RefDec();
					fret=-1;
				}
			};
		}else{
			mLastWriteTick=GetTickCount(); 
			mstate|=enWritePending;
		};
	}else{
		fret=-1;
	};
	mCritical.Unlock();
	RefDec();
	return fret;
}

int Msocket::RecvFrom(void *BuffObject, char *lpBuf, int bflen)
{
	int fret=0;
	RefInc();
	mCritical.Lock();
	if (mSocket==INVALID_SOCKET){
		fret=-1;
	}if (((mstate & enCloseing)==0) &&(rov.mOVstate==stVoid))  {
		rov.mOVstate=stReading;
		rov.mbufobj=BuffObject;
		rov.mbuf=lpBuf; 
		int ret,errno;
		DWORD socflag;
		socflag=0;
		mInWSABUF.buf=lpBuf;
		mInWSABUF.len= bflen;
		mInAddlen=sizeof(mInSockAddr);
		mInSockAddr.sin_family=AF_INET;
		mInSockAddr.sin_addr.S_un.S_addr=INADDR_ANY; 
		mInSockAddr.sin_port=0; 
		RefInc();
		InterlockedIncrement(&M_PendingReadNum);
		ret=WSARecvFrom(mSocket,&mInWSABUF,1,&rov.mIObytes,&socflag,(SOCKADDR*)&mInSockAddr,&mInAddlen,&rov,NULL);
		InterlockedIncrement(&mIssuedIO);  
		if(ret==SOCKET_ERROR){
			errno=WSAGetLastError();
			if(errno==WSA_IO_PENDING){
				mLastReadTick=GetTickCount();
				mstate|=enReadPending;
			}else{
				rov.mOVstate=stVoid;
				rov.mbufobj=NULL;
				rov.mbuf=NULL; 
				if (errno!=WSAEWOULDBLOCK){
//					char str[100];
//					sprintf(str,"Read Data Error! errno is %d",errno);
//					AfxMessageBox(str);
					InterlockedDecrement(&mIssuedIO);
					RefDec();
					fret=-1;
				}else {
					InterlockedDecrement(&mIssuedIO);
					RefDec();
					fret=-1;
//					mLastReadTick=GetTickCount();
//					mstate|=enReadPending;
				}

			};
		}else{
			mLastReadTick=GetTickCount();
			mstate|=enReadPending;
	//			onReading(&rov);
		};

	} else {
		fret=-1;
	}
	mCritical.Unlock();
	RefDec();
	return fret;

}

int Msocket::DoIO(M_OVBUF *pOvl,DWORD nerrno)
{
	
	void *tmpbufobj;
	char *pbuf;
	int  buflen;
	unsigned int rip;
	unsigned short rport;
//	if(InterlockedIncrement(&mPrcessThreadNum)>=100000) return 0 ;
	int deleteflag;
	RefInc();

	deleteflag=FALSE;
	mCritical.Lock();
	switch(pOvl->mOVstate) {
	case stAccepting:
		mstate&=~enAcceptPending;
		if ((mstate & enCloseing)==0) {
	//		onAccept(pOvl);
		} else {
		}
		pOvl->mOVstate=stVoid;
		mstate |= enAccepting;
		onAccept(nerrno,pOvl);
		mstate &= ~enAccepting;
		break;
	case stReading:
		InterlockedDecrement(&M_PendingReadNum);
		mstate&=~enReadPending;
		mstate|=enReading;
		tmpbufobj=pOvl->mbufobj;
		pbuf=pOvl->mbuf;
		buflen=pOvl->mIObytes;
		rip= mInSockAddr.sin_addr.S_un.S_addr;
		rport=htons(mInSockAddr.sin_port); 
		pOvl->mOVstate=stVoid; 
		//mstate=mstate | enReading;
		mLastReadTick=0;
		onReading(nerrno,tmpbufobj,pbuf,buflen,rip,rport);
		mstate&=~enReading; 
		break;
	case stWritting:
		InterlockedDecrement(&M_PendingWriteNum);  
		mstate &=~enWritePending;
		mstate |=enWritting;
		tmpbufobj=pOvl->mbufobj;
		pbuf=pOvl->mbuf;
		buflen=pOvl->mIObytes;
		rip= mOutSockAddr.sin_addr.S_un.S_addr;
		rport=htons(mOutSockAddr.sin_port); 
		pOvl->mOVstate=stVoid; 
		mLastWriteTick=0; 
		onWritting(nerrno,tmpbufobj,pbuf,buflen,rip,rport);
		mstate&=~enWritting;
		break;
	case stConnecting:
		pOvl->mOVstate=stVoid;
		if (pOvl->mIObytes==0) {
			mstate|=enConnected;
			CreateIoCompletionPort( (HANDLE) mSocket, M_hIocps,mSocket, M_CurrThreadNum  );
			AddToList();
			mOutAddlen=sizeof(mOutSockAddr);
			getpeername(mSocket,(struct sockaddr *)&mOutSockAddr,&mOutAddlen);   
			mInAddlen=sizeof(mInSockAddr);
			getsockname(mSocket,(struct sockaddr *)&mInSockAddr,&mInAddlen); 
			onConnect(0); 
		}else {
			DoClose(TRUE);
			onConnect(-1); 
		}
		break;

/*	connect is not an overlape operation, so it is actived by aother
	function.
	case stConnecting:
		pOvl->mOVstate =stVoid;
	//	onConnecting(pOvl);
		break;*/
	};
//	if (InterlockedDecrement(&mPrcessThreadNum)==0) {
	if ((mstate & (enWritePending|enAcceptPending|enReadPending))==0) {
		if ((mstate & enConnected)){
			if ((mstate&(enShutdown|enCloseing))!=0) {
				DoClose(FALSE);
				mAsyncflag=0; 
				deleteflag=TRUE;
			}else if (mSocket==INVALID_SOCKET){
				deleteflag=TRUE;
			}
			mstate &=~enConnected;
		}
	}
	if (mAsyncflag){
		AsyncSelect(mAsyncflag);
	}
	mCritical.Unlock();
	InterlockedDecrement(&mIssuedIO); 
	RefDec();// for the io

	onDataProcessing(); 
	RefDec();
//	if (deleteflag&&(mIssuedIO==0)) {
//		onClose();
//	}
	return 0;
}



int Msocket::DoShutdown()
{
//	BOOL deleteflag=FALSE;
	RefInc();
	mCritical.Lock();
	mstate|=enShutdown; 
	mLastCloseTicker=GetTickCount(); 
/*	if ((mstate & (enWritePending|enAcceptPending|enReadPending|enWritting|enReading|enAccepting))==0) {
		if (mstate & enConnected){
				DoClose(FALSE);
				deleteflag=TRUE;
			mstate &=~enConnected;
		}
	}
*/	
	mCritical.Unlock();
	RefDec();

/*	if (deleteflag) {
		onClose();
	}
*/
//	CheckClose();
	return 0;
}

void Msocket::AsyncSelect(long event)
{
	RefInc();
	if (mCritical.TryLock()) {
		//Lock();

		if (event & FD_WRITE){
			if (wov.mOVstate ==stVoid) onWritting(0,NULL,NULL,0,0,0);
		}
		if (event & FD_READ) {
			if(rov.mOVstate==stVoid) onReading(0,NULL,NULL,0,0,0);
		}
		mAsyncflag=0;
		mCritical.Unlock();
//		CheckClose();
	}else {
		mAsyncflag|=event;
	} 
	RefDec();
}

int Msocket::M_SocketStartUp()
{
	if (InterlockedIncrement(&M_socketInited) !=1) {
		InterlockedDecrement(&M_socketInited); 
		return -1;
	}
	WSADATA wd = { 0 };
	int errno;
	errno = WSAStartup( MAKEWORD( 2, 0 ), &wd );
	if (errno!=0) {
		AfxMessageBox("Start NetWork Service failed");
		InterlockedDecrement(&M_socketInited); 
		exit(-1);
	}
//	{
//		char str[100];
//		sprintf(str,"%d,%d",wd.iMaxUdpDg,wd.iMaxSockets);
//		AfxMessageBox(str);
//	}
	if ( LOBYTE( wd.wVersion ) < 2) {
		AfxMessageBox("This program need the WinSocket 2.0!");
		InterlockedDecrement(&M_socketInited); 
		exit(-1);
	}
	M_hIocps =0;
	M_hIocps = CreateIoCompletionPort( INVALID_HANDLE_VALUE, 0, 0, M_CurrThreadNum);
	if (M_hIocps ==0 ) {
		return -1;
	}else {
//		StartTheard(); 
	}
	mMaintainLoop=TRUE;
	CWinThread *NewThread =AfxBeginThread(MaintainProc, NULL);
//	 _beginthread( MaintainProc, 0, NULL );

	return 0;

}

int Msocket::AcceptNew()
{
	if (IsShuttingDown()) return 0;
	if (mSocket==INVALID_SOCKET) return 0;
	
	int ret=0;
	if (mpAcceptBuff==NULL) {
		mpAcceptBuff=malloc(4098);
		ASSERT (mpAcceptBuff!=NULL);
	}
	
//	mstate|=enAccepting; 
	SOCKET tmpsocket;
	tmpsocket = socket( AF_INET, SOCK_STREAM, 0 );
	if ( tmpsocket== INVALID_SOCKET ){
		AfxMessageBox("Create New Socket error!");
		ret=-1;
	}else{
		BOOL ret;
		rov.mOVstate =stAccepting;
		*((SOCKET*)mpAcceptBuff)=tmpsocket;  
		ret=AcceptEx(mSocket ,tmpsocket,
				(void*)((char*)mpAcceptBuff+sizeof(SOCKET)),0, SocketAddrLen,SocketAddrLen,
				&(rov.mIObytes),&(rov));
		if (ret==FALSE) {
			int errno;
			errno=WSAGetLastError();
			if (errno==ERROR_IO_PENDING) {
				mstate|=enAcceptPending; 
			} else {
				AfxMessageBox("Accept new socket error!");
				ret=-1;
			}
		}else {
			mstate|=enAcceptPending; 
			//DoIO(&rov);
		}
	}
	return ret;
}


int Msocket::DoAccept(M_OVBUF *pOvl, Msocket *tmpsocket)
{
	ASSERT(tmpsocket!=NULL);
	ASSERT(mpAcceptBuff);
	
	int locallen,remotelen; 
	sockaddr_in *plocal = 0, *premote = 0;
	SOCKADDR_IN localSockAddr;
	SOCKADDR_IN rmoteSockAddr;

	tmpsocket->mSocket = *(SOCKET*)mpAcceptBuff; 
	GetAcceptExSockaddrs( (void*)((char*)mpAcceptBuff+sizeof(SOCKET)), 0,
		SocketAddrLen,SocketAddrLen, (sockaddr **) &plocal, &locallen,
		(sockaddr **) &premote, &remotelen );
	memcpy( &(localSockAddr), plocal, sizeof sockaddr_in );
	memcpy( &(rmoteSockAddr), premote, sizeof sockaddr_in );

	CreateIoCompletionPort( (HANDLE) tmpsocket->mSocket , M_hIocps,tmpsocket->mSocket, M_CurrThreadNum  );
//	we still need to associate the newly connected socket to outr IOCP:
//	pLiuSocket->AddClientToIOQueue(tmpclient);
	tmpsocket->mstate|=enConnected;
	tmpsocket->AddToList(); 
//	mSocklst.AddTail(0,(long)tmpsocket);
	tmpsocket->RefInc();
	tmpsocket->onConnecting(pOvl,&localSockAddr,&rmoteSockAddr);
	tmpsocket->onConnect(0);
	tmpsocket->RefDec();	
	return 0;
}



int Msocket::onConnecting(M_OVBUF *pOvl,SOCKADDR_IN *localaddr,SOCKADDR_IN *rmoteaddr)
{
	memcpy( &(mrmoteSockAddr), rmoteaddr, sizeof sockaddr_in );
	return 0;
}

int Msocket::onConnect(DWORD nErrno)
{
	return 0;
}


int Msocket::onReading(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	return 0;
}

int Msocket::onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort)
{
	return 0;
}

int Msocket::onAccept(DWORD nErrno,M_OVBUF *pOvl)
{
	return 0; 
} 

int Msocket::Send(void *BufObject, char *lpBuf, int bflen)
{	
	int fret=0;
	RefInc();
	mCritical.Lock();
	if (mSocket==INVALID_SOCKET){
		fret=-1;
	}else if (((mstate & (enCloseing|enShutdown))==0)&&(wov.mOVstate==stVoid)){
		wov.mOVstate=stWritting;
		wov.mbufobj=BufObject;
		wov.mbuf= lpBuf;
		
		int ret, errno;
		DWORD socflag;
		socflag=0;
		
/*		mOutSockAddr.sin_family =AF_INET;
		mOutSockAddr.sin_addr.S_un.S_addr=nIp;
		mOutSockAddr.sin_port=ntohs (nPort); 
		mOutAddlen= sizeof(mOutSockAddr);
*/
		mOutWSABUF.buf=lpBuf;
		mOutWSABUF.len= bflen;
		RefInc();
		InterlockedIncrement(&M_PendingWriteNum);  
		ret=WSASend(mSocket,&mOutWSABUF,1,&wov.mIObytes,socflag, &wov,NULL);//(SOCKADDR*)&mOutSockAddr,mOutAddlen );
		InterlockedIncrement(&mIssuedIO); 
		if(ret==SOCKET_ERROR){
			errno=WSAGetLastError();
			if(errno==WSA_IO_PENDING){
				mLastWriteTick=GetTickCount(); 
				mstate|=enWritePending;
			}else{
				wov.mOVstate=stVoid;
				wov.mbufobj=NULL;
				wov.mbuf= NULL;

				if (errno!=WSAEWOULDBLOCK){
					InterlockedDecrement(&mIssuedIO);  
					RefDec();
					fret=-1;
				}else {
					InterlockedDecrement(&mIssuedIO);  
					RefDec();
					fret=-1;
//					mLastWriteTick=GetTickCount(); 
//					mstate|=enWritePending;
				}
			};
		}else{
			mLastWriteTick=GetTickCount(); 
			mstate|=enWritePending;
//			onWritting(&wov);
		};
	}else{
		fret=-1;
	};
	mCritical.Unlock();
	RefDec();
	return fret;
}

//**************************************************************
//return -1 : if something is wrog;
//return >=0: Overlap IO has been submit;
//return 1  : Overlap IO has Finish, may have more data coming.
int Msocket::Recv(void *BuffObject, char *lpBuf, int bflen)
{
	RefInc();
	int fret=0;
	mCritical.Lock();
	if (mSocket==INVALID_SOCKET){
		fret=-1;
	}else if (((mstate & enCloseing)==0) &&(rov.mOVstate==stVoid))  {
		rov.mOVstate=stReading;
		rov.mbufobj=BuffObject;
		rov.mbuf=lpBuf; 
		int ret,errno;
		DWORD socflag;
		socflag=0;
		mInWSABUF.buf=lpBuf;
		mInWSABUF.len= bflen;
/*
		mInAddlen=sizeof(mInSockAddr);
		mInSockAddr.sin_family=AF_INET;
		mInSockAddr.sin_addr.S_un.S_addr=INADDR_ANY; 
		mInSockAddr.sin_port=0; 
*/
		RefInc(); // for the IO;
		InterlockedIncrement(&M_PendingReadNum);
		ret=WSARecv(mSocket,&mInWSABUF,1,&rov.mIObytes,&socflag, &rov,NULL);//(SOCKADDR*)&mInSockAddr,&mInAddlen,
		InterlockedIncrement(&mIssuedIO);  
		if(ret==SOCKET_ERROR){
			errno=WSAGetLastError();
			if(errno==WSA_IO_PENDING){
				mLastReadTick=GetTickCount();
				mstate|=enReadPending;
			}else{
				rov.mOVstate=stVoid;
				rov.mbufobj=NULL;
				rov.mbuf=NULL; 
				if (errno!=WSAEWOULDBLOCK){
					//too many outstanding overlap request;
					fret=-1;
					InterlockedDecrement(&mIssuedIO);
					RefDec(); // no io issued;
				}else {
					fret=-1;
					InterlockedDecrement(&mIssuedIO);
					RefDec(); // no io issued;
//					mLastReadTick=GetTickCount();
//					mstate|=enReadPending;
				}
			};
		}else{
			// the completion routine will have already been scheduled to be called once the calling thread is in the alertable state. 
			fret=1;
			mLastReadTick=GetTickCount();
			mstate|=enReadPending;
		};
	} else {
		fret=0;
	}
	mCritical.Unlock();
	RefDec();
	return fret;
}

int Msocket::DoClose(BOOL force)
{
	RefInc();
	mCritical.Lock();
	mstate=mstate|enShutdown;
	mLastCloseTicker=GetTickCount(); 
	if (mSocket==INVALID_SOCKET) {
	}else {
		struct linger li = { 0, 0 }; // default: SO_DONTLINGER
		mstate=mstate|enCloseing;
		if ( force )  {
			li.l_onoff = 1; // SO_LINGER, timeout = 0
			li.l_onoff = 0; 
			setsockopt( mSocket, SOL_SOCKET, SO_LINGER, (char *) &li, sizeof li );
			mstate=mstate|enCloseing;
			shutdown(mSocket,SD_BOTH);
			if(closesocket(mSocket)==0){
				mSocket =INVALID_SOCKET;
				onClose();
			}
		} else {
			li.l_onoff =	1; // SO_LINGER, timeout = 0
			li.l_linger=	1;
			BOOL bli;
			bli=TRUE;
			setsockopt( mSocket, SOL_SOCKET, SO_DONTLINGER,(char*) &bli, sizeof(bli) );
			shutdown(mSocket,SD_RECEIVE);
			if (closesocket(mSocket)==0){
				mSocket =INVALID_SOCKET;
				onClose();
			}
		}
	}
	mCritical.Unlock();
	RefDec();
	return 0;
}

int Msocket::ListenFrom(u_short portnu)
{
	int eno;
/*	if (mSocket!=INVALID_SOCKET) {
		Close();
	}
*/
	ASSERT(mSocket==INVALID_SOCKET);
	Create(portnu,SOCK_STREAM);
	eno = listen( mSocket, 200 );
	if ( eno == SOCKET_ERROR) {
		DWORD erno;
		erno=GetLastError(); 
		//		AfxMessageBox( "Listen Client error ");
		if (erno==WSANOTINITIALISED) {
			return -1;
		}
		return -1;
	}

	AcceptNew();
	return 0;
}

UINT Msocket::ConnectingThread(LPVOID pParam)
{
	Msocket *psock=(Msocket *)pParam;
	fd_set wfds;
	fd_set exceptfds;
	FD_ZERO(&wfds);
	FD_ZERO(&exceptfds);
	FD_SET(psock->GetSocketHandler(),&wfds);
	FD_SET(psock->GetSocketHandler(),&exceptfds);
	timeval tms;
	tms.tv_sec=200;
	tms.tv_usec=0;
	if (select(0,NULL,&wfds,&exceptfds,&tms)) {
		psock->wov.mOVstate =stConnecting;
		psock->wov.mpobj=psock; 

		if (FD_ISSET(psock->GetSocketHandler(),&wfds)) {//connected;
			int ret=PostQueuedCompletionStatus(M_hIocps, 0, psock->GetSocketHandler(),&psock->wov);
			if (ret==0) {
				int errnumber=GetLastError();
					LPCSTR lpMsgBuf;
					FormatMessage( 
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM | 
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						errnumber,
						0, // Default language
						(LPTSTR) &lpMsgBuf,
						0,
						NULL 
					);
					AfxMessageBox(lpMsgBuf);
					LocalFree((LPVOID) lpMsgBuf );

			}
		}else if (FD_ISSET(psock->GetSocketHandler(),&exceptfds)) {//Connection fail;
			PostQueuedCompletionStatus(M_hIocps, -1, psock->GetSocketHandler(),&psock->wov);
		}
	}else {
		//timeout, close this socket;
		PostQueuedCompletionStatus(M_hIocps, -1, psock->GetSocketHandler(),&psock->rov);
	}
	psock->mstate&=~enConnecting;
	psock->RefDec();
	InterlockedDecrement(&psock->mIsConnecting); 
	return TRUE;
}

int Msocket::ConnectTo(DWORD ip, u_short portnu)
{
	
	ASSERT(mSocket==INVALID_SOCKET);

	if (mstate&enConnecting) return -1;

//	Create(0,SOCK_STREAM);
	if (InterlockedIncrement(&mIsConnecting)>1) {
		InterlockedDecrement(&mIsConnecting);
		return -1;
	}
	mstate|=enConnecting;
	SOCKET tmpsock;
	mstate=0;
	tmpsock=socket(AF_INET,SOCK_STREAM,0);
	ASSERT(tmpsock!=INVALID_SOCKET);

	if (m_bNoBlock) {
		DWORD bnoblock=TRUE;
		int ret=ioctlsocket(tmpsock,FIONBIO,&bnoblock);
	}

	SOCKADDR_IN addr;
	addr.sin_family=AF_INET;
	addr.sin_addr.S_un.S_addr = ip;
	addr.sin_port=ntohs(portnu);
	int ret;
	ret=connect(tmpsock,(sockaddr*)&addr,sizeof(addr));
	if (ret==0){//connect successed;
			
		
		CreateIoCompletionPort( (HANDLE) tmpsock, M_hIocps,tmpsock, M_CurrThreadNum  );
		mSocket=tmpsock;
		InterlockedDecrement(&mIsConnecting); 
		AddToList();
		mOutAddlen=sizeof(mOutSockAddr);
		getpeername(mSocket,(struct sockaddr *)&mOutSockAddr,&mOutAddlen);   
		mInAddlen=sizeof(mInSockAddr);
		getsockname(mSocket,(struct sockaddr *)&mInSockAddr,&mInAddlen); 
		mstate|=enConnected;
		onConnecting(NULL,&mInSockAddr,&mOutSockAddr);
		onConnect(0); 
		mstate&=~enConnecting;


	}else {

		int errornumber;
		errornumber=WSAGetLastError();
		if (errornumber==WSAEWOULDBLOCK) {
			RefInc();
			mSocket=tmpsock;
			AfxBeginThread(ConnectingThread, this);
/*
			CreateIoCompletionPort( (HANDLE) tmpsock, M_hIocps,tmpsock, M_CurrThreadNum  );
			mSocket=tmpsock;
			InterlockedDecrement(&mIsConnecting); 
			AddToList();
			mOutAddlen=sizeof(mOutSockAddr);
			getpeername(mSocket,(struct sockaddr *)&mOutSockAddr,&mOutAddlen);   
			mInAddlen=sizeof(mInSockAddr);
			getsockname(mSocket,(struct sockaddr *)&mInSockAddr,&mInAddlen); 
			mstate|=enConnected;
			onConnect(0); 
*/
		}else {
			closesocket(tmpsock);
			InterlockedDecrement(&mIsConnecting); 
			onConnect(errornumber); 
			DoClose(TRUE);
			mstate&=~enConnecting;
			return -1;
		}
	}
	return 0;
}

//DEL BOOL Msocket::CheckClose()
//DEL {
//DEL 	if (mCritical.TryLock()){
//DEL //		mCritical.Lock();
//DEL 		BOOL deleteflag=FALSE;
//DEL 		if(mCritical.GetRecursionCount()==1){//if no other function 
//DEL 			if ((mstate & (enWritePending|enAcceptPending|enReadPending))==0) {
//DEL 				if ((mstate & enConnected)){
//DEL 					if ((mstate&(enShutdown|enCloseing)!=0)||(mSocket==INVALID_SOCKET)) {
//DEL 						if (mSocket!=INVALID_SOCKET) DoClose(FALSE);
//DEL 						deleteflag=TRUE;
//DEL 						mstate &=~enConnected;
//DEL 					} 
//DEL 				} 
//DEL 			}
//DEL 		}
//DEL 		mCritical.Unlock (); 
//DEL 		if (deleteflag&&(mIssuedIO==0)) {
//DEL 			onClose();
//DEL 		}
//DEL 	}
//DEL 	return TRUE;
//DEL }



//DEL int Msocket::GetSendBuff()
//DEL {
//DEL 
//DEL }

int Msocket::GetRcvBuffSZ()
{
	int optval;
	int optlen;
	optlen=sizeof(int);
	getsockopt(mSocket,SOL_SOCKET,
		SO_RCVBUF,
		(char*)&optval,   
		&optlen);
	return optval;



}

BOOL Msocket::SetRcvBuffSZ(int bfsz)
{
	int optval;
	int optlen;
	optval=bfsz;
	optlen=sizeof(int);
	setsockopt(mSocket,SOL_SOCKET,
		SO_RCVBUF,
		(char*)&optval,   
		optlen);
	return optval;

}


// this function tell the client release all the resource.

int Msocket::onClose()
{
	return 0;
}


// this finish virtual function is the last function we call;
// if the connection want to delete, it is ok.

int Msocket::onFinish()
{
	return 0;
}

int Msocket::onDataProcessing()
{
	return 0;
}


int Msocket::CountSocket()
{
	return mSocklst.GetCounter();
}

BOOL Msocket::IsShuttingDown()
{
	if (mstate & enShutdown) return TRUE;
	else return FALSE;
}

// this function is only used as want to close this socket as quick as we can;
BOOL Msocket::RemoveFromList(BOOL bWait)
{
	intitem *PIitem;
	mSocklst.LockLst();
	mSocklst.EnumFirst(&PIitem);
	Msocket *psoc;
	while(PIitem) {
		psoc=(Msocket *)PIitem->val;
		if (psoc==this) {
			if (psoc->mSocket!=INVALID_SOCKET) {
				if (closesocket(psoc->mSocket)==0){
					psoc->mSocket=INVALID_SOCKET;
					psoc->onClose(); 
				}
			}
			if (bWait) {
				int i;
				i=50;
				while(psoc->mIssuedIO!=0){
					if (i<=0) break;
					i--;
					Sleep(100);
				}
			}
			mSocklst.RemoveItem(&PIitem); 
//			in this sitituation, we don't issue the onFinish event;
//			if (psoc->mIssuedIO==0)// if not, we can't delete it;
//				psoc->onFinish();

			break;
		}
		mSocklst.EnumNext(&PIitem);
	}
	mSocklst.UnlockLst();
	return TRUE;
}

BOOL Msocket::AddToList()
{
	intitem *PIitem;
	BOOL badded;
	badded=FALSE;
	mSocklst.LockLst();
	mSocklst.EnumFirst(&PIitem);
	Msocket *psoc;
	while(PIitem) {
		psoc=(Msocket *)PIitem->val;
		if (psoc==this) {
			badded=TRUE;
			break;
		}
		mSocklst.EnumNext(&PIitem);
	}
	if (badded==FALSE) {
		mSocklst.AddTail(0,(long)this); 
	}
	mSocklst.UnlockLst();
	return TRUE;
}

void Msocket::DoSend()
{
	RefInc();
	if (mCritical.TryLock()) {
		if (wov.mOVstate ==stVoid) onWritting(0,NULL,NULL,0,0,0);
		mAsyncflag&=~FD_WRITE;
		mCritical.Unlock();
	}else {
		mAsyncflag|=FD_WRITE;
	} 
	RefDec();
}

BOOL Msocket::OnCheckTimeOut()
{
	return FALSE;
}
