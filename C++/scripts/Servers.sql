SET NAMES 'UTF8';

DROP TABLE IF EXISTS Servers;

CREATE TABLE Servers (
    SvId INTEGER,
    SvName varchar(255),
    SvType INTEGER,
    SvLp INTEGER,
	    SvPort INTEGER,
	    SvPower INTEGER,
		    SvStart INTEGER,
	    SvStop INTEGER
,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	  
) ;