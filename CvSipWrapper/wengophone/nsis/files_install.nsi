Delete "$INSTDIR\${BINARY_NAME}.exe"
Delete "$INSTDIR\*.dll"
Delete "$INSTDIR\styles\*.dll"
Delete "$INSTDIR\sounds\tones\default\*"
Delete "$INSTDIR\sounds\tones\jungle\*"
Delete "$INSTDIR\sounds\tones\*"
Delete "$INSTDIR\sounds\*"
Delete "$INSTDIR\lang\*"
Delete "$INSTDIR\webpages\*"
Delete "$INSTDIR\imageformats\*"

SetOutPath "$INSTDIR"
File "${BUILD_DIR}\${BINARY_NAME}.exe"
CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\${BINARY_NAME}.exe"
CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\${BINARY_NAME}.exe"

File "${BUILD_DIR}\COPYING"
File "${BUILD_DIR}\AUTHORS"

/** .dll files. */
File "${BUILD_DIR}\*.dll"
File "dll\dbghelp.dll"

/** .exe files. */
File "${BUILD_DIR}\*.exe"

/** .pdb files. */
!ifdef WITH_PDB
	File "${BUILD_DIR}\*.pdb"
!endif

/** Gaim plugins. */
;SetOutPath "$INSTDIR\plugins\"
;File "${BUILD_DIR}\plugins\*.dll"

/** Sounds. */
SetOutPath "$INSTDIR\sounds\"
File "${BUILD_DIR}\sounds\*"
SetOutPath "$INSTDIR\sounds\tones\default\"
File "${BUILD_DIR}\sounds\tones\default\*"
SetOutPath "$INSTDIR\sounds\tones\jungle\"
File "${BUILD_DIR}\sounds\tones\jungle\*"

/** Images. */
SetOutPath "$INSTDIR\pics\"
File /r "${BUILD_DIR}\pics\*.*"

/** Css. */
SetOutPath "$INSTDIR\css\"
File /r "${BUILD_DIR}\css\*"

/** Chat. */
SetOutPath "$INSTDIR\chat\"
File "${BUILD_DIR}\chat\README"
SetOutPath "$INSTDIR\chat\default\"
File /r "${BUILD_DIR}\chat\default\*"
SetOutPath "$INSTDIR\chat\minimal\"
File /r "${BUILD_DIR}\chat\minimal\*"

/** Config. */
SetOutPath "$INSTDIR\config\"
File "${BUILD_DIR}\config\*"

/* Translations. */
SetOutPath "$INSTDIR\lang\"
File "${BUILD_DIR}\lang\*.qm"

/** Web pages. */
SetOutPath "$INSTDIR\webpages\windows\"
File "${BUILD_DIR}\webpages\windows\*"

/** Imageformats (GIF, MNG, JPEG) from Qt. */
SetOutPath "$INSTDIR\imageformats\"
File "${BUILD_DIR}\imageformats\*"

/** Phapi plugins */
SetOutPath "$INSTDIR\phapi-plugins\"
File "${BUILD_DIR}\phapi-plugins\*"

/**
 * So that we can execute wengophone.exe
 */
SetOutPath "$INSTDIR"
