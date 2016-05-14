# Install script for directory: D:/qutecom-2.2/wifo

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
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/msdirent/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/owbase/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/owsl/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/netlib/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/ortp/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/srtp/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/libosip2/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/sVoIP/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/eXosip/transport_manager/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/eXosip/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi-util/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/fidlib/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/gsm/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/g722/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/g726/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/amrnb/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/amrwb/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/speex/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/speexec/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/stun/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/phspeexplugin/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/wifo-plugins/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/pyphapi/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/ilbc/cmake_install.cmake")
  INCLUDE("D:/qutecom-2.2/build/wifo/phapi/philbcplugin/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

