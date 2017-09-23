// OrderDB.h: interface for the OrderDB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ORDERDB_H__92C27731_572D_11D4_BCEC_00105A82D2D1__INCLUDED_)
#define AFX_ORDERDB_H__92C27731_572D_11D4_BCEC_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "afxdb.h"

class rstSysID : public CRecordset
{
public:
	rstSysID(CDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(rstSysID)

// Field/Param Data
	//{{AFX_FIELD(rstSysID, CRecordset)
	CString	m_mkey;
	long	m_m_id;
	//}}AFX_FIELD


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(rstSysID)
	public:
	virtual CString GetDefaultConnect();    // Default connection string
	virtual CString GetDefaultSQL();    // Default SQL for Recordset
	virtual void DoFieldExchange(CFieldExchange* pFX);  // RFX support
	//}}AFX_VIRTUAL

// Implementation
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

class OrderDB  
{
public:
	char * FormatSQLTime(time_t ntime, char* buffer);
	BOOL cfgGet(LPCSTR id,LPCSTR subid,char *val,int size);
	BOOL CloseConnection();
	rstSysID m_rstsysid;
	int ChangeSysID(LPCSTR key,int changeval=0);
	BOOL ConnectOrderSV(LPCSTR str);
	CDatabase mdb;
	OrderDB();
	virtual ~OrderDB();
};

#endif // !defined(AFX_ORDERDB_H__92C27731_572D_11D4_BCEC_00105A82D2D1__INCLUDED_)
/////////////////////////////////////////////////////////////////////////////
// rstSysID recordset

