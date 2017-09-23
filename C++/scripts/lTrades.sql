SET NAMES 'UTF8';

DROP TABLE IF EXISTS lTrades;

CREATE TABLE lTrades (
    trid bigint,
    accid  bigint,
	    brid  bigint,
		
			    secsym  varchar(255),
	    route  varchar(255),
	    status  bigint,
	    qty  bigint,
	    exchange  varchar(255),
	    price  decimal,
	    bkrsym  varchar(255),
	    comm  decimal,
	    e_date  bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    

		



	  
);