# Exports target SEAL::seal
#
# Creates variables:
#   SEAL_BUILD_TYPE : The build configuration used
#   SEAL_DEBUG : Set to non-zero value if SEAL is compiled with extra debugging code (very slow!)
#   SEAL_USE_MSGSL : Set to non-zero value if SEAL is compiled with Microsoft GSL support
#   MSGSL_INCLUDE_DIR : Holds the path to Microsoft GSL if SEAL is compiled with Microsoft GSL support

include(CMakeFindDependencyMacro)

set(SEAL_BUILD_TYPE Release)
set(SEAL_USE_CXX17 ON)
set(SEAL_DEBUG OFF)
set(SEAL_USE_MSGSL OFF)
if(SEAL_USE_MSGSL)
    set(MSGSL_INCLUDE_DIR MSGSL_INCLUDE_DIR-NOTFOUND)
endif()

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_dependency(Threads REQUIRED)

include(${CMAKE_CURRENT_LIST_DIR}/SEALTargets.cmake)

message(STATUS "SEAL detected (version ${SEAL_VERSION})")
message(STATUS "SEAL using C++17: ${SEAL_USE_CXX17}")
message(STATUS "SEAL build type: ${SEAL_BUILD_TYPE}")
message(STATUS "SEAL debug mode: ${SEAL_DEBUG}")
message(STATUS "SEAL using Microsoft GSL: ${SEAL_USE_MSGSL}")
