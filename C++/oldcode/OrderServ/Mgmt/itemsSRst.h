// itemsSRst.h: interface for the srstAccIfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_)
#define AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SRECSET\SRecSet.h"
#include "itemsDef.h"	// Added by ClassView
#include "..\Account.h"	// Added by ClassView

class srstAccIfo : public SRecSet  
{
	long bindflag;
public:
	BOOL OnBind(int flag);
	BOOL Open();
	
	itemAccIfo mitem;
	TIMESTAMP_STRUCT mm_date;
	TIMESTAMP_STRUCT mc_date;
	srstAccIfo();
	virtual ~srstAccIfo();

};


class srstAcct_Access : public SRecSet  
{
	TIMESTAMP_STRUCT mm_date;
public:
	BOOL Open(BOOL bRead,long ntrid=0,long nacid=0 );
	BOOL OnBind(int flag);
	AC_TR_relation mitem;
	srstAcct_Access();
	virtual ~srstAcct_Access();

};



class srstPosition : public SRecSet  
{
public:
	LPCSTR GetColLst(BOOL bNewTable);
	BOOL OpenToUpdate(BOOL bNewTable,int naccid, char * psecsym, BYTE btype);
	BOOL OnBind(int flag);
	itemPosition mitem;
	BOOL OpenToRead();
	srstPosition();
	virtual ~srstPosition();
	WORD mDirtyFlag;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstOrders :public SRecSet
{
public:
	BOOL OpenToUpdate(int orderid);
	BOOL OpenToRead();
	srstOrders();
	LPCSTR GetColLst();
	BOOL OnBind(int flag);
	itemOrder mitem;
	DWORD mDirtyFlag;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstTrade : public SRecSet  
{
public:
	BOOL OpenToUpdate(int tradeid);
	BOOL OpenToRead();
	BOOL OnBind(int flag);
	LPCSTR GetColLst();
	srstTrade();
	virtual ~srstTrade();
	itemTrade mitem;
	DWORD mDirtyFlag;
private:
	TIMESTAMP_STRUCT me_date;
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstTicket :public SRecSet
{
public:
	LPCSTR GetColLst();
	BOOL Open();
	BOOL OnBind(int flag);
	itemTicket mitem;

private:
	TIMESTAMP_STRUCT me_date;
	TIMESTAMP_STRUCT mm_date;
};


class srstAccEquity : public SRecSet  
{
public:
	itemAccEquity mitem;
private:
	TIMESTAMP_STRUCT mm_date;
public:
	BOOL OpenToUpdate(BOOL bNewTable,int accid);
	BOOL Open(BOOL bNewTable);
	LPCSTR GetColLst(BOOL bNewTable);
	BOOL OnBind(int flag);
	srstAccEquity();
	virtual ~srstAccEquity();
	

};

class srstAccCtrl: public SRecSet  
{
public:
	itemAccCtrl mitem;
public:
	BOOL OpenToUpdate(int accid);
	BOOL Open();
	LPCSTR GetColLst();
	BOOL OnBind(int flag);
	srstAccCtrl();
	virtual ~srstAccCtrl();
	

};


class srstPassthruFee : public SRecSet
{
public:
	BOOL OpenToUpdate(int dealid);
	BOOL OpenToRead();
	LPCSTR GetColLst();
	BOOL OnBind(int flag);
	long mdealid;
	itemPassthrufee mitem;

};

class srstDeal : public SRecSet
{
public:
	BOOL OpenToUpdate(int dealid);
	BOOL OpenToRead();
	LPCSTR GetColLst();
	BOOL OnBind(int flag);
	itemDeal mitem;
private:
	TIMESTAMP_STRUCT mm_date;
};

class srstAddress: public SRecSet
{
public:
	BOOL OpenToRead(BYTE qtype,long qmodify,long qid,int *brlst); //if brlst ==NULL, only one branch, otherwise it is multiple branch
	BOOL OpenToUpdate(int maddid);
	BOOL OnBind(int flag);
	LPCSTR GetColLst();
	itemAddress mitem;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstBranch : public SRecSet  
{

public:
	BOOL OpenToRead();
	BOOL OpenToUpdate(int mbrid);
	DWORD mDirtyFlag;
	BOOL OnBind(int flag);
	itemBranch mitem;
	LPCSTR GetColLst();
	srstBranch();
	virtual ~srstBranch();
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};


class srstTrader : public SRecSet 
{
public:
	int OpenToUpdate(int trid);
	BOOL OnBind(int flag);
	BOOL OpenToRead();
	LPCSTR GetColLst();
	itemTrader mitem;
	DWORD mDirtyFlag;
	srstTrader();
	virtual ~srstTrader();
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};


class srstPerson : public SRecSet  
{
public:
	BOOL OpenToUpdate(int perid);
	BOOL OpenToRead(BYTE qtype,long qmodify,long qid,int *pbrlst);
	BOOL OnBind(int flag);
	LPCSTR GetColLst();
	itemPerson mitem;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;


};

class srstMessage: public SRecSet
{
public:
	BOOL OpenToRead(int id, time_t tafter);
	srstMessage();
	BOOL AddToDb(itemMessage *pitem);
	BOOL OpenToAdd();
	LPCSTR GetColLst();
	BOOL OnBind(int flag);
	itemMessage mitem;
private:
	BOOL bopenforadd;
	TIMESTAMP_STRUCT ms_date;
};


class srstOrderActive:public SRecSet
{
public:
	BOOL OpenToAdd();
	BOOL OnBind(int flag);
	LPCSTR GetColLst();
	itemOrderActive mitem;
private:
	TIMESTAMP_STRUCT mlog_date;

};


class srstOptsym:public SRecSet
{
public:
	BOOL OpenToRead();
	BOOL OpenToUpdate(char *optsymb);
	BOOL OnBind(int flag);
	LPCSTR GetColLst();
	itemOptsym mitem;
private:
	TIMESTAMP_STRUCT mexp_date;
	TIMESTAMP_STRUCT m_date;
};

#endif // !defined(AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_)
