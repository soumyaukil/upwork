// ChartFile.h: interface for the ChartFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTFILE_H__32AAC9A1_7CE2_11D4_BCFA_00105A82D2D1__INCLUDED_)
#define AFX_CHARTFILE_H__32AAC9A1_7CE2_11D4_BCFA_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../functions/GTime.h"

struct chart_pkg_data{
	double mhigh;
	double mlow;
	double mfirst;
	double mlast;
	DWORD  mvolume;
	DWORD  mflag;
	GTime  mdatetime;
};

struct chart_file_head{
	char secsym[12];
	time_t lastmdtime;
	DWORD	dayitemnu;
	DWORD	dayitemoffset;
	DWORD	minitemnu;
	DWORD	minitemoffset;
	char	reserved[100-32];
};

class ChartFile  
{
private:
	CFile mdatafile;
	int mtype;
	GTime mtstart;
	GTime mtend;
	int mtotalnumber;
	int	mcurrnumber;
	chart_file_head mcfhead;
public:
	BOOL WriteBuff(chart_pkg_data *pdata, DWORD maxitems);
	BOOL ReadToBuff(chart_pkg_data *pdata, DWORD *maxitems);
	BOOL OpenToRW(LPCSTR filename);
	BOOL OpenToRead(LPCSTR filename,GTime tstart,GTime tend);
	BOOL Close();
	BOOL ReadData(chart_pkg_data *pdata);
	ChartFile();
	virtual ~ChartFile();
};

#endif // !defined(AFX_CHARTFILE_H__32AAC9A1_7CE2_11D4_BCFA_00105A82D2D1__INCLUDED_)
