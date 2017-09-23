
/********************************************************************
This script is for alert table trader for 128 bit of status.
Aprl 25, 2005
Jun Liu
*********************************************************************/
Alter table mtrader
add 
[status1] [int] NULL ,
[status2] [int] NULL ,
[status3] [int] NULL

go

Alter table laccount
add
[status1] [int] NULL ,
[status2] [int] NULL ,
[status3] [int] NULL ,
[oldstatus1] [int] NULL ,
[oldstatus2] [int] NULL ,
[oldstatus3] [int] NULL 

go



Alter TRIGGER TRITRADER ON mtrader
FOR INSERT
AS
begin
	declare @strid as int
	declare @stracnoI as varchar(10)
	declare @sbridI as int
	declare @spwdI as varchar(20)
	declare @sstatusI as int	
	declare @sperI as int
	declare @sadd1I as int
	declare @sadd2I as int
	declare @snote as varchar(255)
	declare @sstatus1 as int	
	declare @sstatus2 as int	
	declare @sstatus3 as int	
	

	declare @changeflag as int
	declare @changeifo as varchar(255)

	declare upcursor CURSOR for 
		select I.trid, I.tracno, I.PWD,
		          I.brid,I.status, I.per_id, I.add1_id, I.add2_id,I.note,
				  I.status1, I.status2, I.status3
		from INSERTED I

	open upcursor
	FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@spwdI,
			@sbridI,@sstatusI,
			@sperI,@sadd1I,@sadd2I,@snote,
			@sstatus1,@sstatus2,@sstatus3 ;


	while @@FETCH_STATUS=0 
	begin
		set @changeifo=''
		set @changeifo=ISNULL(@stracnoI,'')
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sbridI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2I,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))

		insert laccount (type,val,oldval,trid,accid,note,opid,logdate,status1,status2,status3) values
			(1,@sstatusI,0,@strid,0,@changeifo,0,getdate(),@sstatus1,@sstatus2,@sstatus3) 
				
		FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@spwdI,
			@sbridI,@sstatusI,
			@sperI,@sadd1I,@sadd2I,@snote,
			@sstatus1,@sstatus2,@sstatus3

	end
	CLOSE upcursor
	DEALLOCATE upcursor
end

go


Alter TRIGGER TRDTRADER ON mtrader
FOR DELETE
AS
begin
	declare @strid as int
	declare @stracnoI as varchar(10)
	declare @sbridI as int
	declare @spwdI as varchar(20)
	declare @sstatusI as int	
	declare @sperI as int
	declare @sadd1I as int
	declare @sadd2I as int
	declare @snote as varchar(255)
	declare @sstatus1 as int	
	declare @sstatus2 as int	
	declare @sstatus3 as int	

	declare @changeflag as int
	declare @changeifo as varchar(255)

	declare upcursor CURSOR for 
		select D.trid, D.tracno, D.PWD,
		          D.brid,D.status, D.per_id,D.add1_id, D.add2_id,D.note,
 				  D.status1, D.status2, D.status3

		from DELETED D

	open upcursor
	FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@spwdI,
			@sbridI,@sstatusI,
			@sperI,@sadd1I,@sadd2I,@snote,
			@sstatus1,@sstatus2,@sstatus3


	while @@FETCH_STATUS=0 
	begin
		set @changeifo=''
		set @changeifo=ISNULL(@stracnoI,'')
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sbridI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperI,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2I,'') as varchar(10))
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))

		insert laccount (type,val,oldval,trid,accid,note,opid,logdate,status1,status2,status3) values
			(2,@sstatusI,0,@strid,0,@changeifo,0,getdate(),@sstatus1,@sstatus2,@sstatus3)
	
		FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@spwdI,
			@sbridI,@sstatusI,
			@sperI,@sadd1I,@sadd2I,@snote,
			@sstatus1,@sstatus2,@sstatus3

	end
	CLOSE upcursor
	DEALLOCATE upcursor
