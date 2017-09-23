
DROP TABLE mbrtktrebate
go

CREATE TABLE mbrtktrebate (
       brid                 int NULL,
       chargetype           int NULL,
       chargevalue1         float NULL,
       chargevalue2         float NULL,
       baseprice            float NULL,
       chargeprice          float NULL
)
go


DROP TABLE mbrpathfee
go

CREATE TABLE mbrpathfee (
       MonthNu              int NULL,
       brid                 int NULL,
       ptfeename            varchar(10) NULL,
       costpertkt           float NULL,
       costperexe           float NULL,
       costpershr           float NULL,
       costpercxl           float NULL
)
go


DROP TABLE mbrSpecChrg
go

CREATE TABLE mbrSpecChrg (
       MonthNu              int NULL,
       brid                 int NULL,
       tracno               varchar(10) NULL,
       status               int NULL,
       charge               float NULL,
       charge1              float NULL
)
go


DROP TABLE mbrdeal
go

CREATE TABLE mbrdeal (
       MonthNu              int NOT NULL,
       brid                 int NOT NULL,
       dname                varchar(10) NULL,
       intval               int NULL,
       dblval               float NULL
)
go




