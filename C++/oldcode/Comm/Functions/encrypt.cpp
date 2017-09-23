// encrypt.cpp: implementation of the encrypt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "encrypt.h"
#include "..\Security\md5.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static DWORD randnu(DWORD i){
	DWORD ret;
// y=(ax +b) mod m,  LCG(m,a,b,x0)
//LCG(3146757, 2098181, 3146245, 2776669)  small LCG
//LCG(2^32,2^7+1,907633385,0) is in turbo pascal
//LCG(10^8, 31415821, 1,0) classic
	ret=(31415821*i +1)% 100000000;
	return ret;
}


encrypt::encrypt()
{

}

encrypt::~encrypt()
{

}

//DEL BOOL encrypt::EncryptPWD(unsigned char *pwd, unsigned char *result)
//DEL {
//DEL 	if (pwd[0]==0) {
//DEL 		result[0]=0;
//DEL 		return TRUE;
//DEL 	}
//DEL 	memset(result,0,20);
//DEL 	int i;
//DEL 	int seed=0;
//DEL 	for (i=0;i<20;i++){
//DEL 		seed+=pwd[i];
//DEL 	}
//DEL 	seed=(seed%64)+32;
//DEL 	result[0]=seed;
//DEL 	int ch;
//DEL 	for (i=0;i<18;i++){
//DEL 		seed=randnu(seed); 
//DEL 		ch=pwd[i]-32;
//DEL 		ch+=seed%64;
//DEL //		if (ch>64) ch-=64;
//DEL 		ch+=32;
//DEL 		result[i+1]=(unsigned char)ch;
//DEL 	}
//DEL 	result[19]=0;
//DEL 	return TRUE;
//DEL }
  

//DEL BOOL encrypt::DecryptPWD(unsigned char *cpwd, unsigned char *pwd)
//DEL {
//DEL 	int seed;
//DEL 	int i;
//DEL 	seed=cpwd[0];
//DEL 	if (seed==0) {
//DEL 		pwd[0]=0;
//DEL 		return TRUE;
//DEL 	}
//DEL 	int ch;
//DEL 	for (i=0;i<18;i++){
//DEL 		seed=randnu(seed);
//DEL 		ch=(char)cpwd[i+1]-32;
//DEL 		ch-=seed%64;
//DEL //		if (ch<0) ch+=64;
//DEL 		ch+=32;
//DEL 		pwd[i]=ch;
//DEL 	}
//DEL 	pwd[19]=0;
//DEL 	return TRUE;
//DEL }
 

BOOL encrypt::DecryptPWD(unsigned char *cpwd, unsigned char *pwd)
{
	DWORD seed;
	int i;
	seed=cpwd[0];
	if (seed==0) {
		pwd[0]=0;
		return TRUE;
	}
	DWORD ch;
	for (i=0;i<18;i++){
		seed=randnu(seed);
		if (cpwd[i+1]==1) {
			ch=(char)0-32;
		}else 
			ch=(char)cpwd[i+1]-32;
		ch^=(seed%64)|0xe0;
//		if (ch<0) ch+=64;
		ch+=32;
		pwd[i]=(unsigned char)ch;
	}
	pwd[19]=0;
	return TRUE;
}

BOOL encrypt::EncryptPWD(unsigned char *pwd, unsigned char *result)
{
	if (pwd[0]==0) {
		result[0]=0;
		return TRUE;
	}
	memset(result,0,20);
	int i;
	DWORD seed=0;
	for (i=0;i<20;i++){
		seed+=pwd[i];
	}
	seed=(seed%64)+32;
	result[0]=(BYTE)seed;
	DWORD ch;
	for (i=0;i<18;i++){
		seed=randnu(seed); 
		ch=pwd[i]-32;
		ch^=(seed%64)|0xe0;
//		if (ch>64) ch-=64;
		ch+=32;
		if (ch==256) ch=1;
		result[i+1]=(unsigned char)ch;
	}
	result[19]=0;
	return TRUE;
}

DWORD encrypt::GetSysID(BOOL bShowID)
{
	DWORD ret;	
	DWORD diskserialnum;
	DWORD osserialnum;
	DWORD sysserialnum;
	char buff[100];
	DWORD maxlen;
	DWORD flag;
	char namebuff[100]; 
	maxlen=100;
	GetVolumeInformation("C:\\",buff,sizeof(buff),&diskserialnum,&maxlen,&flag,namebuff,sizeof(namebuff));
	
	OSVERSIONINFO osifo;
	osifo.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&osifo);
	osserialnum=(osifo.dwMajorVersion<<24)+(osifo.dwPlatformId<<16)+osifo.dwBuildNumber;

	SYSTEM_INFO ifo;
	GetSystemInfo(&ifo);
	sysserialnum=((ifo.dwNumberOfProcessors+
				  ifo.dwProcessorType+
				  ifo.wProcessorLevel)<<16) +
				  ifo.wProcessorRevision;
	ret=diskserialnum ^ osserialnum ^ sysserialnum ^ randnu(2345);
	if (bShowID) {
		char strifo[1000];
		sprintf(strifo,"disk Serial:%x,\r\nOS:major%u,palatformid:%u,Builder:%u,\r\n"
			"sysifo:number of processor:%u,processtype:%u,processorlevel:%u,processorRevision:%u",
			diskserialnum,osifo.dwMajorVersion,osifo.dwPlatformId,osifo.dwBuildNumber,
			ifo.dwNumberOfProcessors,ifo.dwProcessorType,ifo.wProcessorLevel,ifo.wProcessorRevision);
		AfxMessageBox(strifo);

	}
	return ret;
}

