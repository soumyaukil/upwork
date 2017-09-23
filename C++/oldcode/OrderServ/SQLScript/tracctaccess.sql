if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRIRESHP]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䤀刀䔀匀䠀倀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRDRESHIP]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀䐀刀䔀匀䠀䤀倀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[TRURESHIP]') and OBJECTPROPERTY(id, N'IsTrigger') = 1)਍搀爀漀瀀 琀爀椀最最攀爀 嬀搀戀漀崀⸀嬀吀刀唀刀䔀匀䠀䤀倀崀ഀഀ
GO਍ഀഀ
if exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[macct_access]') and OBJECTPROPERTY(id, N'IsUserTable') = 1)਍搀爀漀瀀 琀愀戀氀攀 嬀搀戀漀崀⸀嬀洀愀挀挀琀开愀挀挀攀猀猀崀ഀഀ
GO਍ഀഀ
CREATE TABLE [dbo].[macct_access] (਍ऀ嬀琀爀椀搀崀 嬀椀渀琀崀 一伀吀 一唀䰀䰀 Ⰰഀഀ
	[accid] [int] NOT NULL ,਍ऀ嬀猀琀愀琀甀猀崀 嬀椀渀琀崀 一唀䰀䰀 Ⰰഀഀ
	[dealid] [int] NULL ,਍ऀ嬀洀开搀愀琀攀崀 嬀搀愀琀攀琀椀洀攀崀 一唀䰀䰀 ഀഀ
) ON [PRIMARY]਍䜀伀ഀഀ
਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
CREATE TRIGGER TRIRESHP ON macct_access਍䘀伀刀 䤀一匀䔀刀吀ഀഀ
AS਍䈀䔀䜀䤀一ഀഀ
	declare @strid as int਍ऀ搀攀挀氀愀爀攀 䀀猀愀挀挀椀搀 愀猀 椀渀琀ഀഀ
	declare @sstatusI as int	਍ऀ搀攀挀氀愀爀攀 䀀猀搀攀愀氀椀搀䤀 愀猀 椀渀琀 ഀഀ
਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀昀氀愀最 愀猀 椀渀琀ഀഀ
	declare @changeifo as varchar(255)਍ഀഀ
	declare upcursor CURSOR for ਍ऀऀ猀攀氀攀挀琀 䤀⸀琀爀椀搀Ⰰ䤀⸀愀挀挀椀搀Ⰰ䤀⸀猀琀愀琀甀猀Ⰰ䤀⸀搀攀愀氀椀搀ഀഀ
		from INSERTED I਍ഀഀ
	open upcursor਍ऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@saccid,@sstatusI,@sdealidI਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeifo= cast(ISNULL(@sdealidI,'') as varchar(10))਍ഀഀ
		insert laccount (type,val,oldval,trid,accid,note,opid,logdate) values਍ऀऀऀ⠀㈀　㄀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ　Ⰰ䀀猀琀爀椀搀Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀挀栀愀渀最攀椀昀漀Ⰰ　Ⰰ最攀琀搀愀琀攀⠀⤀⤀ഀഀ
				਍ऀऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
			INTO @strid,@saccid,@sstatusI,@sdealidI਍ഀഀ
	end਍ऀ䌀䰀伀匀䔀 甀瀀挀甀爀猀漀爀ഀഀ
	DEALLOCATE upcursor	਍ഀഀ
END਍ഀഀ
GO਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀䐀刀䔀匀䠀䤀倀 伀一 洀愀挀挀琀开愀挀挀攀猀猀ഀഀ
FOR  DELETE ਍䄀匀ഀഀ
BEGIN਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀椀搀 愀猀 椀渀琀ഀഀ
	declare @saccid as int਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䐀 愀猀 椀渀琀ऀഀഀ
	declare @sdealidD as int ਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select D.trid,D.accid,D.status,D.dealid਍ऀऀ昀爀漀洀 䐀䔀䰀吀䔀䐀 䐀ഀഀ
਍ऀ漀瀀攀渀 甀瀀挀甀爀猀漀爀ഀഀ
	FETCH NEXT FROM upcursor ਍ऀऀ䤀一吀伀 䀀猀琀爀椀搀Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰ䀀猀搀攀愀氀椀搀䐀ഀഀ
਍ऀ眀栀椀氀攀 䀀䀀䘀䔀吀䌀䠀开匀吀䄀吀唀匀㴀　 ഀഀ
	begin਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀 挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀搀攀愀氀椀搀䐀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
਍ऀऀ椀渀猀攀爀琀 氀愀挀挀漀甀渀琀 ⠀琀礀瀀攀Ⰰ瘀愀氀Ⰰ漀氀搀瘀愀氀Ⰰ琀爀椀搀Ⰰ愀挀挀椀搀Ⰰ渀漀琀攀Ⰰ漀瀀椀搀Ⰰ氀漀最搀愀琀攀⤀ 瘀愀氀甀攀猀ഀഀ
			(202,@sstatusD,0,@strid,@saccid,@changeifo,0,getdate())਍ऀऀऀऀഀഀ
		FETCH NEXT FROM upcursor ਍ऀऀऀ䤀一吀伀 䀀猀琀爀椀搀Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰ䀀猀搀攀愀氀椀搀䐀ഀഀ
