; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildFrame
LastTemplate=CMDIChildWnd
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "orderserv.h"
LastPage=0

ClassCount=26
Class1=CChildFrame
Class2=CMainFrame
Class3=rstSysID
Class4=COrderServApp
Class5=CAboutDlg
Class6=COrderServDoc
Class7=COrderServView
Class8=rsttrader

ResourceCount=22
Resource1=IDD_DIALOG1
Class9=rstAccInfo
Class10=rstaccount
Class11=rstTrader
Class12=rstaddress
Class13=rstperson
Resource2=IDD_DLGMSG
Class14=viewSecIfo
Class15=frmSecIfo
Resource3=IDD_EQUITY
Class16=frmEquity
Class17=viewEquity
Resource4=IDD_SECIFO
Class18=frmTraders
Class19=viewTraders
Resource5=IDD_ABOUTBOX
Class20=dlgIPBox
Resource6=IDR_MenuPopUp
Class21=dlgBalance
Resource7=IDD_DIGIPINPUT
Class22=OrderRouteCfg
Resource8=IDD_TRADERS
Class23=OrderRouteCfg1
Resource9=IDR_MAINFRAME (English (U.S.))
Class24=dlgPopMsg
Class25=frmOrderRoute
Class26=frmScript
Resource10=IDD_DLGBALANCE
Resource11=IDD_ROUTECFG
Resource12=IDD_DLGMSG (English (U.S.))
Resource13=IDD_TRADERS (English (U.S.))
Resource14=IDD_DIALOG1 (English (U.S.))
Resource15=IDD_DLGBALANCE (English (U.S.))
Resource16=IDD_EQUITY (English (U.S.))
Resource17=IDD_DIGIPINPUT (English (U.S.))
Resource18=IDD_SECIFO (English (U.S.))
Resource19=IDD_ROUTECFG (English (U.S.))
Resource20=IDD_ABOUTBOX (English (U.S.))
Resource21=IDR_MenuPopUp (English (U.S.))
Resource22=IDR_MAINFRAME

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
LastObject=CChildFrame

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=CMainFrame

[CLS:rstSysID]
Type=0
HeaderFile=OrderDB.h
ImplementationFile=OrderDB.cpp

[CLS:COrderServApp]
Type=0
BaseClass=CWinApp
HeaderFile=OrderServ.h
ImplementationFile=OrderServ.cpp
Filter=N
VirtualFilter=AC
LastObject=COrderServApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=OrderServ.cpp
ImplementationFile=OrderServ.cpp
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[CLS:COrderServDoc]
Type=0
BaseClass=CDocument
HeaderFile=OrderServDoc.h
ImplementationFile=OrderServDoc.cpp
LastObject=COrderServDoc

[CLS:COrderServView]
Type=0
BaseClass=CView
HeaderFile=OrderServView.h
ImplementationFile=OrderServView.cpp
LastObject=COrderServView

[DB:rstSysID]
DB=1

[DB:RstTrader]
DB=1

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STVER,static,1342308352
Control6=IDC_STIFO,static,1342308608
Control7=IDC_BCrash,button,1342242816
Control8=IDC_BPref,button,1342242816

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_VIEW_CFG
Command2=ID_VIEW_LOGINLOG
Command3=ID_VIEW_LOG
Command4=ID_VIEW_TRADERS
Command5=ID_VIEW_THREADS
Command6=ID_APP_EXIT
Command7=ID_AAA_ACCOUNT
Command8=ID_AAA_TMP1
Command9=ID_AAA_POSITION
Command10=ID_AAA_ONLINETRADER
Command11=ID_AAA_DEAL
Command12=ID_AAA_TRADES
Command13=ID_AAA_ORDERS
Command14=ID_AAA_TICKET
Command15=ID_AAA_TEST
Command16=ID_DATA_LOADEQUITY
Command17=ID_DATA_LSHTLST
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_DATA_EXPORTPOSITION
Command21=ID_DATA_SOD_QUEST
Command22=ID_DATA_SOD_BPEQUITY
Command23=ID_DATA_SOD_SHORTLIST
Command24=ID_DATA_SOD_UNSHORTLIST
Command25=ID_DATA_SAVEEQUITY
Command26=ID_DATA_COUNTING
Command27=ID_DATA_SAVEDATA
Command28=ID_DATA_RECOVER_SAVETRADES
Command29=ID_TOOLS_SECIFO
Command30=ID_TOOLS_EQUITY
Command31=ID_TOOLS_BALANCE
Command32=ID_TOOLS_LOADLOGINCTRL
Command33=ID_TOOLS_LOADMMUNTRADEABLE
Command34=ID_TOOLS_LOADROUTECTRL
Command35=ID_TOOLS_COMMAND
Command36=ID_TOOLS_SCRIPTLIST
Command37=ID_WINDOW_CASCADE
Command38=ID_WINDOW_TILE_HORZ
Command39=ID_WINDOW_ARRANGE
Command40=ID_APP_ABOUT
CommandCount=40

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[CLS:rstAccInfo]
Type=0
HeaderFile=recordset\rstac_tr_ifo.h
ImplementationFile=recordset\rstac_tr_ifo.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:rstAccInfo]
DB=1
DBType=ODBC
ColumnCount=12
Column1=[accid], 4, 4
Column2=[name], 12, 20
Column3=[brid], 4, 4
Column4=[type], 4, 4
Column5=[status], 4, 4
Column6=[rrno], 4, 4
Column7=[per_id], 4, 4
Column8=[add1_id], 4, 4
Column9=[add2_id], 4, 4
Column10=[c_date], 11, 16
Column11=[m_date], 11, 16
Column12=[note], 12, 255

