SET NAMES 'UTF8';

DROP TABLE IF EXISTS mTrader;

CREATE TABLE mTrader (
    trid bigint,
    tracno varchar(255),
    pwd varchar(255),
    brid bigint,
    status bigint,
    per_id bigint,
    add1_id bigint,
    add2_id bigint,
    c_date bigint,
    m_date bigint,
    note varchar(255),
    status1 bigint,
    status2 bigint,
    status3 bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

) ;