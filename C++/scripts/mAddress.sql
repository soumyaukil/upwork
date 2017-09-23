SET NAMES 'UTF8';

DROP TABLE IF EXISTS mAddress;

CREATE TABLE mAddress (
    Add_id bigint,
       brid bigint,
    phone varchar(255),
    fax varchar(255),
    address varchar(255),
    city varchar(255),
    state varchar(255),
    zip varchar(255),
    country varchar(255),
    c_date bigint,
    m_date bigint,
    note varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
 
	  
) ;