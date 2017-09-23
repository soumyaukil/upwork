SET NAMES 'UTF8';

DROP TABLE IF EXISTS WatchInfo;

CREATE TABLE WatchInfo (
    WatchName varchar(255),
    ServerID  INTEGER,
    idx INTEGER,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;