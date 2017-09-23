SET NAMES 'UTF8';

DROP TABLE IF EXISTS GenConfig;

CREATE TABLE GenConfig (
    id VARCHAR(255),
    subid  VARCHAR(255),
    content VARCHAR(255)
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;