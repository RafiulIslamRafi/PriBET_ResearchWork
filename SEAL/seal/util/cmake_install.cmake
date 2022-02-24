# Install script for directory: /home/kali/Downloads/SEAL_3.0/SEAL/seal/util

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/seal/util" TYPE FILE FILES
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/aes.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/baseconverter.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/clang.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/clipnormal.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/common.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/config.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/defines.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/gcc.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/globals.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/hash.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/locks.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/mempool.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/modulus.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/msvc.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/ntt.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/numth.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/nussbaumer.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/pointer.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polyarith.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polyarithmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polyarithsmallmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polycore.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polyfftmultmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polyfftmultsmallmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/polymodulus.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/randomtostd.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/smallntt.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/uintarith.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/uintarithmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/uintarithsmallmod.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/uintcore.h"
    )
endif()

