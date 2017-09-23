SET NAMES 'UTF8';

DROP TABLE IF EXISTS laccount;

CREATE TABLE laccount (
    type varchar(255),
    val  bigint,
	    oldval  bigint,
    trid  bigint,
    accid  bigint,
    note  varchar(255),
    opid  bigint,
    logdate  bigint,
    status1  bigint,
    status2  bigint,
    status3  bigint,
    oldstatus1  bigint,
    oldstatus2  bigint,
    oldstatus3  bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;