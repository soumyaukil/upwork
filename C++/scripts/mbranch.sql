SET NAMES 'UTF8';

DROP TABLE IF EXISTS mbranch;

CREATE TABLE mbranch (
    brid bigint,
    short  varchar(255),
    status bigint,
	    name varchar(255),
    mPer_ID bigint,
    Add1_id bigint,
	    c_date bigint,
    m_date bigint,
	    note varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    



	  
) ;