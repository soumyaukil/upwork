// SRecSet.cpp: implementation of the SRecSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SRecSet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

SRecSet::SRecSet()
{
	m_pDatabase = NULL;
	m_hstmt = SQL_NULL_HSTMT;
	m_dwRowsFetched=0;
}

SRecSet::~SRecSet()
{
	int nRetCode;
	ASSERT_VALID(this);

	if (m_hstmt != SQL_NULL_HSTMT){
		AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_DROP));
		if(Check(nRetCode)==FALSE) {
			AfxMessageBox("SRecSet delete hsmt error!"); 
		}
		m_hstmt = SQL_NULL_HSTMT;
	}
	m_pDatabase = NULL;
}

BOOL SRecSet::init(CDatabase *pDatabase) // source database for this resultset;
{
	RETCODE nRetCode;

	ASSERT(pDatabase!=NULL);
	// If not already opened, attempt to open
	ASSERT (pDatabase->IsOpen());
	m_pDatabase=pDatabase;
	if (m_hstmt == SQL_NULL_HSTMT)
	{
		AFX_SQL_SYNC(::SQLAllocStmt(m_pDatabase->m_hdbc, &m_hstmt));
		if (!Check(nRetCode)){
			m_pDatabase = NULL;
			ASSERT(m_hstmt == SQL_NULL_HSTMT);
			return FALSE;
		}
		SetAttr(SQL_ATTR_ROW_ARRAY_SIZE,(SQLPOINTER)1,0);
		SetAttr(SQL_ATTR_ROW_STATUS_PTR, (SQLPOINTER)&m_wRowStatus, 0);
	}
	return TRUE;
}

BOOL SRecSet::Check(RETCODE nRetCode) 
{
	ASSERT_VALID(this);

	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
#ifdef _DEBUG
//		if (afxTraceFlags & traceDatabase)
//		{
//			CDBException e(nRetCode);
//			TRACE0("Warning: ODBC Success With Info, ");
//			e.BuildErrorString(m_pDatabase, m_hstmt);
//		}
		{
			CString str;
			GetErrorMessage(str);
			TRACE("Warning: ODBC Success With Info, ");
			TRACE(str);
		}
#endif

		// Fall through

	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
	case SQL_NEED_DATA:
		return TRUE;
	}

	return FALSE;
}


void SRecSet::ThrowDBException(RETCODE nRetCode, HSTMT hstmt)
{
	ASSERT_VALID(this);
	SRSTErr *perr;
	perr=new SRSTErr;
	perr->merrno = nRetCode;
	GetErrorMessage(perr->merrmsg); 
	throw perr;

//	AfxThrowDBException(nRetCode, m_pDatabase,(hstmt == SQL_NULL_HSTMT)? m_hstmt : hstmt); 
}


BOOL SRecSet::ExecuteSql(LPCSTR lpszSQL)
{
	m_bBOF=m_bEOF=TRUE; 
	RETCODE nRetCode;
	AFX_ODBC_CALL(::SQLExecDirect(m_hstmt,(UCHAR*)(lpszSQL),SQL_NTS));
	ASSERT_VALID(this);

	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
	case SQL_NEED_DATA:
		return TRUE;
	};
	
	ThrowDBException(nRetCode);
	return FALSE;
//	return Check(nRetCode);
}



RETCODE SRecSet::Move(long nRows, WORD wFetchType)
{
	ASSERT_VALID(this);
	ASSERT(m_hstmt != SQL_NULL_HSTMT);

	RETCODE nRetCode;
	OnBind(bindForRead);
	if ((wFetchType == SQL_FETCH_FIRST ||
		wFetchType == SQL_FETCH_LAST ||
		wFetchType == SQL_FETCH_NEXT ||
		wFetchType == SQL_FETCH_PRIOR ||
		wFetchType == SQL_FETCH_RELATIVE))//		&&m_dwOptions & skipDeletedRecords)
	{
		nRetCode=SkipDeletedRecords(wFetchType, nRows);
	}else {
		nRetCode=::SQLFetchScroll(m_hstmt,wFetchType,nRows); 
	}
		ASSERT_VALID(this);

	switch (nRetCode)
	{
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NO_DATA_FOUND:
	case SQL_NEED_DATA:
		break;
	default:
		return nRetCode;
	}
	OnBind(bindFixUp);
//	DataTranslate(FALSE);
	return nRetCode;
}