[CLS:rstaccount]
Type=0
HeaderFile=recordset\rstac_tr_ifo.h
ImplementationFile=recordset\rstac_tr_ifo.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:rstaccount]
DB=1
DBType=ODBC
ColumnCount=12
Column1=[accid], 4, 4
Column2=[OpenBP], 6, 8
Column3=[OpenNBP], 6, 8
Column4=[OpenEQ], 6, 8
Column5=[OpenMGEQ], 6, 8
Column6=[OpenEQRQ], 6, 8
Column7=[CurrBP], 6, 8
Column8=[CurrNBP], 6, 8
Column9=[CurrEQ], 6, 8
Column10=[CurrMGEQ], 6, 8
Column11=[CurrEQRQ], 6, 8
Column12=[m_date], 11, 16

[CLS:rstTrader]
Type=0
HeaderFile=recordset\rstac_tr_ifo.h
ImplementationFile=recordset\rstac_tr_ifo.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:rstTrader]
DB=1
DBType=ODBC
ColumnCount=13
Column1=[trid], 4, 4
Column2=[tracno], 12, 10
Column3=[name], 12, 20
Column4=[PWD], 12, 20
Column5=[brid], 4, 4
Column6=[type], 4, 4
Column7=[status], 4, 4
Column8=[per_id], 4, 4
Column9=[add1_id], 4, 4
Column10=[add2_id], 4, 4
Column11=[c_date], 11, 16
Column12=[m_date], 11, 16
Column13=[note], 12, 255

[CLS:rstaddress]
Type=0
HeaderFile=recordset\rstbranch.h
ImplementationFile=recordset\rstbranch.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:rstaddress]
DB=1
DBType=ODBC
ColumnCount=12
Column1=[Add_id], 4, 4
Column2=[flag], 4, 4
Column3=[phone], 12, 50
Column4=[fax], 12, 50
Column5=[address], 12, 50
Column6=[city], 12, 20
Column7=[state], 12, 11
Column8=[zip], 12, 7
Column9=[country], 12, 20
Column10=[c_date], 11, 16
Column11=[m_date], 11, 16
Column12=[note], 12, 255

[CLS:rstperson]
Type=0
HeaderFile=recordset\rstbranch.h
ImplementationFile=recordset\rstbranch.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r
LastObject=rstperson

[DB:rstperson]
DB=1
DBType=ODBC
ColumnCount=14
Column1=[Per_id], 4, 4
Column2=[flag], 4, 4
Column3=[entity_type], -6, 1
Column4=[title], 12, 20
Column5=[entity_name], 12, 50
Column6=[fname], 12, 20
Column7=[mname], 12, 3
Column8=[lname], 12, 25
Column9=[SSN], 12, 30
Column10=[phone], 12, 50
Column11=[email], 12, 50
Column12=[c_date], 11, 16
Column13=[m_date], 11, 16
Column14=[note], 12, 255

[CLS:rsttrader]
Type=0
HeaderFile=recordset\rstbranch.h
ImplementationFile=recordset\rstbranch.cpp
BaseClass=CRecordset
Filter=N
VirtualFilter=r

