#include "stdafx.h"
#include "items.h"
//#include "Report.h"
#include "..\Functions\strfuncs.h"


int itemOrder::GetPkg(PkgToken *ptok, DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getDWORD();
	if (flag&deforderid) morderid=ptok->getlong();
	if (flag&deftrid) mtrid=ptok->getlong();
	if (flag&defaccid) maccid=ptok->getlong();
	if (flag&defentrid) mentrid=ptok->getlong();
	if (flag&deforigoid) morigoid=ptok->getlong();
	if (flag&deforigtkn) morigtkn=ptok->getlong();
	if (flag&defbrid) mbrid=ptok->getlong();
	if (flag&defroute) ptok->getString(mroute,sizeof(mroute));
	if (flag&defbkrsym) ptok->getString(mbkrsym,sizeof(mbkrsym));
	if (flag&defstatus) mstatus=ptok->getlong();
	if (flag&defexecnum) mexecnum=ptok->getBYTE();
	if (flag&defconftxt) ptok->getString(mconftxt,sizeof(mconftxt));
	if (flag&deftoken) ptok->getString(mtoken,sizeof(mtoken)); 
	if (flag&defqty) mqty=ptok->getlong();
	if (flag&deflvsqty) mlvsqty=ptok->getlong();
	if (flag&defcxlqty) mcxlqty=ptok->getlong();
	if (flag&defsecsym) ptok->getString(msecsym,sizeof(msecsym));
	if (flag&defexchange) mexchange=ptok->getBYTE();
	if (flag&defprice) mprice=ptok->getDouble();
	if (flag&defstopprice) mstopprice=ptok->getDouble();
	if (flag&deftrailprice) mtrailprice=ptok->getDouble();
	if (flag&defaskprice) maskprice=ptok->getDouble();
	if (flag&defbidprice) mbidprice=ptok->getDouble();
	if (flag&defrrno) ptok->getString(mrrno,sizeof(mrrno));
	if (flag&deftmforce) mtmforce=ptok->getWORD();
	if (flag&defc_date) mc_date=ptok->getlong();
	if (flag&defm_date) mm_date=ptok->getlong();
	if (flag&defmexts1) ms1=ptok->getlong();
	if (flag&defmextf1) mf1=ptok->getlong();
	if (flag&defmextf2) mf2=ptok->getlong();
	if (flag&defmextf3) mf3=ptok->getlong();


	*pflag=flag;
	return TRUE;
}

void itemOrder::CopyFrom(itemOrder *pitem, DWORD flag)
{
	if (flag&itemOrder::deforderid) morderid=pitem->morderid;
	if (flag&deftrid		) mtrid=pitem->mtrid;
	if (flag&defaccid		) maccid=pitem->maccid;
	if (flag&defentrid		) mentrid=pitem->mentrid;
	if (flag&deforigoid		) morigoid=pitem->morigoid;
	if (flag&deforigtkn		) morigtkn=pitem->morigtkn;
	if (flag&defbrid		) mbrid=pitem->mbrid;
	if (flag&defroute		) strcpy(mroute,pitem->mroute);
	if (flag&defbkrsym		) strcpy(mbkrsym,pitem->mbkrsym);
	if (flag&defstatus		) mstatus=pitem->mstatus;
	if (flag&defexecnum		) mexecnum=pitem->mexecnum;
	if (flag&defconftxt		) strcpy(mconftxt,pitem->mconftxt);
	if (flag&deftoken		) strcpy(mtoken,pitem->mtoken);
	if (flag&defqty			) mqty=pitem->mqty;
	if (flag&deflvsqty		) mlvsqty=pitem->mlvsqty;
	if (flag&defcxlqty		) mcxlqty=pitem->mcxlqty;
	if (flag&defsecsym		) strcpy(msecsym,pitem->msecsym);
	if (flag&defexchange	) mexchange=pitem->mexchange;
	if (flag&defprice		) mprice=pitem->mprice;
	if (flag&defstopprice	) mstopprice=pitem->mstopprice;
	if (flag&deftrailprice	) mtrailprice=pitem->mtrailprice;
	if (flag&defaskprice	) maskprice=pitem->maskprice;
	if (flag&defbidprice	) mbidprice=pitem->mbidprice;
	if (flag&defrrno		) strcpy(mrrno,pitem->mrrno);
	if (flag&deftmforce		) mtmforce=pitem->mtmforce;
	if (flag&defc_date		) mc_date=pitem->mc_date;
	if (flag&defm_date		) mm_date=pitem->mm_date;
	if (flag&defmexts1      ) ms1=pitem->ms1;
	if (flag&defmextf1		) mf1=pitem->mf1;
	if (flag&defmextf2		) mf2=pitem->mf2;
	if (flag&defmextf3		) mf3=pitem->mf3;


}

