# Install script for directory: D:/qutecom-2.2/libs/owutil

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
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/cutil/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/util/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/serialization/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/serializer/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/thread/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/settings/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/shlibloader/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/socket/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/system/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/http/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/libs/owutil/memorydump/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

