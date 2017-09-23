SET NAMES 'UTF8';

DROP TABLE IF EXISTS RSKMaintain;

CREATE TABLE RSKMaintain (
    secsym VARCHAR(12),
    LongReq DECIMAL,
    ShortReq DECIMAL,
    flag INTEGER
) ;