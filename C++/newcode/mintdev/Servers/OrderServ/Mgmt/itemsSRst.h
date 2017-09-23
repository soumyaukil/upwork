// itemsSRst.h: interface for the srstAccIfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_)
#define AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "itemsDef.h"	// Added by ClassView

class srstAccIfo : public SRecSet  
{
	long bindflag;
public:
	bool OnBind(int flag);
	bool Open();
	
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
	bool Open(bool bRead,long ntrid=0,long nacid=0 );
	bool OnBind(int flag);
	AC_TR_relation mitem;
	srstAcct_Access();
	virtual ~srstAcct_Access();

};



class srstPosition : public SRecSet  
{
public:
	LPCSTR GetColLst(bool bNewTable);
	bool OpenToUpdate(bool bNewTable,int naccid, char * psecsym, BYTE btype);
	bool OnBind(int flag);
	itemPosition mitem;
	bool OpenToRead();
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
	bool OpenToUpdate(int orderid);
	bool OpenToRead();
	srstOrders();
	LPCSTR GetColLst();
	bool OnBind(int flag);
	itemOrder mitem;
	DWORD mDirtyFlag;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstTrade : public SRecSet  
{
public:
	bool OpenToUpdate(int tradeid);
	bool OpenToRead();
	bool OnBind(int flag);
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
	bool Open();
	bool OnBind(int flag);
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
	bool OpenToUpdate(bool bNewTable,int accid);
	bool Open(bool bNewTable);
	LPCSTR GetColLst(bool bNewTable);
	bool OnBind(int flag);
	srstAccEquity();
	virtual ~srstAccEquity();
	

};

class srstAccCtrl: public SRecSet  
{
public:
	itemAccCtrl mitem;
public:
	bool OpenToUpdate(int accid);
	bool Open();
	LPCSTR GetColLst();
	bool OnBind(int flag);
	srstAccCtrl();
	virtual ~srstAccCtrl();
	

};


class srstPassthruFee : public SRecSet
{
public:
	bool OpenToUpdate(int dealid);
	bool OpenToRead();
	LPCSTR GetColLst();
	bool OnBind(int flag);
	long mdealid;
	itemPassthrufee mitem;

};

class srstDeal : public SRecSet
{
public:
	bool OpenToUpdate(int dealid);
	bool OpenToRead();
	LPCSTR GetColLst();
	bool OnBind(int flag);
	itemDeal mitem;
private:
	TIMESTAMP_STRUCT mm_date;
};

class srstAddress: public SRecSet
{
public:
	bool OpenToRead(BYTE qtype,long qmodify,long qid,int *brlst); //if brlst ==NULL, only one branch, otherwise it is multiple branch
	bool OpenToUpdate(int maddid);
	bool OnBind(int flag);
	LPCSTR GetColLst();
	itemAddress mitem;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;
};

class srstBranch : public SRecSet  
{

public:
	bool OpenToRead();
	bool OpenToUpdate(int mbrid);
	DWORD mDirtyFlag;
	bool OnBind(int flag);
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
	bool OnBind(int flag);
	bool OpenToRead();
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
	bool OpenToUpdate(int perid);
	bool OpenToRead(BYTE qtype,long qmodify,long qid,int *pbrlst);
	bool OnBind(int flag);
	LPCSTR GetColLst();
	itemPerson mitem;
private:
	TIMESTAMP_STRUCT mc_date;
	TIMESTAMP_STRUCT mm_date;


};

class srstMessage: public SRecSet
{
public:
	bool OpenToRead(int id, time_t tafter);
	srstMessage();
	bool AddToDb(itemMessage *pitem);
	bool OpenToAdd();
	LPCSTR GetColLst();
	bool OnBind(int flag);
	itemMessage mitem;
private:
	bool bopenforadd;
	TIMESTAMP_STRUCT ms_date;
};


class srstOrderActive:public SRecSet
{
public:
	bool OpenToAdd();
	bool OnBind(int flag);
	LPCSTR GetColLst();
	itemOrderActive mitem;
private:
	TIMESTAMP_STRUCT mlog_date;

};


class srstOptsym:public SRecSet
{
public:
	bool OpenToRead();
	bool OpenToUpdate(char *optsymb);
	bool OnBind(int flag);
	LPCSTR GetColLst();
	itemOptsym mitem;
private:
	TIMESTAMP_STRUCT mexp_date;
	TIMESTAMP_STRUCT m_date;
};

#endif // !defined(AFX_ITEMSSRST_H__80117B31_AB3F_11D4_BD1E_0050041B8FEC__INCLUDED_)
