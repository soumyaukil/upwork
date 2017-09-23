SET NAMES 'UTF8';

DROP TABLE IF EXISTS rptOrders;

CREATE TABLE rptOrders (
    orderid INTEGER PRIMARY KEY,
    sttrid VARCHAR(10),
    staccid VARCHAR(10),
    stentrid VARCHAR(10),
    origoid INTEGER,
    brshort VARCHAR(5),
    route VARCHAR(10),
    bkrsym VARCHAR(5),
    rrno VARCHAR(8),
    Buy VARCHAR(1),
    Short VARCHAR(1),
    Status VARCHAR(10),
    mtype INTEGER,
    limt VARCHAR(1),
    solicit VARCHAR(1),
    execnum INTEGER,
    qty INTEGER,
    lvsqty INTEGER,
    cxlqty INTEGER,
    Price DECIMAL,
    secsym VARCHAR(20),
    exchange VARCHAR(1),
    bidprice DECIMAL,
    askprice DECIMAL,
    stopprice DECIMAL,
    trailer DECIMAL,
    c_date bigint,
    m_date bigint,
    StatusBit DECIMAL
);