void itemOrder::CopyFrom(itemOrder *pitem)
{
	ASSERT(pitem!=NULL);
	memmove(this,pitem,sizeof(itemOrder));
}

DWORD itemOrder::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigoid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigtkn=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mconftxt,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtoken,ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mlvsqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mcxlqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mstopprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mtrailprice=atof(ptmp);
	ptmp=tok.GetStr(',');	maskprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mbidprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mtmforce=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)	ms1=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)	mf1=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)   mf2=atoi(ptmp);
	ptmp=tok.GetStr(',');	if (ptmp)   mf3=atoi(ptmp);
	return 0xffffffff;
}

BOOL itemOrder::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%O,");
	sprintf(tok.SeekToEnd(),"%d,", morderid);
	sprintf(tok.SeekToEnd(),"%d,", mtrid);
	sprintf(tok.SeekToEnd(),"%d,", maccid);
	sprintf(tok.SeekToEnd(),"%d,", mentrid);
	sprintf(tok.SeekToEnd(),"%d,", morigoid);
	sprintf(tok.SeekToEnd(),"%d,", morigtkn);
	sprintf(tok.SeekToEnd(),"%d,", mbrid);
	sprintf(tok.SeekToEnd(),"%s,", mroute);
	sprintf(tok.SeekToEnd(),"%s,", mbkrsym);
	sprintf(tok.SeekToEnd(),"%d,", mstatus);
	sprintf(tok.SeekToEnd(),"%d,", mexecnum);
	sprintf(tok.SeekToEnd(),"%s,", mconftxt);
	sprintf(tok.SeekToEnd(),"%s,", mtoken);
	sprintf(tok.SeekToEnd(),"%d,", mqty);
	sprintf(tok.SeekToEnd(),"%d,", mlvsqty);
	sprintf(tok.SeekToEnd(),"%d,", mcxlqty);
	sprintf(tok.SeekToEnd(),"%s,", msecsym);
	sprintf(tok.SeekToEnd(),"%d,", mexchange);
	sprintf(tok.SeekToEnd(),"%g,", mprice);
	sprintf(tok.SeekToEnd(),"%g,", mstopprice);
	sprintf(tok.SeekToEnd(),"%g,", mtrailprice);
	sprintf(tok.SeekToEnd(),"%g,", maskprice);
	sprintf(tok.SeekToEnd(),"%g,", mbidprice);
	sprintf(tok.SeekToEnd(),"%s,", mrrno);
	sprintf(tok.SeekToEnd(),"%d,", mtmforce);
	sprintf(tok.SeekToEnd(),"%d,", mc_date);
	sprintf(tok.SeekToEnd(),"%d,", mm_date);
	sprintf(tok.SeekToEnd(),"%d,",ms1); 
	sprintf(tok.SeekToEnd(),"%d,",mf1); 
	sprintf(tok.SeekToEnd(),"%d,",mf2); 
	sprintf(tok.SeekToEnd(),"%d,",mf3); 
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}


BOOL itemTrade::GetPkg(PkgToken *ptok, DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getDWORD();
	if (flag&deftradeid	) mtradeid=ptok->getlong();
	if (flag&deftrid	) mtrid=ptok->getlong();
	if (flag&defaccid	) maccid=ptok->getlong();
	if (flag&defentrid	) mentrid=ptok->getlong();
	if (flag&deforigacc	) morigacc=ptok->getlong();
	if (flag&deforigtrid) morigtrid=ptok->getlong();
	if (flag&deforigoid	) morigoid=ptok->getlong(); 
	if (flag&deforderid	) morderid=ptok->getlong();
	if (flag&defbrid	) mbrid=ptok->getlong();
	if (flag&defroute	) ptok->getString(mroute,sizeof(mroute));
	if (flag&defbkrsym	) ptok->getString(mbkrsym,sizeof(mbkrsym));
	if (flag&deftrtype	) mtrtype=ptok->getBYTE();
	if (flag&defstatus	) mstatus=ptok->getlong();
	if (flag&defexecnum	) mexecnum=ptok->getBYTE();
	if (flag&defconftxt	) ptok->getString(mconftxt,sizeof(mconftxt));
	if (flag&deftoken	) ptok->getString(mtoken,sizeof(mtoken));
	if (flag&defqty		) mqty=ptok->getlong();
	if (flag&defprice	) mprice=ptok->getDouble();
	if (flag&defsecsym	) ptok->getString(msecsym,sizeof(msecsym));
	if (flag&defexchange) mexchange=ptok->getBYTE();
	if (flag&defaskprice) maskprice=ptok->getDouble();
	if (flag&defbidprice) mbidprice=ptok->getDouble();
	if (flag&defrrno	) ptok->getString(mrrno,sizeof(mrrno));
	if (flag&defcommision) mcommision=ptok->getDouble();
	if (flag&defe_date	) me_date=ptok->getlong();
	if (flag&defc_date	) mc_date=ptok->getlong();
	if (flag&defm_date	) mm_date=ptok->getlong();
	*pflag=flag;
	return TRUE;
}

