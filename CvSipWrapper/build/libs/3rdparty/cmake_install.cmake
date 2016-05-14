# Install script for directory: D:/qutecom-2.2/libs/3rdparty

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

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/ffmpeg/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/pthread/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/libstdcpp/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/boost/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/boost-test/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/qt4/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/libintl/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/glib/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/openssl/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/curl/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/alsa/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/arts/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/esd/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/tinyxml/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/libxml/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/gnutls/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/iconv/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/zlib/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/libpurple/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/uuid/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/x11/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/python/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/samplerate/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/sndfile/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/psiidle/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/3rdparty/googlebreakpad/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

