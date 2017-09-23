SET NAMES 'UTF8';

DROP TABLE IF EXISTS maccountifo;

CREATE TABLE maccountifo (
    accid bigint,
	
	    name varchar(255),
    brid bigint,
    status bigint,
    rrno varchar(255),
    linkacc varchar(255),
    per_id bigint,
    add1_id bigint,
    add2_id bigint,
    c_date bigint,
    m_date bigint,
    note varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
    
	  
);