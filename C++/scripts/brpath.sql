SET NAMES 'UTF8';

DROP TABLE IF EXISTS brpath;

CREATE TABLE brpath (
    brid INTEGER,
    path varchar(255),
    tkts INTEGER,
    exes INTEGER,
	    shrs INTEGER,

	    cxls INTEGER,
		    pertlts decimal,
	    perexes decimal,
		pershrs decimal,
    percxls decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

	  
) ;