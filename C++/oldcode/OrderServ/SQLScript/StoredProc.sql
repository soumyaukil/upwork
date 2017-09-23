/*
	do_backup_orders------- the init table of this day to start a new day.
*/
drop procedure do_backup_Day_data
go

create procedure do_backup_Day_data as
begin
	
	insert into lOrders 
		select * from mOrders
	delete mOrders

	insert into lTrades 
		select * from mTrades
	delete mTrades

	insert into lTickets 
		select * from mTickets
	delete mTickets

	insert into lPosition
		select * from mPosition
		where datediff(day,m_date,getdate())>0
	
	delete mPosition
	insert into mPosition 
		select * from mNewPosition
	
	update mPosition set InitQty=qty,InitPrice=AvgCost, m_date=getdate()
	where datediff(day,m_date,getdate())>0

	delete mPosition where qty=0

	delete mAccount
	insert into mAccount
		select * from mNewAccount
end
go

/*
	day init -----each day should run this prcedure
			to start a new day.
*/
drop procedure DayInit
GO

CREATE PROCEDURE DayInit AS
declare 
	@today  int, /*the value of the day*/
	@thisday  datetime, /*the get value of the day*/
	@newday  int
begin
	begin transaction
	select @thisday=getdate()

	select @newday=(datepart(year,@thisday)-2000)*0x200 +datepart(month,@thisday)*0x20 +datepart(day,@thisday)

	select @today = m_id from msys_id  TABLOCKX HOLDLOCK where mkey='TODAY' 
	if (@@rowcount=0)
	begin
		insert msys_id (mkey,m_id) values('TODAY',0)
		select @today=0
	end
	
	if (@today<@newday)
	begin
		update msys_id set m_id=@newday where mkey='TODAY'		
		exec do_backup_Day_data 
	end

	delete mMsg

	commit transaction
/*insert into lOrders select * from mOrders
insert into ltrades select * from mTrades
insert into lTickets select * from mTickets
*/
end

go

drop procedure do_restore_Day_data
GO

create procedure do_restore_Day_data @day datetime AS
begin
	begin transaction
	
	delete morders
	insert into morders 
		select distinct * from lorders where
			datediff(day,c_date,@day)=0
	
	delete mtrades
	insert into mtrades
		select distinct * from ltrades where
			datediff(day,e_date,@day)=0

	delete mTickets
	insert into mtickets
		select distinct * from ltickets where
			datediff(day,e_date,@day)=0
	
	delete mposition
	insert into mposition 
		select distinct * from lposition where
			datediff(day,m_date,@day)=0
	commit transaction
end

