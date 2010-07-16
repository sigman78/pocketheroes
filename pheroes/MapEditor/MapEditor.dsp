# Microsoft Developer Studio Project File - Name="MapEditor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=MapEditor - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MapEditor.mak" CFG="MapEditor - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MapEditor - Win32 Debug_Unicode" (based on "Win32 (x86) Application")
!MESSAGE "MapEditor - Win32 Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MapEditor - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_Unicode"
# PROP BASE Intermediate_Dir "Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\obj\MapEditor_w32.wdbg"
# PROP Intermediate_Dir "..\obj\MapEditor_w32.wdbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\common" /I "." /I "..\..\iolib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_UNICODE" /D "STRICT" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin\MapEditor.wdbg.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "MapEditor - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_Unicode"
# PROP BASE Intermediate_Dir "Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\obj\MapEditor_w32.wrel"
# PROP Intermediate_Dir "..\obj\MapEditor_w32.wrel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\common" /I "." /I "..\..\iolib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_UNICODE" /D "STRICT" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"..\bin\MapEditor.exe"

!ENDIF 

# Begin Target

# Name "MapEditor - Win32 Debug_Unicode"
# Name "MapEditor - Win32 Release_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Res"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Artifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\Artifacts.h
# End Source File
# Begin Source File

SOURCE=.\AvatarMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarMgr.h
# End Source File
# Begin Source File

SOURCE=.\Constructions.cpp
# End Source File
# Begin Source File

SOURCE=.\Constructions.h
# End Source File
# Begin Source File

SOURCE=.\defines.cpp
# End Source File
# Begin Source File

SOURCE=.\defines.h
# End Source File
# Begin Source File

SOURCE=.\Items.h
# End Source File
# Begin Source File

SOURCE=.\Nodes.h
# End Source File
# Begin Source File

SOURCE=.\objects.cpp
# End Source File
# Begin Source File

SOURCE=.\objects.h
# End Source File
# Begin Source File

SOURCE=.\resmgr.cpp
# End Source File
# Begin Source File

SOURCE=.\resmgr.h
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundMgr.h
# End Source File
# Begin Source File

SOURCE=.\SpriteMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\SpriteMgr.h
# End Source File
# Begin Source File

SOURCE=.\TextMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TextMgr.h
# End Source File
# End Group
# Begin Group "Export"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ExportDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportDlg.h
# End Source File
# End Group
# Begin Group "Property Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PropertyPages.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyPages.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ChildDlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\ChildDlgs.h
# End Source File
# Begin Source File

SOURCE=.\Commands.cpp
# End Source File
# Begin Source File

SOURCE=.\Commands.h
# End Source File
# Begin Source File

SOURCE=.\Composer.cpp
# End Source File
# Begin Source File

SOURCE=.\Composer.h
# End Source File
# Begin Source File

SOURCE=.\ifaces.h
# End Source File
# Begin Source File

SOURCE=.\mainfrm.h
# End Source File
# Begin Source File

SOURCE=.\MainView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainView.h
# End Source File
# Begin Source File

SOURCE=.\MapEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGen.cpp
# End Source File
# Begin Source File

SOURCE=.\MapGen.h
# End Source File
# Begin Source File

SOURCE=.\MapHandler.cpp
# End Source File
# Begin Source File

SOURCE=.\MapHandler.h
# End Source File
# Begin Source File

SOURCE=.\MapValidateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MapValidateDlg.h
# End Source File
# Begin Source File

SOURCE=.\MinimapView.cpp
# End Source File
# Begin Source File

SOURCE=.\MinimapView.h
# End Source File
# Begin Source File

SOURCE=.\NewMapDlg.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# ADD BASE CPP /Yc"stdafx.h"
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\TextResDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\TextResDlg.h
# End Source File
# Begin Source File

SOURCE=.\UltimatePropsDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\folder.bmp
# End Source File
# Begin Source File

SOURCE=.\res\maintb24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MapEditor.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\MapEditor.ico
# End Source File
# Begin Source File

SOURCE=.\MapEditor.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\res\surftb24.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_clp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\tree_exp.bmp
# End Source File
# End Group
# Begin Group "Common"

# PROP Default_Filter ""
# Begin Group "lzo"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\lzo.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\lzo.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzo\lzoconf.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzo\lzodefs.h
# End Source File
# Begin Source File

SOURCE=..\Common\lzo\minilzo.c
# SUBTRACT CPP /YX /Yc /Yu
# End Source File
# Begin Source File

SOURCE=..\Common\lzo\minilzo.h
# End Source File
# End Group
# Begin Group "xau"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Common\xau\xau.audio.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.audio.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.cfg.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.ppf.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.ppf.impl.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.rice.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xau.rice.impl.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xbs.bs.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xbs.bs.impl.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xbs.bswap.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xbs.cfg.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xbs.math.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.block.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.block.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.boot.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.codec.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.codec.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.dct.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.dct.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.huff.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.huff.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.huff.impl.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.quant.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.quant.h
# End Source File
# Begin Source File

SOURCE=..\Common\xau\xjp.swizzle.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\Common\Army.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\Army.h
# End Source File
# Begin Source File

SOURCE=..\Common\cm_castles.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cm_castles.h
# End Source File
# Begin Source File

SOURCE=..\Common\cm_constructions.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cm_constructions.h
# End Source File
# Begin Source File

SOURCE=..\Common\cm_creatures.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cm_creatures.h
# End Source File
# Begin Source File

SOURCE=..\Common\cm_events.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cm_events.h
# End Source File
# Begin Source File

SOURCE=..\Common\cm_magic.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\cm_magic.h
# End Source File
# Begin Source File

SOURCE=..\Common\common.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\common.h
# End Source File
# Begin Source File

SOURCE=..\Common\isometric.h
# End Source File
# Begin Source File

SOURCE=..\Common\objdef.h
# End Source File
# Begin Source File

SOURCE=..\Common\rtti.h
# End Source File
# Begin Source File

SOURCE=..\Common\serialize.h
# End Source File
# Begin Source File

SOURCE=..\Common\SpanMap.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\SpanMap.h
# End Source File
# Begin Source File

SOURCE=..\Common\treasury_cnst.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\treasury_cnst.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\readme.txt
# End Source File
# End Target
# End Project