[DB:rsttrader]
DB=1
DBType=ODBC
ColumnCount=13
Column1=[trid], 4, 4
Column2=[tracno], 12, 10
Column3=[name], 12, 20
Column4=[PWD], 12, 20
Column5=[brid], 4, 4
Column6=[type], 4, 4
Column7=[status], 4, 4
Column8=[per_id], 4, 4
Column9=[add1_id], 4, 4
Column10=[add2_id], 4, 4
Column11=[c_date], 11, 16
Column12=[m_date], 11, 16
Column13=[note], 12, 255

[DLG:IDD_SECIFO]
Type=1
Class=viewSecIfo
ControlCount=24
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BLoadShortLst,button,1342242816
Control3=IDC_EDSECSYM,edit,1350631560
Control4=IDC_CHKSHORT,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CHKMARG,button,1342242819
Control7=IDC_COMBMKT,combobox,1344340226
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CHKHIRISK,button,1342242819
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDSHORTSIZE,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDSOESTIRE,edit,1350631552
Control14=IDC_BChange,button,1342242816
Control15=IDC_BDEL,button,1342242816
Control16=IDC_STATIC,button,1342177287
Control17=IDC_BClear,button,1342242816
Control18=IDC_BComitChange,button,1342242816
Control19=IDC_BUndel,button,1342242816
Control20=IDC_BFind,button,1342242816
Control21=IDC_BSort,button,1342242816
Control22=IDC_PROGCOMIT,msctls_progress32,1073741825
Control23=IDC_BLoadUnShortLst,button,1342242816
Control24=IDC_BRemoveAll,button,1342242816

[CLS:viewSecIfo]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=ID_AAA_ACCOUNT

[CLS:frmSecIfo]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=frmSecIfo

[DLG:IDD_EQUITY]
Type=1
Class=viewEquity
ControlCount=22
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BLoadBuypower,button,1342242816
Control3=IDC_BChange,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BComitChange,button,1342242816
Control6=IDC_BFind,button,1342242816
Control7=IDC_BSort,button,1342242816
Control8=IDC_PROGCOMIT,msctls_progress32,1073741825
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_DBP,edit,1350631552
Control15=IDC_NBP,edit,1350631552
Control16=IDC_EQ,edit,1350631552
Control17=IDC_MEQ,edit,1350631552
Control18=IDC_ACC,edit,1350631552
Control19=IDC_BLoadEquity,button,1342242816
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_BLoadCCSBalance,button,1342242816

[CLS:frmEquity]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=frmEquity

[CLS:viewEquity]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=ID_AAA_ACCOUNT

[DLG:IDD_TRADERS]
Type=1
Class=viewTraders
ControlCount=8
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BRefresh,button,1342242816
Control3=IDC_BKickOut,button,1342242816
Control4=IDC_BSortAsc,button,1342242816
Control5=IDC_BSortDec,button,1342242816
Control6=IDC_BCount,button,1342242816
Control7=IDC_EDLOGOUTMSG,edit,1353781252
Control8=IDC_BBlock,button,1342242816

[CLS:frmTraders]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=frmTraders

[CLS:viewTraders]
Type=0
HeaderFile=mgmt\frmmgmt.h
ImplementationFile=mgmt\frmmgmt.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=ID_AAA_ACCOUNT

[DLG:IDD_DIGIPINPUT]
Type=1
Class=dlgIPBox
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDPORT,edit,1350639744

[CLS:dlgIPBox]
Type=0
HeaderFile=dlgIPBox.h
ImplementationFile=dlgIPBox.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=dlgIPBox

[DLG:IDD_DLGBALANCE]
Type=1
Class=dlgBalance
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDPORT1,edit,1350631552
Control6=IDC_EDPORT2,edit,1350631552
Control7=IDC_STATIC,static,1342177296
Control8=IDC_BRefresh,button,1342242816
Control9=IDC_EDIP1,edit,1350631552
Control10=IDC_EDIP2,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:dlgBalance]
Type=0
HeaderFile=dlgBalance.h
ImplementationFile=dlgBalance.cpp
BaseClass=CDialog
Filter=D
LastObject=dlgBalance
VirtualFilter=dWC

[CLS:OrderRouteCfg]
Type=0
HeaderFile=OrderRouteCfg.h
ImplementationFile=OrderRouteCfg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_AAA_ACCOUNT

