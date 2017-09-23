SET NAMES 'UTF8';

DROP TABLE IF EXISTS userfeedchange;

CREATE TABLE userfeedchange (
    FirmId varchar(255),
    Branch varchar(255),
    LoginId varchar(255),
    Feed varchar(255),
	    Action varchar(255),
			    ActTime bigint
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;