ow_create_static_library(owqtutil)

ow_use_public_libraries(
	owthread
	qt4
)

ow_add_public_include_dirs(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Because of UIC and MOC
ow_add_private_include_dirs(
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}
)

qt4_wrap_cpp(${PROJECT_NAME}_SRCS
	include/qtutil/EventFilter.h
	include/qtutil/QObjectThreadSafe.h
	include/qtutil/NonResizeableWidget.h
	include/qtutil/WengoStyleLabel.h
	include/qtutil/ToolTipLineEdit.h
	include/qtutil/ClickableLabel.h
	include/qtutil/Wizard.h
	include/qtutil/ThreadEventFilter.h
	include/qtutil/AnimatedButton.h
	include/qtutil/UpQComboBox.h
	include/qtutil/LinkLabel.h
	include/qtutil/ScrollFrame.h
	include/qtutil/ImageSelector.h
	include/qtutil/DropDownWindow.h
	include/qtutil/ClearLineEdit.h
	src/QtThumbnailManager.h
	src/QtThumbnailDirModel.h
)

qt4_wrap_ui(${PROJECT_NAME}_SRCS
	src/Wizard.ui
	src/ImageSelector.ui
)

qt4_add_resources(${PROJECT_NAME}_SRCS
	qtutil.qrc
)

ow_add_sources(
	src/EventFilter.cpp
	src/KeyEventFilter.cpp
	src/MouseEventFilter.cpp
	src/PaintEventFilter.cpp
	src/EnabledChangeEventFilter.cpp
	src/WidgetFactory.cpp
	src/Object.cpp
	src/QObjectThreadSafe.cpp
	src/ResizeEventFilter.cpp
	src/NonResizeableWidget.cpp
	src/StringListConvert.cpp
	src/Widget.cpp
	src/WengoStyleLabel.cpp
	src/ToolTipLineEdit.cpp
	src/ClickableLabel.cpp
	src/WidgetBackgroundImage.cpp
	src/Wizard.cpp
	src/ThreadEventFilter.cpp
	src/PixmapMerging.cpp
	src/SafeConnect.cpp
	src/LanguageChangeEventFilter.cpp
	src/CloseEventFilter.cpp
	src/AnimatedButton.cpp
	src/DesktopService.cpp
	src/UpQComboBox.cpp
	src/LinkLabel.cpp
	src/ScrollFrame.cpp
	src/ImageSelector.cpp
	src/ThumbnailView.cpp
	src/QtThumbnailDirModel.cpp
	src/QtThumbnailManager.cpp
	src/WidgetRatioEnforcer.cpp
	src/WidgetUtils.cpp
	src/ExpandButton.cpp
	src/OsdButton.cpp
	src/FSResourceFileEngineHandler.cpp
	src/KeywordTranslator.cpp
	src/DropDownWindow.cpp
	src/IconLineEdit.cpp
	src/ClearLineEdit.cpp
)

if (APPLE)
	ow_add_sources(
		src/MacDesktopService.cpp
	)
	qt4_wrap_cpp(${PROJECT_NAME}_SRCS
		include/qtutil/MacDesktopService.h
	)
	ow_add_private_include_dirs(
		/Developer/Headers/FlatCarbon/
	)
	ow_use_public_frameworks(
		Cocoa
		Carbon
	)
endif (APPLE)

if (WIN32)
	if (MINGW)
		ow_add_private_definitions(
			-DWINVER=0x500
			-D_WIN32_IE=0x500
		)
	endif (MINGW)

	ow_add_sources(
		src/WindowsDesktopService.cpp
	)
	qt4_wrap_cpp(${PROJECT_NAME}_SRCS
		include/qtutil/WindowsDesktopService.h
	)
endif (WIN32)

if (LINUX)
	ow_use_private_libraries(
		x11
	)
endif (LINUX)

subdirs(
	tests
)

ow_create_project_binary()