void itemTrade::CopyFrom(itemTrade *pitem, DWORD flag)
{
	if (flag&deftradeid	) mtradeid=pitem->mtradeid;
	if (flag&deftrid		) mtrid=pitem->mtrid;
	if (flag&defaccid	) maccid=pitem->maccid;
	if (flag&defentrid	) mentrid=pitem->mentrid;
	if (flag&deforigacc	) morigacc=pitem->morigacc;
	if (flag&deforigtrid	) morigtrid=pitem->morigtrid;
	if (flag&deforigoid	) morigoid=pitem->morigoid;
	if (flag&deforderid	) morderid=pitem->morderid;
	if (flag&defbrid		) mbrid=pitem->mbrid;
	if (flag&defroute	) strcpy(mroute,pitem->mroute);
	if (flag&defbkrsym	) strcpy(mbkrsym,pitem->mbkrsym);
	if (flag&deftrtype	) mtrtype=pitem->mtrtype;
	if (flag&defstatus	) mstatus=pitem->mstatus;
	if (flag&defexecnum	) mexecnum=pitem->mexecnum;
	if (flag&defconftxt	) strcpy(mconftxt,pitem->mconftxt);
	if (flag&deftoken	) strcpy(mtoken,pitem->mtoken);
	if (flag&defqty		) mqty=pitem->mqty;
	if (flag&defprice	) mprice=pitem->mprice;
	if (flag&defsecsym	) strcpy(msecsym,pitem->msecsym);
	if (flag&defexchange	) mexchange=pitem->mexchange;
	if (flag&defaskprice	) maskprice=pitem->maskprice;
	if (flag&defbidprice	) mbidprice=pitem->mbidprice;
	if (flag&defrrno		) strcpy(mrrno,pitem->mrrno);
	if (flag&defcommision) mcommision=pitem->mcommision;
	if (flag&defe_date	) me_date=pitem->me_date;
	if (flag&defc_date	) mc_date=pitem->mc_date;
	if (flag&defm_date	) mm_date=pitem->mm_date;
}

DWORD itemTrade::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	mtradeid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigacc=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morigoid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mtrtype=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mconftxt,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtoken,ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	maskprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mbidprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mcommision=atof(ptmp);
	ptmp=tok.GetStr(',');	me_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	return 0xffffffff;

};

DWORD itemTicket::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mticketid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mentrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtradeid=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mbkrsym,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mexecnum=atoi(ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	mexchange=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mcommision=atof(ptmp);
	ptmp=tok.GetStr(',');	me_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mcomm1=atof(ptmp);
	return 0xffffffff;

}

BOOL itemTicket::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%t,");
	sprintf(tok.SeekToEnd(),"%d,", 	mticketid	);
	sprintf(tok.SeekToEnd(),"%d,", 	mtrid		);
	sprintf(tok.SeekToEnd(),"%d,", 	maccid		);
	sprintf(tok.SeekToEnd(),"%d,", 	mentrid		);
	sprintf(tok.SeekToEnd(),"%d,", 	mtradeid	);
	sprintf(tok.SeekToEnd(),"%d,", 	morderid	);
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid		);
	sprintf(tok.SeekToEnd(),"%s,",	mroute		);
	sprintf(tok.SeekToEnd(),"%s,", 	mbkrsym		);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus		);
	sprintf(tok.SeekToEnd(),"%d,", 	mexecnum	);
	sprintf(tok.SeekToEnd(),"%d,", 	mqty		);
	sprintf(tok.SeekToEnd(),"%g,", 	mprice		);
	sprintf(tok.SeekToEnd(),"%s,", 	msecsym		);
	sprintf(tok.SeekToEnd(),"%d,", 	mexchange	);
	sprintf(tok.SeekToEnd(),"%s,", 	mrrno		);
	sprintf(tok.SeekToEnd(),"%g,", 	mcommision	);
	sprintf(tok.SeekToEnd(),"%d,", 	me_date		);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date		);
	sprintf(tok.SeekToEnd(),"%g,", 	mcomm1		);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