BOOL SRecSet::SetOption(WORD option, DWORD optionvalue)
{
	RETCODE nRetCode;
	nRetCode=::SQLSetStmtAttr (m_hstmt,option,(void*)optionvalue,0); 
	return Check(nRetCode);
}

BOOL SRecSet::MoveNext()
{
	RETCODE ret;
	ret=Move(1, SQL_FETCH_NEXT);
	switch(ret) {
	case SQL_NO_DATA_FOUND:
		m_bEOF=TRUE;
		return TRUE;
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NEED_DATA:
		m_bEOF=m_bBOF=FALSE;
		return TRUE;
	}
	return FALSE;	
}


BOOL SRecSet::MovePrev()
{
	RETCODE ret;
	ret=Move(-1, SQL_FETCH_PRIOR); 
	switch(ret) {
	case SQL_NO_DATA_FOUND:
		m_bBOF=TRUE;
		return TRUE;
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NEED_DATA:
		m_bEOF=m_bBOF=FALSE;
		return TRUE;
	}
	return FALSE;	
}

BOOL SRecSet::MoveLast()
{
	RETCODE ret;
	ret=Move(-1, SQL_FETCH_LAST);
	switch(ret) {
	case SQL_NO_DATA_FOUND:
		m_bEOF=m_bBOF=TRUE;
		return TRUE;
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NEED_DATA:
		m_bEOF=m_bBOF=FALSE;
		
		return TRUE;
	}
	return FALSE;	
}

BOOL SRecSet::MoveFirst()
{
	RETCODE ret;
	ret=Move(1, SQL_FETCH_FIRST); 
	switch(ret) {
	case SQL_NO_DATA_FOUND:
		m_bEOF=m_bBOF=TRUE;
		return TRUE;
	case SQL_SUCCESS_WITH_INFO:
	case SQL_SUCCESS:
	case SQL_NEED_DATA:
		m_bEOF=m_bBOF=FALSE;
		return TRUE;
	}
	return FALSE;	
}

BOOL SRecSet::IsBOF()
{
	return m_bBOF;
}

BOOL SRecSet::IsEOF()
{
	return m_bEOF;
}

BOOL SRecSet::Delete()
{
	UWORD wExpectedRowStatus;
	UWORD wPosOption;
	wPosOption = SQL_DELETE;
	wExpectedRowStatus = SQL_ROW_DELETED;
/*	}
	else
	{
		if (m_nEditMode == edit)
		{
			wPosOption = SQL_UPDATE;
			wExpectedRowStatus = SQL_ROW_UPDATED;
		}
		else
		{
			wPosOption = SQL_ADD;
			wExpectedRowStatus = SQL_ROW_ADDED;
		}
	}

	BindFieldsForUpdate();
*/
	RETCODE nRetCode;
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, 1, wPosOption, SQL_LOCK_NO_CHANGE));
	// This should only fail if SQLSetPos returned SQL_SUCCESS_WITH_INFO explaining why
	if (nRetCode == SQL_SUCCESS_WITH_INFO && m_wRowStatus!= wExpectedRowStatus)
		ThrowDBException(AFX_SQL_ERROR_UPDATE_DELETE_FAILED);

	if (Check(nRetCode)==FALSE) {
		ThrowDBException(nRetCode); 
	}
	return (TRUE);
}

