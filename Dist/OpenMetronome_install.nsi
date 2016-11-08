# NullSoft Installer Script for OpenMetronome
;--------------------------------
  !include "MUI2.nsh"
  !include "FileFunc.nsh"  ; for GetSize and GetTime functions


!define VERSION "6.0"
!define BASENAME "OpenMetronome"
!define COMPANYNAME "BHBSoftware"
!define REGKEY "Software\${COMPANYNAME}\Open Metronome"
!define ARPKEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${BASENAME}"

; The name of the installer
Name "${BASENAME} v6.0"

; The file to write
OutFile "${BASENAME}_install.exe"

; The default installation directory
InstallDir $PROGRAMFILES\${BASENAME}

; Request application privileges for Windows Vista+
RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_WELCOME
  !insertmacro MUI_PAGE_LICENSE "COPYING.txt"
  !insertmacro MUI_PAGE_COMPONENTS
##  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES
  !insertmacro MUI_PAGE_FINISH
  
  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"


;--------------------------------

; The stuff to install
Section "WAV Version" WAV_install
  
  SectionIn RO  ; read-only, user cannot deselect this section in the components list
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put files there
  ;;File ${BASENAME}WAV.exe
  ;;File ${BASENAME}MIDI.exe
  File COPYING.txt
  ;;File /r Samples
  
  ; Create the uninstaller
  WriteUninstaller $INSTDIR\Uninstall_${BASENAME}.exe
  
  ; Add registry keys so user can uninstall from control panel's Add/Remove Programs
  WriteRegStr HKCU "${ARPKEY}" "DisplayName" "${BASENAME} ${VERSION}"
  WriteRegStr HKCU "${ARPKEY}" "UninstallString" "$\"$INSTDIR\Uninstall_${BASENAME}.exe$\""
  WriteRegStr HKCU "${ARPKEY}" "Publisher" "${COMPANYNAME}.com"
  WriteRegStr HKCU "${ARPKEY}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKCU "${ARPKEY}" "DisplayIcon" "$INSTDIR\${BASENAME}WAV.exe"
	
  ; Calculate estimated size
  ${GetSize} "$INSTDIR" "/S=0K" $0 $1 $2
  IntFmt $0 "0x%08X" $0
  WriteRegDWORD HKCU "${ARPKEY}" "EstimatedSize" "$0"
   
  ; Check if this is a fresh install (look for install date registry key)
  ReadRegStr $0 HKCU "${REGKEY}" "OriginalInstallDate"
  ; If NOT found, save the install date/time and load the presets
  IfErrors 0 NotFreshInstall
    ; ${GetTime} "" "L" $day $month $year $day_name $hours $minutes $seconds
    ${GetTime} "" "L" $0 $1 $2 $3 $4 $5 $6
    WriteRegStr HKCU "${REGKEY}" "OriginalInstallDate" "$2-$1-$0 $4:$5:$6"
    call LoadPresets
	
 NotFreshInstall:

  ; Install the required files
  File ${BASENAME}WAV.exe
  File /r Samples
  
  ; Install Start Menu shortcut
  CreateDirectory "$SMPROGRAMS\${COMPANYNAME}"
  CreateShortCut "$SMPROGRAMS\${COMPANYNAME}\${BASENAME}WAV.lnk" $INSTDIR\${BASENAME}WAV.exe
 
SectionEnd

Section "MIDI Version" MIDI_install ; this is an optional install

  File ${BASENAME}MIDI.exe
  CreateDirectory "$SMPROGRAMS\${COMPANYNAME}"
  CreateShortCut "$SMPROGRAMS\${COMPANYNAME}\${BASENAME}WAV.lnk" $INSTDIR\${BASENAME}WAV.exe

SectionEnd

Section "Create Desktop Shortcuts" DS_install ; also optional

  IfFileExists $INSTDIR\${BASENAME}WAV.exe 0 +2
  CreateShortCut $DESKTOP\${BASENAME}WAV.lnk $INSTDIR\${BASENAME}WAV.exe
  IfFileExists $INSTDIR\${BASENAME}MIDI.exe 0 +2
  CreateShortCut $DESKTOP\${BASENAME}MIDI.lnk $INSTDIR\${BASENAME}MIDI.exe

SectionEnd

