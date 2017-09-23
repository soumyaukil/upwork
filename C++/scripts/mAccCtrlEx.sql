SET NAMES 'UTF8';

DROP TABLE IF EXISTS mAccCtrlEx;

CREATE TABLE mAccCtrlEx (
    accid INTEGER,
    flag INTEGER,
    flagex INTEGER,
    maxloss decimal,
	maxshare INTEGER,
	maxtotalshare INTEGER,
	maxsymbshare INTEGER,
	maxsymbs INTEGER,
	maxunrealizedloss DECIMAL,
	starttradetime bigint,
	stoptradetime bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;