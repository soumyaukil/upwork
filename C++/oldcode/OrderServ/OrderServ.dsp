# Microsoft Developer Studio Project File - Name="OrderServ" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=OrderServ - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "OrderServ.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "OrderServ.mak" CFG="OrderServ - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "OrderServ - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "OrderServ - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "OrderServ - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\exe"
# PROP Intermediate_Dir "\Release\OM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /I "D:\Projects\dassrc\comm" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D _WIN32_WINNT=0x0400 /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ws2_32.lib mswsock.lib ../../libs/dbghelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\exe\OM.exe"

!ELSEIF  "$(CFG)" == "OrderServ - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\exe"
# PROP Intermediate_Dir "\Debug\OrderServer"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /I "D:\Projects\dassrc\comm" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_WIN32_WINNT#0x0400" /FR /Yu"stdafx.h" /FD /D /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ws2_32.lib mswsock.lib ../../libs/dbghelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\..\exe\OM.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "OrderServ - Win32 Release"
# Name "OrderServ - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Account.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\Client.cpp
# End Source File
# Begin Source File

SOURCE=.\ClientPortDaemon.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgPopMsg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\FixLenMem.cpp
# End Source File
# Begin Source File

SOURCE=.\Mgmt\frmmgmt.cpp
# ADD CPP /I ".."
# End Source File
# Begin Source File

SOURCE=.\frmOrderRoute.cpp
# End Source File
# Begin Source File

SOURCE=.\frmScript.cpp
# End Source File
# Begin Source File

SOURCE=.\InfoPort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\intmap.cpp
# End Source File
# Begin Source File

SOURCE=.\IssueIfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Mgmt\ItemPool.cpp
# End Source File
# Begin Source File

SOURCE=.\Mgmt\itemsDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Mgmt\itemsSRst.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\MDVersion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\MSocket\MSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderDB.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderRouteCfg.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderRouter.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderServ.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderServ.rc
# End Source File
# Begin Source File

SOURCE=.\OrderServDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\OrderServView.cpp
# End Source File
# Begin Source File

SOURCE=.\OrdScript.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TradeEngine.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Account.h
# End Source File
# Begin Source File

SOURCE=.\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=.\Client.h
# End Source File
# Begin Source File

SOURCE=.\ClientPortDaemon.h
# End Source File
# Begin Source File

SOURCE=.\dlgPopMsg.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\FixLenMem.h
# End Source File
# Begin Source File

SOURCE=.\Mgmt\frmmgmt.h
# End Source File
# Begin Source File

SOURCE=.\frmOrderRoute.h
# End Source File
# Begin Source File

SOURCE=.\frmScript.h
# End Source File
# Begin Source File

SOURCE=.\InfoPort.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\intmap.h
# End Source File
# Begin Source File

SOURCE=.\IssueIfo.h
# End Source File
# Begin Source File

SOURCE=.\Mgmt\ItemPool.h
# End Source File
# Begin Source File

SOURCE=.\Mgmt\itemsDef.h
# End Source File
# Begin Source File

SOURCE=.\Mgmt\itemsSRst.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\MDVersion.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\MSocket\Msocket.h
# End Source File
# Begin Source File

SOURCE=.\OrderConfig.h
# End Source File
# Begin Source File

SOURCE=.\OrderDB.h
# End Source File
# Begin Source File

SOURCE=.\OrderRouteCfg.h
# End Source File
# Begin Source File

SOURCE=.\OrderRouter.h
# End Source File
# Begin Source File

SOURCE=.\OrderServ.h
# End Source File
# Begin Source File

SOURCE=.\OrderServDoc.h
# End Source File
# Begin Source File

SOURCE=.\OrderServView.h
# End Source File
# Begin Source File

SOURCE=.\OrdScript.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\PkgBuff.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\PkgBuff.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\defines\PkgIDDef.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TradeEngine.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\OrderServ.ico
# End Source File
# Begin Source File

SOURCE=.\res\OrderServ.rc2
# End Source File
# Begin Source File

SOURCE=.\res\OrderServDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "GridCtrl"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Comm\gridctrl\CellRange.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCell.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCell.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCellBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCellBase.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCtrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridCtrl.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridDropTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\GridDropTarget.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\InPlaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\InPlaceEdit.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\MemDC.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\TitleTip.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\gridctrl\TitleTip.h
# End Source File
# End Group
# Begin Group "MISC"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Comm\Functions\ControlPos.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\ControlPos.h
# End Source File
# End Group
# Begin Group "Crash"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\Comm\Crash\ExceptionHandler.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Crash\GetWinVer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Crash\MiniVersion.cpp
# End Source File
# End Group
# Begin Group "Structures"

# PROP Default_Filter ""
# End Group
# Begin Source File

SOURCE=..\..\Comm\Functions\CCmdScript.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\CCmdScript.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\Config.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\Config.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\CPUIfo.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\CPUIfo.h
# End Source File
# Begin Source File

SOURCE=.\dlgBalance.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgBalance.h
# End Source File
# Begin Source File

SOURCE=.\dlgIPBox.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgIPBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\encrypt.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\encrypt.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\InputBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\InputBox.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Log\Log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Log\Log.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Log\logframe.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Log\logframe.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Security\md5.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Security\md5.h
# End Source File
# Begin Source File

SOURCE=..\Define\OrderDef.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\Perf.cpp
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\Perf.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\SRecSet\SRecSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\SRecSet\SRecSet.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\strfuncs.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\strfuncs.h
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\synchronal.cpp
# End Source File
# Begin Source File

SOURCE=..\..\Comm\Functions\synchronal.h
# End Source File
# Begin Source File

SOURCE=..\Define\UDPPkgs.h
# End Source File
# End Group
# Begin Group "sql"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AertTab1.sql
# End Source File
# Begin Source File

SOURCE=.\SQLScript\Init.sql
# End Source File
# Begin Source File

SOURCE=.\SQLScript\InitDataBase.sql
# End Source File
# Begin Source File

SOURCE=.\SQLScript\StoredProc.sql
# End Source File
# End Group
# Begin Source File

SOURCE=.\OrderServ.cfg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
