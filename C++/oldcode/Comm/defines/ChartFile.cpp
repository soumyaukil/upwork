// ChartFile.cpp: implementation of the ChartFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "ChartServer.h"
#include "ChartFile.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "D:\liuhome\stockprog\Servers\ChartServer\ChartConfig.h"
extern ChartConfig theConfig ;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ChartFile::ChartFile()
{

}

ChartFile::~ChartFile()
{

}



//DEL BOOL ChartFile::OpenToRead(LPCSTR filename, int type, GTime tstart, GTime tend)
//DEL {
//DEL 	mtype=type;
//DEL 	mtstart=tstart;
//DEL 	mtend=tend;
//DEL 
//DEL 	if (mdatafile.Open(filename,CFile::modeRead)==FALSE) {
//DEL 		return FALSE;
//DEL 	};
//DEL 
//DEL 	if(mdatafile.Read(&mcfhead,sizeof(chart_file_head))!=sizeof(chart_file_head)){
//DEL 		return FALSE;
//DEL 
//DEL 	}
//DEL 	mcurrnumber=0;
//DEL 	DWORD offset;
//DEL 	switch(type) {
//DEL 	case 1://minute;
//DEL 		offset=mcfhead.minitemoffset;
//DEL 		mtotalnumber=mcfhead.minitemnu ;
//DEL 		break;
//DEL 	case 2:
//DEL 		offset=mcfhead.dayitemoffset;
//DEL 		mtotalnumber=mcfhead.dayitemnu;
//DEL 		break;
//DEL 	default:
//DEL 		mdatafile.Close(); 
//DEL 		return FALSE;
//DEL 		
//DEL 	}
//DEL 	mdatafile.Seek(offset,CFile::begin);
//DEL 
//DEL /*	char st[100];
//DEL 	CTime t1(tstart);
//DEL 	CTime t2(tend);
//DEL 	sprintf(st,"open to read time %s to %s totalnumber %d ",(LPCSTR)t1.Format("%y/%m/%d %H:%M:%S") ,(LPCSTR)t2.Format("%y/%m/%d %H:%M:%S"), mtotalnumber );
//DEL 	theConfig.mlog.AddLogStr(st);
//DEL */
//DEL 	return TRUE;
//DEL 
//DEL }

BOOL ChartFile::ReadData(chart_pkg_data *pdata)
{
	int nBytesRead;
	while (TRUE) {
		nBytesRead=mdatafile.Read(pdata,sizeof(chart_pkg_data));
		if (nBytesRead<sizeof(chart_pkg_data)) break;
		if (pdata->mdatetime<mtstart) continue;// this should break to avoid full file search.
		if (pdata->mdatetime>mtend) continue;
//		if ((pdata->mdatetime>=mtstart)&&(pdata->mdatetime<=mtend)) { 
			return TRUE;
//		}
	}
	return FALSE;
}

BOOL ChartFile::Close()
{
	mdatafile.Close();
	return TRUE;
}

//DEL BOOL ChartFile::ReadToBuff(LPCSTR filename, chart_pkg_data *daydata, DWORD *maxdayitems, chart_pkg_data *minutedata, DWORD *maxminitems)
//DEL {
//DEL 	
//DEL 	if (mdatafile.Open(filename,CFile::modeRead)==FALSE) {
//DEL 		*maxdayitems=0;
//DEL 		*maxminitems=0;
//DEL 		return FALSE;
//DEL 	};
//DEL 
//DEL 	if(mdatafile.Read(&mcfhead,sizeof(chart_file_head))!=sizeof(chart_file_head)){
//DEL 		*maxdayitems=0;
//DEL 		*maxminitems=0;
//DEL 		mdatafile.Close();
//DEL 		return FALSE;
//DEL 	}
//DEL 
//DEL 	int daynum;
//DEL 	int minnum;
//DEL 	daynum=minnum=0;
//DEL 	int ret;
//DEL 	if (mcfhead.dayitemnu <*maxdayitems ) {
//DEL 		daynum=mcfhead.dayitemnu;
//DEL 	}else {
//DEL 		daynum=*maxdayitems;
//DEL 	}
//DEL 	if (daynum>0) {
//DEL 		mdatafile.Seek(mcfhead.dayitemoffset,CFile::begin);
//DEL 		ret=mdatafile.Read(daydata,sizeof(chart_pkg_data)*daynum);
//DEL 		if (ret>0) {
//DEL 			daynum=(ret/sizeof(chart_pkg_data));
//DEL 		}else {
//DEL 			daynum=0;
//DEL 		}
//DEL 		
//DEL 	}
//DEL 
//DEL 	if (mcfhead.minitemnu <*maxminitems) {
//DEL 		minnum =mcfhead.minitemnu;
//DEL 	}else {
//DEL 		minnum =*maxminitems;
//DEL 	}
//DEL 	if (minnum >0) {
//DEL 		mdatafile.Seek(mcfhead.minitemoffset,CFile::begin);
//DEL 		ret=mdatafile.Read(minutedata,sizeof(chart_pkg_data)*minnum);
//DEL 		if (ret>0) {
//DEL 			minnum=(ret/sizeof(chart_pkg_data));
//DEL 		}else {
//DEL 			minnum=0;
//DEL 		}
//DEL 		
//DEL 	}
//DEL 	*maxdayitems=daynum;
//DEL 	*maxminitems=minnum;
//DEL 	mdatafile.Close();
//DEL 	return TRUE;
//DEL }

