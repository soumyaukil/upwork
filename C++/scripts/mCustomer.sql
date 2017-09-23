SET NAMES 'UTF8';

DROP TABLE IF EXISTS mCustomer;

CREATE TABLE mCustomer (
    Per_id bigint,
    brid bigint,
    entity_type varchar(255),
    title varchar(255),
    entity_name varchar(255),
    fname varchar(255),
    mname varchar(255),
    lname varchar(255),
    SSN varchar(255),
    phone varchar(255),
    email varchar(255),
    c_date bigint,

	    m_date bigint,
    note varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    



	  
) ;