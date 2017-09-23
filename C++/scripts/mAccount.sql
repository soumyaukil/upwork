SET NAMES 'UTF8';

DROP TABLE IF EXISTS mAccount;

CREATE TABLE mAccount (
    accid bigint,
    OpenBP decimal,
    OpenNBP decimal,
    OpenEQ decimal,
    OpenMGEQ decimal,
    OpenEQRQ decimal,
    CurrBP decimal,
    CurrNBP decimal,
    CurrHoldBP decimal,
    currEQ decimal,
    CurrMGEQ decimal,
    CurrEQRQ decimal,
    Comm decimal,
    m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
    

	  
) ;