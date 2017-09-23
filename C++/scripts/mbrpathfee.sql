SET NAMES 'UTF8';

DROP TABLE IF EXISTS mbrpathfee;

CREATE TABLE mbrpathfee (
    MonthNu INTEGER,
    brid INTEGER,
    ptfeename varchar(255),
    costpertkt decimal,
	    costperexe decimal,
	    costpershr decimal,
			    costpercxl decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;