[DLG:IDD_ROUTECFG]
Type=1
Class=OrderRouteCfg1
ControlCount=21
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIP,edit,1350631552
Control5=IDC_EDPORT,edit,1350639744
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDUID,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDPWD,edit,1350631552
Control10=IDC_STRoute,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDSTART,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDSTOP,edit,1350631552
Control16=IDC_CAutoConnect,button,1342242819
Control17=IDC_CHKCancelReplace,button,1342242819
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDEXCHFILTER,edit,1350631552
Control20=IDC_CHKHide,button,1342242819
Control21=IDC_CHKDisableCustomerOrder,button,1342251011

[CLS:OrderRouteCfg1]
Type=0
HeaderFile=orderroutecfg.h
ImplementationFile=orderroutecfg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_AAA_ACCOUNT

[DLG:IDD_DLGMSG]
Type=1
Class=dlgPopMsg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_EDMSG,edit,1342244932
Control3=IDC_STICON,static,1342177283

[CLS:dlgPopMsg]
Type=0
HeaderFile=dlgPopMsg.h
ImplementationFile=dlgPopMsg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=dlgPopMsg

[CLS:frmOrderRoute]
Type=0
HeaderFile=frmOrderRoute.h
ImplementationFile=frmOrderRoute.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=frmOrderRoute

[MNU:IDR_MenuPopUp]
Type=1
Class=?
Command1=ID_ROUTE_CONNECT
Command2=ID_ROUTE_DISCONNECT
Command3=ID_ROUTE_CONFIG
Command4=ID_ROUTE_CONNECTALL
Command5=ID_ROUTE_DISCONNECTALL
CommandCount=5

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_VIEW_CFG
Command2=ID_ROUTE_CONFIG
Command3=ID_LogFile
Command4=ID_TOOLS_DBRECONN
Command5=ID_TOOLS_COMMAND
CommandCount=5

[CLS:frmScript]
Type=0
HeaderFile=frmScript.h
ImplementationFile=frmScript.cpp
BaseClass=CMDIChildWnd
Filter=M
VirtualFilter=mfWC
LastObject=frmScript

[DLG:IDD_DIALOG1]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIALOG1 (English (U.S.))]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[DLG:IDD_DIGIPINPUT (English (U.S.))]
Type=1
Class=?
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIP,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_EDPORT,edit,1350639744

[DLG:IDD_DLGBALANCE (English (U.S.))]
Type=1
Class=?
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDPORT1,edit,1350631552
Control6=IDC_EDPORT2,edit,1350631552
Control7=IDC_STATIC,static,1342177296
Control8=IDC_BRefresh,button,1342242816
Control9=IDC_EDIP1,edit,1350631552
Control10=IDC_EDIP2,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[DLG:IDD_DLGMSG (English (U.S.))]
Type=1
Class=?
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_EDMSG,edit,1342244932
Control3=IDC_STICON,static,1342177283

[DLG:IDD_EQUITY (English (U.S.))]
Type=1
Class=?
ControlCount=22
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BLoadBuypower,button,1342242816
Control3=IDC_BChange,button,1342242816
Control4=IDC_STATIC,button,1342177287
Control5=IDC_BComitChange,button,1342242816
Control6=IDC_BFind,button,1342242816
Control7=IDC_BSort,button,1342242816
Control8=IDC_PROGCOMIT,msctls_progress32,1073741825
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_STATIC,static,1342308352
Control14=IDC_DBP,edit,1350631552
Control15=IDC_NBP,edit,1350631552
Control16=IDC_EQ,edit,1350631552
Control17=IDC_MEQ,edit,1350631552
Control18=IDC_ACC,edit,1350631552
Control19=IDC_BLoadEquity,button,1342242816
Control20=IDC_STATIC,static,1342308352
Control21=IDC_STATIC,static,1342308352
Control22=IDC_BLoadCCSBalance,button,1342242816

[DLG:IDD_ROUTECFG (English (U.S.))]
Type=1
Class=?
ControlCount=21
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIP,edit,1350631552
Control5=IDC_EDPORT,edit,1350639744
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDUID,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDPWD,edit,1350631552
Control10=IDC_STRoute,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDSTART,edit,1350631552
Control14=IDC_STATIC,static,1342308352
Control15=IDC_EDSTOP,edit,1350631552
Control16=IDC_CAutoConnect,button,1342242819
Control17=IDC_CHKCancelReplace,button,1342242819
Control18=IDC_STATIC,static,1342308352
Control19=IDC_EDEXCHFILTER,edit,1350631552
Control20=IDC_CHKHide,button,1342242819
Control21=IDC_CHKDisableCustomerOrder,button,1342251011

