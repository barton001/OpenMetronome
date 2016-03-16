<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: weirdmet_wce - Win32 (WCE ARMV4) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\Users\Mark\AppData\Local\Temp\RSP3771.tmp" with contents
[
/nologo /W3 /GR /GX /Oxt /I "." /I "NewCore" /I "Support" /D _WIN32_WCE=420 /D "WIN32_PLATFORM_PSPC=400" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4Rel/weirdmet_wce.pch" /YX /Fo"ARMV4Rel/" /MC /c 
"C:\ZZ_Stuff\codeBiscuit\Open_Metronome_Source_Code_for_Programmers\src\metronomeDlg.cpp"
]
Creating command line "clarm.exe @C:\Users\Mark\AppData\Local\Temp\RSP3771.tmp" 
Creating temporary file "C:\Users\Mark\AppData\Local\Temp\RSP3782.tmp" with contents
[
aygshell.lib commctrl.lib coredll.lib rtti\Arm_V4\ccrtrtti.lib Mmtimer.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"ARMV4Rel/oMet_WCE.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"../Dist/oMet_WCE.exe" /subsystem:windowsce,4.20 /align:"4096" /MACHINE:ARM 
.\ARMV4Rel\AboutDlg.obj
.\ARMV4Rel\EditboxCatchKeypress.obj
.\ARMV4Rel\HotKeyDlg.obj
.\ARMV4Rel\metronome.obj
.\ARMV4Rel\StdAfx.obj
.\ARMV4Rel\BscBblDlg.obj
.\ARMV4Rel\BscSubclassWnd.obj
.\ARMV4Rel\BscToolTip.obj
.\ARMV4Rel\cBscFileVersion.obj
.\ARMV4Rel\BeatBox_WAV.obj
.\ARMV4Rel\MetBlinker.obj
.\ARMV4Rel\general_midi.obj
.\ARMV4Rel\group_beat.obj
.\ARMV4Rel\GroupEdit.obj
.\ARMV4Rel\WaveFile.obj
.\ARMV4Rel\metronomeDlg.obj
.\ARMV4Rel\metronomeDlg_RegPersist.obj
.\ARMV4Rel\metronome.res
]
Creating command line "link.exe @C:\Users\Mark\AppData\Local\Temp\RSP3782.tmp"
<h3>Output Window</h3>
Compiling...
metronomeDlg.cpp
Linking...





<h3>Results</h3>
oMet_WCE.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
