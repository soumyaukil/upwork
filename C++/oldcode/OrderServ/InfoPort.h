// InfoPort.h: interface for the InfoPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INFOPORT_H__AA2BD412_E5B1_11D5_BD9E_0050041B8FEC__INCLUDED_)
#define AFX_INFOPORT_H__AA2BD412_E5B1_11D5_BD9E_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MSOCKET\Msocket.h"
#include "Functions\PkgBuff.h"
#include "FUNCTIONS\CPUIfo.h"	// Added by ClassView

class InfoPort : public Msocket  
{
	dabfchain mChainOutBuf;
public:
	int   mInfoId;
	DWORD mOutIp;
	WORD  mOutPort;
	DWORD mInPort;


	int mServerStatus;
	int mCPUPrecent;
	int mMEMPrecent;
	DWORD mLastTkCounter;

public:
	void ResetPort();
	int PkgUdpOrder(LPCSTR pkgbuff);
	void exit();
	void GetStatus();
	void SendHeartBeat();
	CPUIfo mCPUIfo;
	int SendPkg(WORD ntype, LPCSTR pbuf, int nlen);
	int Init();
	InfoPort();
	virtual ~InfoPort();

private:
	int PkgBalance(LPCSTR buff);
	virtual int onWritting(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
	virtual int onReading(DWORD nErrno, void *BuffObject, char *lpBuf, int bflen, unsigned long nIp, unsigned short nPort);
};

#endif // !defined(AFX_INFOPORT_H__AA2BD412_E5B1_11D5_BD9E_0050041B8FEC__INCLUDED_)
