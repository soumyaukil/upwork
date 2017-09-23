#ifndef __FEED_DEFINE_H
#define __FEED_DEFINE_H
#pragma once

enum enFeedItem {

	enFeedL1=0,
	enFeedL2=1,
	enFeedTrade=2,
	enFeedChart=3, // this is for minute chart point retrive
	enFeedOption=4,
	enFeedBkTmSel=6,
	enFeedBkIsld=5,
	enFeedBkBrut=7,
	enFeedBkArca=8,
	enFeedBkInca=9,
	enFeedTopList=10,
	enFeedOptionLv2,
	enFeedBkOpen,
	enFeedPinkSheetL1,
	enFeedPinkSheetL2,
	enFeedCanadaL1,
	enFeedMaxItem
};



class DFeedConfig 
{
	unsigned long  m_ip[enFeedMaxItem];
	unsigned short m_port[enFeedMaxItem]; 
public:
	static const char *GetFeedName(enFeedItem feed);
	static enFeedItem GetFeedId(const char *pname);
	static int	GetFeedIP_PORT(enFeedItem feed, unsigned long &nip, unsigned short &nport);
	static int	SetFeedIP_PORT(enFeedItem feed, unsigned long nip, unsigned short nport);
};

#endif