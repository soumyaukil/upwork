// Msocket.h: interface for the Msocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSOCKET_H__7A68A710_15F5_11D4_BC95_00105A82D2D1__INCLUDED_)
#define AFX_MSOCKET_H__7A68A710_15F5_11D4_BC95_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"mswsock.lib")
#endif // _MSC_VER > 1000
#include <winsock2.h>
#include "../functions/synchronal.h"
#include "..\FUNCTIONS\intmap.h"	// Added by ClassView
//#include <mswsock.h>

#define  enReading 1
#define  enWritting 2
#define  enAccepting 4
#define  enCloseing	 8  //close as soon as possiable.
#define  enShutdown 0x10 //close the socket after write the all pending package.
#define	 enReadPending 0x100
#define  enWritePending 0x200
#define  enAcceptPending 0x400
#define  enConnecting    0x800
#define  enConnected 0x10000 //the connection is setup.

enum enuMOVState{
	stVoid=0,
	stAccepting=1,
	stReading=2,
	stWritting=3,
	stConnecting=4
};

class Msocket;
struct M_OVBUF: public OVERLAPPED {
	/*  stVoid=0,
		stAccepting=1,
		stReading=2,
		stWritting=3,
		stConnecting=4	*/
	enuMOVState mOVstate;
	DWORD	mIObytes;
	Msocket * mpobj;
	void * mbufobj;
	char * mbuf;
	M_OVBUF()
		{ Internal = InternalHigh = Offset = OffsetHigh = 0; hEvent = 0; 
		  mOVstate=stVoid;
		  mIObytes=0;
		  mpobj=NULL;
		  mbufobj=NULL;
		  mbuf=NULL;
		}
};

class Msocket  
{
public:
	static long mMaxThreads;
	static long mCurrThreads;
	static intlst mSocklst;
	static long mMaintain;
	static BOOL mMaintainLoop;

private:
	static UINT WorkerProc( LPVOID pParam );
	static UINT MaintainProc(LPVOID pParam);
	static UINT ConnectingThread(LPVOID pParam);
	// enReading 1 enWritting 2 enAccepting 4 enCloseing
	// enShutdown 16
	long mstate;
	long mIssuedIO; // how many io we already issued.
	long mPrcessThreadNum; //this is the number that how many thread runs at the same time.
	long mAsyncflag;       //which will check the flag;
	void * mpAcceptBuff;
	long mReference;// how many reference to this client made.

	DWORD mLastCloseTicker;
	long mIsConnecting; //if this number is not zero, means it is in connecting status;

public:
	inline long RefInc() {return InterlockedIncrement(&mReference);};
	inline long RefDec() {return InterlockedDecrement(&mReference);};
	inline long GetIssuedIONum() { return mIssuedIO;};
	inline long GetReferenceNum() { return mReference;};
	inline long GetState() { return mstate;};
	inline DWORD GetPeerAddr() {return mrmoteSockAddr.sin_addr.S_un.S_addr;};

protected:
	M_OVBUF rov; //read ovl
	M_OVBUF wov; //write ovl
	DWORD mLastReadTick;
	DWORD mLastWriteTick;
	SOCKET mSocket; // the connection's socket handle
	WSABUF mInWSABUF;
	WSABUF mOutWSABUF;
	SOCKADDR_IN mInSockAddr;
	int mInAddlen;
	SOCKADDR_IN mOutSockAddr;
	int mOutAddlen;
	SOCKADDR_IN mrmoteSockAddr;

	BOOL m_bNoBlock;
	
	
protected:
	APICritical mCritical;
protected:
	virtual int onClose();
	virtual int onFinish();
	virtual int onWritting(DWORD nErrno,void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	virtual int onReading(DWORD nErrno,void * BuffObject, char * lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	//virtual function when connection setup,
	//happens in accept a new socket addr or connect a new location
	virtual int onConnecting(M_OVBUF *pOvl,SOCKADDR_IN *localaddr,SOCKADDR_IN *rmoteaddr);
	virtual int onConnect(DWORD nErrno);
	
	virtual int onAccept(DWORD nErrno,M_OVBUF * pOvl);
	//this function is right before DoIO;
	//usually used to process the data; this function is not Locked in the mCritical, use extreamly careful
	virtual int onDataProcessing();


	virtual BOOL OnCheckTimeOut(); //return TRUE if want the maintaince procedure to kick in to clean the connection;

public:
	//this function is not for call directly, only for thread call.
	int DoIO(M_OVBUF *pOvl,DWORD nerrno);
protected:
	int AcceptNew();
	int DoClose(BOOL force);
	int DoAccept(M_OVBUF * pOvl, Msocket * tmpsocket);
	int Recv(void *BuffObject, char *lpBuf, int bflen);
	int Send(void *BufObject, char *lpBuf, int bflen);

	int RecvFrom(void * BuffObject,char * lpBuf, int bflen);
	int SendTo(void *BufObject, char *lpBuf, int bflen, unsigned long nIp,unsigned short nPort);
public:
	void DoSend();
	//this is just check if the socket is connected;
	BOOL IsConnected(){return ((mSocket !=INVALID_SOCKET)&&(mstate&enConnected));}
	BOOL AddToList();
	BOOL RemoveFromList(BOOL bWait);
	BOOL IsShuttingDown();
	static int CountSocket();
	BOOL SetRcvBuffSZ(int bfsz);
	int GetRcvBuffSZ();
	static int M_SocketStartUp();
	static int M_SocketShutdown();
	static int M_StartThread();
	void AsyncSelect(long event=FD_READ|FD_WRITE);
	int DoShutdown();
	BOOL Attach( SOCKET hSocket);
	SOCKET GetSocketHandler() {return mSocket;};
	//this function is used to create an udp or tcp connection
	int Create(UINT nSocketPort = 0, int nSocketType = SOCK_STREAM,BOOL reuse=FALSE);
	//this function is used to listen an tcp port and waitting for a connection
	int ListenFrom(u_short portnu);
	//this is the function to connection;
	int ConnectTo(DWORD ip, u_short portnu);

	Msocket();
	virtual ~Msocket();

};


#endif // !defined(AFX_MSOCKET_H__7A68A710_15F5_11D4_BC95_00105A82D2D1__INCLUDED_)
