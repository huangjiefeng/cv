ow_create_static_library(netlib)

ow_use_private_libraries(
	owbase
	owsl
	curl
)

ow_add_public_include_dirs(
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

ow_add_sources(
	src/netlib.cpp
	src/netlib_util.cpp
	src/stun_func.cpp
	src/udp_func.cpp
	src/bugfix.cpp
)

if (MSVC)
	ow_add_private_libraries(
		user32
		shell32
		ws2_32
		advapi32
		wininet
		urlmon
	)
	ow_add_sources(
		src/strndup.c
	)
endif (MSVC)

if (APPLE)
	ow_add_sources(
		src/strndup.c
	)
endif (APPLE)

subdirs(
	tests
)

ow_create_project_binary()
