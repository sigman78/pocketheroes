# Microsoft Developer Studio Project File - Name="Xe" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Xe - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Xe.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Xe.mak" CFG="Xe - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Xe - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Xe - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Xe - Win32 Debug_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "Xe - Win32 Release_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "Xe"
# PROP Scc_LocalPath "."
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Xe - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Xe.arel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /Zm700 /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\Xe.arel.lib"

!ELSEIF  "$(CFG)" == "Xe - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Xe.adbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\Libs\Xe.adbg.lib"

!ELSEIF  "$(CFG)" == "Xe - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Xe___Win32_Debug_Unicode"
# PROP BASE Intermediate_Dir "Xe___Win32_Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Xe.wdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /Fp"Debug/Xe.pch" /YX /FD /GZ /Zm700 /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "_DEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /GZ /Zm700 /c
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Xe_d.lib"
# ADD LIB32 /nologo /out:"..\Libs\Xe.wdbg.lib"

!ELSEIF  "$(CFG)" == "Xe - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Xe___Win32_Release_Unicode"
# PROP BASE Intermediate_Dir "Xe___Win32_Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\libs\"
# PROP Intermediate_Dir "..\objs\Xe.wrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /Fp"Release/Xe.pch" /YX /FD /Zm700 /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "png\zlib" /I "png\lpng125" /I "JPG" /D "WIN32" /D "NDEBUG" /D "_UNICODE" /D "_LIB" /YX /FD /Zm700 /QIfist /c
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"..\Libs\Xe.lib"
# ADD LIB32 /nologo /out:"..\Libs\Xe.wrel.lib"

!ENDIF 

# Begin Target

# Name "Xe - Win32 Release"
# Name "Xe - Win32 Debug"
# Name "Xe - Win32 Debug_Unicode"
# Name "Xe - Win32 Release_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\xe.color.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.color.h
# End Source File
# Begin Source File

SOURCE=.\xe.cursor.h
# End Source File
# Begin Source File

SOURCE=.\xe.dc.h
# End Source File
# Begin Source File

SOURCE=.\xe.dc.hlp.h
# End Source File
# Begin Source File

SOURCE=.\xe.dib.8bit.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.8bit.h
# End Source File
# Begin Source File

SOURCE=.\xe.dib.cache.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.cache.h
# End Source File
# Begin Source File

SOURCE=.\xe.dib.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.dchandl.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.filters.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.fx.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.grayhandl.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.h
# End Source File
# Begin Source File

SOURCE=.\xe.dib.llproc.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.llproc.h
# End Source File
# Begin Source File

SOURCE=.\xe.dib.loader.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.mem.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.primitives.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.saver.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.text.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.transform.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.dib.transform.fscale.cpp
# End Source File
# Begin Source File

SOURCE=.\xe.font.h
# End Source File
# Begin Source File

SOURCE=.\xe.gray.dib.h
# End Source File
# Begin Source File

SOURCE=.\xe.memorydc.h
# End Source File
# Begin Source File

SOURCE=.\xe.std.h
# End Source File
# End Group
# Begin Group "PNG"

# PROP Default_Filter ""
# Begin Group "ZLIB"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PNG\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\maketree.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\PNG\zlib\zutil.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\PNG\lpng125\png.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\png.h
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngget.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngread.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngset.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\PNG\lpng125\pngwutil.c
# End Source File
# End Group
# End Target
# End Project
