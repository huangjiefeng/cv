# Bugfix, spaces don't work with install target
string(REPLACE "CMAKEBUGWITHSPACES" " " BUILD_DIR ${BUILD_DIR})
string(REPLACE "CMAKEBUGWITHSPACES" " " SOURCE_DIR ${SOURCE_DIR})

find_file(NSIS_PROGRAM
	NAMES
		makensis.exe
	PATHS
		"C:/Program Files/NSIS/"
)

if (NSIS_PROGRAM)
	message(STATUS "Found NSIS: ${NSIS_PROGRAM}")
else (NSIS_PROGRAM)
	message(FATAL_ERROR "Could not find NSIS")
endif (NSIS_PROGRAM)


set(INSTALLER_NAME
	${APPLICATION_NAME}-${APPLICATION_VERSION}-setup
)

set(NSIS_ARGS
	/DPRODUCT_NAME=${APPLICATION_NAME}
	/DPRODUCT_VERSION=${APPLICATION_VERSION}
	/DBUILD_DIR=${BUILD_DIR}
	/DQTDIR=$ENV{QTDIR}
	/DBINARY_NAME=${BINARY_NAME}
	/DPRODUCT_VERSION_1=${APPLICATION_VERSION_1}
	/DPRODUCT_VERSION_2=${APPLICATION_VERSION_2}
	/DPRODUCT_VERSION_3=${APPLICATION_VERSION_3}
	/DPRODUCT_VERSION_4=${APPLICATION_VERSION_4}
)

string(TOLOWER ${BUILD_TYPE} BUILD_TYPE)
if (BUILD_TYPE MATCHES debug)
	# Generate installer with .pdb files
	# Debug mode = with .pdb files always
	# MinSizeRel mode = no .pdb files
	# RelWithDebInfo mode = 2 setup.exe: one with .pdb files and one without
	set(NSIS_ARGS
		${NSIS_ARGS}
		/DDEBUG
		/DWITH_PDB
	)
endif (BUILD_TYPE MATCHES debug)

set(INSTALLER_NAME ${INSTALLER_NAME}-${BUILD_TYPE})

# Because BuildBot cannot handle random setup names
# Generates 2 setup.exe: one with the -revrevnumber.exe and one with -rev${SVN_REVISION}.exe
set(INSTALLER_NAME_REVNUMBER
	${INSTALLER_NAME}-revnumber
)

if (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)
	set(INSTALLER_NAME ${INSTALLER_NAME}-norev)
else (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)
	set(INSTALLER_NAME ${INSTALLER_NAME}-${SVN_REVISION})
endif (NOT SVN_REVISION OR SVN_REVISION EQUAL 0)

set(INSTALLER_DIR
	${SOURCE_DIR}/wengophone/nsis
)

execute_process(
	COMMAND ${NSIS_PROGRAM} ${NSIS_ARGS} /DINSTALLER_NAME=${INSTALLER_NAME}.exe ${INSTALLER_DIR}/installer_2005.nsi
)

# Because BuildBot cannot handle random setup names
# Generates 2 setup.exe: one with the -revnumber.exe and one with -${SVN_REVISION}.exe
exec_program(
		${CMAKE_COMMAND}
	ARGS
		"-E copy \"${INSTALLER_DIR}/${INSTALLER_NAME}.exe\" \"${INSTALLER_DIR}/${INSTALLER_NAME_REVNUMBER}.exe\""
)

if (BUILD_TYPE MATCHES relwithdebinfo)
	# Generate installer with .pdb files
	# Debug mode = with .pdb files always
	# MinSizeRel mode = no .pdb files
	# RelWithDebInfo mode = 2 setup.exe: one with .pdb files and one without
	set(NSIS_ARGS
		${NSIS_ARGS}
		/DWITH_PDB
	)

	execute_process(
		COMMAND ${NSIS_PROGRAM} ${NSIS_ARGS} /DINSTALLER_NAME=${INSTALLER_NAME}-pdb.exe ${INSTALLER_DIR}/installer.nsi
	)

	# Because BuildBot cannot handle random setup names
	# Generates 2 setup.exe: one with the -revnumber.exe and one with -${SVN_REVISION}.exe
	exec_program(
			${CMAKE_COMMAND}
		ARGS
			"-E copy \"${INSTALLER_DIR}/${INSTALLER_NAME}-pdb.exe\" \"${INSTALLER_DIR}/${INSTALLER_NAME_REVNUMBER}-pdb.exe\""
	)
endif (BUILD_TYPE MATCHES relwithdebinfo)
