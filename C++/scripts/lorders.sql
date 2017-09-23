SET NAMES 'UTF8';

DROP TABLE IF EXISTS lorders;

CREATE TABLE lorders (
    orderid bigint,
	    trid bigint,
    accid bigint,
    entrid bigint,
    origoid bigint,
    origtkn bigint,
    brid bigint,
	
    route varchar(255),
    bkrsym varchar(255),
    status bigint,
    execnum INTEGER,
    conftxt varchar(255),
	
    token varchar(255),
    qty bigint,
    lvqty bigint,
    cxlqty bigint,
	
    secsym varchar(255),
    exchange varchar(255),
    price decimal,
    stopprice decimal,
    trailer varchar(255),
    ask decimal,
	    bid decimal,
    rrno varchar(255),
    timeforce bigint,
    c_date bigint,
    m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    




	  
) ;