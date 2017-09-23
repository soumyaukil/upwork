if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRITRADER]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䤀吀刀䄀䐀䔀刀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRDTRADER]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䐀吀刀䄀䐀䔀刀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRUTRADER]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀唀吀刀䄀䐀䔀刀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[mtrader]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)਍搀爀漀瀀 琀愀戀氀攀 嬀搀戀漀崀⸀嬀洀琀爀愀搀攀爀崀ഀഀ
GO਍ഀഀ
CREATE TABLE [dbo].[mtrader] (਍ऀ嬀琀爀椀搀崀 嬀椀渀琀崀 一伀吀 一唀䰀䰀 Ⰰഀഀ
	[tracno] [varchar] (10) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,਍ऀ嬀渀愀洀攀崀 嬀瘀愀爀挀栀愀爀崀 ⠀㈀　⤀ 䌀伀䰀䰀䄀吀䔀 匀儀䰀开䰀愀琀椀渀㄀开䜀攀渀攀爀愀氀开䌀倀㄀开䌀䤀开䄀匀 一唀䰀䰀 Ⰰഀഀ
	[PWD] [varchar] (20) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ,਍ऀ嬀戀爀椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[status] [int] NULL ,਍ऀ嬀瀀攀爀开椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[add1_id] [int] NULL ,਍ऀ嬀愀搀搀㈀开椀搀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[c_date] [datetime] NULL ,਍ऀ嬀洀开搀愀琀攀崀 嬀搀愀琀攀琀椀洀攀崀 一唀䰀䰀 Ⰰഀഀ
	[note] [varchar] (255) COLLATE SQL_Latin1_General_CP1_CI_AS NULL ਍⤀ 伀一 嬀倀刀䤀䴀䄀刀夀崀ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀䤀吀刀䄀䐀䔀刀 伀一 洀琀爀愀搀攀爀ഀഀ
FOR INSERT਍䄀匀ഀഀ
begin਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀椀搀 愀猀 椀渀琀ഀഀ
	declare @stracnoI as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀戀爀椀搀䤀 愀猀 椀渀琀ഀഀ
	declare @spwdI as varchar(20)਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䤀 愀猀 椀渀琀ऀഀഀ
	declare @sperI as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㄀䤀 愀猀 椀渀琀ഀഀ
	declare @sadd2I as int਍ऀ搀攀挀氀愀爀攀 䀀猀渀漀琀攀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀昀氀愀最 愀猀 椀渀琀ഀഀ
	declare @changeifo as varchar(255)਍ഀഀ
	declare upcursor CURSOR for ਍ऀऀ猀攀氀攀挀琀 䤀⸀琀爀椀搀Ⰰ 䤀⸀琀爀愀挀渀漀Ⰰ 䤀⸀倀圀䐀Ⰰഀഀ
		          I.brid,I.status, I.per_id, I.add1_id, I.add2_id,I.note਍ऀऀ昀爀漀洀 䤀一匀䔀刀吀䔀䐀 䤀ഀഀ
਍ऀ漀瀀攀渀 甀瀀挀甀爀猀漀爀ഀഀ
	FETCH NEXT FROM upcursor ਍ऀऀ䤀一吀伀 䀀猀琀爀椀搀Ⰰ䀀猀琀爀愀挀渀漀䤀Ⰰ䀀猀瀀眀搀䤀Ⰰഀഀ
			@sbridI,@sstatusI,਍ऀऀऀ䀀猀瀀攀爀䤀Ⰰ䀀猀愀搀搀㄀䤀Ⰰ䀀猀愀搀搀㈀䤀Ⰰ䀀猀渀漀琀攀ഀഀ
਍ऀ眀栀椀氀攀 䀀䀀䘀䔀吀䌀䠀开匀吀䄀吀唀匀㴀　 ഀഀ
	begin਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀∀∀ഀഀ
		set @changeifo=ISNULL(@stracnoI,'')਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀⬀䤀匀一唀䰀䰀⠀䀀猀瀀眀搀䤀Ⰰ✀✀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sbridI,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀猀琀愀琀甀猀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sperI,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀 ⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㄀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sadd2I,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀渀漀琀攀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀　⤀⤀ഀഀ
਍ऀऀ椀渀猀攀爀琀 氀愀挀挀漀甀渀琀 ⠀琀礀瀀攀Ⰰ瘀愀氀Ⰰ漀氀搀瘀愀氀Ⰰ琀爀椀搀Ⰰ愀挀挀椀搀Ⰰ渀漀琀攀Ⰰ漀瀀椀搀Ⰰ氀漀最搀愀琀攀⤀ 瘀愀氀甀攀猀ഀഀ
			(1,@sstatusI,0,@strid,0,@changeifo,0,getdate())਍ऀऀऀऀഀഀ
		FETCH NEXT FROM upcursor ਍ऀऀ䤀一吀伀 䀀猀琀爀椀搀Ⰰ䀀猀琀爀愀挀渀漀䤀Ⰰ䀀猀瀀眀搀䤀Ⰰഀഀ
			@sbridI,@sstatusI,਍ऀऀऀ䀀猀瀀攀爀䤀Ⰰ䀀猀愀搀搀㄀䤀Ⰰ䀀猀愀搀搀㈀䤀Ⰰ䀀猀渀漀琀攀ഀഀ
	end਍ऀ䌀䰀伀匀䔀 甀瀀挀甀爀猀漀爀ഀഀ
	DEALLOCATE upcursor਍攀渀搀ഀഀ
਍䜀伀ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
CREATE TRIGGER TRDTRADER ON mtrader਍䘀伀刀 䐀䔀䰀䔀吀䔀ഀഀ
AS਍戀攀最椀渀ഀഀ
	declare @strid as int਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀愀挀渀漀䤀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sbridI as int਍ऀ搀攀挀氀愀爀攀 䀀猀瀀眀搀䤀 愀猀 瘀愀爀挀栀愀爀⠀㈀　⤀ഀഀ
	declare @sstatusI as int	਍ऀ搀攀挀氀愀爀攀 䀀猀瀀攀爀䤀 愀猀 椀渀琀ഀഀ
	declare @sadd1I as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㈀䤀 愀猀 椀渀琀ഀഀ
	declare @snote as varchar(255)਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select D.trid, D.tracno, D.PWD,਍ऀऀ          䐀⸀戀爀椀搀Ⰰ䐀⸀猀琀愀琀甀猀Ⰰ 䐀⸀瀀攀爀开椀搀Ⰰ䐀⸀愀搀搀㄀开椀搀Ⰰ 䐀⸀愀搀搀㈀开椀搀Ⰰ䐀⸀渀漀琀攀ഀഀ
		from DELETED D਍ഀഀ
	open upcursor਍ऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@stracnoI,@spwdI,਍ऀऀऀ䀀猀戀爀椀搀䤀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰഀഀ
			@sperI,@sadd1I,@sadd2I,@snote਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeifo=""਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䤀匀一唀䰀䰀⠀䀀猀琀爀愀挀渀漀䤀Ⰰ✀✀⤀ഀഀ
		set @changeifo=@changeifo +'|'+ISNULL(@spwdI,'')਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀戀爀椀搀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀瀀攀爀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㈀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@snote,'') as varchar(250))਍ഀഀ
		insert laccount (type,val,oldval,trid,accid,note,opid,logdate) values਍ऀऀऀ⠀㈀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ　Ⰰ䀀猀琀爀椀搀Ⰰ　Ⰰ䀀挀栀愀渀最攀椀昀漀Ⰰ　Ⰰ最攀琀搀愀琀攀⠀⤀⤀ഀഀ
	਍ऀऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@stracnoI,@spwdI,਍ऀऀऀ䀀猀戀爀椀搀䤀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰഀഀ
			@sperI,@sadd1I,@sadd2I,@snote਍ऀ攀渀搀ഀഀ
	CLOSE upcursor਍ऀ䐀䔀䄀䰀䰀伀䌀䄀吀䔀 甀瀀挀甀爀猀漀爀ഀഀ
