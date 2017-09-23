// PkgBuff.h: interface for the CPkgBuff class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_)
#define AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_
//#include <afxmt.h>
#include "FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define BF_SZ 2048
#define BF_NU 400

class  dabuff
{
public:
	dabuff * pnext;
	WORD bufbegin; // the package 
	WORD bufend;
	DWORD mdestIp;  //the ipaddress send to the client, For UDP
	WORD  mdestPort;// the port send to the client, For UDP 
	char DataBuff[BF_SZ];

	int TrimPkgBuff();
	char * GetPkgBuff();
	BOOL AddPkgBuff(LPCSTR pkgbuf);
	inline int GetPkgLen(){ return bufend-bufbegin; };
	inline int GetLeftBFLen() {return BF_SZ-bufend; };
	inline char *GetBuffAddr() {return (DataBuff+bufbegin);};
public:
	//this function just to add raw package which already know length,
	//the package may not begin with length;
	int AddRawPkg(const char *pkg, DWORD len);
	BOOL AddUdpPkg(WORD type, LPCSTR pbuf, int len);
	BOOL CheckPkgIntegrate();
	BOOL GetPkgBuff(char ** pbuff);
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
	CCriticalSection mCritLock;
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
	BOOL FillPkgMax(dabuff *pkg);
	void     Lock(){mCritLock.Lock();     }
	void     Unlock() {mCritLock.Unlock();}
	dabuff * GetTail() {return pend; }

	BOOL IsEmpty() {return (phead==NULL);};
	//this function consolidate all the package in to the first package
	//to make a bigger package. the maxmum package size is defined by dabuff:m_mtusize
	int ConsolidatePackage();
	//this function just to add raw package which already know length,
	//the package may not begin with length;
	BOOL AddRawPkg(const char *pkg, DWORD len);
	BOOL AddUdpPkg(WORD type, LPCSTR pbuf, int len);
	BOOL AddPkgBuff(LPCSTR pkgbuff);
	//this function Fill the data to the pkg, 
	//which if send to socket, multiple package can be merge as a big package.
	//this will make send bigger package;
	BOOL FillPkg(dabuff *pkg);

	BOOL FillPkgByIP(dabuff *pkg);
	inline int GetBuffCount() { return (mbuffnum); };
	int PeekFirstPkgLen();
	int PutDabuff(dabuff * pbf, BOOL bTail=TRUE);
	dabuff * GetDabuff();
};

/*class CPkgBuff  
{
public:
	int PutVoidDaBuff( dabuff* pdabuff);
	dabuff* GetVoidDaBuff();
	BOOL Init();
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
	WORD mbufflen;	//the package buff length;
	int pkglen;		//the actual buff length
	WORD currpos;	//the current post of the buff

public:
	//get the buffer left length, for get data
	inline WORD GetLeftLen() { if (currpos>pkglen) return 0; else return (pkglen-currpos); };
	//get the buffer left length, for set data
	inline WORD GetBuffLeftSize()	{
		if (pkgbuff==NULL) return 0;
		if (mbufflen<currpos) return 0;
			return (mbufflen-currpos);
	};
	inline WORD GetPos(){ return currpos;};
	//set the newpos and return the old position;
	inline WORD SetPos(WORD newpos){
		WORD oldpos; 
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
	int InitToSet(char * ppkg,WORD len=0);
	int InitToGet(char * ppkg,WORD begin=2);

	char * setmsgpkg(char* pkgbuff, WORD pkgtype, WORD pkgsubtype, WORD pkgval, LPCSTR pkgmsg);

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
	void setVarStr(LPCSTR pstr);
	CString getCString();
	char* getString(char *ptr, int len);
	
	void setFixStr(LPCSTR pstr,WORD len);
	void setFixData(LPCSTR pstr, WORD len);
	void getFixData(char * pstr, WORD len);
	
	long getlong();
	void setlong(long val);

	double getDouble();
	void setDouble(double val);

	void setDWORD(DWORD val);
	DWORD getDWORD();
	void setWORD(WORD val);
	WORD getWORD();
	void setBYTE(BYTE val);
	BYTE getBYTE();
	int  IncrCounter(int pos);
};

#endif // !defined(AFX_PKGBUFF_H__68BFDB40_F9E6_11D3_BC66_00105A82D2D1__INCLUDED_)