int itemAccIfo::GetPkg(PkgToken *ptok,DWORD *pflag)
{
	DWORD flag;
	flag=ptok->getWORD();
	if (flag & 1)		maccid=ptok->getlong(); 
	if (flag & (1<<1))	ptok->getString(mname,sizeof(mname));
	if (flag & (1<<2))	mbrid=ptok->getDWORD();
	if (flag & (1<<3))  mstatus=ptok->getDWORD();
	if (flag & (1<<4))  ptok->getString(mrrno,sizeof(mrrno));
	if (flag & (1<<5))	mlinkacc =ptok->getDWORD();
	if (flag & (1<<7)) 	mper_id =ptok->getDWORD();
	if (flag & (1<<8))	maddr1_id=ptok->getDWORD();
	if (flag & (1<<9))	maddr2_id =ptok->getDWORD();
	if (flag & (1<<10)) mc_date =ptok->getlong();
	if (flag & (1<<11)) mm_date =ptok->getlong();
	if (flag & (1<<12)) ptok->getString(mnote,sizeof(mnote));
	*pflag=flag;
	return flag;
}

void itemAccIfo::CopyFrom(itemAccIfo *pitem)
{
	memcpy(this,pitem,sizeof(itemAccIfo));
}

void itemAccIfo::CopyFrom(itemAccIfo *pitem, DWORD flag)
{
	if (flag & 1)		maccid=pitem->maccid; 
	if (flag & (1<<1))	strcpy(mname,pitem->mname);
	if (flag & (1<<2))	mbrid=pitem->mbrid;
	if (flag & (1<<3))  mstatus=pitem->mstatus;
	if (flag & (1<<4))  strcpy(mrrno,pitem->mrrno);
	if (flag & (1<<5))	mlinkacc =pitem->mlinkacc;
	if (flag & (1<<7)) 	mper_id =pitem->mper_id;
	if (flag & (1<<8))	maddr1_id=pitem->maddr1_id;
	if (flag & (1<<9))	maddr2_id =pitem->maddr2_id;
	if (flag & (1<<10)) mc_date =pitem->mc_date;
	if (flag & (1<<11)) mm_date =pitem->mm_date;
	if (flag & (1<<12)) strcpy(mnote,pitem->mnote);

}

BOOL itemAccIfo::SaveStr(char *buff, int bufflen, DWORD flag)
{
	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%A,");
	sprintf(tok.SeekToEnd(),"%d,", 	maccid);
	sprintf(tok.SeekToEnd(),"%s,", 	mname);
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus);
	sprintf(tok.SeekToEnd(),"%s,", 	mrrno);
	sprintf(tok.SeekToEnd(),"%d,", 	mlinkacc);
	sprintf(tok.SeekToEnd(),"%d,", 	mper_id);
	sprintf(tok.SeekToEnd(),"%d,", 	maddr1_id);
	sprintf(tok.SeekToEnd(),"%d,", 	maddr2_id);
	sprintf(tok.SeekToEnd(),"%d,", 	mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 	mnote);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;

}

DWORD itemAccIfo::LoadStr(char *buff)
{

	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mname,ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mrrno,ptmp);
	ptmp=tok.GetStr(',');	mlinkacc=atoi(ptmp);
	ptmp=tok.GetStr(',');	mper_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	maddr1_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	maddr2_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	return 0xffffffff;
}

int itemAccEquity::GetPkg(PkgToken *ptok,DWORD *pflag)
{
	DWORD flag=0;
	flag=ptok->getWORD();
	if (flag&defaccid		)	maccid=ptok->getlong();
	if (flag&defOpenBP		)	mOpenBP=ptok->getDouble();		// open buying powering
	if (flag&defOpenNBP		)	mOpenNBP=ptok->getDouble() ;	// open Night buying powing
	if (flag&defOpenEQ		)	mOpenEQ=ptok->getDouble() ;		// open equity
	if (flag&defOpenMGEQ	)	mOpenMGEQ=ptok->getDouble() ;	// open margin equity
	if (flag&defOpenEQRQ	)	mOpenEQRQ=ptok->getDouble() ;	// open Equity require;
	if (flag&defCurrBP		)	mCurrBP=ptok->getDouble() ;		// current buying poweing 
	if (flag&defCurrNBP		)	mCurrNBP=ptok->getDouble() ;	// current overnight buying power,
	if (flag&defCurrHoldBP	)	mCurrHoldBP=ptok->getDouble() ;	// current Order Hold Buying power;
	if (flag&defCurrEQ		)	mCurrEQ=ptok->getDouble() ;		// current EQutiy
	if (flag&defCurrMGEQ	)	mCurrMGEQ=ptok->getDouble() ;	// current Margin EQuity;
	if (flag&defCurrEQRQ	)	mCurrEQRQ=ptok->getDouble() ;	// current EQurity Require;
	if (flag&defComm		)   mComm=ptok->getDouble();		// current commision if this account
	if (flag&deftktnum		)	mtktnum=ptok->getlong();		// the last modify time;
	if (flag&defRealized	)   mRealized=ptok->getDouble(); 
	if (flag&defm_time		)   mm_time=ptok->getlong(); 
	*pflag=flag;
	return flag;
}



