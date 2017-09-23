SET NAMES 'UTF8';

DROP TABLE IF EXISTS mPosition;

CREATE TABLE mPosition (
    accid bigint,
    secsym varchar(255),
    ptype varchar(255),
    brid bigint,
    InitQty bigint,
    InitPrice decimal,
    qty bigint,
    AvgCost decimal,
    c_date bigint,
    m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;