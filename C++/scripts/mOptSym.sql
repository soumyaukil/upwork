SET NAMES 'UTF8';

DROP TABLE IF EXISTS mOptSym;

CREATE TABLE mOptSym (
    optsymb varchar(255),
    expday  bigint,
    strikepice bigint,
	    undsymb varchar(255),
    pmundlyshr bigint,
    m_date bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


	  
) ;