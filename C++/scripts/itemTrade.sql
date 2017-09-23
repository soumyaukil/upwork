SET NAMES 'UTF8';

DROP TABLE IF EXISTS itemTrade;

CREATE TABLE itemTrade (
    tradeid INTEGER,
    trid INTEGER,
    accid INTEGER,
    enteid INTEGER,
	    origacc INTEGER,

	    origtrid INTEGER,
		    origoid INTEGER,
	    orderid INTEGER,
		brid varchar(255),
    route varchar(255),
    bkrsym INTEGER,

	    trtype INTEGER,

    status INTEGER,
    execnum VARCHAR(255),
	conftxt VARCHAR(255),
	token INTEGER,
	qty INTEGER,
	

	secsym VARCHAR(255),
	exchange INTEGER,
	price decimal,
	
	askprice decimal,
	bidprice decimal,
		commision decimal,

	rrno VARCHAR(255),
		e_date bigint,
	c_date bigint,
	m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
) ;