// TagParser.cpp: implementation of the CTagParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TagPaser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTagParser::CTagParser()
{

}

CTagParser::~CTagParser()
{

}

int  CTagParser::GetTag(char* &pstr)
{
	int tag=-1;
	char *ptagst;
	pstr="";
	if (m_isend) return tag;

	ptagst=m_buffer+m_currpos;
	while (1) {
		if (m_buffer[m_currpos]==m_fildsep) {
			m_buffer[m_currpos]=0;
			m_currpos++;	
			return tag;
		}else if (m_buffer[m_currpos]=='=') {
			pstr=m_buffer+m_currpos+1;
			m_buffer[m_currpos]=0;
			m_currpos++;
			tag=atoi(ptagst);
			continue;
		}else if (m_buffer[m_currpos]==m_msgsep) {
			m_isend=1;
			m_buffer[m_currpos]=0;
			return tag;
		}
	}
}

void CTagParser::SetBuffer(char *buf, char fildsep, char msgsep)
{
	m_buffer=buf;
	m_fildsep=fildsep;
	m_msgsep=msgsep;
	m_currpos=0;
	m_isend=0;
}

int CTagParser::AddTag(int fildid, const char *pstr, int isend)
{
	if (pstr==NULL) {
//		m_currpos+=sprintf(m_buffer+m_currpos,"%d=%s%c",fildid,pstr,(isend==0)?m_fildsep:m_msgsep);
	}else {
		m_currpos+=sprintf(m_buffer+m_currpos,"%d=%s%c",fildid,pstr,(isend==0)?m_fildsep:m_msgsep);
	}
	return m_currpos;
}