BOOL SRecSet::AddNew()
{
	UWORD wExpectedRowStatus;
	UWORD wPosOption;
	wPosOption = SQL_ADD;
	wExpectedRowStatus = SQL_ROW_ADDED;

	RETCODE nRetCode;
	OnBind(bindForNew);
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, 1, wPosOption, SQL_LOCK_NO_CHANGE));
	// This should only fail if SQLSetPos returned SQL_SUCCESS_WITH_INFO explaining why
	if (nRetCode == SQL_SUCCESS_WITH_INFO && m_wRowStatus!= wExpectedRowStatus)
		ThrowDBException(AFX_SQL_ERROR_UPDATE_DELETE_FAILED);
	if (Check(nRetCode)==FALSE){
		ThrowDBException(nRetCode);
	};
	return TRUE;
}

BOOL SRecSet::Update()
{
	UWORD wExpectedRowStatus;
	UWORD wPosOption;
	wPosOption = SQL_UPDATE;
	wExpectedRowStatus = SQL_ROW_UPDATED;
	RETCODE nRetCode;
	OnBind(bindForUpdate);
	AFX_ODBC_CALL(::SQLSetPos(m_hstmt, 1, wPosOption, SQL_LOCK_NO_CHANGE));
	// This should only fail if SQLSetPos returned SQL_SUCCESS_WITH_INFO explaining why
	if (nRetCode == SQL_SUCCESS_WITH_INFO && m_wRowStatus!= wExpectedRowStatus)
		ThrowDBException(AFX_SQL_ERROR_UPDATE_DELETE_FAILED);
	if (Check(nRetCode)==FALSE) 
		ThrowDBException(nRetCode);
	

	return TRUE;
}


BOOL SRecSet::UnBind(int fieldId)
{
	SQLRETURN retcode;
	retcode=SQLBindCol(m_hstmt,fieldId,SQL_C_DEFAULT,NULL,0,NULL);
	if  (SQL_SUCCEEDED(retcode))
		return TRUE;
	else 
		return FALSE;
}

/*  SQL_CONCUR_READ_ONLY (DEFAULT)
	SQL_CONCUR_LOCK
	SQL_CONCUR_ROWVER
	SQL_CONCUR_VALUES */
BOOL SRecSet::SetAttr(long attribute, void *var, long varlen)
{
	RETCODE nRetCode;
	nRetCode=::SQLSetStmtAttr (m_hstmt,attribute,var,varlen); 
	return (SQL_SUCCEEDED(nRetCode));
}

BOOL SRecSet::GetAttr(long attribute, void *var, long varlen)
{
	RETCODE nRetCode;
	nRetCode=::SQLGetStmtAttr(m_hstmt,attribute,var,varlen,NULL); 
	return (SQL_SUCCEEDED(nRetCode));
}

/*
SQL_CURSOR_FORWARD_ONLY  (DEFAULT)
SQL_CURSOR_STATIC
SQL_CURSOR_KEYSET_DRIVEN
SQL_CURSOR_DYNAMIC         
*/
BOOL SRecSet::SetCursorType(int type)
{
	return SetAttr(SQL_ATTR_CURSOR_TYPE,(SQLPOINTER)type,0);
}
/* 
SQL_CONCUR_READ_ONLY (DEFAULT)
SQL_CONCUR_LOCK
SQL_CONCUR_ROWVER
SQL_CONCUR_VALUES */

BOOL SRecSet::SetConCurrency(int type)
{
	return SetAttr(SQL_ATTR_CONCURRENCY,(SQLPOINTER)type,0);
}

BOOL SRecSet::UnBindAll()
{	RETCODE retcode;
	retcode=::SQLFreeStmt(m_hstmt,SQL_UNBIND);
	return (SQL_SUCCEEDED(retcode));
}

