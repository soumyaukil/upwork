SET NAMES 'UTF8';

DROP TABLE IF EXISTS UserTktIfo;

CREATE TABLE UserTktIfo (
    FirmID varchar(255),
    Branch varchar(255),
    LoginID varchar(255),
    Route varchar(255),
	    Secsym varchar(255),
	    Exch varchar(255),
		    Tickets INTEGER,
	    Shares INTEGER,
		MktVal decimal,
				Comm decimal,
						ECN decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;