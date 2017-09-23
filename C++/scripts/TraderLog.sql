SET NAMES 'UTF8';

DROP TABLE IF EXISTS TraderLog;

CREATE TABLE TraderLog (
    mevent varchar(255),
    mtrader varchar(255),
    mtime varchar(255),
    mnote varchar(255),
	    mstatus varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;