if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRIACCIFO]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䤀䄀䌀䌀䤀䘀伀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRDACCIFO]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䐀䄀䌀䌀䤀䘀伀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRUACCIFO]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀唀䄀䌀䌀䤀䘀伀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[maccountifo]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)਍搀爀漀瀀 琀愀戀氀攀 嬀搀戀漀崀⸀嬀洀愀挀挀漀甀渀琀椀昀漀崀ഀഀ
GO਍ഀഀ
CREATE TABLE [dbo].[maccountifo] (਍ऀ嬀愀挀挀椀搀崀 嬀椀渀琀崀 一伀吀 一唀䰀䰀 Ⰰഀഀ
	[name] [varchar] (10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,਍ऀ嬀戀爀椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[status] [int] NULL ,਍ऀ嬀爀爀渀漀崀 嬀瘀愀爀挀栀愀爀崀 ⠀㠀⤀ 䌀伀䰀䰀䄀吀䔀 匀儀䰀开䰀愀琀椀渀㄀开䜀攀渀攀爀愀氀开䌀倀㄀开䌀䤀开䄀匀 一唀䰀䰀 Ⰰഀഀ
	[linkacc] [int] NULL ,਍ऀ嬀瀀攀爀开椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[add1_id] [int] NULL ,਍ऀ嬀愀搀搀㈀开椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[c_date] [datetime] NULL ,਍ऀ嬀洀开搀愀琀攀崀 嬀搀愀琀攀琀椀洀攀崀 一唀䰀䰀 Ⰰഀഀ
	[note] [varchar] (255) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ਍⤀ 伀一 嬀倀刀䤀䴀䄀刀夀崀ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀䤀䄀䌀䌀䤀䘀伀 伀一 洀愀挀挀漀甀渀琀椀昀漀ഀഀ
FOR INSERT਍䄀匀ഀഀ
begin਍ऀ搀攀挀氀愀爀攀 䀀猀愀挀挀椀搀 愀猀 椀渀琀ഀഀ
	declare @snameI as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀戀爀椀搀䤀 愀猀 椀渀琀ഀഀ
	declare @sstatusI as int	਍ऀ搀攀挀氀愀爀攀 䀀猀爀爀渀漀䤀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sperI as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㄀䤀 愀猀 椀渀琀ഀഀ
	declare @sadd2I as int਍ऀ搀攀挀氀愀爀攀 䀀猀渀漀琀攀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀昀氀愀最 愀猀 椀渀琀ഀഀ
	declare @changeifo as varchar(255)਍ഀഀ
	declare upcursor CURSOR for ਍ऀऀ猀攀氀攀挀琀 䤀⸀愀挀挀椀搀Ⰰ 䤀⸀渀愀洀攀Ⰰ  䤀⸀戀爀椀搀Ⰰ䤀⸀猀琀愀琀甀猀Ⰰഀഀ
			I.rrno, I.per_id, I.add1_id, I.add2_id,I.note਍ऀऀ昀爀漀洀 䤀一匀䔀刀吀䔀䐀 䤀ഀഀ
਍ऀ漀瀀攀渀 甀瀀挀甀爀猀漀爀ഀഀ
	FETCH NEXT FROM upcursor ਍ऀऀ䤀一吀伀 䀀猀愀挀挀椀搀Ⰰ䀀猀渀愀洀攀䤀Ⰰ䀀猀戀爀椀搀䤀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰഀഀ
			@srrnoI,@sperI,@sadd1I,@sadd2I,@snote਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeifo=""਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䤀匀一唀䰀䰀⠀䀀猀渀愀洀攀䤀Ⰰ✀✀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sbridI,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀猀琀愀琀甀猀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +ISNULL(@srrnoI,'')਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀瀀攀爀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㈀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))਍ഀഀ
		insert laccount (type,val,oldval,trid,accid,note,opid,logdate) values਍ऀऀऀ⠀㄀　㄀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ　Ⰰ　Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀挀栀愀渀最攀椀昀漀Ⰰ　Ⰰ最攀琀搀愀琀攀⠀⤀⤀ഀഀ
				਍ऀऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
			INTO @saccid,@snameI,@sbridI,@sstatusI,਍ऀऀऀऀ䀀猀爀爀渀漀䤀Ⰰ䀀猀瀀攀爀䤀Ⰰ䀀猀愀搀搀㄀䤀Ⰰ䀀猀愀搀搀㈀䤀Ⰰ䀀猀渀漀琀攀ഀഀ
	end਍ऀ䌀䰀伀匀䔀 甀瀀挀甀爀猀漀爀ഀഀ
	DEALLOCATE upcursor਍攀渀搀ഀഀ
