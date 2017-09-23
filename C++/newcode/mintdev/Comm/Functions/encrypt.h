// encrypt.h: interface for the encrypt class.
//
//////////////////////////////////////////////////////////////////////

#include <stdint.h>
#include <time.h>

#if !defined(AFX_ENCRYPT_H__BC029E31_6BDD_11D5_BD50_0050041B8FEC__INCLUDED_)
#define AFX_ENCRYPT_H__BC029E31_6BDD_11D5_BD50_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class encrypt  
{
public:
	char mProgName[20];
	time_t mValidTo;
	int  mSysID;
	char mGiveUp[10];

public:
	bool MakeDASLicense(char *pstr);
	int ToBinary(char *psrc, unsigned char *pdesc);
	bool ToASCII(unsigned char *psrc,int srclen, char *pdesc);
	bool DecryptLicenseSt(char *psrc, char *pdec, uint32_t sysid);
	bool EncryptLicenseSt(char *psrc, char *pdec,uint32_t sysid);
	static uint32_t GetSysID(bool bShowID=false);
	bool EncryptPWD(unsigned char *pwd, unsigned char *result);
	bool DecryptPWD(unsigned char *cpwd, unsigned char *pwd);
	encrypt();
	virtual ~encrypt();

};

#endif // !defined(AFX_ENCRYPT_H__BC029E31_6BDD_11D5_BD50_0050041B8FEC__INCLUDED_)
