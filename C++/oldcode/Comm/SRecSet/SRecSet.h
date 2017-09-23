// SRecSet.h: interface for the SRecSet class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SRECSET_H__534FA0F1_A10E_11D4_BD19_0050041B8FEC__INCLUDED_)
#define AFX_SRECSET_H__534FA0F1_A10E_11D4_BD19_0050041B8FEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxdb.h>

//define as the maxume number of colume for the SRecSet
#define MAXCOLNUMBER 40


class SRSTErr{
public:
	int merrno;
	CString merrmsg;
};

class SRecSet : public CObject  
{

	long mcolstatus[MAXCOLNUMBER];

public:

	CDatabase *m_pDatabase; // source database for this resultset;
	HSTMT	  m_hstmt;// source statement for this result;
public:
	BOOL	m_bEOF;
	BOOL	m_bBOF;
	DWORD	  m_dwRowsFetched; // how many rows fetched.
	WORD     m_wRowStatus; //current row status; only 1 row supported.
public:

	BOOL GetErrorMessage(CString &str);

	//call when need to bind the data to update or get data.
	//nBindFlag, 0: For getdate,2: for Update 
	//           
	enum {
		bindForRead=0,	//bind for getdata from database.
		bindFixUp=1,	//bind after data goted, do data transfer
		bindForUpdate=2,//bind for updatedata.
		bindForNew=3	//bind for new record, new data.
	};

	virtual BOOL OnBind(int nBindFlag);
	void Close();
	static void TranslateDate(time_t *ptm, TIMESTAMP_STRUCT *ptmstamp,BOOL bTo);
	//this function is used to translate data to the datetime 
	//to the time_t transfer, used for the derivated calss.
	long GetRowCount();
	BOOL EnableBookMark(BOOL bEnable=TRUE);
	BOOL UnBindAll();
/*  SQL_CONCUR_READ_ONLY (DEFAULT)
	SQL_CONCUR_LOCK
	SQL_CONCUR_ROWVER
	SQL_CONCUR_VALUES */
	BOOL SetConCurrency(int type);
/*type= SQL_CURSOR_FORWARD_ONLY  (DEFAULT)
        SQL_CURSOR_STATIC
    	SQL_CURSOR_KEYSET_DRIVEN
		SQL_CURSOR_DYNAMIC         */
	BOOL SetCursorType(int type);
	BOOL GetAttr(long attribute,void * var, long varlen);
	BOOL SetAttr(long attribute, void* var, long varlen);
	BOOL UnBind(int fieldId);
	BOOL Update();
	BOOL AddNew();
	BOOL Delete();
	BOOL IsEOF();
	BOOL IsBOF();
	BOOL MoveLast();
	BOOL MovePrev();
	BOOL MoveNext();
	BOOL SetOption(WORD option,DWORD optionvalue);
	RETCODE Move(long nRows, WORD wFetchType);
	BOOL MoveFirst();
	BOOL ExecuteSql(LPCSTR lpszSQL);
	void BindText(int bindflag,int fieldid, char*pstr,long strlen);
	void BindLong(int bindflag,int fieldid, long *plong);
	void BindWORD(int bindflag,int fieldid, WORD *pword);
	void BindBYTE(int bindflag,int fieldid, BYTE *pbyte);
	void BindDouble(int bindflag,int fieldid, double *plong);
	void BindTimeStruct(int bindflag,int fieldid,TIMESTAMP_STRUCT *ptime);
	void BindTime_t(int bindflag,int fieldid,time_t *ptm_t,TIMESTAMP_STRUCT *ptmbuff);

	virtual void ThrowDBException(RETCODE nRetCode, HSTMT hstmt = SQL_NULL_HSTMT);
//	void ThrowDBException(RETCODE nRetCode, HSTMT hstmt = SQL_NULL_HSTMT);
	BOOL init(CDatabase *pDatabase);
	SRecSet();
	virtual ~SRecSet();
private:
	RETCODE SkipDeletedRecords(UWORD wFetchType, long nRows);
	BOOL Check(RETCODE nRetCode);
};

#endif // !defined(AFX_SRECSET_H__534FA0F1_A10E_11D4_BD19_0050041B8FEC__INCLUDED_)
