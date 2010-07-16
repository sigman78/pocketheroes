# Microsoft Developer Studio Project File - Name="Pa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=Pa - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Pa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Pa.mak" CFG="Pa - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Pa - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "Pa - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "Pa - Win32 Debug_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE "Pa - Win32 Release_Unicode" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Pa - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\..\libs"
# PROP BASE Intermediate_Dir ".\..\objs\Pa.adbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\..\libs"
# PROP Intermediate_Dir ".\..\objs\Pa.adbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./libxml/src\\" /I "../" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "./" /I "./libxml/src/" /I "../" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_MBCS" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\..\libs\Pa.adbg.lib"
# ADD LIB32 /nologo /out:".\..\libs\Pa.adbg.lib"

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\..\libs"
# PROP BASE Intermediate_Dir ".\..\objs\Pa.arel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\..\libs"
# PROP Intermediate_Dir ".\..\objs\Pa.arel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W3 /GX /Zi /I "./" /I "./libxml/src\\" /I "../" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MD /W3 /GX /Zi /I "./" /I "./libxml/src/" /I "../" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_MBCS" PRECOMP_VC7_TOBEREMOVED /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\..\libs\Pa.arel.lib"
# ADD LIB32 /nologo /out:".\..\libs\Pa.arel.lib"

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\..\libs"
# PROP BASE Intermediate_Dir ".\..\objs\Pa.wdbg"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\..\libs"
# PROP Intermediate_Dir ".\..\objs\Pa.wdbg"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MDd /W2 /Gm /GX /ZI /Od /I "../" /I "./" /I "./libxml/src\\" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /GZ PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MDd /W2 /Gm /GX /ZI /Od /I "./" /I "./libxml/src/" /I "../" /D "WIN32" /D "_DEBUG" /D "_LIB" /D "_UNICODE" /GZ /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\..\libs\Pa.wdbg.lib"
# ADD LIB32 /nologo /out:".\..\libs\Pa.wdbg.lib"

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\..\libs"
# PROP BASE Intermediate_Dir ".\..\objs\Pa.wrel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\..\libs"
# PROP Intermediate_Dir ".\..\objs\Pa.wrel"
# PROP Target_Dir ""
MTL=midl.exe
# ADD BASE MTL /nologo /win32
# ADD MTL /nologo /win32
# ADD BASE CPP /nologo /MD /W2 /GX /Zi /I "./" /I "./libxml/src\\" /I "../" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" PRECOMP_VC7_TOBEREMOVED /c
# ADD CPP /nologo /MD /W2 /GX /Zi /I "./" /I "./libxml/src/" /I "../" /D "WIN32" /D "NDEBUG" /D "_LIB" /D "_UNICODE" /c
# ADD BASE RSC /l 0x409
# ADD RSC /l 0x409
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:".\..\libs\Pa.wrel.lib"
# ADD LIB32 /nologo /out:".\..\libs\Pa.wrel.lib"

!ENDIF 

# Begin Target

# Name "Pa - Win32 Debug"
# Name "Pa - Win32 Release"
# Name "Pa - Win32 Debug_Unicode"
# Name "Pa - Win32 Release_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;def;odl;idl;hpj;bat;asm;asmx"
# Begin Source File

SOURCE=.\pa.autovalue.h
# End Source File
# Begin Source File

SOURCE=.\pa.convert.h
# End Source File
# Begin Source File

SOURCE=.\pa.convert.utf8.cpp
DEP_CPP_PA_CO=\
	"..\He\he.cpuf.h"\
	"..\He\he.defines.h"\
	"..\He\he.memory.dynbuff.h"\
	"..\He\he.memory.h"\
	"..\He\he.metrics.h"\
	"..\He\he.std.h"\
	"..\He\he.string.h"\
	"..\He\he.string.std.h"\
	"..\He\he.types.h"\
	".\pa.convert.utf8.h"\
	".\pa.except.h"\
	{$(INCLUDE)}"mmintrin.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pa.convert.utf8.h
# End Source File
# Begin Source File

SOURCE=.\pa.convert.xml.h
# End Source File
# Begin Source File

SOURCE=.\pa.except.h
# End Source File
# Begin Source File

SOURCE=.\pa.map.h
# End Source File
# Begin Source File

