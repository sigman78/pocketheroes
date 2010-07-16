; must be called like that
; makensis.exe /DCAB=PocketHeroes.SA2K3_ENG.v1.02.cab /DPRODUCT_VER=1.02 /DPRODUCT_LANG=eng PocketHeroes.nsi

!define PRODUCT_NAME 	"Pocket Heroes"
!define PRODUCT_NSNAME  "PocketHeroes"

;!define PRODUCT_VER 	"1.02"
;!define PRODUCT_LANG 	"eng"
!define PRODUCT_OUTDIR 	"out"

;externals
!ifndef CAB
!error "No source cab specified (should not contain path)"
!endif
!ifndef PRODUCT_VER
!error "No product version str given (1.02)"
!endif
!ifndef PRODUCT_LANG
!error "No product language is given (eng,rus)"
!endif

; derivatives
!define PRODUCT_INSTNAME "${PRODUCT_NSNAME}_${PRODUCT_VER}_${PRODUCT_LANG}.exe"
;!define CAB_FILENAME 

Name "${PRODUCT_NAME} ${PRODUCT_VER}"

OutFile "${PRODUCT_OUTDIR}\${PRODUCT_INSTNAME}"

InstallDir "$TEMP\${PRODUCT_NSNAME}"

SetCompressor /FINAL lzma
 
;Var to hold the location of CeAppMgr.exe
var CeAppMgrLocation
 
Section PathObtain
	;It can be obtained from the registry at this location always
	ReadRegStr $CeAppMgrLocation HKLM \
		"SOFTWARE\Microsoft\Windows\CurrentVersion\App Paths\CEAPPMGR.EXE" ""
SectionEnd

Section PocketHeroes
	SetOutPath "$INSTDIR"
	File ${PRODUCT_OUTDIR}\${CAB}
	; we are going to create ini file 

	; one-time initialization needed for InstallCAB subroutine 
	ReadRegStr $1 HKEY_LOCAL_MACHINE "software\Microsoft\Windows\CurrentVersion\App Paths\CEAppMgr.exe" "" 
	IfErrors Error
	Goto End
	Error:
	MessageBox MB_OK|MB_ICONEXCLAMATION \
	"Unable to find Application Manager for PocketPC applications. Please install ActiveSync and reinstall YourApp."
	End:
    StrCpy $0 "$INSTDIR\${PRODUCT_NSNAME}.ini"
	Call InstallCAB
	RMDir /r  $INSTDIR
SectionEnd
 
; Installs a PocketPC cab-application
; It expects $0 to contain the absolute location of the ini file 
; to be installed.
Function InstallCAB
	; File PocketHeroes.ini
	WriteINIStr $0 "CEAppManager" "Component" "${PRODUCT_NSNAME}"
	WriteINIStr $0 "CEAppManager" "Version" "${PRODUCT_VER}"
	WriteINIStr $0 "${PRODUCT_NSNAME}" "Description" "${PRODUCT_NAME} ${PRODUCT_VER}"
	WriteINIStr $0 "${PRODUCT_NSNAME}" "CabFiles" "${CAB}"
	FlushINI $0

  ExecWait '"$1" "$0"'
FunctionEnd