਍䜀伀ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
CREATE TRIGGER TRDACCIFO ON maccountifo਍䘀伀刀 䐀䔀䰀䔀吀䔀ഀഀ
AS਍戀攀最椀渀ഀഀ
	declare @saccid as int਍ऀ搀攀挀氀愀爀攀 䀀猀渀愀洀攀䐀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sbridD as int਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䐀 愀猀 椀渀琀ऀഀഀ
	declare @srrnoD as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀瀀攀爀䐀 愀猀 椀渀琀ഀഀ
	declare @sadd1D as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㈀䐀 愀猀 椀渀琀ഀഀ
	declare @snote as varchar(255)਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select D.accid, D.name, D.brid,D.status,਍ऀऀऀ䐀⸀爀爀渀漀Ⰰ 䐀⸀瀀攀爀开椀搀Ⰰ 䐀⸀愀搀搀㄀开椀搀Ⰰ 䐀⸀愀搀搀㈀开椀搀Ⰰ䐀⸀渀漀琀攀ഀഀ
		from DELETED D਍ഀഀ
	open upcursor਍ऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @saccid,@snameD,@sbridD,@sstatusD,਍ऀऀऀ䀀猀爀爀渀漀䐀Ⰰ䀀猀瀀攀爀䐀Ⰰ䀀猀愀搀搀㄀䐀Ⰰ䀀猀愀搀搀㈀䐀Ⰰ䀀猀渀漀琀攀ഀഀ
਍ऀ眀栀椀氀攀 䀀䀀䘀䔀吀䌀䠀开匀吀䄀吀唀匀㴀　 ഀഀ
	begin਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀∀∀ഀഀ
		set @changeifo=ISNULL(@snameD,'')਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀戀爀椀搀䐀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusD,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀䤀匀一唀䰀䰀⠀䀀猀爀爀渀漀䐀Ⰰ✀✀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperD,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀 ⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㄀䐀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2D,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀渀漀琀攀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀　⤀⤀ഀഀ
਍ऀऀ椀渀猀攀爀琀 氀愀挀挀漀甀渀琀 ⠀琀礀瀀攀Ⰰ瘀愀氀Ⰰ漀氀搀瘀愀氀Ⰰ琀爀椀搀Ⰰ愀挀挀椀搀Ⰰ渀漀琀攀Ⰰ漀瀀椀搀Ⰰ氀漀最搀愀琀攀⤀ 瘀愀氀甀攀猀ഀഀ
			(102,@sstatusD,0,0,@saccid,@changeifo,0,getdate())਍ऀऀऀऀഀഀ
		FETCH NEXT FROM upcursor ਍ऀऀऀ䤀一吀伀 䀀猀愀挀挀椀搀Ⰰ䀀猀渀愀洀攀䐀Ⰰ䀀猀戀爀椀搀䐀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰഀഀ
				@srrnoD,@sperD,@sadd1D,@sadd2D,@snote਍ऀ攀渀搀ഀഀ
	CLOSE upcursor਍ऀ䐀䔀䄀䰀䰀伀䌀䄀吀䔀 甀瀀挀甀爀猀漀爀ഀഀ
end਍ഀഀ
GO਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀唀䄀䌀䌀䤀䘀伀 伀一 洀愀挀挀漀甀渀琀椀昀漀ഀഀ
FOR UPDATE਍䄀匀ഀഀ
begin਍ऀ搀攀挀氀愀爀攀 䀀猀愀挀挀椀搀 愀猀 椀渀琀ഀഀ
	declare @snameI as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀渀愀洀攀䐀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sbridI as int਍ऀ搀攀挀氀愀爀攀 䀀猀戀爀椀搀䐀 愀猀 椀渀琀ഀഀ
	declare @sstatusI as int	਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䐀 愀猀 椀渀琀ऀഀഀ
	declare @srrnoI as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀爀爀渀漀䐀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sperI as int਍ऀ搀攀挀氀愀爀攀 䀀猀瀀攀爀䐀 愀猀 椀渀琀ഀഀ
	declare @sadd1I as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㄀䐀 愀猀 椀渀琀ഀഀ
	declare @sadd2I as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㈀䐀 愀猀 椀渀琀ഀഀ
	declare @snote as varchar(255)਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select I.accid, I.name, D.name,  I.brid, D.brid, I.status, D.status,਍ऀऀऀ䤀⸀爀爀渀漀Ⰰ䐀⸀爀爀渀漀Ⰰ 䤀⸀瀀攀爀开椀搀Ⰰ䐀⸀瀀攀爀开椀搀Ⰰ 䤀⸀愀搀搀㄀开椀搀Ⰰ 䐀⸀愀搀搀㄀开椀搀Ⰰ䤀⸀愀搀搀㈀开椀搀Ⰰ䐀⸀愀搀搀㈀开椀搀Ⰰ 䤀⸀渀漀琀攀ഀഀ
		from INSERTED I, DELETED D਍ऀऀ眀栀攀爀攀 䤀⸀愀挀挀椀搀㴀䐀⸀愀挀挀椀搀ഀഀ
