SET NAMES 'UTF8';

DROP TABLE IF EXISTS mSymbLst;

CREATE TABLE mSymbLst (
    ListId bigint,
    SecSymb  varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


	  
) ;