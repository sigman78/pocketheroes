# Microsoft Developer Studio Project File - Name="HMM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=HMM - Win32 Debug_Unicode
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "HMM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "HMM.mak" CFG="HMM - Win32 Debug_Unicode"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "HMM - Win32 Debug_Unicode" (based on "Win32 (x86) Application")
!MESSAGE "HMM - Win32 Release_Unicode" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "HMM - Win32 Debug_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_Unicode"
# PROP BASE Intermediate_Dir "Debug_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\obj\HMM_w32.wdbg"
# PROP Intermediate_Dir "..\obj\HMM_w32.wdbg"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\common" /I "..\..\gxlib" /I "." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "OS_WIN32" /D "_HMM_GAME_" /D "DEV_VER" /D "GAME_LANG_ENG" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "_DEBUG"
# ADD RSC /l 0x419 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\bin\Game\HMM.wdbg.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "HMM - Win32 Release_Unicode"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release_Unicode"
# PROP BASE Intermediate_Dir "Release_Unicode"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\obj\HMM_w32.wrel"
# PROP Intermediate_Dir "..\obj\HMM_w32.wrel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\common" /I "..\..\gxlib" /I "." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_UNICODE" /D "UNICODE" /D "OS_WIN32" /D "_HMM_GAME_" /D "DEV_VER" /D "GAME_LANG_ENG" /FAs /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x419 /d "NDEBUG"
# ADD RSC /l 0x419 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"..\bin\Game\HMM.exe"
# SUBTRACT LINK32 /debug

!ENDIF 

# Begin Target

# Name "HMM - Win32 Debug_Unicode"
# Name "HMM - Win32 Release_Unicode"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "Interface"

# PROP Default_Filter ""
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Dlg_ArmyRoom.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ArmyRoom.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ArtInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ArtInfo.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleHero.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleHero.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleLog.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleLog.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleResult.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_BattleResult.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CapArt.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CapArt.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CastSpell.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CastSpell.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Chest.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Chest.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreatInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_CreatInfo.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_FightGuard.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_FightGuard.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_HallOfFame.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_HallOfFame.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_KingdomOverview.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_KingdomOverview.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_LeaveGuards.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_LeaveGuards.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Level.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Level.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Message.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Message.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_MLInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_MLInfo.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_NewWeek.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_NewWeek.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Other.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Other.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_PuzzleMap.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_PuzzleMap.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Recruit.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Recruit.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Rewards.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Rewards.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_RiseSkeletons.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_RiseSkeletons.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Save.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Save.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ScenList.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ScenList.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ScenProps.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ScenProps.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_Settings.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SkillInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SkillInfo.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpellInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SpellInfo.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SplitCreatures.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SplitCreatures.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_SurrenderCond.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_SurrenderCond.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_TimeEvent.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_TimeEvent.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_TownList.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_TownList.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_WitchHut.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_WitchHut.h
# End Source File
# Begin Source File

SOURCE=.\GameMenuDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GameMenuDlg.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ArmyList.cpp
# End Source File
# Begin Source File

SOURCE=.\ArmyList.h
# End Source File
# Begin Source File

SOURCE=.\ArtifactList.cpp
# End Source File
# Begin Source File

SOURCE=.\ArtifactList.h
# End Source File
# Begin Source File

SOURCE=.\CommonControls.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonControls.h
# End Source File
# Begin Source File

SOURCE=.\CommonDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\CommonDialogs.h
# End Source File
# Begin Source File

SOURCE=.\SpellListView.cpp
# End Source File
# Begin Source File

SOURCE=.\SpellListView.h
# End Source File
# Begin Source File

SOURCE=.\ViewDefs.h
# End Source File
# End Group
# Begin Group "GameCore"

# PROP Default_Filter ""
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AvatarMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\AvatarMgr.h
# End Source File
# Begin Source File

SOURCE=.\GfxManager.cpp
# End Source File
# Begin Source File

SOURCE=.\GfxManager.h
# End Source File
# Begin Source File

SOURCE=.\ItemMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ItemMgr.h
# End Source File
# Begin Source File

SOURCE=.\SfxManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SfxManager.h
# End Source File
# Begin Source File

SOURCE=.\TextManager.cpp
# End Source File
# Begin Source File

SOURCE=.\TextManager.h
# End Source File
# End Group
# Begin Group "Items"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Artifacts.cpp
# End Source File
# Begin Source File

