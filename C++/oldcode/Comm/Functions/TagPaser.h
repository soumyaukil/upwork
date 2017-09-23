// TagPaser.h: interface for the CTagParser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TAGPASER_H__87286A7F_A966_407E_8D2B_C848DA4CDB70__INCLUDED_)
#define AFX_TAGPASER_H__87286A7F_A966_407E_8D2B_C848DA4CDB70__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTagParser  
{
	char  m_fildsep;
	char  m_msgsep;
	char *m_buffer;
	int   m_currpos;
	int   m_isend;
public:
	CTagParser();
	virtual ~CTagParser();

	void SetBuffer(char *buf,char fildsep,char msgsep);
	int  GetTag(char* &pstr);
	int AddTag(int fildid, const char *pstr,int isend);
	inline int MsgLen() {return m_currpos;}
};

#endif // !defined(AFX_TAGPASER_H__87286A7F_A966_407E_8D2B_C848DA4CDB70__INCLUDED_)
