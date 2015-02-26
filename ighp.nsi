;======================================================
; Include

!include "MUI2.nsh"
!include LogicLib.nsh
!include x64.nsh

;======================================================
;Version Information

!define UNINST_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp"

!define APP_NAME "iTunes Global Hotkeys Plugin"
!define VERSION "0.1.1"
;!define IS64BIT

VIProductVersion "${VERSION}.0"
VIAddVersionKey /LANG=${LANG_ENGLISH} "ProductName" "${APP_NAME}"
VIAddVersionKey /LANG=${LANG_ENGLISH} "LegalCopyright" "Copyright (c) 2015 Stefan Cosma, pezcode"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileDescription" "${APP_NAME} Setup"
VIAddVersionKey /LANG=${LANG_ENGLISH} "FileVersion" "${VERSION}.0"

;======================================================
; Installer Information

Name "${APP_NAME}"
!ifdef IS64BIT
  OutFile "ighp_${VERSION}_x64.exe"
  InstallDir "$PROGRAMFILES64\iTunes\Plug-ins"
!else
  OutFile "ighp_${VERSION}.exe"
  InstallDir "$PROGRAMFILES\iTunes\Plug-ins"
!endif

;======================================================
; Request application privileges for Windows Vista
RequestExecutionLevel admin

;======================================================
; Modern Interface Configuration

!define MUI_ABORTWARNING
!define MUI_FINISHPAGE
!define MUI_FINISHPAGE_TEXT "Thank you for installing ${APP_NAME}. You need to restart iTunes before using the plugin."

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

Section "64BitCheck"

  !ifdef IS64BIT
    ${IfNot} ${RunningX64}
      MessageBox MB_OK|MB_ICONEXCLAMATION "You are trying to install a 64-bit version of ${APP_NAME} on a 32-bit Windows.\r$\n\
                                           Please use the appropriate 32-bit setup."
      Quit
    ${EndIf}
  !endif

SectionEnd

Section "VersionCheck"

  Push $0

  ReadRegStr $0 HKLM "${UNINST_PATH}" "DisplayVersion " ; old (pre 0.1.0) versions wrote the wrong reg key
  StrCmp $0 "" check_new

  ; they use 2 dlls with other names, so we can't replace them
  ; enforce uninstall to prevent plugin from being loaded twice
  goto abort_version

check_new:

  ReadRegStr $0 HKLM "${UNINST_PATH}" "DisplayVersion"
  StrCmp $0 "" end ; not found or error

  ; specific version checks here
  ;StrCmp $0 "0.0.4" abort_version
  ;StrCmp $0 "0.0.3" abort_version
  ;StrCmp $0 "0.0.2" abort_version
  ;StrCmp $0 "0.0.1" abort_version

  goto end

abort_version:

  MessageBox MB_OK|MB_ICONEXCLAMATION "Found an incompatible version of ${APP_NAME}$\r$\n\
                                       Please remove it before installing a new version."
  Quit

end:

  Pop $0

SectionEnd

Section "Plugin"

  SetOutPath $INSTDIR

  !ifdef IS64BIT
    File "x64\release\GlobalHotkeys.dll"
  !else
    File "release\GlobalHotkeys.dll"
  !endif
  File "/oname=Global Hotkeys License.txt" "License.txt"
  File "/oname=Global Hotkeys Readme.txt" "Readme.txt"

  ;Create uninstaller

  WriteUninstaller "$INSTDIR\UninstallGlobalHotkeys.exe"
  WriteRegStr HKLM "${UNINST_PATH}" "DisplayName" "iTunes Global Hotkeys Plugin"
  WriteRegStr HKLM "${UNINST_PATH}" "UninstallString" "$\"$INSTDIR\UninstallGlobalHotkeys.exe$\""
  WriteRegStr HKLM "${UNINST_PATH}" "Publisher" "pezcode"
  WriteRegStr HKLM "${UNINST_PATH}" "URLInfoAbout" "http://pezcode.github.com/ighp/"
  WriteRegStr HKLM "${UNINST_PATH}" "DisplayVersion" "${VERSION}"
  WriteRegDWORD HKLM "${UNINST_PATH}" "NoModify" 1
  WriteRegDWORD HKLM "${UNINST_PATH}" "NoRepair" 1

SectionEnd

Section "Uninstall"

  ;Delete plugin files
  Delete "$INSTDIR\GlobalHotkeys.dll"
  Delete "$INSTDIR\Global Hotkeys License.txt"
  Delete "$INSTDIR\Global Hotkeys Readme.txt"
  Delete "$INSTDIR\UninstallGlobalHotkeys.exe"

  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ighp"

SectionEnd
