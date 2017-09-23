SET NAMES 'UTF8';

DROP TABLE IF EXISTS mAccCtrlEx_Route;

CREATE TABLE mAccCtrlEx_Route (
    accid INTEGER,
    mroute  varchar(255),
    mflag INTEGER
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;