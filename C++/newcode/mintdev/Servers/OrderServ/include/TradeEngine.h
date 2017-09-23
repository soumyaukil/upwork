// TradeEngine.h: interface for the TradeEngine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRADEENGINE_H__3659E981_D81B_11D4_BD27_0050041B8FEC__INCLUDED_)
#define AFX_TRADEENGINE_H__3659E981_D81B_11D4_BD27_0050041B8FEC__INCLUDED_

#include "FUNCTIONS\intmap.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TradeEngine  
{
public:
	//the fix buff for the position;
	FixLenMem mBufPosition;

public:
	BOOL InitData();
	intmap MapAccid;
	BOOL InitBuffs();
	TradeEngine();
	virtual ~TradeEngine();

};

#endif // !defined(AFX_TRADEENGINE_H__3659E981_D81B_11D4_BD27_0050041B8FEC__INCLUDED_)
