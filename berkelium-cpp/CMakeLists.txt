CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-cpp)

IF(NOT DEFINED BERKELIUM_BUILD)
	MESSAGE(FATAL_ERROR "this project should be build using berkelium build system!")
ENDIF()

###########
## Projects
## --------
ADD_SUBDIRECTORY(src/lib build/berkelium-cpp/lib)
ADD_SUBDIRECTORY(src/host/shared build/berkelium-cpp/host/shared)
ADD_SUBDIRECTORY(src/host/fake build/berkelium-cpp/host/fake)
ADD_SUBDIRECTORY(src/test build/berkelium-cpp/test)
ADD_SUBDIRECTORY(src/demo build/berkelium-cpp/demo)
