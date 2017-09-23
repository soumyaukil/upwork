SET NAMES 'UTF8';

DROP TABLE IF EXISTS mExchAgree;

CREATE TABLE mExchAgree (
    trid INTEGER,
    type INTEGER,
    event INTEGER,
    note varchar(255),
	    logDate bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;