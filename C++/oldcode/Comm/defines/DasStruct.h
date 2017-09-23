#ifndef _DAS_STRUCTURES
#define _DAS_STRUCTURES

//this structure is for the risk account control, for risk management
class st_Acc_RiskMgmt
{
	long   maccid; //account id;
	int	   mmaxshare; // max share this trader can trade.
	double mmaxvalue; // the max value this trader can trade;
	double mmaxloss;  //the max loss this trader can take;
	

};


#endif