SOURCE=.\Artifacts.h
# End Source File
# Begin Source File

SOURCE=.\CastleCnsts.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleCnsts.h
# End Source File
# Begin Source File

SOURCE=.\Castles.cpp
# End Source File
# Begin Source File

SOURCE=.\Castles.h
# End Source File
# Begin Source File

SOURCE=.\Constructions.cpp
# End Source File
# Begin Source File

SOURCE=.\Constructions.h
# End Source File
# Begin Source File

SOURCE=.\hero.cpp
# End Source File
# Begin Source File

SOURCE=.\hero.h
# End Source File
# Begin Source File

SOURCE=.\Magic.cpp
# End Source File
# Begin Source File

SOURCE=.\Magic.h
# End Source File
# Begin Source File

SOURCE=.\objects.cpp
# End Source File
# Begin Source File

SOURCE=.\objects.h
# End Source File
# Begin Source File

SOURCE=.\player.cpp
# End Source File
# Begin Source File

SOURCE=.\player.h
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ai.cpp
# End Source File
# Begin Source File

SOURCE=.\ai.dmap.cpp
# End Source File
# Begin Source File

SOURCE=.\ai.explore.cpp
# End Source File
# Begin Source File

SOURCE=.\ai.explore.h
# End Source File
# Begin Source File

SOURCE=.\ai.h
# End Source File
# Begin Source File

SOURCE=.\ai.scanner.cpp
# End Source File
# Begin Source File

SOURCE=.\ai.scanner.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\dbg.cpp
# End Source File
# Begin Source File

SOURCE=.\dbg.h
# End Source File
# Begin Source File

SOURCE=.\fogmap.cpp
# End Source File
# Begin Source File

SOURCE=.\fogmap.h
# End Source File
# Begin Source File

SOURCE=.\GameWorld.cpp
# End Source File
# Begin Source File

SOURCE=.\GameWorld.h
# End Source File
# Begin Source File

SOURCE=.\GfxHlp.cpp
# End Source File
# Begin Source File

SOURCE=.\GfxHlp.h
# End Source File
# Begin Source File

SOURCE=.\helpers.cpp
# End Source File
# Begin Source File

SOURCE=.\helpers.h
# End Source File
# Begin Source File

SOURCE=.\Map.cpp
# End Source File
# Begin Source File

SOURCE=.\Map.h
# End Source File
# Begin Source File

SOURCE=.\PathFinding.cpp
# End Source File
# Begin Source File

SOURCE=.\PathFinding.h
# End Source File
# Begin Source File

SOURCE=.\SoundMap.cpp
# End Source File
# Begin Source File

SOURCE=.\SoundMap.h
# End Source File
# Begin Source File

SOURCE=.\TextComposer.cpp
# End Source File
# Begin Source File

SOURCE=.\TextComposer.h
# End Source File
# End Group
# Begin Group "Overland"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AniObject.cpp
# End Source File
# Begin Source File

SOURCE=.\AniObject.h
# End Source File
# Begin Source File

SOURCE=.\Composer.cpp
# End Source File
# Begin Source File

SOURCE=.\Composer.h
# End Source File
# Begin Source File

SOURCE=.\OverlandView.cpp
# End Source File
# Begin Source File

SOURCE=.\OverlandView.h
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupDlg.h
# End Source File
# End Group
# Begin Group "Castle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CastleView.cpp
# End Source File
# Begin Source File

SOURCE=.\CastleView.h
# End Source File
# Begin Source File

SOURCE=.\Ctl_CnstPage.h
# End Source File
# Begin Source File

SOURCE=.\Ctl_MarketPage.h
# End Source File
# Begin Source File

SOURCE=.\Ctl_MGuildPage.h
# End Source File
# Begin Source File

SOURCE=.\Ctl_RecrPage.h
# End Source File
# Begin Source File

SOURCE=.\Ctl_TavernPage.h
# End Source File
# End Group
# Begin Group "Hero"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Hero_Army.h
# End Source File
# Begin Source File

SOURCE=.\Hero_Artifacts.h
# End Source File
# Begin Source File

SOURCE=.\Hero_Skills.h
# End Source File
# Begin Source File

SOURCE=.\Hero_Spells.h
# End Source File
# Begin Source File

SOURCE=.\HeroView.cpp
# End Source File
# Begin Source File

