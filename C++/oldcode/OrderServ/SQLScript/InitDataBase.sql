delete laccount;
delete lbranch;
delete loptsym;
delete lorders;
delete lposition;
delete ltickets;
delete ltrades;
delete macclnkifo;
delete maccount;
delete maccountifo;
delete mAccCtrlEx;
delete mAccCtrlEx_Route;
delete mAccCtrlEx_Symb;
delete macct_access;
delete maddress;
delete mbranch;
delete mbrdeal;
delete mbrpathfee;
delete mbrspecchrg;
delete mbrtktrebate;
delete mcustomer;
delete mdeal where dealid<>0;
delete mLOA;
delete mMMUntradable;
delete mMsg;
delete mNewAccount;
delete mNewPosition;
delete mOperator;
delete mOptsym;
delete mOrders;
delete mpassthrufee where dealid<>0;
delete mposition;
delete mSecIfo;
delete mtickets;
delete mtrader where trid<>1;
update msys_id set m_id=2 where mkey like 'Curr%'
delete mTrades;

