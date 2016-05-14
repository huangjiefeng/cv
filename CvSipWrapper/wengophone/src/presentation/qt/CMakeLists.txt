set(BINARY_NAME ${APPLICATION_NAME})
string(REPLACE " " "" BINARY_NAME ${BINARY_NAME})
if (LINUX)
	string(TOLOWER ${BINARY_NAME} BINARY_NAME)
endif (LINUX)
ow_create_executable(${BINARY_NAME})

configure_file(buildconfig.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/buildconfig.h)

subdirs(
	lang
)

ow_use_private_libraries(
	owphone
	owbuildid
	owqtutil
	pixertool
	webcam
	psiidle
	owtrayicon
	owbrowser
	owsoftupdater
)

if (SIPWRAPPER_BACKEND_SIPX)
	ow_add_private_definitions(
		-DSIPXWRAPPER
	)

	ow_use_private_libraries(
		sipxwrapper
	)
else (SIPWRAPPER_BACKEND_SIPX)
if (SIPWRAPPER_BACKEND_PHAPI)
	ow_add_private_definitions(
		-DPHAPIWRAPPER
	)

	ow_use_private_libraries(
		owphapiwrapper
	)
else (SIPWRAPPER_BACKEND_PHAPI)
if (IMWRAPPER_BACKEND_MULTI)
	ow_add_private_definitions(
		-DMULTIIMWRAPPER
	)

	ow_use_private_libraries(
		owmultiimwrapper
	)
endif (IMWRAPPER_BACKEND_MULTI)
endif (SIPWRAPPER_BACKEND_PHAPI)
endif (SIPWRAPPER_BACKEND_SIPX)

