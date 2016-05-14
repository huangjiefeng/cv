Delete "$INSTDIR\styles\*"
RMDir "$INSTDIR\styles\"

Delete "$INSTDIR\icons\*"
RMDir "$INSTDIR\icons\"

Delete "$INSTDIR\lang\*"
RMDir "$INSTDIR\lang\"

Delete "$INSTDIR\sounds\tones\default\*"
RMDir "$INSTDIR\sounds\tones\default\"

Delete "$INSTDIR\sounds\tones\jungle\*"
RMDir "$INSTDIR\sounds\tones\jungle\"

Delete "$INSTDIR\sounds\tones\*"
RMDir "$INSTDIR\sounds\tones\"

Delete "$INSTDIR\sounds\*"
RMDir "$INSTDIR\sounds\"

Delete "$INSTDIR\emoticons\*"
RMDir "$INSTDIR\emoticons\"

Delete "$INSTDIR\plugins\*"
RMDir "$INSTDIR\plugins\"

Delete "$INSTDIR\config\*"
RMDir "$INSTDIR\config\"

RMDir /r "$INSTDIR\pics\"

/* Delete css dir if it was left empty */
Delete "$INSTDIR\css\README.txt"
RMDir "$INSTDIR\css\"

/* Delete distributed themes but not custom ones */
Delete "$INSTDIR\chat\README"
RMDir /r "$INSTDIR\chat\default\"
RMDir /r "$INSTDIR\chat\minimal\"
RMDir "$INSTDIR\chat\"

Delete "$INSTDIR\webpages\windows\*"
RMDir "$INSTDIR\webpages\windows\"

Delete "$INSTDIR\webpages\*"
RMDir "$INSTDIR\webpages\"

Delete "$INSTDIR\imageformats\*"
RMDir "$INSTDIR\imageformats\"

Delete "$INSTDIR\phapi-plugins\*"
RMDir "$INSTDIR\phapi-plugins\"

Delete "$INSTDIR\*"
RMDir "$INSTDIR"

Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
Delete "$SMPROGRAMS\${PRODUCT_NAME}\Website.lnk"
Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
RMDir "$SMPROGRAMS\${PRODUCT_NAME}"

/**
 * Removes the configuration file + the contacts.
 * Only if the user asks for it.
 */
/*Delete "${WENGODIR}\contacts\*"
RMDir "${WENGODIR}\contacts\"
Delete "${WENGODIR}\*"
RMDir "${WENGODIR}"*/

DeleteRegKey HKLM "${PRODUCT_UNINSTALL_KEY}"
DeleteRegKey HKCU "${PRODUCT_REGKEY}"
SetAutoClose true
