SET NAMES 'UTF8';

DROP TABLE IF EXISTS mMsg;

CREATE TABLE mMsg (
    mFr varchar(255),
    mTo  varchar(255),
    mid bigint,
	    title varchar(255),
    flag bigint,
    prior varchar(255),
	    txt1 varchar(255),
    txt2 varchar(255),
    txt3 varchar(255),
    txt4 varchar(255),
	    s_date bigint
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	


	  
) ;