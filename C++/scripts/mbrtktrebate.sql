SET NAMES 'UTF8';

DROP TABLE IF EXISTS mbrtktrebate;

CREATE TABLE mbrtktrebate (
    brid INTEGER,
    chargetype INTEGER,
    chargevalue1 float,
    chargevalue2 float,
	    baseprice float,
			    chargeprice float,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;