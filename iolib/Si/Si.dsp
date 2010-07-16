# Microsoft Developer Studio Project File - Name="Si" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Si - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Si.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Si.mak" CFG="Si - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Si - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Si - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Si - Win32 Debug_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "Si - Win32 Release_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Si"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "Si - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Si.arel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\Si.arel.lib"

!ELSEIF  "$(CFG)" == "Si - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Si.adbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\Si.adbg.lib"

!ELSEIF  "$(CFG)" == "Si - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Si___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Si___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Si.wdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"Debug/Si.pch" /YX /FD /GZ /Zm700 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Si_d.lib"
# ADD LIB32 /nologo /out:"..\Libs\Si.wdbg.lib"

!ELSEIF  "$(CFG)" == "Si - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Si___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Si___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Si.wrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"Release/Si.pch" /YX /FD /Zm700 /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Si.lib"
# ADD LIB32 /nologo /out:"..\Libs\Si.wrel.lib"

!ENDIF 

# Begin Target

# Name "Si - Win32 Release"
# Name "Si - Win32 Debug"
# Name "Si - Win32 Debug_Unicode"
# Name "Si - Win32 Release_Unicode"
# Begin Group "Root"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "NW_Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\si.nwcontrol.base.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.base.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.base.ttip.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.base.ttip.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.button.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.button.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.flatsbar.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.label.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.label.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.popupbtn.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.popupbtn.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.sbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.sbutton.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.slider.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.slider.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.swbutton.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.swbutton.h
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.tab.cpp
# End Source File
# Begin Source File

SOURCE=.\si.nwcontrol.tab.h
# End Source File
# End Group
# Begin Group "WND_Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\si.wctrl.tree.cpp
# End Source File
# Begin Source File

SOURCE=.\si.wctrl.tree.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\si.common.h
# End Source File
# Begin Source File

SOURCE=.\si.nwinterfaces.h
# End Source File
# Begin Source File

SOURCE=.\si.osspec.cpp
# End Source File
# Begin Source File

SOURCE=.\si.osspec.h
# End Source File
# Begin Source File

SOURCE=.\si.std.h
# End Source File
# Begin Source File

SOURCE=.\si.tooltip.disp.h
# End Source File
# Begin Source File

SOURCE=.\si.window.basenw.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.basenw.h
# End Source File
# Begin Source File

SOURCE=.\si.window.frame.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.frame.h
# End Source File
# Begin Source File

SOURCE=.\si.window.generic.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.generic.h
# End Source File
# Begin Source File

SOURCE=.\si.window.popup.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.popup.h
# End Source File
# Begin Source File

SOURCE=.\si.window.popupmenu.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.popupmenu.h
# End Source File
# Begin Source File

SOURCE=.\si.window.topmost.cpp
# End Source File
# Begin Source File

SOURCE=.\si.window.topmost.h
# End Source File
# Begin Source File

SOURCE=.\si.winmgr.h
# End Source File
# End Group
# End Target
# End Project
