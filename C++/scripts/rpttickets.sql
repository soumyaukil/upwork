SET NAMES 'UTF8';

DROP TABLE IF EXISTS rpttickets;

CREATE TABLE rpttickets (
    ticketid INTEGER PRIMARY KEY,
    sttrid VARCHAR(10),
    staccid VARCHAR(10),
    stentrid VARCHAR(10),
    Orderid INTEGER,
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
    e_date bigint,
    c_date bigint,
    StatusBit INTEGER,
    EcnFee DECIMAL
);