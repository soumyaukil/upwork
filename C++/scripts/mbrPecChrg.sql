SET NAMES 'UTF8';

DROP TABLE IF EXISTS mbrPecChrg;

CREATE TABLE mbrPecChrg (
    MonthNu INTEGER,
    brid INTEGER,
    tracno varchar(255),
    status INTEGER,
	    charge decimal,
	    charge1 decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;