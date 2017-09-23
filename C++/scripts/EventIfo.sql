SET NAMES 'UTF8';

DROP TABLE IF EXISTS EventIfo;

CREATE TABLE EventIfo (
    groupid INTEGER,
    eventid INTEGER,
    eventName varchar(255),
    expTime INTEGER,
	recvstatus INTEGER,
	  recvtime INTEGER,
	  recvmsg varchar(255)

,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;