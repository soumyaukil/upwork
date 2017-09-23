SET NAMES 'UTF8';

DROP TABLE IF EXISTS mNewAccount;

CREATE TABLE mNewAccount (
    accid bigint,
    OpenBP  decimal,
    OpenNBP decimal,
	    openEQ decimal,
    OpenMGEQ decimal,
    OpenEQRQ decimal,
	    CurrBP decimal,
    CurrNBP decimal,
    CurrHoldBP decimal,

    currEQ decimal,
    CurrMGEQ decimal,
    CurrEQRQ decimal,
    Comm decimal,
	    m_date bigint
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


	  
) ;