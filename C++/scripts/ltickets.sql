SET NAMES 'UTF8';

DROP TABLE IF EXISTS ltickets;

CREATE TABLE ltickets (
    brid bigint,
    route  varchar(255),
    secsym varchar(255),
	    bkrsym varchar(255),
    status bigint,
    qty bigint,
	    comm decimal,
    trid bigint,
    exchange decimal,
	    price decimal,
    comm1 decimal
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	


	  
) ;