਍ऀ攀渀搀ഀഀ
	CLOSE upcursor਍ऀ䐀䔀䄀䰀䰀伀䌀䄀吀䔀 甀瀀挀甀爀猀漀爀ऀഀഀ
਍䔀一䐀ഀഀ
਍ഀഀ
GO਍匀䔀吀 儀唀伀吀䔀䐀开䤀䐀䔀一吀䤀䘀䤀䔀刀 伀䘀䘀 ഀഀ
GO਍匀䔀吀 䄀一匀䤀开一唀䰀䰀匀 伀一 ഀഀ
GO਍ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍䌀刀䔀䄀吀䔀 吀刀䤀䜀䜀䔀刀 吀刀唀刀䔀匀䠀䤀倀 伀一 洀愀挀挀琀开愀挀挀攀猀猀ഀഀ
FOR UPDATE਍䄀匀ഀഀ
BEGIN਍ऀ搀攀挀氀愀爀攀 䀀猀琀爀椀搀 愀猀 椀渀琀ഀഀ
	declare @saccid as int਍ऀ搀攀挀氀愀爀攀 䀀猀猀琀愀琀甀猀䤀 愀猀 椀渀琀ऀഀഀ
	declare @sstatusD as int	਍ऀ搀攀挀氀愀爀攀 䀀猀搀攀愀氀椀搀䤀 愀猀 椀渀琀 ഀഀ
	declare @sdealidD as int ਍ഀഀ
	declare @changeflag as int਍ऀ搀攀挀氀愀爀攀 䀀挀栀愀渀最攀椀昀漀 愀猀 瘀愀爀挀栀愀爀⠀㈀㔀㔀⤀ഀഀ
਍ऀ搀攀挀氀愀爀攀 甀瀀挀甀爀猀漀爀 䌀唀刀匀伀刀 昀漀爀 ഀഀ
		select I.trid,I.accid,I.status,D.status,I.dealid,D.dealid਍ऀऀ昀爀漀洀 䤀一匀䔀刀吀䔀䐀 䤀Ⰰ䐀䔀䰀䔀吀䔀䐀 䐀ഀഀ
		where I.trid=D.trid and I.accid=D.accid਍ഀഀ
	open upcursor਍ऀ䘀䔀吀䌀䠀 一䔀堀吀 䘀刀伀䴀 甀瀀挀甀爀猀漀爀 ഀഀ
		INTO @strid,@saccid,@sstatusI,@sstatusD,@sdealidI,@sdealidD਍ഀഀ
	while @@FETCH_STATUS=0 ਍ऀ戀攀最椀渀ഀഀ
		set @changeifo= cast(ISNULL(@sdealidI,'') as varchar(10))਍ऀऀ猀攀琀 䀀挀栀愀渀最攀椀昀漀㴀䀀挀栀愀渀最攀椀昀漀 ⬀✀簀✀ ⬀挀愀猀琀⠀䤀匀一唀䰀䰀⠀䀀猀搀攀愀氀椀搀䐀Ⰰ✀✀⤀ 愀猀 瘀愀爀挀栀愀爀⠀㄀　⤀⤀ഀഀ
਍ऀऀ椀渀猀攀爀琀 氀愀挀挀漀甀渀琀 ⠀琀礀瀀攀Ⰰ瘀愀氀Ⰰ漀氀搀瘀愀氀Ⰰ琀爀椀搀Ⰰ愀挀挀椀搀Ⰰ渀漀琀攀Ⰰ漀瀀椀搀Ⰰ氀漀最搀愀琀攀⤀ 瘀愀氀甀攀猀ഀഀ
			(203,@sstatusI,@sstatusD,@strid,@saccid,@changeifo,0,getdate())਍ऀऀऀऀഀഀ
		FETCH NEXT FROM upcursor ਍ऀऀऀ䤀一吀伀 䀀猀琀爀椀搀Ⰰ䀀猀愀挀挀椀搀Ⰰ䀀猀猀琀愀琀甀猀䤀Ⰰ䀀猀猀琀愀琀甀猀䐀Ⰰ䀀猀搀攀愀氀椀搀䤀Ⰰ䀀猀搀攀愀氀椀搀䐀ഀഀ
	end਍ऀ䌀䰀伀匀䔀 甀瀀挀甀爀猀漀爀ഀഀ
	DEALLOCATE upcursor	਍ഀഀ
END਍䜀伀ഀഀ
SET QUOTED_IDENTIFIER OFF ਍䜀伀ഀഀ
SET ANSI_NULLS ON ਍䜀伀ഀഀ
਍