LangString DESC_WAV_install ${LANG_ENGLISH} "Installs the WAV version of OpenMetronome along with the required sound sample library."
LangString DESC_MIDI_install ${LANG_ENGLISH} "Installs the MIDI version of OpenMetronome (standalone executable)."
LangString DESC_DS_install ${LANG_ENGLISH} "Create desktop shortcuts to the installed executables."

!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${WAV_install} $(DESC_WAV_install)
  !insertmacro MUI_DESCRIPTION_TEXT ${MIDI_install} $(DESC_MIDI_install)
  !insertmacro MUI_DESCRIPTION_TEXT ${DS_install} $(DESC_DS_install)
!insertmacro MUI_FUNCTION_DESCRIPTION_END
  
Function LoadPresets ; "Initialize Sample Presets"
  !define PRESET "${REGKEY}\Busy"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[4*]5(53)3343(39)(973)(75)(7345)(7345)(35)43(38)3"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\Pop"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[2*](35)3(34)33(35)(34)3"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\Rock"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[2*](35)0(34)0(35)5(34)0"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\Rock2"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[2*](365)030(34)030(35)4(35)0(34)030"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\Swing"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[3*](35)05(34)05"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\TomCraze"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000006
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[4*](59)9(37)9(5479)93(79)(95)(97)39(9745)9(38)9"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001

  !undef PRESET
  !define PRESET "${REGKEY}\WalkThisWay"
  WriteRegDWORD HKCU "${PRESET}" "BPMinute" 0x00000078
  WriteRegDWORD HKCU "${PRESET}" "BPMeasure" 0x00000004
  WriteRegDWORD HKCU "${PRESET}" "Metronome Style" 0x00000002
  WriteRegDWORD HKCU "${PRESET}" "Sound 0" 0x0000002a
  WriteRegDWORD HKCU "${PRESET}" "Volume 0" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 0" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 1" 0x00000029
  WriteRegDWORD HKCU "${PRESET}" "Volume 1" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 1" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 2" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 2" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 2" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 3" 0x00000003
  WriteRegDWORD HKCU "${PRESET}" "Volume 3" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 3" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 4" 0x00000001
  WriteRegDWORD HKCU "${PRESET}" "Volume 4" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 4" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 5" 0x00000014
  WriteRegDWORD HKCU "${PRESET}" "Volume 5" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 5" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 6" 0x00000007
  WriteRegDWORD HKCU "${PRESET}" "Volume 6" 0x0000001c
  WriteRegDWORD HKCU "${PRESET}" "Blinker 6" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 7" 0x0000000c
  WriteRegDWORD HKCU "${PRESET}" "Volume 7" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 7" 0x00000008
  WriteRegDWORD HKCU "${PRESET}" "Sound 8" 0x0000000f
  WriteRegDWORD HKCU "${PRESET}" "Volume 8" 0x0000007f
  WriteRegDWORD HKCU "${PRESET}" "Blinker 8" 0x00000008
  WriteRegStr   HKCU "${PRESET}" "Custom Measure" "[4*](56)070 (34)075 (35)0(75)0 (34)070"
  WriteRegDWORD HKCU "${PRESET}" "Blinking" 0x00000001
  
FunctionEnd
; 
Section "Uninstall" 
  
  ; Remove files
  Delete $INSTDIR\Uninstall_${BASENAME}.exe 
  Delete $INSTDIR\${BASENAME}WAV.exe
  Delete $INSTDIR\${BASENAME}MIDI.exe
  Delete $INSTDIR\COPYING.txt
  RMDir /r $INSTDIR\Samples
  RMDir $INSTDIR
  
  ; Remove desktop shortcut
  Delete $DESKTOP\${BASENAME}WAV.lnk
  Delete $DESKTOP\${BASENAME}MIDI.lnk
  Delete "$SMPROGRAMS\${COMPANYNAME}\${BASENAME}WAV.lnk"
  Delete "$SMPROGRAMS\${COMPANYNAME}\${BASENAME}MIDI.lnk"
  RMDir "$SMPROGRAMS\${COMPANYNAME}" ; only works if empty

  ; Remove registry branch where OpenMetronome keeps its settings
  DeleteRegKey HKCU "${REGKEY}"

  ; Remove registry entry for the Add/Remove Programs list
  DeleteRegKey HKCU "${ARPKEY}"
  
SectionEnd ; end of uninstall section
