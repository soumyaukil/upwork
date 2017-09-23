SET NAMES 'UTF8';

DROP TABLE IF EXISTS macct_access;

CREATE TABLE macct_access (
    trid bigint,
	
	    accid bigint,
    status bigint,
    dealid bigint,
    m_date bigint,
    opdealid bigint,
    ftdealid bigint,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    


    
	  
) ;