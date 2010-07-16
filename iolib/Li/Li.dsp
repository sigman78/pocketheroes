# Microsoft Developer Studio Project File - Name="Li" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Li - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Li.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Li.mak" CFG="Li - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Li - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Li - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Li - Win32 Debug_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "Li - Win32 Release_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Li"
# PROP Scc_LocalPath "."
CPP=xicl6.exe
RSC=rc.exe

!IF  "$(CFG)" == "Li - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Li.arel"
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
# ADD LIB32 /nologo /out:"..\Libs\Li.arel.lib"

!ELSEIF  "$(CFG)" == "Li - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Li.adbg"
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
# ADD LIB32 /nologo /out:"..\Libs\Li.adbg.lib"

!ELSEIF  "$(CFG)" == "Li - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Li___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Li___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Li.wdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"Debug/Li.pch" /YX /FD /GZ /Zm700 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Li_d.lib"
# ADD LIB32 /nologo /out:"..\Libs\Li.wdbg.lib"

!ELSEIF  "$(CFG)" == "Li - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Li___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Li___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Li.wrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"Release/Li.pch" /YX /FD /Zm700 /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=xilink6.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Li.lib"
# ADD LIB32 /nologo /out:"..\Libs\Li.wrel.lib"

!ENDIF 

# Begin Target

# Name "Li - Win32 Release"
# Name "Li - Win32 Debug"
# Name "Li - Win32 Debug_Unicode"
# Name "Li - Win32 Release_Unicode"
# Begin Group "Root"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\li.cmndlg.cpp
# End Source File
# Begin Source File

SOURCE=.\li.cmndlg.h
# End Source File
# Begin Source File

SOURCE=.\li.dialog.base.h
# End Source File
# Begin Source File

SOURCE=.\li.dialog.progress.cpp
# End Source File
# Begin Source File

SOURCE=.\li.dialog.progress.h
# End Source File
# End Group
# Begin Group "Controls"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\li.ctrl.alevel.h
# End Source File
# Begin Source File

SOURCE=.\li.ctrl.editbtn.h
# End Source File
# Begin Source File

SOURCE=.\li.ctrl.outlookbar.h
# End Source File
# Begin Source File

SOURCE=.\li.ctrl.statimg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\li.common.h
# End Source File
# Begin Source File

SOURCE=.\li.helpers.h
# End Source File
# Begin Source File

SOURCE=.\li.std.h
# End Source File
# Begin Source File

SOURCE=.\li.tbpane.h
# End Source File
# Begin Source File

SOURCE=.\li.tray.h
# End Source File
# Begin Source File

SOURCE=.\li.wtl.hlp.h
# End Source File
# End Group
# End Target
# End Project
