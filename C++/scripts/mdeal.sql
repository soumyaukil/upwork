SET NAMES 'UTF8';

DROP TABLE IF EXISTS mdeal;

CREATE TABLE mdeal (
    dealid bigint,
    name VARCHAR(255),
    defdeal bigint,
    sharelimit bigint,
    shareadj decimal,
    limitadj decimal,
    marketadj decimal,
    baseshare decimal,
    basecharge decimal,
    sharecap bigint,
    commcap decimal,
    mincomm decimal,
    note VARCHAR(255),
    m_date bigint,
    brid bigint,
    flag bigint,
    valueadj decimal,
    sharecap1 decimal,
    shareadj1 decimal,
    symbolfee decimal,
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
	
	
	
) ;