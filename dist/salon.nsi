; -------------------------------
; project defines
!define PROJECTNAME "Салон"
!define PROJECTLATNAME "salon"
!define PROJECTVERSION ${version}
!define APPNAME "Салон" ; Имя приложения
!define APPNAMEANDVERSION "${APPNAME} ${version}" ; Имя приложения и версия (передается снаружи)
!define EXE_FILE "salon.exe"
!define COMPANYNAME "PDV"
!define DESCRIPTION "Салон"
!define INSTALL_REG "Software\${COMPANYNAME}\salon"
!define UNINSTALL_REG "Software\Microsoft\Windows\CurrentVersion\Uninstall\${COMPANYNAME} ${PROJECTNAME}"
!define ENVIRONMEN_REG "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"
!define CMAKEDIR "$PROGRAMFILES\CMake 2.8\share\cmake-2.8\Modules"

; included extentions
!include LogicLib.nsh
!include "MUI.nsh"
!include "MUI2.nsh"
!include "winmessages.nsh"

; -------------------------------
; installer options
RequestExecutionLevel admin ;Require admin rights on NT6+ (When UAC is turned on)
Name "${PROJECTNAME}"
InstallDir "$PROGRAMFILES\${COMPANYNAME}\${PROJECTLATNAME}"
CRCCheck On ;use checksum on installed files
OutFile "${PROJECTLATNAME}_setup.exe"
XPStyle On
   
; Modern interface settings
!define MUI_ICON "salon.ico" 
!define MUI_ABORTWARNING

;Опция установки ярлыка на рабочий стол
;!define MUI_FINISHPAGE_SHOWREADME ""
;!define MUI_FINISHPAGE_SHOWREADME_TEXT "Создать ярлык на рабочем столе"
;!define MUI_FINISHPAGE_SHOWREADME_FUNCTION FinishPageAction
;Опция запуска приложения
!define MUI_FINISHPAGE_RUN "$INSTDIR\${EXE_FILE}"
!define MUI_FINISHPAGE_RUN_TEXT "Запустить Салон"

; Modern interface pages
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

; Modern interface language
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_RESERVEFILE_LANGDLL

; -------------------------------
; common macros

!macro VerifyUserIsAdmin
UserInfo::GetAccountType
pop $0
${If} $0 != "admin" ;Require admin rights on NT4+
        messageBox mb_iconstop "Для установки требуются права администратора!"
        setErrorLevel 740 ;ERROR_ELEVATION_REQUIRED
        quit
${EndIf}
!macroend

; -------------------------------
; Installer
Function FinishPageAction
	;CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\${EXE_FILE}" ""
FunctionEnd

Function .onInit
	setShellVarContext all ; Исправляет удаление иконок для Windows Vista и выше
	!insertmacro VerifyUserIsAdmin
FunctionEnd
 
Section install

	; Ищем установленный Ingres, если его нет - ставим
	SetOutPath $TEMP
	
	setOutPath $INSTDIR
	file /r deploy\*
 
	; install uninstaller
	writeUninstaller "$INSTDIR\uninstall.exe"

	; Registry information of our program
	WriteRegStr HKLM "${INSTALL_REG}" "Version" "${PROJECTVERSION}"
	WriteRegStr HKLM "${INSTALL_REG}" "Directory" "$INSTDIR"
	
	; Registry environment variable
	WriteRegExpandStr HKLM "${ENVIRONMEN_REG}" "CHVA_DIR" "$INSTDIR"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
	
	; Registry information for add/remove programs
	WriteRegStr HKLM "${UNINSTALL_REG}" "DisplayName" "${DESCRIPTION}"
	WriteRegStr HKLM "${UNINSTALL_REG}" "UninstallString" "$\"$INSTDIR\uninstall.exe$\""
	WriteRegStr HKLM "${UNINSTALL_REG}" "QuietUninstallString" "$\"$INSTDIR\uninstall.exe$\" /S"
	WriteRegStr HKLM "${UNINSTALL_REG}" "InstallLocation" "$INSTDIR"
	WriteRegStr HKLM "${UNINSTALL_REG}" "Publisher" "${COMPANYNAME}"
	WriteRegStr HKLM "${UNINSTALL_REG}" "DisplayVersion" "${PROJECTVERSION}"
	WriteRegDWORD HKLM "${UNINSTALL_REG}" "NoModify" 1
	WriteRegDWORD HKLM "${UNINSTALL_REG}" "NoRepair" 1

	;Создаем ярлык на рабочем столе
	CreateShortCut "$DESKTOP\${APPNAME}.lnk" "$INSTDIR\${EXE_FILE}" "" "$INSTDIR\pics\salon.ico"

	; Создаем ярлыки в меню Пуск
	CreateDirectory "$SMPROGRAMS\${APPNAME}"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk" "$INSTDIR\${EXE_FILE}" "" "$INSTDIR\pics\salon.ico"
	CreateShortCut "$SMPROGRAMS\${APPNAME}\Удалить.lnk" "$INSTDIR\uninstall.exe"

SectionEnd
 
; -------------------------------
; Ininstaller
 
Function un.onInit
	SetShellVarContext all
	!insertmacro VerifyUserIsAdmin
FunctionEnd
 
Section uninstall
	; Remove files
	rmDir /r $INSTDIR
	
	; Remove uninstaller information from the registry
	DeleteRegKey HKLM "${UNINSTALL_REG}"
	DeleteRegKey HKLM "${INSTALL_REG}"
	DeleteRegValue HKLM "${ENVIRONMEN_REG}" "SALON_DIR"
	SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

	# second, remove the link from the start menu
	Delete "$DESKTOP\${APPNAME}.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\${APPNAME}.lnk"
	Delete "$SMPROGRAMS\${APPNAME}\Удалить.lnk"
    rmDir /r "$SMPROGRAMS\${APPNAME}"
SectionEnd
