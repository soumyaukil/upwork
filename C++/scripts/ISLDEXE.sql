SET NAMES 'UTF8';

DROP TABLE IF EXISTS ISLDEXE;

CREATE TABLE ISLDEXE (
    branch varchar(255),
    trader varchar(255),
    account varchar(255),
    AddLiqExe INTEGER,
	    AddLiqshr INTEGER,
	    RedLiqExe INTEGER,
		    RedLiqshr INTEGER,
	    tfrom bigint,
		tto bigint
	  ,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
) ;