RETCODE SRecSet::SkipDeletedRecords(UWORD wFetchType, long nRows)
{
	ASSERT(wFetchType == SQL_FETCH_RELATIVE ||
		wFetchType == SQL_FETCH_FIRST ||
		wFetchType == SQL_FETCH_NEXT ||
		wFetchType == SQL_FETCH_LAST ||
		wFetchType == SQL_FETCH_PRIOR);

	UWORD wDeletedFetchType = wFetchType;
	DWORD dwDeletedRows = abs(nRows);
	BOOL m_bDone;
	RETCODE ret;
	switch (wFetchType)
	{
	case SQL_FETCH_FIRST:
		wDeletedFetchType = SQL_FETCH_NEXT;
		break;

	case SQL_FETCH_LAST:
		wDeletedFetchType = SQL_FETCH_PRIOR;
		break;

	case SQL_FETCH_RELATIVE:
		if (nRows > 0)
			wDeletedFetchType = SQL_FETCH_NEXT;
		else
			wDeletedFetchType = SQL_FETCH_PRIOR;
		break;
	}

	// First fetch is as expected unless relative fetch
	if (wFetchType != SQL_FETCH_RELATIVE)
	{
		ret=::SQLFetchScroll(m_hstmt,wFetchType, nRows);
		if (!SQL_SUCCEEDED(ret)) return ret;
		m_bDone = !(m_wRowStatus==SQL_ROW_DELETED);
	}else{
		ret= ::SQLFetchScroll (m_hstmt,wDeletedFetchType, 1);
		if (!(m_wRowStatus==SQL_ROW_DELETED)){
			dwDeletedRows--;
			m_bDone = dwDeletedRows == 0;
		}
		else
			m_bDone = FALSE;
	}

	// Continue fetching until all req'd deleted records skipped
	while (ret != SQL_NO_DATA_FOUND && !m_bDone)
	{
		ret = ::SQLFetchScroll(m_hstmt,wDeletedFetchType, 1);
		if (wFetchType == SQL_FETCH_RELATIVE){
			if (!(m_wRowStatus==SQL_ROW_DELETED))
			{
				dwDeletedRows--;
				m_bDone = dwDeletedRows == 0;
			}
			else
				m_bDone = FALSE;
		}
		else
			m_bDone = !(m_wRowStatus==SQL_ROW_DELETED);
	}
	return ret;
}

void SRecSet::BindTimeStruct(int bindflag,int fieldid, TIMESTAMP_STRUCT *ptime)
{
	ASSERT(fieldid<MAXCOLNUMBER);
	long *retlen=&mcolstatus[fieldid];
	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:///For read
		*retlen=sizeof(TIMESTAMP_STRUCT);
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_TIMESTAMP,ptime,sizeof(TIMESTAMP_STRUCT),retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else{
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp: // check if it is null;
		if (*retlen==SQL_NULL_DATA) {
			memset(ptime,0,sizeof(TIMESTAMP_STRUCT));
		}
		break;
	case bindForUpdate:
	case bindForNew:  // for update and new;
		if (ptime->year==00) {
			*retlen=SQL_NULL_DATA;
		}else {
			*retlen=sizeof(TIMESTAMP_STRUCT);
		}
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_TIMESTAMP,ptime,sizeof(TIMESTAMP_STRUCT),retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else{
			ThrowDBException(retcode);
		}			
	}
	return ;
}

void SRecSet::BindDouble(int bindflag,int fileld, double *pdouble)
{
	ASSERT(fileld<MAXCOLNUMBER);
	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:
	case bindForUpdate:
	case bindForNew:  // for update and new;
		mcolstatus[fileld]=sizeof(long);
		retcode=SQLBindCol(m_hstmt,fileld,SQL_C_DOUBLE,pdouble,sizeof(double),&mcolstatus[fileld]);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else 
			ThrowDBException(retcode);
		break;
	case bindFixUp:
		if (mcolstatus[fileld]==SQL_NULL_DATA) {
			*pdouble=0;
		}
		break;
	}
	return ;
	
}

