SET NAMES 'UTF8';

DROP TABLE IF EXISTS UserExelfo;

CREATE TABLE UserExelfo (
    FirmId varchar(255),
    Branch varchar(255),
    LoginID varchar(255),
    Route varchar(255),
	    Secsym varchar(255),

	    Exch varchar(255),
		    Shares INTEGER,
	    ECN decimal,
		Liqtype varchar(255),
    ndate bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;