਍ऀ漀瀀攀渀 甀瀀挀甀爀猀漀爀ഀഀ
	FETCH NEXT FROM upcursor ਍ऀऀ䤀一吀伀 䀀猀愀挀挀椀搀Ⰰ䀀猀渀愀洀攀䤀Ⰰ䀀猀渀愀洀攀䐀Ⰰ䀀猀戀爀椀搀䤀Ⰰ䀀猀戀爀椀搀䐀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰഀഀ
			@srrnoI,@srrnoD,@sperI,@sperD,@sadd1I,@sadd1D,@sadd2I,@sadd2D,@snote਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeflag =0਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀∀∀ഀഀ
਍ऀऀ椀昀 ⠀䀀猀渀愀洀攀䤀㰀㸀䀀猀渀愀洀攀䐀⤀  猀攀琀 䀀挀栀愀渀最攀昀氀愀最 㴀 䀀挀栀愀渀最攀昀氀愀最 簀 ㈀ഀഀ
		set @changeifo=ISNULL(@snameI,'')਍ഀഀ
		if (@sbridI<>@sbridD)  set @changeflag=@changeflag | 4਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀戀爀椀搀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		਍ऀऀ椀昀 ⠀䀀猀猀琀愀琀甀猀䤀㰀㸀䀀猀猀琀愀琀甀猀䐀⤀ 猀攀琀  䀀挀栀愀渀最攀昀氀愀最 㴀䀀挀栀愀渀最攀昀氀愀最 簀 㠀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))਍ഀഀ
		if (@srrnoI<>@srrnoD) set  @changeflag =@changeflag |16਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀䤀匀一唀䰀䰀⠀䀀猀爀爀渀漀䤀Ⰰ✀✀⤀ഀഀ
	਍ऀऀ椀昀 ⠀䀀猀瀀攀爀䤀㰀㸀䀀猀瀀攀爀䐀⤀ 猀攀琀  䀀挀栀愀渀最攀昀氀愀最㴀䀀挀栀愀渀最攀昀氀愀最 簀㄀㈀㠀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperI,'') as varchar(10))਍ഀഀ
		if (@sadd1I<>@sadd1D) set  @changeflag=@changeflag |256਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀 ⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㄀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
਍ऀऀ椀昀 ⠀䀀猀愀搀搀㈀䤀㰀㸀䀀猀愀搀搀㈀䐀⤀ 猀攀琀  䀀挀栀愀渀最攀昀氀愀最㴀䀀挀栀愀渀最攀昀氀愀最 簀㔀㄀㈀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2I,'') as varchar(10))਍ऀऀഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))਍ഀഀ
		insert laccount (type,val,oldval,trid,accid,note,opid,logdate) values਍ऀऀऀ⠀㄀　㌀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰ䀀挀栀愀渀最攀昀氀愀最Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀挀栀愀渀最攀椀昀漀Ⰰ　Ⰰ最攀琀搀愀琀攀⠀⤀⤀ഀഀ
				਍ऀऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
			INTO @saccid,@snameI,@snameD,@sbridI,@sbridD,@sstatusI,@sstatusD,਍ऀऀऀऀ䀀猀爀爀渀漀䤀Ⰰ䀀猀爀爀渀漀䐀Ⰰ䀀猀瀀攀爀䤀Ⰰ䀀猀瀀攀爀䐀Ⰰ䀀猀愀搀搀㄀䤀Ⰰ䀀猀愀搀搀㄀䐀Ⰰ䀀猀愀搀搀㈀䤀Ⰰ䀀猀愀搀搀㈀䐀Ⰰ䀀猀渀漀琀攀ഀഀ
	਍ऀ攀渀搀ഀഀ
਍ऀ䌀䰀伀匀䔀 甀瀀挀甀爀猀漀爀ഀഀ
	DEALLOCATE upcursor਍ഀഀ
end਍ഀഀ
਍ഀഀ
਍䜀伀ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍�