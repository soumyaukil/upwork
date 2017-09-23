SET NAMES 'UTF8';

DROP TABLE IF EXISTS brfeed;

CREATE TABLE brfeed (
    brid INTEGER,
    logon varchar(255),
    FeeItem varchar(255),
    StartTm varchar(255),
	 EndTm varchar(255),
	RptMonth varchar(255),
	FeeTkts INTEGER,
	FeeDays INTEGER,
	BranchPays decimal,
    CustomerPays decimal,
     version INTEGER,
	 CreateDatetime timestamp,
	 Active bit,
	 UpdateRef VARCHAR(255)
    

	  
) ;