ow_create_shared_library(owutil)

ow_use_public_libraries(
	owcutil
	boost
)

ow_add_sources(
	src/Base64.cpp
	src/CountryList.cpp
	src/Date.cpp
	src/File.cpp
	src/Identifiable.cpp
	src/Logger.cpp
	src/OWPicture.cpp
	src/Path.cpp
	src/String.cpp
	src/StringList.cpp
	src/Time.cpp
	src/Uuid.cpp
	src/WebBrowser.cpp
	src/NonCopyable.cpp
)

if (WIN32)
	ow_add_sources(
		src/win/UuidWin.cpp
	)
	ow_add_private_libraries(
		Rpcrt4
	)
endif (WIN32)

if (APPLE)
	ow_add_sources(
		src/mac/UuidMac.cpp
	)
	ow_use_private_frameworks(
		Carbon
		CoreFoundation
	)
endif (APPLE)

if (LINUX)
	ow_use_public_libraries(
		uuid
	)
	ow_add_sources(
		src/unix/UuidUnix.cpp
	)
endif (LINUX)

if (LOGGER_ENABLE_EVENT)
   ow_add_private_definitions(
       -DLOGGER_ENABLE_EVENT
   )
endif (LOGGER_ENABLE_EVENT)

ow_create_project_binary()

subdirs(
	tests
)