BOOL itemAccEquity::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setWORD((WORD)flag);
	if (flag & defaccid		)	ptok->setlong(maccid);
	if (flag & defOpenBP	)	ptok->setDouble(mOpenBP);		// open buying powering
	if (flag & defOpenNBP	)	ptok->setDouble(mOpenNBP) ;	// open Night buying powing
	if (flag & defOpenEQ	)	ptok->setDouble(mOpenEQ) ;		// open equity
	if (flag & defOpenMGEQ	)	ptok->setDouble(mOpenMGEQ) ;	// open margin equity
	if (flag & defOpenEQRQ	)	ptok->setDouble(mOpenEQRQ) ;	// open Equity require;
	if (flag & defCurrBP	)	ptok->setDouble(mCurrBP) ;		// current buying poweing 
	if (flag & defCurrNBP	)	ptok->setDouble(mCurrNBP) ;	// current overnight buying power,
	if (flag & defCurrHoldBP)	ptok->setDouble(mCurrHoldBP) ;	// current Order Hold Buying power;
	if (flag & defCurrEQ	)	ptok->setDouble(mCurrEQ) ;		// current EQutiy
	if (flag & defCurrMGEQ	)	ptok->setDouble(mCurrMGEQ) ;	// current Margin EQuity;
	if (flag & defCurrEQRQ	)	ptok->setDouble(mCurrEQRQ) ;	// current EQurity Require;
	if (flag & defComm		)	ptok->setDouble(mComm);			//current commision;
	if (flag & deftktnum	)	ptok->setlong(mtktnum);		// the last modify time;
	if (flag & defRealized	)	ptok->setDouble(mRealized);
	if (flag & defm_time	)	ptok->setlong(mm_time);		//=ptok->getlong(); 
	return TRUE;
}


void itemAccEquity::CopyFrom(itemAccEquity *pitem)
{
	memcpy(this,pitem,sizeof(itemAccEquity));
}

void itemAccEquity::CopyFrom(itemAccEquity *pitem, DWORD flag)
{
	if (flag & defaccid			)	maccid=pitem->maccid;
	if (flag & defOpenBP		)	mOpenBP=pitem->mOpenBP;// open buying powering
	if (flag & defOpenNBP		)	mOpenNBP=pitem->mOpenNBP;	// open Night buying powing
	if (flag & defOpenEQ		)	mOpenEQ=pitem->mOpenEQ;
	if (flag & defOpenMGEQ		)	mOpenMGEQ=pitem->mOpenMGEQ;
	if (flag & defOpenEQRQ		)	mOpenEQRQ=pitem->mOpenEQRQ;
	if (flag & defCurrBP		)	mCurrBP=pitem->mCurrBP;
	if (flag & defCurrNBP		)	mCurrNBP=pitem->mCurrNBP;
	if (flag & defCurrHoldBP	)	mCurrHoldBP=pitem->mCurrHoldBP;
	if (flag & defCurrEQ		)	mCurrEQ=pitem->mCurrEQ;
	if (flag & defCurrMGEQ		)	mCurrMGEQ=pitem->mCurrMGEQ;
	if (flag & defCurrEQRQ		)	mCurrEQRQ=pitem->mCurrEQRQ;
	if (flag & defComm			)	mComm=pitem->mComm; 
	if (flag & deftktnum		)	mtktnum=pitem->mtktnum;
	if (flag & defRealized		)   mRealized=pitem->mRealized;
	if (flag & defm_time		)   mm_time=pitem->mm_time;
}


