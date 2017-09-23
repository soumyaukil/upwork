SET NAMES 'UTF8';

DROP TABLE IF EXISTS userfeed;

CREATE TABLE userfeed (
    FirmId varchar(255),
    Branch varchar(255),
    LoginId varchar(255),
    Feed varchar(255),
     version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
    
) ;