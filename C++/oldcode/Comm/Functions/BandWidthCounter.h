#ifndef BANDWIDTHCOUNTER
#define BANDWIDTHCOUNTER

class CBandWidthCounter
{
	DWORD m_nLastSec;
	DWORD m_nLastBytes;
	DWORD m_nLastPkgs;

public:

	DWORD m_nTotalBytes;
	DWORD m_nTotalPkgs;

	int m_ByteSec;
	int m_PkgSec;

public:
	void Clear();
	CBandWidthCounter();
	void Count();
	void GetPkg(int nByte);
};

#endif