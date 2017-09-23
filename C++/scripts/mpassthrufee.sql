SET NAMES 'UTF8';

DROP TABLE IF EXISTS mpassthrufee;

CREATE TABLE mpassthrufee (
    brid bigint,
    MonthNu  INTEGER,
    dealid bigint,
	    path varchar(255),
    costpertkt decimal,
    costperexe decimal,
	    costpershr decimal,
    costpercxl decimal,
    flag bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	


	  
) ;