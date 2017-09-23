SET NAMES 'UTF8';

DROP TABLE IF EXISTS BalanceInfo;

CREATE TABLE BalanceInfo (
    osid INTEGER,
    qsid  INTEGER,
    status VARCHAR(255),
    version INTEGER,  
	CreateDatetime timestamp, 
	Active bit,
	UpdateRef VARCHAR(255)
    
	  
) ;