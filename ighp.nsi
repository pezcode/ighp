;======================================================
; Include
 
!include "MUI2.nsh"

;======================================================
; Installer Information
 
Name "iTunes Global Hotkeys Plugin v0.1.0"
OutFile "ighp_0.1.0.exe"
InstallDir "$PROGRAMFILES\iTunes\Plug-ins"

;======================================================
; Modern Interface Configuration
 
!define MUI_ABORTWARNING
!define MUI_FINISHPAGE
!define MUI_FINISHPAGE_TEXT "Thank you for installing iTunes Global Hotkeys Plugin. You need to restart iTunes before using the plugin"

;======================================================
; Pages
 
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_LICENSE License.txt
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

;======================================================
; Languages
 
!insertmacro MUI_LANGUAGE "English"

;======================================================
; Sections

Section "Plugin" pluginSection
  ; Version checking logic
  ; TODO: for the next version

  SetOutPath $INSTDIR
  
  File "release\GlobalHotkeys.dll"
  File "/oname=Global Hotkeys License.txt" "License.txt"
  File "/oname=Global Hotkeys Readme.txt" "Readme.txt" 
  ;Create uninstaller
  WriteUninstaller "$INSTDIR\UninstallGlobalHotkeys.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                   "DisplayName" "iTunes Global Hotkeys Plugin"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                   "UninstallString" "$\"$INSTDIR\UninstallGlobalHotkeys.exe$\""
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                   "Publisher" "Stefan Cosma"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                   "URLInfoAbout" "http://github.com/pezcode/ighp"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                   "DisplayVersion " "0.1.0" 
  WriteRegDWORD  HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                      "NoModify" 1
  WriteRegDWORD  HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp" \
                      "NoRepair" 1

SectionEnd

Section "Uninstall"

  ;Delete plugin files
  Delete "$INSTDIR\GlobalHotkeys.dll"
  Delete "$INSTDIR\Global Hotkeys License.txt"
  Delete "$INSTDIR\Global Hotkeys Readme.txt"
  Delete "$INSTDIR\UninstallGlobalHotkeys.exe"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp"

SectionEnd
