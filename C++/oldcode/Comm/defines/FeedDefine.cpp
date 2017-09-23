#include "stdafx.h"
#include "FeedDefine.h"

static char* s_FeedItem[]={
	"Level 1",		//enFeedL1=0,
	"Level 2",		//enFeedL2=1,
	"Time&Sale",		//enFeedTrade=2,
	"Chart",		//enFeedChart=3, //
	"Option",	//enFeedOption=4,
	"ECN TimeSale",	//enFeedBkTmSel=6,
	"ISLD Book",	//enFeedBkIsld=5,
	"BRUT Book",	//enFeedBkBrut=7,
	"ARCA Book",	//enFeedBkArca=8,
	"INCR Book",	//enFeedBkInca=9,
	"Toplist",		//enFeedTopList=10,
	"Option Lv2",   //the Option Level 2;
	"OPEN Book",	//NYSE book;
	"PinkSheetL1",   //PinkSheet quotes;
	"PinkSheetL2",   //PinkSheet quotes;
	"CanadaStocks",  //Canada stocks;
	NULL
};


static DFeedConfig *theFeedCfg=NULL;;


const char *DFeedConfig::GetFeedName(enFeedItem feed)
{
	if ((feed<0) ||(feed>=enFeedMaxItem)) return "N/A";
	return s_FeedItem[feed];
}


enFeedItem DFeedConfig::GetFeedId(const char *pname)
{
	int i;
	for (i=0;s_FeedItem[i];i++) {
		if (stricmp(pname,s_FeedItem[i])==0) {
			return (enFeedItem)i;
		}
	}
	return (enFeedItem)-1;
}


int	DFeedConfig::GetFeedIP_PORT(enFeedItem feed, unsigned long &nip, unsigned short &nport)
{
	if ((feed<0) ||(feed>=enFeedMaxItem)) return 0;
	if (theFeedCfg==NULL) return 0;
	nip=theFeedCfg->m_ip[feed];
	nport=theFeedCfg->m_port[feed]; 
	return 1;
};

int	DFeedConfig::SetFeedIP_PORT(enFeedItem feed, unsigned long nip, unsigned short nport)
{
	if ((feed<0)||(feed>=enFeedMaxItem)) return 0;
	if (theFeedCfg==NULL){
		theFeedCfg=new DFeedConfig;
		for (int i=0;i<enFeedMaxItem;i++){
			theFeedCfg->m_ip[i]=0;
			theFeedCfg->m_port[i]=0;
		}
	}
	theFeedCfg->m_ip[feed]=nip;
	theFeedCfg->m_port[feed]=nport; 
	return 1;
};
