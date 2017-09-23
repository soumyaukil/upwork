SET NAMES 'UTF8';

DROP TABLE IF EXISTS mbrdeal;

CREATE TABLE mbrdeal (
    MonthNu INTEGER,
    brid INTEGER,
    dname varchar(255),
    intval INTEGER,
	    dblval decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;