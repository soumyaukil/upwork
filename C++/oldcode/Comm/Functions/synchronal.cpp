// synchronal.cpp: implementation of the APICritical class.
//
//////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
//#include "orderserv.h"
#include "synchronal.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

APICritical::APICritical()
{
	InitializeCriticalSection(&m_sect);
}

APICritical::~APICritical()
{
	DeleteCriticalSection(&m_sect);
}

