SET NAMES 'UTF8';

DROP TABLE IF EXISTS mLOA;

CREATE TABLE mLOA (
    trid bigint,
    accid  bigint,
    orderid bigint,
	    flag bigint,
    secsym varchar(255),
    route varchar(255),
	    price decimal,
    qty bigint,
    logtime bigint,
    note varchar(255)
	,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


	  
) ;