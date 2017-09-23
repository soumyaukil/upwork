#pragma once

#include "..\Functions\intmap.h"
#include "..\defines\items.h"
#include "..\defines\itemsquote.h"


class itemsSet 
{
public:
//Orders item;
	intmap m_ordermap;
	intmap m_tradesmap;
	intmap m_accountmap;
	intmap m_positionmap;
	intmap m_tradermap;
	intmap m_equitymap;
	intmap m_accountlink;
//Quotes item;
	strmap m_symbmap;
public:
	itemsyminfo * GetSymbol(LPCSTR symbol, BOOL baddnew=TRUE);

	const char * GetAccName(int accid);
	int GetAccid(char *acc);
	void Init();
	itemAcct_Link * FindAcc_Link(int trid, int accid);
};

extern itemsSet *GetItemSet();// the calling function should implment.