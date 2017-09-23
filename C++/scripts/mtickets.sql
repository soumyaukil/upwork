SET NAMES 'UTF8';

DROP TABLE IF EXISTS mTickets;

CREATE TABLE mTickets (
    TicketID bigint,
	    trid bigint,
    accid bigint,
    entrid bigint,
    tradeid bigint,
    orderid bigint,
    brid bigint,
    route bigint,
    bkrsym varchar(255),
    status bigint,
    execnum varchar(255),
    qty bigint,
    price decimal,
    secsym varchar(255),
    exchange varchar(255),
    rrno varchar(255),
    comm decimal,
    e_date bigint,
    m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;