BOOL encrypt::EncryptLicenseSt(char *psrc, char *pdec,DWORD sysid)
{
	int seed;
	int i;
	BYTE tmp;
	BYTE sum1;
	BYTE sum2;
	sum1=0;
	for (i=0;i<8;i++) {
		sum1^=*(BYTE*)(psrc+i);
	}
	sum2=0;
	for (i=0;i<8;i++) {
		sum2+=*(BYTE*)(psrc+i);
	}
	
	seed=sum1*sum2;
	sum1=seed&0xff;
	sum2=(seed>>8)&0xff;
	sprintf(pdec,"%02x%02x",sum1,sum2);
	seed=randnu(seed); 
	for (i=0;i<10;i++){
		tmp=*(BYTE*)(psrc+i);
		tmp=tmp^(seed%255);
		seed=randnu(seed); 
		sprintf(pdec+i*2+4,"%02x",tmp);
	}
	pdec[i*2+4]=0;
	return TRUE;

}

BOOL encrypt::DecryptLicenseSt(char *psrc, char *pdec, DWORD sysid)
{
	int seed;
	int i;
	WORD sum1,sum2;
	sscanf(psrc,"%02x%02x",&sum1,&sum2);
	seed=(sum2<<8)+sum1;
	seed=randnu(seed); 
	BYTE tmp;
	for (i=0;i<10;i++){
		tmp=0;
		sscanf(psrc+i*2+4,"%02x",&tmp);
		tmp=tmp^(seed%255);
		*(BYTE*)(pdec+i)=tmp;
		seed=randnu(seed); 
	}
	return TRUE;

}



static const char* to_b64 =
         "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
 
 /* encode 72 characters per line */
 #define CHARS_PER_LINE  72
 
BOOL encrypt::ToASCII(unsigned char *psrc, int srclen, char *pdec)
{
         int div = srclen / 3;
         int rem = srclen % 3;
         int chars = div*4 + rem + 1;
         int newlines = (chars + CHARS_PER_LINE - 1) / CHARS_PER_LINE;

         register char* buf = pdec;

         chars = 0;

         /*@+charindex@*/
         while (div > 0)
         {
                 buf[0] = to_b64[ (psrc[0] >> 2) & 0x3f];
                 buf[1] = to_b64[((psrc[0] << 4) & 0x30) + ((psrc[1] >> 4) & 0xf)];
                 buf[2] = to_b64[((psrc[1] << 2) & 0x3c) + ((psrc[2] >> 6) & 0x3)];
                 buf[3] = to_b64[  psrc[2] & 0x3f];
                 psrc += 3;
                 buf += 4;
                 div--;
                 chars += 4;
                 if (chars == CHARS_PER_LINE)
                 {
                         chars = 0;
                         *(buf++) = '\n';
                 }
         }

         switch (rem)
         {
         case 2:
                 buf[0] = to_b64[ (psrc[0] >> 2) & 0x3f];
                 buf[1] = to_b64[((psrc[0] << 4) & 0x30) + ((psrc[1] >> 4) & 0xf)];
                 buf[2] = to_b64[ (psrc[1] << 2) & 0x3c];
                 buf[3] = '=';
                 buf += 4;
                 chars += 4;
                 break;
         case 1:
                 buf[0] = to_b64[ (psrc[0] >> 2) & 0x3f];
                 buf[1] = to_b64[ (psrc[0] << 4) & 0x30];
                 buf[2] = '=';
                 buf[3] = '=';
                 buf += 4;
                 chars += 4;
                 break;
         }
         /*@=charindex@*/

 /*      *(buf++) = '\n'; This would result in a buffer overrun */
         *buf = '\0';
	return TRUE;
 }


int encrypt::ToBinary(char *psrc, unsigned char *pdec)
{
	static BOOL binit=TRUE;
	static char dtab[256];
 	register int i, qw = 0, tw = 0;
    register byte* data = pdec;

	if (binit) {
		binit=0;
		for (i=0;i<256;i++) dtab[i]=0;
		for (i=0;i<64;i++) dtab[to_b64[i]]=i;
	}
    //length = strlen(psrc);
    for (i = 0; psrc[i]; i++)  {
             register char ch = psrc[i];
             register byte bits;

             if (ch==' ')continue;

             bits = dtab[ch];
			 if (ch == '=')
                     break;

             switch (qw++)
             {
             case 0:
                     data[tw+0] = (bits << 2) & 0xfc;
                     break;
             case 1:
                     data[tw+0] |= (bits >> 4) & 0x03;
                     data[tw+1] = (bits << 4) & 0xf0;
                     break;
             case 2:
                     data[tw+1] |= (bits >> 2) & 0x0f;
                     data[tw+2] = (bits << 6) & 0xc0;
                     break;
             case 3:
                     data[tw+2] |= bits & 0x3f;
                     break;
             }

             if (qw == 4){
                     qw = 0;
                     tw += 3;
             }
    }
	if (qw==2) qw=1;
	else if (qw==3) qw=2;
	return tw+qw;
}

	




BOOL encrypt::MakeDASLicense(char *pstr)
{
	MD5_CTX ctx;
	MD5Init(&ctx);
	MD5Update(&ctx,(unsigned char*)mProgName,strlen(mProgName));
	MD5Update(&ctx,(unsigned char*)&mSysID,sizeof(mSysID));
	MD5Update(&ctx,(unsigned char*)&mValidTo,sizeof(mValidTo));
	MD5Update(&ctx,(unsigned char*)mGiveUp,sizeof(mGiveUp));
	MD5Final((unsigned char*)pstr,&ctx);
	return TRUE;
}