void SRecSet::BindTime_t(int bindflag,int fieldid,time_t *ptm_t,TIMESTAMP_STRUCT *ptmbuff)
{
	ASSERT(fieldid<MAXCOLNUMBER);
	long *retlen=&mcolstatus[fieldid];
	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:///For read
		*retlen=sizeof(TIMESTAMP_STRUCT);
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_TIMESTAMP,ptmbuff,sizeof(TIMESTAMP_STRUCT),retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp: // check if it is null;
		if (*retlen==SQL_NULL_DATA) {
			memset(ptmbuff,0,sizeof(TIMESTAMP_STRUCT));
			ptm_t=0;
		}else {
			TranslateDate(ptm_t,ptmbuff,FALSE);  
		}
		break;
	case bindForUpdate:
	case bindForNew:  // for update and new;
		if (ptm_t ==0) {
			*retlen=SQL_NULL_DATA;
			memset(ptmbuff,0,sizeof(TIMESTAMP_STRUCT));
		}else {
			*retlen=sizeof(TIMESTAMP_STRUCT);
			TranslateDate(ptm_t,ptmbuff,TRUE);
		}
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_TIMESTAMP,ptmbuff,sizeof(TIMESTAMP_STRUCT),retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else 
			ThrowDBException(retcode);
	}
	return;

}

void SRecSet::BindLong(int bindflag,int fileld, long *plong)
{
	ASSERT(fileld<MAXCOLNUMBER);

	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:
	case bindForUpdate:
	case bindForNew:  // for update and new;
		mcolstatus[fileld]=sizeof(long);
		retcode=SQLBindCol(m_hstmt,fileld,SQL_C_LONG,plong,sizeof(long),&mcolstatus[fileld]);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp:
		if (mcolstatus[fileld]==SQL_NULL_DATA) {
			*plong=0;
		}
		break;
	}
	return;
	
}

void SRecSet::BindWORD(int bindflag,int fileld, WORD *pword)
{
	ASSERT(fileld<MAXCOLNUMBER);

	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:
	case bindForUpdate:
	case bindForNew:  // for update and new;
		mcolstatus[fileld]=sizeof(WORD);
		retcode=SQLBindCol(m_hstmt,fileld,SQL_C_SHORT,pword,sizeof(WORD),&mcolstatus[fileld]);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp:
		if (mcolstatus[fileld]==SQL_NULL_DATA) {
			*pword=0;
		}
		break;
	}
	return ;
	
}

void SRecSet::BindBYTE(int bindflag,int fieldid, BYTE *pbyte)
{
	ASSERT(fieldid<MAXCOLNUMBER);
	long *retlen=&mcolstatus[fieldid];
	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:
	case bindForUpdate:
	case bindForNew:  // for update and new;
		*retlen=sizeof(long);
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_TINYINT,pbyte,sizeof(BYTE),retlen);
		if  (SQL_SUCCEEDED(retcode))
			return;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp:
		if (*retlen==SQL_NULL_DATA) {
			*pbyte=0;
		}
		break;
	}
	return;
}

void SRecSet::BindText(int bindflag,int fieldid, char *pstr, long strlen)
{
	ASSERT(fieldid<MAXCOLNUMBER);
	long *retlen=&mcolstatus[fieldid];
	RETCODE retcode;
	switch(bindflag) {
	case bindForRead:
		*retlen=SQL_NTS;
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_CHAR,pstr,strlen,retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindForUpdate:
	case bindForNew:  // for update and new;
		if (pstr[0]==0){
			*retlen=SQL_NULL_DATA;
		}else {
			*retlen=SQL_NTS;
		}
		retcode=SQLBindCol(m_hstmt,fieldid,SQL_C_CHAR,pstr,strlen,retlen);
		if  (SQL_SUCCEEDED(retcode))
			return ;
		else {
			ThrowDBException(retcode);
		}
		break;
	case bindFixUp:
		if (*retlen==SQL_NULL_DATA) {
			*pstr=0;
		}
		break;
	}
	return;
}

BOOL SRecSet::EnableBookMark(BOOL bEnable)
{
	int val;
	if (bEnable) val=SQL_UB_OFF;
	else val=SQL_UB_VARIABLE;
	return SetAttr(SQL_ATTR_USE_BOOKMARKS,(SQLPOINTER)val,0);
}

