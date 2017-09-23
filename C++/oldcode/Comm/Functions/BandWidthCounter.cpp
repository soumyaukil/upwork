#include "stdafx.h"
#include "BandWidthCounter.h"

CBandWidthCounter::CBandWidthCounter()
{
	Clear();
}

void CBandWidthCounter::GetPkg(int nByte)
{
	m_nTotalBytes+=nByte;
	m_nTotalPkgs++;
}

void CBandWidthCounter::Count()
{
	DWORD ntm=GetTickCount();
	if (m_nLastSec==0) m_nLastSec=ntm; 
	if (ntm==m_nLastSec) return;
	m_ByteSec=(m_nTotalBytes-m_nLastBytes)*1000/(ntm-m_nLastSec);
	m_PkgSec =(m_nTotalPkgs-m_nLastPkgs)*1000/(ntm-m_nLastSec);
	
/*	char str[1000];
	sprintf(str,"%d %d %d %d %d",m_nTotalBytes,m_nLastBytes,ntm,m_nLastSec,m_ByteSec);
	AfxMessageBox(str);
*/
	
	m_nLastBytes=m_nTotalBytes;
	m_nLastPkgs=m_nTotalPkgs;
	m_nLastSec=ntm;

}

void CBandWidthCounter::Clear()
{
	m_nTotalBytes=0;
	m_nTotalPkgs=0;

	m_nLastSec=0;
	m_nLastBytes=0;
	m_nLastPkgs=0;

	m_ByteSec=0;
	m_PkgSec=0;

}