SOURCE=.\pa.sax.format.h
# End Source File
# Begin Source File

SOURCE=.\pa.sax.h
# End Source File
# Begin Source File

SOURCE=.\pa.sax.handler.h
# End Source File
# Begin Source File

SOURCE=.\pa.sax.parser.cpp
DEP_CPP_PA_SA=\
	"..\He\he.container.simple.array.h"\
	"..\He\he.cpuf.h"\
	"..\He\he.defines.h"\
	"..\He\he.memory.dynbuff.h"\
	"..\He\he.memory.h"\
	"..\He\he.metrics.h"\
	"..\He\he.std.h"\
	"..\He\he.string.h"\
	"..\He\he.string.std.h"\
	"..\He\he.types.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\pa.convert.h"\
	".\pa.except.h"\
	".\pa.sax.parser.h"\
	{$(INCLUDE)}"mmintrin.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pa.sax.parser.h
# End Source File
# Begin Source File

SOURCE=.\pa.sax.pathhandler.h
# End Source File
# Begin Source File

SOURCE=.\pa.serialize.h
# End Source File
# Begin Source File

SOURCE=.\pa.variant.h
# End Source File
# Begin Source File

SOURCE=.\pa.xml.builder.cpp
DEP_CPP_PA_XM=\
	"..\He\he.cpuf.h"\
	"..\He\he.defines.h"\
	"..\He\he.memory.dynbuff.h"\
	"..\He\he.memory.h"\
	"..\He\he.metrics.h"\
	"..\He\he.std.h"\
	"..\He\he.string.h"\
	"..\He\he.string.std.h"\
	"..\He\he.types.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\pa.convert.h"\
	".\pa.convert.utf8.h"\
	".\pa.except.h"\
	".\pa.xml.builder.h"\
	{$(INCLUDE)}"mmintrin.h"\
	
# End Source File
# Begin Source File

SOURCE=.\pa.xml.builder.h
# End Source File
# End Group
# Begin Group "libxml2"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libxml\c14n.h
# End Source File
# Begin Source File

SOURCE=.\libxml\catalog.h
# End Source File
# Begin Source File

SOURCE=.\libxml\debugXML.h
# End Source File
# Begin Source File

SOURCE=.\libxml\DOCBparser.h
# End Source File
# Begin Source File

SOURCE=.\libxml\encoding.h
# End Source File
# Begin Source File

SOURCE=.\libxml\entities.h
# End Source File
# Begin Source File

SOURCE=.\libxml\globals.h
# End Source File
# Begin Source File

SOURCE=.\libxml\hash.h
# End Source File
# Begin Source File

SOURCE=.\libxml\HTMLparser.h
# End Source File
# Begin Source File

SOURCE=.\libxml\HTMLtree.h
# End Source File
# Begin Source File

SOURCE=.\libxml\list.h
# End Source File
# Begin Source File

SOURCE=.\libxml\nanoftp.h
# End Source File
# Begin Source File

SOURCE=.\libxml\nanohttp.h
# End Source File
# Begin Source File

SOURCE=.\libxml\parser.h
# End Source File
# Begin Source File

SOURCE=.\libxml\parserInternals.h
# End Source File
# Begin Source File

SOURCE=.\libxml\SAX.h
# End Source File
# Begin Source File

SOURCE=.\libxml\tree.h
# End Source File
# Begin Source File

SOURCE=.\libxml\uri.h
# End Source File
# Begin Source File

SOURCE=.\libxml\valid.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xinclude.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xlink.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlautomata.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlerror.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlIO.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlmemory.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlversion.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xmlwin32version.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xpath.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xpathInternals.h
# End Source File
# Begin Source File

SOURCE=.\libxml\xpointer.h
# End Source File
# End Group
# Begin Group "libxml2src"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\libxml\src\c14n.c
DEP_CPP_C14N_=\
	".\libxml\c14n.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	".\libxml\xpathInternals.h"\
	
NODEP_CPP_C14N_=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\catalog.c
DEP_CPP_CATAL=\
	".\libxml\catalog.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_CATAL=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\debugXML.c
DEP_CPP_DEBUG=\
	".\libxml\debugXML.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\HTMLtree.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	".\libxml\xpathInternals.h"\
	