ow_add_private_include_dirs(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_wrap_cpp(${PROJECT_NAME}_SRCS
	QtWengoPhone.h
	QtDialpad.h
	QtSVGDialpad.h
	QtHttpProxyLogin.h
	QtIdle.h
	QtLanguage.h
	QtToolBar.h
	QtSystray.h
	QtFactory.h
	QtPresenceMenuManager.h
	QtBrowserWidget.h
	QtIMAccountMonitor.h
	QtNoWengoAlert.h

	phoneline/QtPhoneLine.h

	phonecall/QtPhoneCall.h
	phonecall/QtVideoQt.h
	phonecall/QtContactCallListWidget.h
	phonecall/QtPhoneCallEventFilter.h
	phonecall/QtAudioSmileysWindow.h

	toaster/QtChatToaster.h
	toaster/QtCallToaster.h
	toaster/QtToaster.h

	login/ILogin.h
	login/QtLoginDialog.h
	login/QtLogAccount.h
	login/QtAddAccount.h
	login/QtAddWengoAccount.h
	login/QtAddSIPAccount.h

	profile/QtProfileDetails.h
	profile/QtUserProfile.h
	profile/QtUserProfileHandler.h

	imcontact/QtAdvancedIMContactManager.h
	imcontact/QtAddIMContact.h
	imcontact/QtSimpleIMContactManager.h
	imcontact/QtIMContactManagerInterface.h

	imaccount/QtIMAccountManager.h
	imaccount/QtIMAccountSettings.h
	imaccount/QtIMAccountPlugin.h
	imaccount/QtMSNSettings.h
	imaccount/QtYahooSettings.h
	imaccount/QtAIMSettings.h
	imaccount/QtICQSettings.h
	imaccount/QtJabberSettings.h
	imaccount/QtGoogleTalkSettings.h

	contactlist/QtContactList.h
	contactlist/ContactGroupPopupMenu.h
	contactlist/QtContactWidget.h
	contactlist/QtContactTreeMouseFilter.h
	contactlist/QtContactManager.h
	contactlist/QtContact.h
	contactlist/QtTreeViewDelegate.h
	contactlist/QtContactTreeKeyFilter.h
	contactlist/QtRenameGroup.h
	contactlist/QtConferenceAction.h

	conference/QtConferenceCallWidget.h

	chat/emoticons/QtEmoticonsWidget.h
	chat/emoticons/QtEmoticonButton.h
	chat/chatroom/QtChatRoomInviteDlg.h
	chat/QtChatTabWidget.h
	chat/QtChatAvatarFrame.h
	chat/QtChatAvatarWidget.h
	chat/QtChatHandler.h
	chat/QtChatWidget.h
	chat/QtChatWindow.h
	chat/QtChatEditWidget.h
	chat/QtChatHistoryWidget.h
	chat/QtWengoStyleBar.h
	chat/QtChatLogViewer.h

	webservices/sms/QtSms.h
	webservices/softupdate/QtSoftUpdate.h
	webservices/softupdate/QtBrowserSoftUpdate.h

	callbar/QtCallBar.h
	callbar/QtPhoneComboBox.h

	statusbar/QtStatusBar.h

	config/QtAdvancedConfig.h
	config/QtAccountSettings.h
	config/QtAdvancedSettings.h
	config/QtAppearanceSettings.h
	config/QtAudioSettings.h
	config/QtCallForwardSettings.h
	config/QtGeneralSettings.h
	config/QtNotificationSettings.h
	config/QtPrivacySettings.h
	config/QtVideoSettings.h
	config/QtLanguagesSettings.h
	config/QtWengoConfigDialog.h
	config/QtVoicemailSettings.h
	config/QtSecuritySettings.h

	profilebar/QtEventWidget.h
	profilebar/QtIMProfileWidget.h
	profilebar/QtProfileBar.h
	profilebar/QtCreditWidget.h
	profilebar/QtIMAccountPresenceMenuManager.h

	history/QtHistoryWidget.h
	history/QtHistory.h

	filetransfer/QtFileTransfer.h
	filetransfer/QtFileTransferAcceptDialog.h
	filetransfer/QtFileTransferWidget.h
	filetransfer/QtFileTransferItem.h
	filetransfer/QtFileTransferDownloadItem.h
	filetransfer/QtFileTransferUploadItem.h
	filetransfer/QtFileTransferNotifyDialog.h

	webdirectory/QtWebDirectory.h
)

ow_add_sources(
	../main.cpp
	QtWengoPhone.cpp
	QtDialpad.cpp
	QtSVGDialpad.cpp
	QtAbout.cpp
	QtHttpProxyLogin.cpp
	QtIdle.cpp
	QtEnumIMProtocol.cpp
	QtLanguage.cpp
	QtToolBar.cpp
	QtSystray.cpp
	QtFactory.cpp
	QtPresenceMenuManager.cpp
	QtWengoStyle.cpp
	QtBrowserWidget.cpp
	QtIMAccountMonitor.cpp
	QtPresencePixmapHelper.cpp
	QtIMAccountHelper.cpp
	QtNoWengoAlert.cpp

	phoneline/QtPhoneLine.cpp

	phonecall/QtPhoneCall.cpp
	phonecall/QtVideo.cpp
	phonecall/QtVideoQt.cpp
	phonecall/QtContactCallListWidget.cpp
	phonecall/QtPhoneCallEventFilter.cpp
	phonecall/QtAudioSmileysWindow.cpp

	toaster/QtChatToaster.cpp
	toaster/QtCallToaster.cpp
	toaster/QtToaster.cpp

	login/ILogin.cpp
	login/QtLoginDialog.cpp
	login/QtLogAccount.cpp
	login/QtAddAccount.cpp
	login/QtAddWengoAccount.cpp
	login/QtAddSIPAccount.cpp
	login/QtAddAccountUtils.cpp

	imcontact/QtAdvancedIMContactManager.cpp
	imcontact/QtAddIMContact.cpp
	imcontact/QtIMContactItem.cpp
	imcontact/QtSimpleIMContactManager.cpp
	imcontact/QtIMContactManagerInterface.cpp

	profile/QtProfileDetails.cpp
	profile/QtUserProfile.cpp
	profile/QtUserProfileHandler.cpp

	imaccount/QtIMAccountManager.cpp
	imaccount/QtIMAccountSettings.cpp
	imaccount/QtIMAccountPlugin.cpp
	imaccount/QtMSNSettings.cpp
	imaccount/QtYahooSettings.cpp
	imaccount/QtAIMSettings.cpp
	imaccount/QtICQSettings.cpp
	imaccount/QtJabberSettings.cpp
	imaccount/QtGoogleTalkSettings.cpp

	wenbox/QtWenboxPlugin.cpp

	contactlist/QtContactList.cpp
	contactlist/ContactGroupPopupMenu.cpp
	contactlist/QtContactWidget.cpp
	contactlist/QtContactTreeMouseFilter.cpp
	contactlist/QtContactManager.cpp
	contactlist/QtContactListManager.cpp
	contactlist/QtContact.cpp
	contactlist/QtTreeViewDelegate.cpp
	contactlist/QtContactPixmap.cpp
	contactlist/QtContactInfo.cpp
	contactlist/QtContactTreeKeyFilter.cpp
	contactlist/QtRenameGroup.cpp
	contactlist/QtConferenceAction.cpp
	contactlist/QtContactMenu.cpp

	conference/QtConferenceCallWidget.cpp

	chat/emoticons/QtEmoticonsWidget.cpp
	chat/emoticons/QtEmoticon.cpp
	chat/emoticons/QtEmoticonButton.cpp
	chat/emoticons/QtEmoticonsManager.cpp
	chat/chatroom/QtChatRoomInviteDlg.cpp
	chat/chatroom/QtChatRoomTreeWidgetItem.cpp
	chat/chatroom/QtChatRoomListWidgetItem.cpp
	chat/QtChatTabWidget.cpp
	chat/QtChatAvatarFrame.cpp
	chat/QtChatAvatarWidget.cpp
	chat/QtChatHandler.cpp
	chat/QtChatWidget.cpp
	chat/QtChatWindow.cpp
	chat/QtChatEditWidget.cpp
	chat/QtChatHistoryWidget.cpp
	chat/QtWengoStyleBar.cpp
	chat/QtChatUtils.cpp
	chat/QtChatLogViewer.cpp
	chat/QtChatTheme.cpp

	webservices/sms/QtSms.cpp
	webservices/softupdate/QtSoftUpdate.cpp
	webservices/softupdate/QtBrowserSoftUpdate.cpp

	callbar/QtCallBar.cpp
	callbar/QtPhoneComboBox.cpp

	statusbar/QtStatusBar.cpp

	config/QtAdvancedConfig.cpp
	config/QtAccountSettings.cpp
	config/QtAdvancedSettings.cpp
	config/QtAppearanceSettings.cpp
	config/QtAudioSettings.cpp
	config/QtCallForwardSettings.cpp
	config/QtGeneralSettings.cpp
	config/QtNotificationSettings.cpp
	config/QtPrivacySettings.cpp
	config/QtVideoSettings.cpp
	config/QtLanguagesSettings.cpp
	config/QtWengoConfigDialog.cpp
	config/QtVoicemailSettings.cpp
	config/QtSecuritySettings.cpp

	profilebar/QtEventWidget.cpp
	profilebar/QtIMProfileWidget.cpp
	profilebar/QtCreditWidget.cpp
	profilebar/QtProfileBar.cpp
	profilebar/QtIMAccountPresenceMenuManager.cpp

	history/QtHistoryWidget.cpp
	history/QtHistory.cpp

	filetransfer/QtFileTransfer.cpp
	filetransfer/QtFileTransferAcceptDialog.cpp
	filetransfer/QtFileTransferWidget.cpp
	filetransfer/QtFileTransferItem.cpp
	filetransfer/QtFileTransferDownloadItem.cpp
	filetransfer/QtFileTransferUploadItem.cpp
	filetransfer/QtFileTransferNotifyDialog.cpp

	webdirectory/QtWebDirectory.cpp
)

if (WIN32)
	qt4_wrap_cpp(${PROJECT_NAME}_SRCS
		WinApp.h
	)

	# Create a 4 digit, comma separated, version of the APPLICATION_VERSION
	# variable. That is, from x.y.z to x,y,z,0.
	string(REPLACE "." "," APPLICATION_VERSION_4DIGITS ${APPLICATION_VERSION})
	set(APPLICATION_VERSION_4DIGITS "${APPLICATION_VERSION_4DIGITS},0,0,0")
	string(REGEX REPLACE "([0-9]+,[0-9]+,[0-9]+,[0-9]+).*" "\\1" APPLICATION_VERSION_4DIGITS "${APPLICATION_VERSION_4DIGITS}")

	configure_file(
		win32/application.rc
		${BUILD_DIR}/application.rc
		)

	ow_add_sources(
		${BUILD_DIR}/application.rc
		WinApp.cpp
	)
	ow_add_private_libraries(
		qtmain.lib
	)
endif (WIN32)

if (UNIX AND NOT WIN32)
	if (APPLE)
		set_source_files_properties(macosx/QtMacApplication.cpp
			PROPERTIES
				COMPILE_FLAGS "-x objective-c++"
		)

		qt4_wrap_cpp(${PROJECT_NAME}_SRCS
			macosx/QtMacApplication.h
		)

		ow_add_sources(
			macosx/QtMacApplication.cpp
		)
	else (APPLE)
		if (WENGOPHONE_XV_SUPPORT)
			if (X11_XV_FOUND)
				ow_use_private_libraries(
					x11
				)

				ow_add_private_definitions(
					-DXV_HWACCEL
				)

				qt4_wrap_cpp(${PROJECT_NAME}_SRCS
					phonecall/XVWindow.h
					phonecall/QtVideoXV.h
				)

				ow_add_sources(
					phonecall/XVWindow.cpp
					phonecall/QtVideoXV.cpp
				)
			endif (X11_XV_FOUND)
		endif (WENGOPHONE_XV_SUPPORT)
	endif (APPLE)
endif (UNIX AND NOT WIN32)

qt4_wrap_ui(${PROJECT_NAME}_SRCS
	WengoPhoneWindow.ui
	AboutWindow.ui
	HttpProxyLoginWindow.ui
	DialpadWidget.ui
	NoWengoAlert.ui

	login/LogAccount.ui
	login/AddAccount.ui
	login/AddWengoAccount.ui
	login/AddSIPAccount.ui

	imaccount/IMAccountTemplate.ui
	imaccount/AIMSettings.ui
	imaccount/ICQSettings.ui
	imaccount/GoogleTalkSettings.ui
	imaccount/IMAccountManager.ui
	imaccount/JabberSettings.ui
	imaccount/MSNSettings.ui
	imaccount/YahooSettings.ui

	profile/ProfileDetails.ui

	imcontact/IMContactManager.ui
	imcontact/AddIMContact.ui
	imcontact/SimpleIMContactManager.ui

	webservices/sms/SmsWindow.ui
	webservices/softupdate/SoftUpdateWindow.ui
	webservices/softupdate/BrowserSoftUpdateWindow.ui

	conference/ConferenceCallWidget.ui

	config/AccountSettings.ui
	config/AdvancedConfigWindow.ui
	config/GeneralSettings.ui
	config/AdvancedSettings.ui
	config/AppearanceSettings.ui
	config/AudioSettings.ui
	config/CallForwardSettings.ui
	config/LanguagesSettings.ui
	config/ManageBlockedUsersSettings.ui
	config/NotificationSettings.ui
	config/NotificationSettingsItem.ui
	config/PrivacySettings.ui
	config/VideoSettings.ui
	config/WengoConfigDialog.ui
	config/VoicemailSettings.ui
	config/SecuritySettings.ui


	chat/ChatMainWindow.ui
	chat/ChatWidget.ui
	chat/UserFrame.ui
	chat/AvatarFrame.ui
	chat/AvatarWidget.ui
	chat/ChatLogViewer.ui

	chat/chatroom/ChatRoomInviteWidget.ui

	contactlist/RenameGroupDialog.ui
	contactlist/ContactWidget.ui
	contactlist/ContactList.ui

	profilebar/CreditWidget.ui
	profilebar/EventWidget.ui
	profilebar/IMProfileWidget.ui

	toaster/QtChatToaster.ui
	toaster/QtCallToaster.ui

	phonecall/PhoneCallWidget.ui
	phonecall/VideoWindow.ui

	filetransfer/AcceptDialog.ui
	filetransfer/FileTransferDialog.ui
	filetransfer/FileTransferItem.ui
	filetransfer/NotifyDialog.ui

	history/HistoryWidget.ui

	statusbar/StatusBarWidget.ui
)

if (ENABLE_CRASHREPORT)
    ow_use_private_libraries(
            owmemorydump
    )
    ow_add_private_definitions(
            -DENABLE_CRASHREPORT
    )
endif (ENABLE_CRASHREPORT)

if (CUSTOM_ACCOUNT)
		ow_add_private_definitions(
			-DCUSTOM_ACCOUNT
		)
		message(STATUS "Custom account activated")
endif (CUSTOM_ACCOUNT)

if (DISABLE_CALL_FORWARD)
		ow_add_private_definitions(
			-DDISABLE_CALL_FORWARD
		)
		message(STATUS "Call forward deactivated")
endif (DISABLE_CALL_FORWARD)

if (DISABLE_SMS)
		ow_add_private_definitions(
			-DDISABLE_SMS
		)
		message(STATUS "Sms deactivated")
endif (DISABLE_SMS)

if (DISABLE_VOICE_MAIL)
		ow_add_private_definitions(
			-DDISABLE_VOICE_MAIL
		)
		message(STATUS "Voice mail deactivated")
endif (DISABLE_VOICE_MAIL)

if (APPLE)
	ow_use_private_frameworks(
		Cocoa
		Carbon
	)
	ow_add_private_include_dirs(
		/Developer/Headers/FlatCarbon/
	)
	ow_use_private_libraries(
		glib2 #comes from gaimimwrapper <- gaim
	)
endif (APPLE)

if (LINUX)
	# Define QT_TRANSLATION_DIR, this is used on Linux to load Qt translations
	EXEC_PROGRAM(${QT_QMAKE_EXECUTABLE}
		ARGS "-query QT_INSTALL_PREFIX"
		OUTPUT_VARIABLE QT_INSTALL_PREFIX)
	ow_add_private_definitions(
		-DQT_TRANSLATION_DIR='"${QT_INSTALL_PREFIX}/translations"'
	)
endif (LINUX)

if (WIN32)
   if (ENABLE_CONSOLE)
      ow_create_project_binary()
   else(ENABLE_CONSOLE)
      ow_create_project_binary(WIN32)

   endif(ENABLE_CONSOLE)
  
else (WIN32)
      ow_create_project_binary(MACOSX_BUNDLE)
endif (WIN32)

ow_locale_release(
	${CMAKE_CURRENT_SOURCE_DIR}/lang
)

add_dependencies(${BINARY_NAME} lrelease)

# Bugfix, spaces don't work with install target
string(REPLACE " " "CMAKEBUGWITHSPACES" TMP_BUILD_DIR ${BUILD_DIR})
string(REPLACE " " "CMAKEBUGWITHSPACES" TMP_SOURCE_DIR ${CMAKE_SOURCE_DIR})

# Passes variables to the install script
if (APPLE)
	ow_declare_install_script()
else (APPLE)
	install(
		CODE "
		set(SOURCE_DIR ${TMP_SOURCE_DIR})
		set(APPLICATION_NAME ${APPLICATION_NAME})
		set(APPLICATION_VERSION ${APPLICATION_VERSION})
		set(APPLICATION_VERSION_1 ${APPLICATION_VERSION_1})
		set(APPLICATION_VERSION_2 ${APPLICATION_VERSION_2})
		set(APPLICATION_VERSION_3 ${APPLICATION_VERSION_3})
		set(APPLICATION_VERSION_4 ${APPLICATION_VERSION_4})
		set(BUILD_DIR ${TMP_BUILD_DIR})
		set(BUILD_TYPE ${CMAKE_BUILD_TYPE})
		set(DATA_INSTALL_DIR ${DATA_INSTALL_DIR})
		set(INSTALLER_WITH_PDB ${INSTALLER_WITH_PDB})
		set(SVN_REVISION ${SVN_REVISION})
		set(BINARY_NAME ${BINARY_NAME})
		"
	)
endif (APPLE)

if (LINUX)
	include(CMakeLists-install-linux.txt)
endif (LINUX)

if (WIN32)
	# NSIS installer script
	install(SCRIPT ../../../nsis/CMakeLists.txt)
endif (WIN32)


# zip target: zip ${BUILD_DIR}
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
set(ZIP_NAME
	${CMAKE_BINARY_DIR}/${APPLICATION_NAME}-${APPLICATION_VERSION}-${BUILD_TYPE}
)

# Because BuildBot cannot handle random zip names
# Generates 2 zip: one with the -revnumber.tar.bz2 and one with -${SVN_REVISION}.tar.bz2
set(ZIP_NAME_REVNUMBER
	${CMAKE_BINARY_DIR}/${APPLICATION_NAME}-${APPLICATION_VERSION}-${BUILD_TYPE}-revnumber
)
if (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)
	set(ZIP_NAME ${ZIP_NAME}-norev)
else (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)
	set(ZIP_NAME ${ZIP_NAME}-${SVN_REVISION})
endif (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)

set(ZIP_DIR_NAME
	${CMAKE_BINARY_DIR}/${APPLICATION_NAME}-${APPLICATION_VERSION}-${BUILD_TYPE}
)

if (NOT WIN32)
configure_file(
	${CMAKE_SOURCE_DIR}/scripts/wengophone.sh
	${BUILD_DIR}/${BINARY_NAME}.sh
	@ONLY
)
endif (NOT WIN32)

add_custom_target(zip
	COMMAND
		"${CMAKE_COMMAND}"
		-DSOURCE_DIR="${CMAKE_SOURCE_DIR}"
		-DBUILD_DIR="${BUILD_DIR}"
		-DZIP_NAME="${ZIP_NAME}"
		-DZIP_NAME_REVNUMBER="${ZIP_NAME_REVNUMBER}"
		-DZIP_DIR_NAME="${ZIP_DIR_NAME}"
		-DBINARY_NAME="${BINARY_NAME}"
		-P "${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists-zip.txt"
	WORKING_DIRECTORY
		${CMAKE_BINARY_DIR}
)
