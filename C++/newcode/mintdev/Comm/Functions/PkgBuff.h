// PkgBuff.h: interface for the CPkgBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_)
#define AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_
//#include <afxmt.h>
#include "FixLenMem.h"	// Added by ClassView
#include <mutex>
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define BF_SZ 2048
#define BF_NU 400

class  dabuff
{
public:
	dabuff * pnext;
	uint16_t bufbegin; // the package 
	uint16_t bufend;
	uint32_t mdestIp;  //the ipaddress send to the client, For UDP
	uint16_t  mdestPort;// the port send to the client, For UDP 
	char DataBuff[BF_SZ];

	int TrimPkgBuff();
	char * GetPkgBuff();
	bool AddPkgBuff(const char* pkgbuf);
	inline int GetPkgLen(){ return bufend-bufbegin; };
	inline int GetLeftBFLen() {return BF_SZ-bufend; };
	inline char *GetBuffAddr() {return (DataBuff+bufbegin);};
public:
	//this function just to add raw package which already know length,
	//the package may not begin with length;
	int AddRawPkg(const char *pkg, uint32_t len);
	bool AddUdpPkg(uint16_t type, const char* pbuf, int len);
	bool CheckPkgIntegrate();
	bool GetPkgBuff(char ** pbuff);
	static void GetMemUsage(int &ntotal,int &nvoid);
	static int PutVoidDaBuff(dabuff *pdabuff);
	static dabuff* GetVoidDaBuff();
	static int InitBuff(int blockbufnum);

	static int m_mtusize;
private:
	static FixLenMem mPkgBuff;
};

class dabfchain
{
	dabuff *phead;
	dabuff *pend;
	int    mbuffnum;
	std::mutex mCritLock;
public:
	dabfchain() {
		phead=NULL;
		pend=NULL;
		mbuffnum=0;
	}
	~dabfchain() {
		Clear();
	}
	void Clear() {
		Lock();
		dabuff *p1;
		while(phead) {
			p1=phead;
			phead=phead->pnext;
			dabuff::PutVoidDaBuff(p1);
		}
		phead=NULL;
		pend=NULL;
		mbuffnum=0;
		Unlock(); 
	}
public:
	bool FillPkgMax(dabuff *pkg);
	void     Lock(){mCritLock.lock();     }
	void     Unlock() {mCritLock.unlock();}
	dabuff * GetTail() {return pend; }

	bool IsEmpty() {return (phead==NULL);};
	//this function consolidate all the package in to the first package
	//to make a bigger package. the maxmum package size is defined by dabuff:m_mtusize
	int ConsolidatePackage();
	//this function just to add raw package which already know length,
	//the package may not begin with length;
	bool AddRawPkg(const char *pkg, uint32_t len);
	bool AddUdpPkg(uint16_t type, const char* pbuf, int len);
	bool AddPkgBuff(const char* pkgbuff);
	//this function Fill the data to the pkg, 
	//which if send to socket, multiple package can be merge as a big package.
	//this will make send bigger package;
	bool FillPkg(dabuff *pkg);

	bool FillPkgByIP(dabuff *pkg);
	inline int GetBuffCount() { return (mbuffnum); };
	int PeekFirstPkgLen();
	int PutDabuff(dabuff * pbf, bool bTail=true);
	dabuff * GetDabuff();
};

/*class CPkgBuff  
{
public:
	int PutVoidDaBuff( dabuff* pdabuff);
	dabuff* GetVoidDaBuff();
	bool Init();
	char * mbuff;
	dabfchain mfreechain;
	CPkgBuff();
	virtual ~CPkgBuff();
protected:
	long mbfnu;
	HANDLE hEvent;
};
*/

class PkgToken  
{
private:
	char * pkgbuff; //the package buff;
	uint16_t mbufflen;	//the package buff length;
	int pkglen;		//the actual buff length
	uint16_t currpos;	//the current post of the buff

public:
	//get the buffer left length, for get data
	inline uint16_t GetLeftLen() { if (currpos>pkglen) return 0; else return (pkglen-currpos); };
	//get the buffer left length, for set data
	inline uint16_t GetBuffLeftSize()	{
		if (pkgbuff==NULL) return 0;
		if (mbufflen<currpos) return 0;
			return (mbufflen-currpos);
	};
	inline uint16_t GetPos(){ return currpos;};
	//set the newpos and return the old position;
	inline uint16_t SetPos(uint16_t newpos){
		uint16_t oldpos; 
		oldpos=currpos;
		currpos=newpos;
		return oldpos;
	};
	//this function incress positon by nbyte;
	//return the old pointer;
	inline char *IncrPos(int nbyte){
		register char *ret=pkgbuff+currpos;
		currpos+=nbyte;
		return ret;
	};

	char * finalizeSet();
	int InitToSet(char * ppkg,uint16_t len=0);
	int InitToGet(char * ppkg,uint16_t begin=2);

	char * setmsgpkg(char* pkgbuff, uint16_t pkgtype, uint16_t pkgsubtype, uint16_t pkgval, const char* pkgmsg);

	PkgToken();
	virtual ~PkgToken();

public:
	double getCpDouble();
	void setCpDouble(double val);
	long getLongDiff1(long nbase);
	void setLongDiff1(long nbase,long nval);
	long getLongDiff(long nbase);
	void setLongDiff(long nbase,long nval);
	unsigned long getShareInt();
	void setShareInt(unsigned long share);
	void getPrice(double *lastprice, double *newprice);
	void setPrice(double *lastprice, double newprice);
	// get a string from the buffer;
	void setVarStr(const char* pstr);
	std::string getCString();
	char* getString(char *ptr, int len);
	
	void setFixStr(const char* pstr,uint16_t len);
	void setFixData(const char* pstr, uint16_t len);
	void getFixData(char * pstr, uint16_t len);
	
	long getlong();
	void setlong(long val);

	double getDouble();
	void setDouble(double val);

	void setuint32_t(uint32_t val);
	uint32_t getuint32_t();
	void setuint16_t(uint16_t val);
	uint16_t getuint16_t();
	void setuint8_t(uint8_t val);
	uint8_t getuint8_t();
	int  IncrCounter(int pos);
};

#endif // !defined(AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_)