[DLG:IDD_TRADERS (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BRefresh,button,1342242816
Control3=IDC_BKickOut,button,1342242816
Control4=IDC_BSortAsc,button,1342242816
Control5=IDC_BSortDec,button,1342242816
Control6=IDC_BCount,button,1342242816
Control7=IDC_EDLOGOUTMSG,edit,1353781252
Control8=IDC_BBlock,button,1342242816

[DLG:IDD_SECIFO (English (U.S.))]
Type=1
Class=?
ControlCount=24
Control1=IDC_STGRID,static,1342308352
Control2=IDC_BLoadShortLst,button,1342242816
Control3=IDC_EDSECSYM,edit,1350631560
Control4=IDC_CHKSHORT,button,1342242819
Control5=IDC_STATIC,static,1342308352
Control6=IDC_CHKMARG,button,1342242819
Control7=IDC_COMBMKT,combobox,1344340226
Control8=IDC_STATIC,static,1342308352
Control9=IDC_CHKHIRISK,button,1342242819
Control10=IDC_STATIC,static,1342308352
Control11=IDC_EDSHORTSIZE,edit,1350631552
Control12=IDC_STATIC,static,1342308352
Control13=IDC_EDSOESTIRE,edit,1350631552
Control14=IDC_BChange,button,1342242816
Control15=IDC_BDEL,button,1342242816
Control16=IDC_STATIC,button,1342177287
Control17=IDC_BClear,button,1342242816
Control18=IDC_BComitChange,button,1342242816
Control19=IDC_BUndel,button,1342242816
Control20=IDC_BFind,button,1342242816
Control21=IDC_BSort,button,1342242816
Control22=IDC_PROGCOMIT,msctls_progress32,1073741825
Control23=IDC_BLoadUnShortLst,button,1342242816
Control24=IDC_BRemoveAll,button,1342242816

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_CFG
Command2=ID_VIEW_LOGINLOG
Command3=ID_VIEW_LOG
Command4=ID_VIEW_TRADERS
Command5=ID_VIEW_THREADS
Command6=ID_APP_EXIT
Command7=ID_AAA_ACCOUNT
Command8=ID_AAA_TMP1
Command9=ID_AAA_POSITION
Command10=ID_AAA_ONLINETRADER
Command11=ID_AAA_DEAL
Command12=ID_AAA_TRADES
Command13=ID_AAA_ORDERS
Command14=ID_AAA_TICKET
Command15=ID_AAA_TEST
Command16=ID_DATA_LOADEQUITY
Command17=ID_DATA_LSHTLST
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_DATA_EXPORTPOSITION
Command21=ID_DATA_SOD_QUEST
Command22=ID_DATA_SOD_BPEQUITY
Command23=ID_DATA_SOD_SHORTLIST
Command24=ID_DATA_SOD_UNSHORTLIST
Command25=ID_DATA_SAVEEQUITY
Command26=ID_DATA_COUNTING
Command27=ID_DATA_SAVEDATA
Command28=ID_DATA_RECOVER_SAVETRADES
Command29=ID_TOOLS_SECIFO
Command30=ID_TOOLS_EQUITY
Command31=ID_TOOLS_BALANCE
Command32=ID_TOOLS_LOADLOGINCTRL
Command33=ID_TOOLS_LOADMMUNTRADEABLE
Command34=ID_TOOLS_LOADROUTECTRL
Command35=ID_TOOLS_COMMAND
Command36=ID_TOOLS_SCRIPTLIST
Command37=ID_WINDOW_CASCADE
Command38=ID_WINDOW_TILE_HORZ
Command39=ID_WINDOW_ARRANGE
Command40=ID_APP_ABOUT
CommandCount=40

[MNU:IDR_MenuPopUp (English (U.S.))]
Type=1
Class=?
Command1=ID_ROUTE_CONNECT
Command2=ID_ROUTE_DISCONNECT
Command3=ID_ROUTE_CONFIG
Command4=ID_ROUTE_CONNECTALL
Command5=ID_ROUTE_DISCONNECTALL
CommandCount=5

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=8
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STVER,static,1342308352
Control6=IDC_STIFO,static,1342308608
Control7=IDC_BCrash,button,1342242816
Control8=IDC_BPref,button,1342242816

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_VIEW_CFG
Command2=ID_ROUTE_CONFIG
Command3=ID_LogFile
Command4=ID_TOOLS_DBRECONN
Command5=ID_TOOLS_COMMAND
CommandCount=5

