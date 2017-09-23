// Property.h: interface for the Property class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTY_H__C60C9471_49ED_11D4_BCD5_00105A82D2D1__INCLUDED_)
#define AFX_PROPERTY_H__C60C9471_49ED_11D4_BCD5_00105A82D2D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "FixLenMem.h"
struct prop_float
{
	char name[12];		// length is 12 means that they can memory aliemented
	double val;			// the value for this property
	prop_float * pnext;	// the next property
};

class Property  
{	
simplelock mlock;
public:
	BOOL SetPropertyBag(Property *pprobag);
	BOOL ClearAll();
	prop_float * GetNext();
	prop_float * GetFistPosition();
	BOOL SetDBLProperty(LPCSTR prname, double prval);
	double GetDBLProperty(LPCSTR prName);
	static FixLenMem mdatabuff;
	Property();
	virtual ~Property();
	prop_float *phead;
	prop_float *pCurr;

};

#endif // !defined(AFX_PROPERTY_H__C60C9471_49ED_11D4_BCD5_00105A82D2D1__INCLUDED_)
