# Microsoft Developer Studio Project File - Name="He" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=He - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "He.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "He.mak" CFG="He - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "He - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "He - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "He - Win32 Debug_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "He - Win32 Release_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "He"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "He - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\He.arel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\He.arel.lib"

!ELSEIF  "$(CFG)" == "He - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\He.adbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /EHac /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\He.adbg.lib"

!ELSEIF  "$(CFG)" == "He - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "He___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "He___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\He.wdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"Debug/He.pch" /YX /FD /GZ /Zm700 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\He_d.lib"
# ADD LIB32 /nologo /out:"..\Libs\He.wdbg.lib"

!ELSEIF  "$(CFG)" == "He - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "He___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "He___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\He.wrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"Release/He.pch" /YX /FD /Zm700 /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\He.lib"
# ADD LIB32 /nologo /out:"..\Libs\He.wrel.lib"

!ENDIF 

# Begin Target

# Name "He - Win32 Release"
# Name "He - Win32 Debug"
# Name "He - Win32 Debug_Unicode"
# Name "He - Win32 Release_Unicode"
# Begin Group "Root"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\he.buff.h
# End Source File
# Begin Source File

SOURCE=.\he.console.h
# End Source File
# Begin Source File

SOURCE=.\he.debug.log.cpp
# End Source File
# Begin Source File

SOURCE=.\he.debug.log.h
# End Source File
# Begin Source File

SOURCE=.\he.defines.h
# End Source File
# Begin Source File

SOURCE=.\he.fixpoint.h
# End Source File
# Begin Source File

SOURCE=.\he.handle.h
# End Source File
# Begin Source File

SOURCE=.\he.ipc.anpipes.h
# End Source File
# Begin Source File

SOURCE=.\he.ipc.nmpipes.h
# End Source File
# Begin Source File

SOURCE=.\he.localization.cpp
# End Source File
# Begin Source File

SOURCE=.\he.localization.h
# End Source File
# Begin Source File

SOURCE=.\he.net.address.h
# End Source File
# Begin Source File

SOURCE=.\he.net.blocksock.h
# End Source File
# Begin Source File

SOURCE=.\he.pair.h
# End Source File
# Begin Source File

SOURCE=.\he.smartptr.h
# End Source File
# Begin Source File

SOURCE=.\he.std.cpp
# End Source File
# Begin Source File

SOURCE=.\he.std.h
# End Source File
# Begin Source File

SOURCE=.\he.string.cpp
# End Source File
# Begin Source File

SOURCE=.\he.string.h
# End Source File
# Begin Source File

SOURCE=.\he.string.std.h
# End Source File
# Begin Source File

SOURCE=.\he.tables.cpp
# End Source File
# Begin Source File

SOURCE=.\he.tables.h
# End Source File
# Begin Source File

SOURCE=.\he.wnd.helpers.h
# End Source File
# End Group
# Begin Group "Containers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\he.container.cache.h
# End Source File
# Begin Source File

SOURCE=.\he.container.fast.array.h
# End Source File
# Begin Source File

SOURCE=.\he.container.fast.ilist.h
# End Source File
# Begin Source File

SOURCE=.\he.container.hash.h
# End Source File
# Begin Source File

SOURCE=.\he.container.map.h
# End Source File
# Begin Source File

SOURCE=.\he.container.simple.array.h
# End Source File
# Begin Source File

SOURCE=.\he.container.simple.map.h
# End Source File
# Begin Source File

SOURCE=.\he.container.tree.h
# End Source File
# End Group
# Begin Group "Kernel"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\he.cpuf.cpp
# End Source File
# Begin Source File

SOURCE=.\he.cpuf.h
# End Source File
# Begin Source File

SOURCE=.\he.memory.compress.cpp
# End Source File
# Begin Source File

SOURCE=.\he.memory.compress.h
# End Source File
# Begin Source File

SOURCE=.\he.memory.cpp
# End Source File
# Begin Source File

SOURCE=.\he.memory.dynbuff.h
# End Source File
# Begin Source File

SOURCE=.\he.memory.h
# End Source File
# Begin Source File

SOURCE=.\he.threads.cpp
# End Source File
# Begin Source File

SOURCE=.\he.threads.h
# End Source File
# End Group
# Begin Group "File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\he.crc32.cpp
# End Source File
# Begin Source File

SOURCE=.\he.crc32.h
# End Source File
# Begin Source File

SOURCE=.\he.file.cpp
# End Source File
# Begin Source File

SOURCE=.\he.file.h
# End Source File
# Begin Source File

SOURCE=.\he.file.name.cpp
# End Source File
# Begin Source File

SOURCE=.\he.file.name.h
# End Source File
# Begin Source File

SOURCE=.\he.file.std.h
# End Source File
# Begin Source File

SOURCE=.\he.inifile.h
# End Source File
# End Group
# Begin Group "Metrics"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\he.metrics.cpp
# End Source File
# Begin Source File

SOURCE=.\he.metrics.h
# End Source File
# Begin Source File

SOURCE=.\he.range.h
# End Source File
# Begin Source File

SOURCE=.\he.types.h
# End Source File
# Begin Source File

SOURCE=.\he.vector.h
# End Source File
# End Group
# End Target
# End Project
