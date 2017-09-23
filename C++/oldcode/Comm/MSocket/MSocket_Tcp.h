// CClient.h: interface for the CClient class.

#ifndef _MSOCKET_TCP__H__
#define _MSOCKET_TCP__H__

#pragma once

#include "Msocket.h"
#include "../functions/pkgbuff.h"

//this class should dynamically allocated and will be deleted at on Finish.
class Msocket_tcp: public Msocket  
{
protected:
	dabfchain mChainOutBuf;// only for count buffer;
	dabfchain mChainInBuf; // the Incoming buffer;
	dabuff *mInBuf;
	dabuff *mOutBuf;

	long m_LastConnectTicker;// for the autoconnect,  last ticker 
public:
	BOOL mbAutoConnect;
	BOOL mbShutdowAfterSend;
	long m_total_bytes_recv;
	long m_total_bytes_send;
public:
	//the overloaded function provide ip and port;
	virtual BOOL ConnectToServer();
	BOOL AutoConnect(BOOL bNewThread);

	void ResetBuffer();
	void SendPkg(char *ppkg) { mChainOutBuf.AddPkgBuff(ppkg); };

	int onWritting(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	int onReading(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
// 	int onClose();
	int onConnect(DWORD nErrno);

	virtual int ProcessPkg(char *pbuff);
	virtual int OnNoMoreDataToRead();//this function is for some process for idle process.

	Msocket_tcp();
	virtual ~Msocket_tcp();
};

#endif // !defined(AFX_FIXCLIENT_H__8D405733_FEBB_4857_97D4_A0D1C6E5451E__INCLUDED_)
