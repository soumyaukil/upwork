SET NAMES 'UTF8';

DROP TABLE IF EXISTS mSecIfo;

CREATE TABLE mSecIfo (
    secsym varchar(255),
	flag INTEGER,
	issuemkt INTEGER,
	tiresize INTEGER,
	shortsize INTEGER,
	yClose	decimal,
	version INTEGER,  CreateDatetime timestamp, Active bit,UpdateRef VARCHAR(255)
);