-- Function: dayinit()

-- DROP FUNCTION dayinit();

CREATE OR REPLACE FUNCTION dayinit()
  RETURNS timestamp without time zone AS
$BODY$
declare 
	today  INTEGER;
	thisday  timestamp;
	newday  INTEGER;
	rowcount INTEGER;
BEGIN
	select now() into thisday;

	select EXTRACT(year from thisday) + EXTRACT(month from thisday) + EXTRACT(day from  thisday) into newday;

	select m_id from msys_id where mkey='TODAY' into today ;
	select count(*) into rowcount from msys_id where mkey='TODAY' ;
	
	IF rowcount=0 then
		insert into msys_id (mkey,m_id) values('TODAY',0);
		select 0 into today;
	END IF;
	
	if today<newday then
		update msys_id set m_id=newday where mkey='TODAY';		
		select do_backup_Day_data() into rowcount ;
	END IF;

	delete from mMsg;

	return thisday ;
END;$BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION dayinit()
  OWNER TO postgres;

  
-- Function: do_backup_day_data()

-- DROP FUNCTION do_backup_day_data();

CREATE OR REPLACE FUNCTION do_backup_day_data()
  RETURNS integer AS
$BODY$
DECLARE
	tempInt INTEGER;
BEGIN
	
	insert into lOrders(Orderid,trid,accid,entrid,origoid,origtkn,brid,route,bkrsym,status,execnum,conftxt,token,qty,cxlqty,secsym,exchange,price,ask,bid,rrno,timeforce,c_date,m_date,version,CreateDatetime,Active,UpdateRef)
		select Orderid,trid,accid,entrid,origoid,origtkn,brid,route,bkrsym,status,execnum,conftxt,token,qty,cxlqty,secsym,exchange,price,ask,bid,rrno,timeforce,c_date,m_date,version,CreateDatetime,Active,UpdateRef from mOrders;
	delete from mOrders;

	insert into lTrades (trid,accid,brid,secsym,route,status,qty,exchange,price,bkrsym,comm,e_date,version,CreateDatetime,Active,UpdateRef)
		select trid,accid,brid,secsym,route,status,qty,exchange,price,bkrsym,comm,e_date,version,CreateDatetime,Active,UpdateRef from mTrades;  
	delete from mTrades;

	insert into lTickets (brid,route,secsym,bkrsym,status,qty,comm,trid,exchange,price,version,CreateDatetime,Active,UpdateRef)
		select brid,route,secsym,bkrsym,status,qty,comm,trid,exchange,price,version,CreateDatetime,Active,UpdateRef from mTickets;
	delete from mTickets;

	insert into lPosition(accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef)
		select accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef from mPosition
		where DATE_PART('day',TO_TIMESTAMP(m_date / 1000)-localtimestamp)>0;
	
	delete from mPosition;
	insert into mPosition (accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef)
		select accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef from mNewPosition;
	
	update mPosition set InitQty=qty,InitPrice=AvgCost, m_date=extract('epoch' from CURRENT_TIMESTAMP)::bigint
	where DATE_PART('day',TO_TIMESTAMP(m_date / 1000)-localtimestamp)>0;

	delete from mPosition where qty=0;

	delete from mAccount;
	insert into mAccount(accid,OpenBP,OpenNBP,OpenEQ,OpenMGEQ,OpenEQRQ,CurrBP,CurrNBP,CurrHoldBP,currEQ,CurrMGEQ,CurrEQRQ,Comm,m_date,version,CreateDatetime,Active,UpdateRef)
		select accid,OpenBP,OpenNBP,OpenEQ,OpenMGEQ,OpenEQRQ,CurrBP,CurrNBP,CurrHoldBP,currEQ,CurrMGEQ,CurrEQRQ,Comm,m_date,version,CreateDatetime,Active,UpdateRef from mNewAccount;

	select 1 into tempInt;
	RETURN tempInt;
END; $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION do_backup_day_data()
  OWNER TO postgres;

  
  -- Function: do_restore_day_data()

-- DROP FUNCTION do_restore_day_data();

CREATE OR REPLACE FUNCTION do_restore_day_data()
  RETURNS timestamp without time zone AS
$BODY$
declare day timestamp;

begin
	
	delete from morders;
	insert into morders (Orderid,trid,accid,entrid,origoid,origtkn,brid,route,bkrsym,status,execnum,conftxt,token,qty,cxlqty,secsym,exchange,price,ask,bid,rrno,timeforce,c_date,m_date,version,CreateDatetime,Active,UpdateRef)
		select distinct Orderid,trid,accid,entrid,origoid,origtkn,brid,route,bkrsym,status,execnum,conftxt,token,qty,cxlqty,secsym,exchange,price,ask,bid,rrno,timeforce,c_date,m_date,version,CreateDatetime,Active,UpdateRef from lorders where
			 DATE_PART('day',TO_TIMESTAMP(c_date / 1000)-day)=0;

	
	delete from mtrades;
	insert into mtrades(trid,accid,brid,secsym,route,status,qty,exchange,price,bkrsym,comm,e_date,version,CreateDatetime,Active,UpdateRef)
		select distinct trid,accid,brid,secsym,route,status,qty,exchange,price,bkrsym,comm,e_date,version,CreateDatetime,Active,UpdateRef from ltrades where
			 DATE_PART('day',TO_TIMESTAMP(e_date / 1000)-day)=0;


	delete from mTickets;
	insert into mtickets(brid,route,secsym,bkrsym,status,qty,comm,trid,exchange,price,version,CreateDatetime,Active,UpdateRef)
		select distinct brid,route,secsym,bkrsym,status,qty,comm,trid,exchange,price,version,CreateDatetime,Active,UpdateRef from ltickets where
			 DATE_PART('day',TO_TIMESTAMP((select e_date from mTickets) / 1000)-day)=0;
	
	delete from mposition;
	insert into mposition (accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef)
		select distinct accid,secsym,ptype,brid,InitQty,InitPrice,qty,AvgCost,c_date,m_date,version,CreateDatetime,Active,UpdateRef from lposition where
			 DATE_PART('day',TO_TIMESTAMP(m_date / 1000)-day)=0;
	return day;
	END; $BODY$
  LANGUAGE plpgsql VOLATILE
  COST 100;
ALTER FUNCTION do_restore_day_data()
  OWNER TO postgres;