end

go



Alter TRIGGER TRUTRADER ON mtrader
FOR UPDATE
AS
begin
	declare @strid as int
	declare @stracnoI as varchar(10)
	declare @stracnoD as varchar(10)
	declare @sbridI as int
	declare @sbridD as int
	declare @spwdI as varchar(20)
	declare @spwdD as varchar(20)
	declare @sstatusI as int	
	declare @sstatusD as int
	declare @sperI as int
	declare @sperD as int
	declare @sadd1I as int
	declare @sadd1D as int
	declare @sadd2I as int
	declare @sadd2D as int
	declare @snote as varchar(255)
	declare @sstatus1I as int	
	declare @sstatus1D as int
	declare @sstatus2I as int	
	declare @sstatus2D as int
	declare @sstatus3I as int	
	declare @sstatus3D as int

	declare @changeflag as int
	declare @changeifo as varchar(255)

	declare upcursor CURSOR for 
		select I.trid, I.tracno, D.tracno, I.PWD,D.PWD,
		          I.brid,D.brid,I.status,  D.status,  
		          I.per_id,D.per_id,I.add1_id,D.add1_id,I.add2_id,D.add2_id, I.note,
 				  I.status1, D.status1, 
				  I.status2, D.status2,
				  I.status3, D.status3

		from INSERTED I,DELETED D
		where I.trid=D.trid

	open upcursor
	FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@stracnoD,@spwdI,@spwdD,
			@sbridI,@sbridD,@sstatusI,@sstatusD,
			@sperI,@sperD,@sadd1I,@sadd1D,@sadd2I,@sadd2D,@snote,
			@sstatus1I,@sstatus1D,
			@sstatus2I,@sstatus2D,
			@sstatus3I,@sstatus3D

	while @@FETCH_STATUS=0 
	begin
		set @changeflag=0
		set @changeifo=''
		if (@stracnoI<>@stracnoD)	 set @changeflag=@changeflag |2
		set @changeifo=ISNULL(@stracnoI,'')

		if (@spwdI<>@spwdD)		 set @changeflag=@changeflag| 4

		if (@sbridI<>@sbridD)		 set @changeflag=@changeflag|8
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sbridI,'') as varchar(10))

		if (@sstatusI<>@sstatusD) 	 set @changeflag=@changeflag|16
		if (@sstatus1I<>@sstatus1D) 	 set @changeflag=@changeflag|16
		if (@sstatus2I<>@sstatus2D) 	 set @changeflag=@changeflag|16
		if (@sstatus3I<>@sstatus3D) 	 set @changeflag=@changeflag|16

		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))

		if (@sperI<>@sperD)		 set @changeflag=@changeflag|32
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperI,'') as varchar(10))
		
		if (@sadd1I<>@sadd1D)		set @changeflag=@changeflag|64
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))

		if (@sadd2I<>@sadd2D)		set @changeflag=@changeflag|128
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2I,'') as varchar(10))
		
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))
		
		if (@changeflag<>0)		
		begin
			insert into laccount (type,val,oldval,trid,accid,note,opid,logdate,
				status1,oldstatus1,status2,oldstatus2,status3,oldstatus3) values
				(3,@sstatusI,@sstatusD,@strid,@changeflag,@changeifo,0,getdate(),
				 @sstatus1I,@sstatus1D,@sstatus2I,@sstatus2D,@sstatus3I,@sstatus3D)
		end
		
		FETCH NEXT FROM upcursor 
		INTO @strid,@stracnoI,@stracnoD,@spwdI,@spwdD,
			@sbridI,@sbridD,@sstatusI,@sstatusD,
			@sperI,@sperD,@sadd1I,@sadd1D,@sadd2I,@sadd2D,@snote,
			@sstatus1I,@sstatus1D,
			@sstatus2I,@sstatus2D,
			@sstatus3I,@sstatus3D
	end
	CLOSE upcursor
	DEALLOCATE upcursor
end


go