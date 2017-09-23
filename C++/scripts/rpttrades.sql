SET NAMES 'UTF8';

DROP TABLE IF EXISTS rpttrades;

CREATE TABLE rpttrades (
    tradeid INTEGER PRIMARY KEY,
    sttrid VARCHAR(10),
    staccid VARCHAR(10),
    stentrid VARCHAR(10),
    storigacc VARCHAR(10),
    storigtrid VARCHAR(10),
    ststatus VARCHAR(1),
    origoid INTEGER,
    orderid INTEGER,
    brshort VARCHAR(5),
    route VARCHAR(10),
    bkrsym VARCHAR(5),
    rrno VARCHAR(8),
    Buy VARCHAR(1),
    Short VARCHAR(1),
    mtype INTEGER,
    Limt VARCHAR(1),
    solicit VARCHAR(1),
    execnum INTEGER,
    qty INTEGER,
    Price DECIMAL,
    secsym VARCHAR(20),
    exchange VARCHAR(1),
    Commision DECIMAL,
    bidprice DECIMAL,
    askprice DECIMAL,
    c_date BIGINT,
    e_date BIGINT,
    StatusBit INTEGER,
    EcnFee DECIMAL,
    TrType VARCHAR(1)
);