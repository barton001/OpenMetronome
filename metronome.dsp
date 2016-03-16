# Microsoft Developer Studio Project File - Name="metronome" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=metronome - Win32 WAV Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "metronome.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "metronome.mak" CFG="metronome - Win32 WAV Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "metronome - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "metronome - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "metronome - Win32 WAV Release" (based on "Win32 (x86) Application")
!MESSAGE "metronome - Win32 WAV Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "metronome - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "Support" /I "NewCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "USE_WEIRD_MIDI" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB comdlg32.lib /nologo /subsystem:windows /machine:I386 /out:"../Dist/oMetronome_MIDI.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "metronome - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "Support" /I "NewCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "USE_WEIRD_MIDI" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB comdlg32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Dist/d_oMetronome_MIDI.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "metronome - Win32 WAV Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "metronome___Win32_WAV_Release0"
# PROP BASE Intermediate_Dir "metronome___Win32_WAV_Release0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "metronome___Win32_WAV_Release"
# PROP Intermediate_Dir "metronome___Win32_WAV_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GR /GX /O2 /I "Support" /I "NewCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "USE_WEIRD_MIDI" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /O2 /I "Support" /I "NewCore" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB /nologo /subsystem:windows /machine:I386 /out:"../Dist/Weird Metronome.exe"
# SUBTRACT BASE LINK32 /incremental:yes
# ADD LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB comdlg32.lib /nologo /subsystem:windows /machine:I386 /out:"../Dist/oMetronome_WAV.exe"
# SUBTRACT LINK32 /incremental:yes

!ELSEIF  "$(CFG)" == "metronome - Win32 WAV Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "metronome___Win32_WAV_Debug0"
# PROP BASE Intermediate_Dir "metronome___Win32_WAV_Debug0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "metronome___Win32_WAV_Debug"
# PROP Intermediate_Dir "metronome___Win32_WAV_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GR /GX /ZI /Od /I "Support" /I "NewCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "USE_WEIRD_MIDI" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GR /GX /ZI /Od /I "Support" /I "NewCore" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x809 /d "_DEBUG"
# ADD RSC /l 0x809 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB /nologo /subsystem:windows /debug /machine:I386 /out:"../Dist/d_Weird Metronome.exe" /pdbtype:sept
# ADD LINK32 user32.lib gdi32.lib advapi32.lib shell32.lib winmm.lib version.lib COMCTL32.LIB comdlg32.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Dist/d_oMetronome_WAV.exe" /pdbtype:sept
# SUBTRACT LINK32 /profile

!ENDIF 

# Begin Target

# Name "metronome - Win32 Release"
# Name "metronome - Win32 Debug"
# Name "metronome - Win32 WAV Release"
# Name "metronome - Win32 WAV Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\metronome.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.H
# End Source File
# Begin Source File

SOURCE=.\NewCore\BeatBox_MID.h
# End Source File
# Begin Source File

SOURCE=.\NewCore\BeatBox_WAV.h
# End Source File
# Begin Source File

SOURCE=.\Support\BscBblDlg.h
# End Source File
# Begin Source File

SOURCE=.\Support\BscSubclassWnd.h
# End Source File
# Begin Source File

SOURCE=.\Support\BscToolTip.h
# End Source File
# Begin Source File

SOURCE=.\Support\cBscFileVersion.h
# End Source File
# Begin Source File

SOURCE=.\EditboxCatchKeypress.h
# End Source File
# Begin Source File

SOURCE=.\general_midi.h
# End Source File
# Begin Source File

SOURCE=.\group_beat.h
# End Source File
# Begin Source File

SOURCE=.\GroupEdit.h
# End Source File
# Begin Source File

SOURCE=.\HotKeyDlg.H
# End Source File
# Begin Source File

SOURCE=.\NewCore\IBeatBox.h
# End Source File
# Begin Source File

SOURCE=.\MetBlinker.h
# End Source File
# Begin Source File

SOURCE=.\MetDefs.h
# End Source File
# Begin Source File

SOURCE=.\metronome.h
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg.h
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg_RegPersist.H
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg_RegPersist_DragSize.H
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\NewCore\WaveFile.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\metronome.ico
# End Source File
# Begin Source File

SOURCE=.\metronome.rc
# End Source File
# Begin Source File

SOURCE=.\res\metronome.rc2
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "NewCore"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\NewCore\BeatBox_MID.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCore\BeatBox_WAV.cpp
# End Source File
# Begin Source File

SOURCE=.\MetBlinker.cpp
# End Source File
# Begin Source File

SOURCE=.\NewCore\WaveFile.cpp
# End Source File
# End Group
# Begin Group "Support"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Support\BscBblDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Support\BscSubclassWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Support\BscToolTip.cpp
# End Source File
# Begin Source File

SOURCE=.\Support\cBscFileVersion.cpp
# End Source File
# Begin Source File

SOURCE=.\EditboxCatchKeypress.cpp
# End Source File
# End Group
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\general_midi.cpp
# End Source File
# Begin Source File

SOURCE=.\group_beat.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupEdit.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\HotKeyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg_RegPersist.cpp
# End Source File
# Begin Source File

SOURCE=.\metronomeDlg_RegPersist_DragSize.cpp
# End Source File
# End Target
# End Project
