SET NAMES 'UTF8';

DROP TABLE IF EXISTS mOrders;

CREATE TABLE mOrders (
    Orderid bigint,
    trid bigint,
    accid bigint,
    entrid bigint,
    origoid bigint,
    origtkn bigint,
   brid bigint,
    route varchar(255),
    bkrsym varchar(255),
	
    status bigint,
    execnum VARCHAR(255),
	conftxt VARCHAR(255),
	token varchar(255),
	qty bigint,
	lvsqty bigint,
	cxlqty bigint,

	
	price decimal,
	stopprice decimal,
	trailer decimal,

	
	secsym VARCHAR(255),
	exchange INTEGER,
	ask decimal,
	bid decimal,
	rrno VARCHAR(255),
	timeforce bigint,

	c_date bigint,
	m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255),
	 s1 int,
	f1 int,
	f2 int,
	f3 int
    
) ;