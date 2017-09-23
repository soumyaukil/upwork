SET NAMES 'UTF8';

DROP TABLE IF EXISTS mTrades;

CREATE TABLE mTrades (
    TradeID bigint,
    trid bigint,
    accid bigint,
    entrid bigint,
    origAcc INTEGER, 
	origtrid INTEGER,
    origoid INTEGER,
    OrderID INTEGER,
   brid bigint,
    route varchar(255),
    bkrsym varchar(255),
    trtype varchar(255),
    status bigint,
    execnum VARCHAR(255),
	conftxt VARCHAR(255),
	token INTEGER,
	qty INTEGER,
		price decimal,
	secsym VARCHAR(255),
	exchange INTEGER,
	ask decimal,
	bid decimal,
	rrno VARCHAR(255),
	comm decimal,
	e_date bigint,
	c_date bigint,
	m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
) ;