BOOL itemTrader::SaveStr(char *buff, int bufflen, DWORD flag)
{

	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%L,");
	sprintf(tok.SeekToEnd(),"%d,", 		mtrid);
	sprintf(tok.SeekToEnd(),"%s,", 		mtracno);

	int i;
	for (i=0;mPWD[i];i++){
		sprintf(tok.SeekToEnd(),"%2.2x",((unsigned char)mPWD[i]));
	}
	sprintf(tok.SeekToEnd(),",");

	sprintf(tok.SeekToEnd(),"%d,", 		mbrid);
	sprintf(tok.SeekToEnd(),"%d,", 		mstatus);
	sprintf(tok.SeekToEnd(),"%d,", 		mtype);
	sprintf(tok.SeekToEnd(),"%d,", 		mPerid);
	sprintf(tok.SeekToEnd(),"%d,", 		mAdd1ID);
	sprintf(tok.SeekToEnd(),"%d,", 		mAdd2ID);
	sprintf(tok.SeekToEnd(),"%d,", 		mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 		mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 		mnote);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

DWORD itemTrader::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mtracno,ptmp);

	ptmp=tok.GetStr(',');	
	int i;
	for (i=0;ptmp[i];i+=2){
		sscanf(ptmp+i,"%2.2x",&mPWD[i/2]);
	}
	mPWD[i/2]=0;

	strcpy(mPWD,ptmp);
	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	mtype=atoi(ptmp);
	ptmp=tok.GetStr(',');	mPerid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mAdd1ID=atoi(ptmp);
	ptmp=tok.GetStr(',');	mAdd2ID=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	return 0xffffffff;
}


BOOL itemBranch::SaveStr(char *buff, int bufflen, DWORD flag)
{

	CStrFunc tok;
	tok.InitToSet(buff);
	tok.AppendStr("%B,");
	sprintf(tok.SeekToEnd(),"%d,", 	mbrid);
	sprintf(tok.SeekToEnd(),"%s,", 	mshort);
	sprintf(tok.SeekToEnd(),"%d,", 	mstatus);
	sprintf(tok.SeekToEnd(),"%s,", 	mname);
	sprintf(tok.SeekToEnd(),"%d,",	mper_id);
	sprintf(tok.SeekToEnd(),"%d,", 	madd_id);
	sprintf(tok.SeekToEnd(),"%d,", 	mc_date);
	sprintf(tok.SeekToEnd(),"%d,", 	mm_date);
	sprintf(tok.SeekToEnd(),"%s,", 	mnote);
	ASSERT(strlen(buff)<(unsigned)bufflen);
	return TRUE;
}

DWORD itemBranch::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);

	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');

	ptmp=tok.GetStr(',');	mbrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mshort,ptmp);
	ptmp=tok.GetStr(',');	mstatus=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mname,ptmp);
	ptmp=tok.GetStr(',');	mper_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	madd_id=atoi(ptmp);
	ptmp=tok.GetStr(',');	mc_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	mm_date=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(mnote,ptmp);
	return 0xffffffff;
}


const char * itemOrder::GetStatusStr()
{
	const char *ret;
	if ((mlvsqty!=0)&&(mstatus&1)==0) {
		ret="Closed";
	}else if(mstatus&(1<<2)) {
		ret="Canceled";
	}else if(mstatus&(1<<3)) {
		ret="Rejected";
	}else if(mstatus&(1<<1)) {
		ret="Executed";
	}else if(mstatus&(1<<5)) {
		ret="Time out";
	}else if (mstatus&(1<<4)) {
		ret="Accepted";
	}else if((mstatus&1)==0) {
		ret="Closed";
	}else{
		ret="Sending";
	}
	return ret;
}



void itemPosition::Clear(BOOL clearall)
{
	if (clearall) {
		maccid=0;
		memset(msecsym,0,sizeof(msecsym));
		mptype=0;
	}
	mbrid=0;
	mInitQty=0;
	mInitPrice=0;
	mqty=0;
	mavgcost=0;
	mBuyPwer=0;
	mRealize=0;
	mc_date=0;
	mm_date=0;
}

int itemPosition::GetPkg(PkgToken *ptok,DWORD *pflag)
{
	WORD flag;
	flag=ptok->getWORD();
	if (flag & iPosAcc) maccid=ptok->getlong();
	if (flag & iPosSec) ptok->getString(msecsym,sizeof(msecsym));
	if (flag & iPosType) mptype=ptok->getBYTE();
	if (flag & iPosBrid) mbrid=ptok->getlong();
	if (flag & iPosInitQty) mInitQty=ptok->getlong();
	if (flag & iPosInitPric) mInitPrice=ptok->getDouble();
	if (flag & iPosQty) mqty=ptok->getlong();
	if (flag & iPosAvgPric) mavgcost=ptok->getDouble();
	if (flag & iPosBP) mBuyPwer=ptok->getDouble();
	if (flag & iPosReal) mRealize=ptok->getDouble();
	if (flag & iPosCdat) mc_date=ptok->getlong();
	if (flag & iPosMdat) mm_date=ptok->getlong();
	*pflag=flag;
	return 1;
}