//DEL BOOL ChartFile::WriteBuff(LPCSTR secsym,LPCSTR filename, chart_pkg_data *daydata, DWORD dayitems, chart_pkg_data *minutedata, DWORD minitems)
//DEL {
//DEL 
//DEL 	if (mdatafile.Open(filename,CFile::modeWrite|CFile::modeCreate)==FALSE) {
//DEL 		theConfig.mlog.AddLogStr("open file  to write error!"); 
//DEL 		return FALSE;
//DEL 	};
//DEL 
//DEL 	memset(&mcfhead,0,sizeof(mcfhead));
//DEL 	time(&mcfhead.lastmdtime);
//DEL 	mcfhead.dayitemoffset=sizeof(mcfhead);
//DEL 	mcfhead.dayitemnu=dayitems;
//DEL 	mcfhead.minitemoffset=mcfhead.dayitemoffset+dayitems*sizeof(chart_pkg_data);
//DEL 	mcfhead.minitemnu=minitems;
//DEL 	memccpy(mcfhead.secsym,secsym,0,sizeof(secsym));
//DEL 	try{
//DEL 		mdatafile.Write(&mcfhead,sizeof(mcfhead));
//DEL 		mdatafile.Write(daydata,dayitems*sizeof(chart_pkg_data));
//DEL 		mdatafile.Write(minutedata,minitems*sizeof(chart_pkg_data));
//DEL 		mdatafile.Close();
//DEL 		return TRUE;
//DEL 	}
//DEL 	catch(CFileException *e) {
//DEL 		char st[1000];
//DEL 		sprintf(st,"error in write file:%s, %s,%d %d",filename,secsym,dayitems,minitems);
//DEL 		AfxMessageBox(st);
//DEL 		e->GetErrorMessage(st,sizeof(st));
//DEL 		AfxMessageBox(st);
//DEL 		e->Delete();
//DEL 		return FALSE;
//DEL 	}
//DEL }

BOOL ChartFile::OpenToRead(LPCSTR filename,GTime tstart,GTime tend)
{
	if (mdatafile.Open(filename,CFile::modeRead)==FALSE) {
		return FALSE;
	};
	mtstart=tstart;
	mtend=tend;
	return TRUE;
}

BOOL ChartFile::OpenToRW(LPCSTR filename)
{
	CFileException ex;
	if (mdatafile.Open(filename,CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate,&ex)==FALSE) {
		
      TCHAR szError[1024];
      ex.GetErrorMessage(szError, 1024);
      AfxMessageBox("Couldn't open source file: ");
      AfxMessageBox(szError);
      return FALSE;
	};
	return TRUE;
}

BOOL ChartFile::ReadToBuff(chart_pkg_data *pdata, DWORD *maxitems)
{
	long ret;
	DWORD itemnum;
	ret=mdatafile.Read(pdata,sizeof(chart_pkg_data)*(*maxitems));
	if (ret>0) {
		itemnum=(ret/sizeof(chart_pkg_data));
	}else {
		itemnum=0;
	}
	*maxitems=itemnum;
	return TRUE;
}

BOOL ChartFile::WriteBuff(chart_pkg_data *pdata, DWORD maxitems)
{

	mdatafile.SeekToBegin(); 
	try{
		mdatafile.Write(pdata,maxitems*sizeof(chart_pkg_data));
	}catch(CFileException *e) {
		char st[1000];
		sprintf(st,"error in write file:%d",maxitems);
		AfxMessageBox(st);
		e->GetErrorMessage(st,sizeof(st));
		AfxMessageBox(st);
		e->Delete();
		return FALSE;
	}
	return TRUE;

}
