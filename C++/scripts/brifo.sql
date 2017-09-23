SET NAMES 'UTF8';

DROP TABLE IF EXISTS brifo;

CREATE TABLE brifo (
    brid INTEGER,
    brshort varchar(255),
    brname varchar(255),
    rptmonth varchar(255),
	    rptfrom varchar(255),

	    rptto varchar(255),
		    tickets INTEGER,
	    chargePerTkt decimal,
		GrossComm decimal,
    CreateDate bigint,
     version INTEGER, 
	 CreateDatetime timestamp,
	 Active bit,
	 UpdateRef VARCHAR(255)
    

	  
) ;