end਍ഀഀ
਍ഀഀ
਍ഀഀ
਍ഀഀ
਍ഀഀ
GO਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀唀吀刀䄀䐀䔀刀 伀一 洀琀爀愀搀攀爀ഀഀ
FOR UPDATE਍䄀匀ഀഀ
begin਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀椀搀 愀猀 椀渀琀ഀഀ
	declare @stracnoI as varchar(10)਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀愀挀渀漀䐀 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀ഀഀ
	declare @sbridI as int਍ऀ搀攀挀氀愀爀攀 䀀猀戀爀椀搀䐀 愀猀 椀渀琀ഀഀ
	declare @spwdI as varchar(20)਍ऀ搀攀挀氀愀爀攀 䀀猀瀀眀搀䐀 愀猀 瘀愀爀挀栀愀爀⠀㈀　⤀ഀഀ
	declare @sstatusI as int	਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䐀 愀猀 椀渀琀ഀഀ
	declare @sperI as int਍ऀ搀攀挀氀愀爀攀 䀀猀瀀攀爀䐀 愀猀 椀渀琀ഀഀ
	declare @sadd1I as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㄀䐀 愀猀 椀渀琀ഀഀ
	declare @sadd2I as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀搀搀㈀䐀 愀猀 椀渀琀ഀഀ
	declare @snote as varchar(255)਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select I.trid, I.tracno, D.tracno, I.PWD,D.PWD,਍ऀऀ          䤀⸀戀爀椀搀Ⰰ䐀⸀戀爀椀搀Ⰰ䤀⸀猀琀愀琀甀猀Ⰰ  䐀⸀猀琀愀琀甀猀Ⰰ  ഀഀ
		          I.per_id,D.per_id,I.add1_id,D.add1_id,I.add2_id,D.add2_id, I.note਍ऀऀ昀爀漀洀 䤀一匀䔀刀吀䔀䐀 䤀Ⰰ䐀䔀䰀䔀吀䔀䐀 䐀ഀഀ
		where I.trid=D.trid਍ഀഀ
	open upcursor਍ऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@stracnoI,@stracnoD,@spwdI,@spwdD,਍ऀऀऀ䀀猀戀爀椀搀䤀Ⰰ䀀猀戀爀椀搀䐀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰഀഀ
			@sperI,@sperD,@sadd1I,@sadd1D,@sadd2I,@sadd2D,@snote਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeflag=0਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀∀∀ഀഀ
		if (@stracnoI<>@stracnoD)	 set @changeflag=@changeflag |2਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䤀匀一唀䰀䰀⠀䀀猀琀爀愀挀渀漀䤀Ⰰ✀✀⤀ഀഀ
