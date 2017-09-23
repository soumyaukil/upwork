// IssueIfo.h: interface for the IssueIfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ISSUEIFO_H__AC857161_194E_11D5_BD2B_0050041B8FEC__INCLUDED_)
#define AFX_ISSUEIFO_H__AC857161_194E_11D5_BD2B_0050041B8FEC__INCLUDED_

#include "FUNCTIONS\intmap.h"	// Added by ClassView
#include "FUNCTIONS\FixLenMem.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SRECSET\SRecSet.h"


class IssueIfo  
{
public:
	void Clear();
	inline BOOL IsShortAble(){return ((mflag & (1))==(1));};
	inline BOOL IsShortAble1(){return ((mflag & (8))==(8));};
	inline BOOL IsMarginAble(){return ((mflag& (1<<1))==(1<<1));};
	inline BOOL IsHiRisk(){return ((mflag & (1<<2))==(1<<2));};
	
	char msecsym[12];
	//	Flag: int, bit flag for this security;
	//	0x1:shortable, 0x2:marginable.0x4: internet stocks.
	//  0x8: shortable list 1;
	long mflag;
	BYTE mIssueMarket;
	long mtiresize; //this tiresize is for SOES, we don't need anymore.
	long mshortsize;
	double mYClose; //yesterday close for this Issue;
};

class FutureIfo
{
public:
	char msecsym[12]; //ES is EMini;
	char mname[50];
	char mExch[10]; 

	double m_tickersize;// the tickersize;
	double m_multiplier;// the multiplier for this symbol.
	double m_DayInit; //the day trading Init require for the open ;
	double m_InitReq;	//the cash amount for the Init Require amount
	double m_Maintain;	//the cash need for maintain this position;

	
};


class srstSecIfo : public SRecSet  
{
public:
	int OpenToModify(char *pstr);
	BOOL OnBind(int flag);
	BOOL Open();
	LPCSTR GetColLst();
	IssueIfo mitem;
	srstSecIfo();
	virtual ~srstSecIfo();

};


//the maxium symbol list;
//0: untradeable list
//1: tradablelist 
#define MAXSYMBLIST 2  

class IssueSet
{
	FixLenMem mIssueBuff;
//	simplelock mlock; 
public:
	void ClearnNotMarginList();
	void ResetItemToDeault(IssueIfo *pifo);
	//ext539 is a file that the symbol can not be margain;
	//04010E109,07,AGX,ARGAN INC.,A,0,1,Y,Y,,,,Y,O,,,,,,,,,S,,,,,,,EQTY,T,0,0,,0000,ARGAN INC,,,08/22/2007,10/15/2007,,,0,0,0,0,US,,,,0,0000,,,,,,N,,,N,MASH,0,0,10/15/2007

	BOOL LoadPensonExt539(char *pfile);
	FutureIfo * GetFutureInfo(LPCSTR psymbol);
	BOOL LoadFuturesInfo(LPCSTR filename);
	void SaveIssueInfo();
	void ExportIssueInfo(LPCSTR filename);
	//clearn the shortlist;
	void ClearnShortList(int nid);
	//load a short list form file;
	BOOL LoadShortListFromFile(LPCSTR filename,int nid,BOOL bShortAbleLst);
	BOOL LoadUnTradableLst();
	BOOL RMAllUnTradable(int lstid);
	BOOL RMUnTradable(int lstid, char *secsym);
	BOOL AddUnTradable(int lstid, LPCSTR secsym);
	double GetYestDayClose(LPCSTR secsym);
	IssueIfo * GetIssueIfo(LPCSTR secsym);
	void ClearIssueMap();
	BOOL LoadIssueInfo();
	strmap mSecMap;
	strmap mSymbLst[MAXSYMBLIST];// the marketmaker untrade able;
	
	strmap mFutureInfo; //the futures symbol information, it is the map for FutureIfo;

	BOOL mbDefShortable;
	BOOL mbDefShortable1;


	IssueIfo * GetNewBuff(){
		return (IssueIfo*)mIssueBuff.GetVoidBuff();
	};
	inline void ReleaseBuff(IssueIfo *pifo){
		mIssueBuff.PutVoidBuff((char*)pifo);
	};
	IssueSet();
	~IssueSet();
};
#endif // !defined(AFX_ISSUEIFO_H__AC857161_194E_11D5_BD2B_0050041B8FEC__INCLUDED_)