void itemPosition::CopyFrom(itemPosition *pitem, DWORD flag)
{
	if (flag & iPosAcc) maccid=pitem->maccid;
	if (flag & iPosSec) memccpy(msecsym,pitem->msecsym,0,sizeof(msecsym));
	if (flag & iPosType) mptype=pitem->mptype;
	if (flag & iPosBrid) mbrid=pitem->mbrid;
	if (flag & iPosInitQty) mInitQty=pitem->mInitQty;
	if (flag & iPosInitPric) mInitPrice=pitem->mInitPrice;
	if (flag & iPosQty) mqty=pitem->mqty;
	if (flag & iPosAvgPric) mavgcost=pitem->mavgcost;
	if (flag & iPosBP) mBuyPwer=pitem->mBuyPwer;
	if (flag & iPosReal) mRealize=pitem->mRealize;
	if (flag & iPosCdat) mc_date=pitem->mc_date;
	if (flag & iPosMdat) mm_date=pitem->mm_date;
}

static char * s_pos_type_str[]={"Cash","Margin","Short","Bullet",NULL};
char* itemPosition::GetTypeStr()
{
	switch(mptype) {
	case 1: return s_pos_type_str[0];
	case 2: return s_pos_type_str[1];
	case 3: return s_pos_type_str[2];
	case 4: return s_pos_type_str[3];
	}
	return "Error";
}

char itemPosition::GetTypeVal(LPCSTR ptypename)
{
	if (strcmp(ptypename,s_pos_type_str[0])==0) return 1;
	if (strcmp(ptypename,s_pos_type_str[1])==0) return 2;
	if (strcmp(ptypename,s_pos_type_str[2])==0) return 3;
	if (strcmp(ptypename,s_pos_type_str[3])==0) return 4; //bullet;
	return 0;

}

BOOL itemOrder::SetPkg(PkgToken *ptok, DWORD flag)
{
	ptok->setDWORD(flag);
	if (flag&deforderid) ptok->setlong(morderid);
	if (flag&deftrid) ptok->setlong(mtrid);
	if (flag&defaccid) ptok->setlong(maccid);
	if (flag&defentrid) ptok->setlong(mentrid);
	if (flag&deforigoid) ptok->setlong(morigoid);
	if (flag&deforigtkn) ptok->setlong(morigtkn);
	if (flag&defbrid)	ptok->setlong(mbrid);
	if (flag&defroute) ptok->setVarStr(mroute);
	if (flag&defbkrsym) ptok->setVarStr(mbkrsym);
	if (flag&defstatus) ptok->setlong(mstatus);
	if (flag&defexecnum) ptok->setBYTE(mexecnum);
	if (flag&defconftxt) ptok->setVarStr(mconftxt);
	if (flag&deftoken) ptok->setVarStr(mtoken);
	if (flag&defqty) ptok->setlong(mqty);
	if (flag&deflvsqty) ptok->setlong(mlvsqty);
	if (flag&defcxlqty) ptok->setlong(mcxlqty);
	if (flag&defsecsym) ptok->setVarStr(msecsym);
	if (flag&defexchange) ptok->setBYTE(mexchange);
	if (flag&defprice) ptok->setDouble(mprice);
	if (flag&defstopprice) ptok->setDouble(mstopprice);
	if (flag&deftrailprice) ptok->setDouble(mtrailprice);
	if (flag&defaskprice) ptok->setDouble(maskprice);
	if (flag&defbidprice) ptok->setDouble(mbidprice);
	if (flag&defrrno) ptok->setVarStr(mrrno);
	if (flag&deftmforce) ptok->setWORD(mtmforce);
	if (flag&defc_date) ptok->setlong(mc_date);
	if (flag&defm_date) ptok->setlong(mm_date);
	if (flag&defmexts1) ptok->setlong(ms1);
	if (flag&defmextf1) ptok->setlong(mf1);
	if (flag&defmextf2) ptok->setlong(mf2);
	if (flag&defmextf3) ptok->setlong(mf3);

	return TRUE;
}


BOOL itemAcct_Link::GetStatusStr(char *pbuff)
{
	int i;
	i=0;
	//set the status check button
	if (mstatus & 1) pbuff[i++]='E';
	switch(mstatus &((1<<1)|(1<<2))) {
	case 0:
		pbuff[i++]='O';
		break;
	case (1<<1):
		pbuff[i++]='L';
		break;
	case (1<<2):
		pbuff[i++]='D';
	}
	if (mstatus & (1<<1)) pbuff[i++]='O';
	if (mstatus & (1<<2)) pbuff[i++]='T';

	if (mstatus & (1<<3)) pbuff[i++]='T';
	if (mstatus & (1<<4)) pbuff[i++]='F';
	if (mstatus & (1<<5)) pbuff[i++]='S';
	pbuff[i]=0;
	return TRUE;
}


