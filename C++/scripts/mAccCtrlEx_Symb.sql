SET NAMES 'UTF8';

DROP TABLE IF EXISTS mAccCtrlEx_Symb;

CREATE TABLE mAccCtrlEx_Symb (
    accid INTEGER,
    symbol varchar(255),
    maxpos INTEGER,
    maxloss decimal,
	    maxorder INTEGER
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;