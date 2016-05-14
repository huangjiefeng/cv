# Install script for directory: D:/qutecom-2.2

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files/owbuild")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE PROGRAM FILES
    "C:/WINDOWS/system32/msvcp71.dll"
    "C:/WINDOWS/system32/msvcr71.dll"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" MATCHES "^(Unspecified)$")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/portaudio/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/qtutil/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/timer/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/pixertool/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/webcam/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/wenbox/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owbrowser/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/trayicon/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/softupdater/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/sound/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/imwrapper/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/imwrapper/src/purple/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/sipwrapper/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/sipwrapper/src/phapi/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/imwrapper/src/multiim/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wengophone/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/crashreport/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "D:/qutecom-2.2/build/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "D:/qutecom-2.2/build/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