਍ऀऀ椀昀 ⠀䀀猀瀀眀搀䤀㰀㸀䀀猀瀀眀搀䐀⤀ऀऀ 猀攀琀 䀀挀栀愀渀最攀昀氀愀最㴀䀀挀栀愀渀最攀昀氀愀最簀 㐀ഀഀ
		set @changeifo=@changeifo +'|'+ISNULL(@spwdI,'')਍ഀഀ
		if (@sbridI<>@sbridD)		 set @changeflag=@changeflag|8਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀戀爀椀搀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
਍ऀऀ椀昀 ⠀䀀猀猀琀愀琀甀猀䤀㰀㸀䀀猀猀琀愀琀甀猀䐀⤀ ऀ 猀攀琀 䀀挀栀愀渀最攀昀氀愀最㴀䀀挀栀愀渀最攀昀氀愀最簀㄀㘀ഀഀ
		set @changeifo=@changeifo +'|' +cast(ISNULL(@sstatusI,'') as varchar(10))਍ഀഀ
		if (@sperI<>@sperD)		 set @changeflag=@changeflag|32਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀瀀攀爀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		਍ऀऀ椀昀 ⠀䀀猀愀搀搀㄀䤀㰀㸀䀀猀愀搀搀㄀䐀⤀ऀऀ猀攀琀 䀀挀栀愀渀最攀昀氀愀最㴀䀀挀栀愀渀最攀昀氀愀最簀㘀㐀ഀഀ
		set @changeifo=@changeifo +'|' +cast (ISNULL(@sadd1I,'') as varchar(10))਍ഀഀ
		if (@sadd2I<>@sadd2D)		set @changeflag=@changeflag|128਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀愀搀搀㈀䤀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
		਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀渀漀琀攀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀　⤀⤀ഀഀ
		਍ऀऀ椀昀 ⠀䀀挀栀愀渀最攀昀氀愀最㰀㸀　⤀ऀऀഀഀ
		begin਍ऀऀऀ椀渀猀攀爀琀 氀愀挀挀漀甀渀琀 ⠀琀礀瀀攀Ⰰ瘀愀氀Ⰰ漀氀搀瘀愀氀Ⰰ琀爀椀搀Ⰰ愀挀挀椀搀Ⰰ渀漀琀攀Ⰰ漀瀀椀搀Ⰰ氀漀最搀愀琀攀⤀ 瘀愀氀甀攀猀ഀഀ
				(3,@sstatusI,@sstatusD,@strid,@changeflag,@changeifo,0,getdate())਍ऀऀ攀渀搀ഀഀ
		਍ऀऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@stracnoI,@stracnoD,@spwdI,@spwdD,਍ऀऀऀ䀀猀戀爀椀搀䤀Ⰰ䀀猀戀爀椀搀䐀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰഀഀ
			@sperI,@sperD,@sadd1I,@sadd1D,@sadd2I,@sadd2D,@snote਍ऀ攀渀搀ഀഀ
	CLOSE upcursor਍ऀ䐀䔀䄀䰀䰀伀䌀䄀吀䔀 甀瀀挀甀爀猀漀爀ഀഀ
end਍ഀഀ
਍ഀഀ
਍ഀഀ
਍ഀഀ
਍ഀഀ
਍ഀഀ
਍䜀伀ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍�