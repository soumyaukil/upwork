SET NAMES 'UTF8';

DROP TABLE IF EXISTS mNewsInfo;

CREATE TABLE mNewsInfo (
    nid bigint,
    msgbody  varchar(255),
    tsp bigint,
	    cat bigint,
	    symb varchar(255),
    headline varchar(255),
    msglen bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


	  
) ;