long SRecSet::GetRowCount()
{
	RETCODE retcode;
	SQLINTEGER retnum; 
	retcode=::SQLRowCount(m_hstmt,&retnum);
	if (SQL_SUCCEEDED(retcode)) {
		return retnum;
	}else {
		return -1;
	}
}


void SRecSet::TranslateDate(time_t *ptm, TIMESTAMP_STRUCT *ptmstamp, BOOL bTo)
{
	struct tm tm1;
	if (bTo) { //copy form ptm to tmpstamp;
		struct tm *ptm1;
		ptm1=localtime(ptm);
		if(ptm1!=NULL){
			memcpy(&tm1,ptm1,sizeof(struct tm));
			ptmstamp->year = tm1.tm_year +1900;
			ptmstamp->month= tm1.tm_mon +1;
			ptmstamp->day= tm1.tm_mday;
			ptmstamp->hour=tm1.tm_hour;
			ptmstamp->minute=tm1.tm_min;
			ptmstamp->second=tm1.tm_sec;
			ptmstamp->fraction=0; 
		}else {
			memset(ptmstamp,0,sizeof(TIMESTAMP_STRUCT));
		}
	}else { //copy from tmpstamp to ptm;
		if (ptmstamp->year==0) {
			*ptm=0;
			return;
		}
		tm1.tm_year=ptmstamp->year -1900;
		tm1.tm_mon=ptmstamp->month-1;
		tm1.tm_mday=ptmstamp->day;
		tm1.tm_hour=ptmstamp->hour;
		tm1.tm_min=ptmstamp->minute;
		tm1.tm_sec=ptmstamp->second; 
		tm1.tm_isdst=-1; // let computer to set the dst.
		*ptm=mktime(&tm1);
	}	
}

void SRecSet::Close()
{
	int nRetCode;
	ASSERT_VALID(this);

	if (m_hstmt != SQL_NULL_HSTMT){
		AFX_SQL_SYNC(::SQLFreeStmt(m_hstmt, SQL_DROP));
		if(Check(nRetCode)==FALSE) {
			ThrowDBException(nRetCode); 
			AfxMessageBox("SRecSet delete hsmt error!"); 
		}
		m_hstmt = SQL_NULL_HSTMT;
	}
	m_pDatabase = NULL;
}

BOOL SRecSet::OnBind(int nBindFlag)
{
	return TRUE;
}

//DEL BOOL SRecSet::GetErrorMessage(char *lpzError, int mMaxError)
//DEL {
//DEL /*	int msglen,i;
//DEL 	int len;
//DEL 	SQLCHAR  SqlState[6],Msg[SQL_MAX_MESSAGE_LENGTH];
//DEL 	SQLINTEGER    NativeError;
//DEL 	
//DEL 
//DEL 	len=0;
//DEL 	i=1;
//DEL 	
//DEL 	while (SQL_NO_DATA!=SQLGetDiagRec(SQL_HANDLE_STMT, m_hstmt, i, SqlState, &NativeError,
//DEL             Msg, sizeof(Msg), &MsgLen)) {
//DEL //      DisplayError(SqlState,NativeError,Msg,MsgLen);
//DEL       i++;
//DEL    }
//DEL */
//DEL 	return TRUE;
//DEL }

BOOL SRecSet::GetErrorMessage(CString &str)
{
	SQLSMALLINT MsgLen,i;
	SQLCHAR  SqlState[SQL_SQLSTATE_SIZE+1],Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER    NativeError;
	i=1;
	
	str.Empty();
	while (SQL_NO_DATA!=SQLGetDiagRec(SQL_HANDLE_STMT, m_hstmt, i, SqlState, &NativeError,
            Msg, sizeof(Msg), &MsgLen)) {
		if (! str.IsEmpty()) {
			str+="\xd\xa";
		}
		str+=CString(SqlState);
		str+=":";
		str+=CString(Msg);
//      DisplayError(SqlState,NativeError,Msg,MsgLen);
      i++;
   }
   return TRUE;
}