SOURCE=.\HeroView.h
# End Source File
# Begin Source File

SOURCE=.\MeetView.cpp
# End Source File
# Begin Source File

SOURCE=.\MeetView.h
# End Source File
# End Group
# Begin Group "Minimap"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MinimapView.cpp
# End Source File
# Begin Source File

SOURCE=.\MinimapView.h
# End Source File
# End Group
# Begin Group "Battle"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AutoBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBattle.h
# End Source File
# Begin Source File

SOURCE=.\BattleAI.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleAI.h
# End Source File
# Begin Source File

SOURCE=.\BattleEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleEngine.h
# End Source File
# Begin Source File

SOURCE=.\BattleFort.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleFort.h
# End Source File
# Begin Source File

SOURCE=.\BattleInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleInfo.h
# End Source File
# Begin Source File

SOURCE=.\BattleObstacles.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleObstacles.h
# End Source File
# Begin Source File

SOURCE=.\BattlePF.cpp
# End Source File
# Begin Source File

SOURCE=.\BattlePF.h
# End Source File
# Begin Source File

SOURCE=.\BattleUnit.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleUnit.h
# End Source File
# Begin Source File

SOURCE=.\BattleView.cpp
# End Source File
# Begin Source File

SOURCE=.\BattleView.h
# End Source File
# Begin Source File

SOURCE=.\Dlg_ChooseBattleGroup.cpp
# End Source File
# Begin Source File

SOURCE=.\Dlg_ChooseBattleGroup.h
# End Source File
# Begin Source File

SOURCE=.\iBattleGroupBtn.cpp
# End Source File
# Begin Source File

SOURCE=.\iBattleGroupBtn.h
# End Source File
# Begin Source File

SOURCE=.\InteractBattle.cpp
# End Source File
# Begin Source File

SOURCE=.\InteractBattle.h
# End Source File
# End Group
# Begin Group "Game menu"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Credits.cpp
# End Source File
# Begin Source File

SOURCE=.\Credits.h
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\IntroDlg.h
# End Source File
# Begin Source File

SOURCE=.\MenuView.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuView.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\hmm.cpp
# End Source File
# Begin Source File

SOURCE=.\hmm.h
# End Source File
# Begin Source File

SOURCE=.\Settings.cpp
# End Source File
# Begin Source File

SOURCE=.\Settings.h
# End Source File
# Begin Source File

SOURCE=.\stdafx.cpp
# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# Begin Group "GXLIB"

# PROP Default_Filter ""
# Begin Group "GUI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\gxlib\gxl.dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.dialog.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.popupview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.popupview.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.stdctrl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.stdctrl.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.topmostview.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.topmostview.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.view.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.view.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.viewmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.viewmgr.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\GXLIB\gxl.application.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.application.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.audio.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.audio.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.cmdline.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.cmdline.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.common.def.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.common.def.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.common.tpl.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.config.file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.config.file.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.ctr.array.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.ctr.cbuff.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.ctr.farray.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.ctr.list.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.ctr.map.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.debug.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dib.cache.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dib.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.dib.fx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dib.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dib.loader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dib.transform.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dibfont.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.dibfont.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.display.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.display.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.file.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.file.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.fixpoint.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.font.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.inc.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.input.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.input.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.logmgr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.logmgr.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.math.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.memdbg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.memdc.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.memory.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.metrics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.metrics.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.pair.h
# End Source File
# Begin Source File

SOURCE=..\..\gxlib\gxl.rnd.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.smartptr.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.string.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.string.std.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.timer.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.vector.h
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\GXLIB\gxl.window.h
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

SOURCE=..\Common\xau\xau.math.h
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

SOURCE=..\Common\cnst_gfx.h
# End Source File
# Begin Source File

SOURCE=..\Common\cnst_text.h
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

SOURCE=..\Common\lang_eng.cpp
# End Source File
# Begin Source File

SOURCE=..\Common\rtti.h
# End Source File
# Begin Source File

SOURCE=..\Common\serialize.h
# End Source File
# Begin Source File

SOURCE=..\Common\SortArray.h
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
# End Group
# Begin Source File

SOURCE=..\features.old.txt
# End Source File
# Begin Source File

SOURCE=..\features.txt
# End Source File
# Begin Source File

SOURCE=..\readme.txt
# End Source File
# End Target
# End Project
