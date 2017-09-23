cmake_minimum_required(VERSION 3.0.2)

project(OrderServ)

include_directories(. ../define ../../comm)

set(RESOURCE OrderServ.rc)

set(COMMONFILES 
../../Comm/Functions/FixLenMem.cpp
../../Comm/Functions/intmap.cpp
../../Comm/Functions/MDVersion.cpp
../../Comm/MSocket/MSocket.cpp
../../Comm/Functions/FixLenMem.h
../../Comm/Functions/intmap.h
../../Comm/Functions/MDVersion.h
../../Comm/MSocket/Msocket.h
../../Comm/Functions/PkgBuff.cpp
../../Comm/Functions/PkgBuff.h
../../Comm/defines/PkgIDDef.h
../../Comm/gridctrl/CellRange.h
../../Comm/gridctrl/GridCell.cpp
../../Comm/gridctrl/GridCell.h
../../Comm/gridctrl/GridCellBase.cpp
../../Comm/gridctrl/GridCellBase.h
../../Comm/gridctrl/GridCtrl.cpp
../../Comm/gridctrl/GridCtrl.h
../../Comm/gridctrl/GridDropTarget.cpp
../../Comm/gridctrl/GridDropTarget.h
../../Comm/gridctrl/InPlaceEdit.cpp
../../Comm/gridctrl/InPlaceEdit.h
../../Comm/gridctrl/MemDC.h
../../Comm/gridctrl/TitleTip.cpp
../../Comm/gridctrl/TitleTip.h
../../Comm/Functions/ControlPos.cpp
../../Comm/Functions/ControlPos.h
../../Comm/Crash/ExceptionHandler.cpp
../../Comm/Crash/GetWinVer.cpp
../../Comm/Crash/MiniVersion.cpp
../../Comm/Functions/CCmdScript.cpp
../../Comm/Functions/CCmdScript.h
../../Comm/Functions/Config.cpp
../../Comm/Functions/Config.h
../../Comm/Functions/CPUIfo.cpp
../../Comm/Functions/CPUIfo.h
../../Comm/Functions/encrypt.cpp
../../Comm/Functions/encrypt.h
../../Comm/Functions/InputBox.cpp
../../Comm/Functions/InputBox.h
../../Comm/Log/Log.cpp
../../Comm/Log/Log.h
../../Comm/Log/logframe.cpp
../../Comm/Log/logframe.h
../../Comm/Security/md5.cpp
../../Comm/Security/md5.h
../Define/OrderDef.h
../../Comm/Functions/Perf.cpp
../../Comm/Functions/Perf.h
../../Comm/SRecSet/SRecSet.cpp
../../Comm/SRecSet/SRecSet.h
../../Comm/Functions/strfuncs.cpp
../../Comm/Functions/strfuncs.h
../../Comm/Functions/synchronal.cpp
../../Comm/Functions/synchronal.h
../Define/UDPPkgs.h)

file(GLOB_RECURSE HEADERS "*.h")
file(GLOB_RECURSE SOURCES "*.cpp")
#file(GLOB_RECURSE COMMONFILES ../../comm/* )
#file(GLOB_RECURSE COMMONFILESHEADER ../../comm/*.h )
#file(GLOB_RECURSE COMMONFILESSOURCE ../../comm/*.cpp )

IF(WIN32) # Check if we are on Windows
	if(MSVC) # Check if we are using the Visual Studio compiler
		add_definitions( -D_AFXDLL )
		add_definitions( -D_CRT_SECURE_NO_DEPRECATE )
		add_definitions( -D_CRT_NONSTDC_NO_DEPRECATE )
		add_definitions( -D_CRT_SECURE_NO_WARNINGS )
		add_definitions( -D_SCL_SECURE_NO_WARNINGS )
		set(CMAKE_MFC_FLAG 2)
		#set_target_properties(NewsServer PROPERTIES LINK_FLAGS_DEBUG "/SUBSYSTEM:CONSOLE")
		#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /subsystem:windows")
	endif()
ENDIF()

add_executable(${PROJECT_NAME} WIN32 ${HEADERS} ${SOURCES} ${RESOURCE} ${COMMONFILES} )

target_link_libraries( ${PROJECT_NAME} dbghelp.lib )

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/config"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>)