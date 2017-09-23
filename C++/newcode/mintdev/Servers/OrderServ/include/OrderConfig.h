#ifndef OrderConfig_h
#define OrderConfig_h

#include <spdlog/spdlog.h>
#include <stdint.h>
#include <Functions/intmap.h>
#include <Functions/Config.h>

#define MaxServerNumber 50

class OrderConfig : CConfig
{
public:
        //The MktOpen and MktClose decide the market hours.
        time_t mtmMktOpen;
        time_t mtmMktClose;

        time_t mtmFirstSecond;// today's first second;
        time_t mtmQuit;
        time_t mtmClose; //the time to close today's trade. time t
        long   mIsClosed; //0; not closed, 1: closing, 2: closed
        char   mlicense[100];
        char   mlicenseDeCode[100];

public:
        std::string mdbConnectString;
        char mSystemName[50];
        uint16_t mServerPort1;
        uint16_t mServerPort2;
        uint16_t mServerPort3;
        char mCompanyID[12];  //give up
        char mCompanyIDEx[12];//subid of a giveup
        bool mCompanyIDCheck;
        char mQSVToken[20];
        int  mServerNumber ;
        uint8_t mServerIDlst[MaxServerNumber];
        uint32_t mServerIPlst[MaxServerNumber];
        uint16_t mServerPortlst[MaxServerNumber];
        std::string mWelcomeMsg;
        uint8_t mOrderServerID;
        int mBranchNumber;// possible branch number for harsh table
        int mAccountNumber;//possible Account number
        int mTraderNumber;//possible Trader Number

private:
        uint32_t mCurrOrderID;// the current orderid of this order server;
        uint32_t mCurrTradeID;//the current Trade ID, the hi byte is server id;
        uint32_t mCurrTicketID; //the new ticket id,

        bool  mbResetOrderID;//every day restart will reset the orderid. default is TRUE;
public:
        strmap mECNLST;
        strmap mETFLST;

        strmap mLogonAllow;
        strmap mLogonReject;
        intmap mLogonSysidBlock;

        intlst mScriptTask;

        strmap mScriptLogon; //only the user in this map able to run script;

        char   mrouteboxlst[10][256];//this is for client to retrive, the trading window route dropdown box.
        int    mrouteboxlstidx;

public:// order handle  
        double mMKTOrderBPHoldRate;

        //mMKTHoldSWCHPrice: this is for the orders. how much we should full requirement.
        //usually this value will be little bigger than mFullRequireSwitch.
        //mFullRequireSwitch is NASD rule. but mMKTHoldSWCHprice if for broker 
        //who want more hold buying power for orders incase order got executed

        double mMKTHoldSWCHPrice;
        double mCommBPPreHold;

        //bit 0: shortable list 0, default shortable;
        //bit 1: shortable list 1: default shortable;
        int mbDefShortAble;//if TRUE, for security is shortable if not in shortlist;
                                                //if FALSE for security is not shortable if not in shortlist;
        bool mbEnableOption;
        double mShortPriceSwitch;
        double mShortPriceSwitch1;

        int mDayMargBPRate;
        int mNightMargBPRate;

        bool mbRecvBPForCash;//if TRUE, for cash position, don't recover the buyingpower
        double mFullRequireSwitch;

        // how many times Order bp can excess the position bp for the SPECIAL ACCOUNT
        // In Account infor, status bit 17, the position BP control;
        int mOrderBpCtrlRatio;

        uint16_t m_VersionBlock;
        uint16_t m_VersionUpGrade;
        std::string m_msgBlock;
        std::string m_msgUpGrade;

        char mMMRoute[10];
        bool mMMRouteEnable;
        bool mAutoConnectTest;
        int msqlInitTimeOut;
        int msqlTimeOut;
                        // if this set to true, we will do the automatic database write everytime data come back.
                        // otherwise only write to log file.
                        // and when day close, we write it to database.
        bool mRealTimeDBWrite;

        bool mbEcnFeeInCap; //for the deal, if the ecn fee not in cap, max/per ticket not count ECN Fee
        bool mbTradeEcnFee; //Caculate the ecn fee for each Trade; set to false may speed trading.
        bool mbDVPKeepPosition;//if this is seted, the dvp account will not erase position;
        bool mbARCAUNoSpecialCharge;//for NBLE and NBLT, ARCAU charge will be the same under 1$
        int mTSXByBasisPoint; //if this is not 0,we charge TSX ECN fee by basis points.---for SUSA

        bool mbRecOrderActiveToLog; //Record the order active to log file. 
public:
        uint32_t GetNewTicketID();
        uint32_t GetNewTradeID();
        uint32_t GetNewOrderID();
        bool IsMktHours(time_t t1);
        void LoadRouteCtrl();
        void DoDayClose();
        bool AutoConnectRoute();
        int GetSecType(char *symbol, char nexchange);
        bool ConnectAllRoute();
        time_t GetExpTime();
        void SetMaxTradeID(uint32_t tid);
        void SetMaxOrderID(uint32_t moid);
        bool DisConnectAllRoute();
        bool IsTestSecurity(char *secsym);
        int GetLoginCtrl(const char* id,uint32_t ip);
        void GetLogonCtrlItem(const char* id,const char* subid,const char* value);
        void LoadLogonCtrl();
        bool SetBalance(uint8_t ServerID, uint32_t ServerIp,uint16_t ServerPort);
        bool GetMarginAble(char *secsym,uint8_t cIssueMkt);
        bool IsECN(char *pmmid);
        int GetShortAbleSize(char * secsym,uint8_t cIssueMkt,int nlist);
        bool Exit();
        bool init();
        std::shared_ptr<spdlog::logger> mlog;
        std::shared_ptr<spdlog::logger> mLoginLog;
        std::string    mPathLog;
        int OnGetCfg(const char* id, const char* subid, const char* value);
        OrderConfig();
        virtual ~OrderConfig();
};

#endif
