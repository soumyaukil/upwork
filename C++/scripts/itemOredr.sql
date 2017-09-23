SET NAMES 'UTF8';

DROP TABLE IF EXISTS itemOrder;

CREATE TABLE itemOrder (
    orderId INTEGER,
    trid INTEGER,
    accid INTEGER,
    enteid INTEGER,
    origtkn INTEGER,
    brid varchar(255),
    route varchar(255),
    bkrsym INTEGER,
    status INTEGER,
    execnum VARCHAR(255),
	conftxt VARCHAR(255),
	token INTEGER,
	qty INTEGER,
	lvsqty INTEGER,
	cxlqty VARCHAR(255),
	secsym VARCHAR(255),
	exchange INTEGER,
	price decimal,
	stopprice decimal,
	trailprice decimal,
	askprice decimal,
	bidprice decimal,
	rrno VARCHAR(255),
	c_date bigint,
	m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
) ;