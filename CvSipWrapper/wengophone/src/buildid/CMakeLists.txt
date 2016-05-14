ow_create_static_library(owbuildid)

ow_use_private_libraries(
	glib2
	libpurple
	tinyxml
	curl
	ffmpeg
	owutil
)

if (NOT APPLE AND OWSOUND_PORTAUDIO_SUPPORT)
	ow_use_private_libraries(
		portaudio
	)
endif (NOT APPLE AND OWSOUND_PORTAUDIO_SUPPORT)

if (BUILDID_SUPPORT)
	ow_add_private_definitions(
		-DDD_BUILDID=${CURRENT_DATE_TIME}LL
		-DDD_VERSION=${APPLICATION_VERSION}
		-DDD_REVISION=${SVN_REVISION}
		-DDD_SOFTPHONE_NAME=wengo
	)
else (BUILDID_SUPPORT)
	ow_add_private_definitions(
		-DDD_BUILDID=0LL
		-DDD_VERSION=0
		-DDD_REVISION=0
		-DDD_SOFTPHONE_NAME=wengo
	)
endif (BUILDID_SUPPORT)

ow_add_public_include_dirs(
	${CMAKE_CURRENT_SOURCE_DIR}
)

ow_add_sources(
	../WengoPhoneBuildId.cpp
)

ow_create_project_binary()
