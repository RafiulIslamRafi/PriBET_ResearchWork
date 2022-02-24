# Install script for directory: /home/kali/Downloads/SEAL_3.0/SEAL/seal

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/seal" TYPE FILE FILES
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/batchencoder.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/bigpolyarray.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/bigpoly.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/biguint.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/ciphertext.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/ckks.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/context.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/decryptor.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/defaultparams.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/encoder.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/encryptionparams.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/encryptor.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/evaluator.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/galoiskeys.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/intarray.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/keygenerator.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/memorymanager.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/plaintext.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/publickey.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/randomgen.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/relinkeys.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/seal.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/secretkey.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/smallmodulus.h"
    "/home/kali/Downloads/SEAL_3.0/SEAL/seal/utilities.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/kali/Downloads/SEAL_3.0/SEAL/seal/util/cmake_install.cmake")

endif()