void itemAcct_Link::CopyItem(itemAcct_Link *pitem)
{
	maccid=pitem->maccid;
	mtrid=pitem->mtrid;
	mdealid=pitem->mdealid;
	mOpdealid=pitem->mOpdealid;
	mFtdealid=pitem->mFtdealid;
	mm_date=pitem->mm_date;
	mstatus=pitem->mstatus;
}

void itemAcct_Link::Clear()
{
	maccid=mtrid=mdealid=mstatus=0;
	mm_date =0;
}

void itemOrderActive::Clear()
{
	memset(this,0,sizeof(itemOrderActive));
}

BOOL itemOrderActive::GetPkg(PkgToken *ptok)
{
	mtrid=ptok->getlong();
	maccid=ptok->getlong();
	mflag=ptok->getlong();
	morderid=ptok->getlong();
	ptok->getString(msecsym,sizeof(msecsym));
	ptok->getString(mroute,sizeof(mroute));
	mprice=ptok->getDouble();
	mqty=ptok->getlong();
	mlogtime=ptok->getlong();
	ptok->getString(mnote,sizeof(mnote));
	return TRUE;
}

BOOL itemOrderActive::SetPkg(PkgToken *ptok)
{
	ptok->setlong(mtrid);
	ptok->setlong(maccid);
	ptok->setlong(mflag);
	ptok->setlong(morderid);
	ptok->setVarStr(msecsym);
	ptok->setVarStr(mroute);
	ptok->setDouble(mprice);
	ptok->setlong(mqty);
	ptok->setlong(mlogtime);
	ptok->setVarStr(mnote); 
	return TRUE;
}



BYTE itemOrderActive::GetType()
{
	return (BYTE) mflag&0xff;

}


char * itemOrderActive::GetTypeStr(char * st)
{
	char stname[20];
	char stt[40];
	sprintf(st,"%s %s",GetTypeName(stname),GetSide(stt));
	return st;
}

char *itemOrderActive::GetTypeName(char *st)
{
	BYTE ntype;
	ntype=GetType();
	switch (ntype) {
	case LOASending:
		strcpy(st,"Sending");
		break;
	case LOASendRej:
		strcpy(st,"Send_Rej");
		break;
	case LOAAccept:
		strcpy(st,"Accept");
		break;
	case LOACanceling:	
		strcpy(st,"Canceling");
		break;
	case LOACanceled:
		strcpy(st,"Canceled");
		break;
	case LOACancelRej:
		strcpy(st,"CancelRej");
		break;
	case LOATimeout:
		strcpy(st,"TimeOut");
		break;
	case LOAExecuted:
		strcpy(st,"Execute");
		break;
	case LOAClose:
		strcpy(st,"Close");
		break;
	default:
		strcpy(st,"N/A");
		break;
	}
	return st;
}

char * itemOrderActive::GetSide(char *st)
{

	if (mflag & 0x100) {
		if (mflag& 0x200){
			strcpy(st,"Buy");
		}else {
			strcpy(st,"Buy");
		}	
	}else {
		if (mflag& 0x200){
			strcpy(st,"Shrt");
		}else {
			strcpy(st,"Sell");
		}	
	}
	return st;
}

DWORD itemOrderActive::LoadStr(char *buff)
{
	CStrFunc tok;
	tok.InitToGet(buff);
	char *ptmp;
	ptmp=tok.GetStr(','); 
	ASSERT(ptmp[0]=='%');
	ptmp=tok.GetStr(',');	mtrid=atoi(ptmp);
	ptmp=tok.GetStr(',');	maccid=atoi(ptmp);
	ptmp=tok.GetStr(',');	mflag=atoi(ptmp);
	ptmp=tok.GetStr(',');	morderid=atoi(ptmp);
	ptmp=tok.GetStr(',');	strcpy(msecsym,ptmp);
	ptmp=tok.GetStr(',');	strcpy(mroute,ptmp);
	ptmp=tok.GetStr(',');	mprice=atof(ptmp);
	ptmp=tok.GetStr(',');	mqty=atoi(ptmp);
	ptmp=tok.GetStr(',');	mlogtime=atoi(ptmp);
	ptmp=tok.GetStr(0);		strcpy(mnote,ptmp);
	return 0xffffffff;

}
