// OrderDB.cpp: implementation of the OrderDB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OrderServ.h"
#include "OrderDB.h"
#include "OrderConfig.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

OrderDB theOrderDB;
extern OrderConfig theCfgFile;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OrderDB::OrderDB()
{

}

OrderDB::~OrderDB()
{

}

BOOL OrderDB::ConnectOrderSV(LPCSTR str)
{	
	BOOL dbret;
	dbret = mdb.OpenEx(str);// , CDatabase::useCursorLib);
	if (dbret==FALSE) {
		return FALSE;
	}
	if (mdb.IsOpen()==FALSE) {
		return FALSE;
	}
	m_rstsysid.m_pDatabase=&mdb;
	m_rstsysid.Open();// CRecordset::snapshot, NULL, CRecordset::none);
	return TRUE;
	
}

BOOL OrderDB::CloseConnection()
{
	if(m_rstsysid.IsOpen()) {
		m_rstsysid.Close();
	}
	if (mdb.IsOpen()==TRUE) {
		mdb.Close();
	}
	return TRUE;

}

int OrderDB::ChangeSysID(LPCSTR key, int changeval)
{	char filter[100];
	int retcnu;
	
	sprintf(filter,"mkey='%s'",key);
	m_rstsysid.m_strFilter=filter;
	m_rstsysid.Requery(); 
	retcnu=m_rstsysid.GetRecordCount();
	if (retcnu==1){
		if (changeval==0) return m_rstsysid.m_m_id;
		else {
			m_rstsysid.Edit();
			m_rstsysid.m_m_id+=+changeval;
			m_rstsysid.Update();
			return m_rstsysid.m_m_id;
		}
	}else if(retcnu==0){
		if (changeval==0) return 0;
		else {
			m_rstsysid.AddNew();
			m_rstsysid.m_mkey=key;
			m_rstsysid.m_m_id=changeval;
			m_rstsysid.Update();
			return changeval;
		}
	}else {
		theCfgFile.mlog.AddLogStr("Error! Multi item in the sys id");
		AfxMessageBox("Error! Multi item in the sys id");
		return 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// rstSysID

IMPLEMENT_DYNAMIC(rstSysID, CRecordset)

rstSysID::rstSysID(CDatabase* pdb)
	: CRecordset(pdb)
{
	//{{AFX_FIELD_INIT(rstSysID)
	m_mkey = _T("");
	m_m_id = 0;
	m_nFields = 2;
	//}}AFX_FIELD_INIT
	m_nDefaultType = dynaset;
}


CString rstSysID::GetDefaultConnect()
{
	return _T("");
}

CString rstSysID::GetDefaultSQL()
{
	return _T("select * from msys_id");
}

void rstSysID::DoFieldExchange(CFieldExchange* pFX)
{
	//{{AFX_FIELD_MAP(rstSysID)
	pFX->SetFieldType(CFieldExchange::outputColumn);
	RFX_Text(pFX, _T("[mkey]"), m_mkey);
	RFX_Long(pFX, _T("[m_id]"), m_m_id);
	//}}AFX_FIELD_MAP
}

/////////////////////////////////////////////////////////////////////////////
// rstSysID diagnostics

#ifdef _DEBUG
void rstSysID::AssertValid() const
{
	CRecordset::AssertValid();
}

void rstSysID::Dump(CDumpContext& dc) const
{
	CRecordset::Dump(dc);
}
#endif //_DEBUG


BOOL OrderDB::cfgGet(LPCSTR id, LPCSTR subid, char *val, int size)
{
	CString str;
	CString retstr;
	CString tmpstr;
	memset(val,0,size);
	if (subid==NULL) {
		tmpstr="''";
	}else {
		tmpstr.Format("'%s'",subid);
	}
	str.Format("Select content from  msys_info where id='%s' and subid = %s", id,tmpstr);
	CRecordset mrecst;
	mrecst.m_pDatabase=&mdb; 
	mrecst.Open(AFX_DB_USE_DEFAULT_TYPE,str);
	val[0]=0;
	if (mrecst.IsEOF() && mrecst.IsBOF() )  {
	} else {
		mrecst.MoveFirst();
		CString tmpst;
		mrecst.GetFieldValue("content",tmpst);
		strncpy(val,tmpst,size);
	}
	mrecst.Close();
	return TRUE;
}


//This function format the time string form sql server;
//format string should be "%m/%d/%y %H:%M:%S"
//this function is copy from CConfig::FormatTimeYMDHMS.
char * OrderDB::FormatSQLTime(time_t ntime, char *buffer)
{
	if (ntime<=0) {
		time(&ntime);
	}

	struct tm* ptmTemp = localtime(&ntime);
	if (ptmTemp == NULL ||
		!_tcsftime(buffer, 20, "%m/%d/%y %H:%M:%S", ptmTemp))
		buffer[0] = '\0';


	return buffer;
}