NODEP_CPP_DEBUG=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\DOCBparser.c
DEP_CPP_DOCBP=\
	".\libxml\DOCBparser.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_DOCBP=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\encoding.c
DEP_CPP_ENCOD=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_ENCOD=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\entities.c
DEP_CPP_ENTIT=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_ENTIT=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\error.c
DEP_CPP_ERROR=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_ERROR=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\globals.c
DEP_CPP_GLOBA=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_GLOBA=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\hash.c
DEP_CPP_HASH_=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_HASH_=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\HTMLparser.c
DEP_CPP_HTMLP=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\HTMLtree.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_HTMLP=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\HTMLtree.c
DEP_CPP_HTMLT=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\HTMLtree.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_HTMLT=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\list.c
DEP_CPP_LIST_=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_LIST_=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\nanoftp.c
DEP_CPP_NANOF=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\nanoftp.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_NANOF=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\nanohttp.c
DEP_CPP_NANOH=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\nanohttp.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_NANOH=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\parser.c
DEP_CPP_PARSE=\
	".\libxml\catalog.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	
NODEP_CPP_PARSE=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\parserInternals.c
DEP_CPP_PARSER=\
	".\libxml\catalog.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_PARSER=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\SAX.c
DEP_CPP_SAX_C=\
	".\libxml\debugXML.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\HTMLtree.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	
NODEP_CPP_SAX_C=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\threads.c
DEP_CPP_THREA=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_THREA=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\tree.c
DEP_CPP_TREE_=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\HTMLtree.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_TREE_=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\uri.c
DEP_CPP_URI_C=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_URI_C=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\valid.c
DEP_CPP_VALID=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_VALID=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xinclude.c
DEP_CPP_XINCL=\
	".\libxml\debugXML.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xinclude.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	".\libxml\xpointer.h"\
	
NODEP_CPP_XINCL=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xlink.c
DEP_CPP_XLINK=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_XLINK=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xmlIO.c
DEP_CPP_XMLIO=\
	".\libxml\catalog.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\nanoftp.h"\
	".\libxml\nanohttp.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_XMLIO=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xmlmemory.c
DEP_CPP_XMLME=\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	
NODEP_CPP_XMLME=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xpath.c
DEP_CPP_XPATH=\
	".\libxml\debugXML.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\triodef.h"\
	".\libxml\src\trionan.c"\
	".\libxml\src\trionan.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	".\libxml\xpathInternals.h"\
	".\libxml\xpointer.h"\
	
NODEP_CPP_XPATH=\
	".\libxml\src\config.h"\
	".\libxml\src\doc\doc_nan.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\libxml\src\xpointer.c
DEP_CPP_XPOIN=\
	".\libxml\debugXML.h"\
	".\libxml\encoding.h"\
	".\libxml\entities.h"\
	".\libxml\globals.h"\
	".\libxml\hash.h"\
	".\libxml\HTMLparser.h"\
	".\libxml\list.h"\
	".\libxml\parser.h"\
	".\libxml\parserInternals.h"\
	".\libxml\SAX.h"\
	".\libxml\src\libxml.h"\
	".\libxml\src\win32config.h"\
	".\libxml\src\wsockcompat.h"\
	".\libxml\threads.h"\
	".\libxml\tree.h"\
	".\libxml\uri.h"\
	".\libxml\valid.h"\
	".\libxml\xlink.h"\
	".\libxml\xmlautomata.h"\
	".\libxml\xmlerror.h"\
	".\libxml\xmlIO.h"\
	".\libxml\xmlmemory.h"\
	".\libxml\xmlregexp.h"\
	".\libxml\xmlversion.h"\
	".\libxml\xpath.h"\
	".\libxml\xpathInternals.h"\
	".\libxml\xpointer.h"\
	
NODEP_CPP_XPOIN=\
	".\libxml\src\config.h"\
	".\libxml\src\trio.h"\
	

!IF  "$(CFG)" == "Pa - Win32 Debug"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release"

# ADD CPP /nologo /GX

!ELSEIF  "$(CFG)" == "Pa - Win32 Debug_Unicode"

# ADD CPP /nologo /GX /GZ

!ELSEIF  "$(CFG)" == "Pa - Win32 Release_Unicode"

# ADD CPP /nologo /GX

!ENDIF 

# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
