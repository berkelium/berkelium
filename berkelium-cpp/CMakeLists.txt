CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-cpp)

####################
## Output Directorys
## -----------------
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/run")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/run")

###############
## Header Files
## ------------
INCLUDE_DIRECTORIES(include)

##############
## File Groups
## -----------
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

################
## GCC smart_ptr
## -------------
IF(CMAKE_COMPILER_IS_GNUCXX)
	# enable all warnings
	ADD_DEFINITIONS(-Wall)
	SET(ENABLE_CXX11 "-std=c++11")
	EXECUTE_PROCESS(COMMAND "${CMAKE_CXX_COMPILER} -dumpversion" OUTPUT_VARIABLE GCC_VERSION)
	IF(GCC_VERSION VERSION_LESS 4.7)
		SET(ENABLE_CXX11 "-std=c++0x")
	ENDIF()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ENABLE_CXX11} -fPIC")
	ADD_DEFINITIONS(-D__GXX_EXPERIMENTAL_CXX0X__)
ENDIF()

################
## Boost Library
## -------------
find_package(Boost 1.46 COMPONENTS filesystem system REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

#################
## System Defines
## --------------
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	ADD_DEFINITIONS(-DLINUX)
ENDIF()

###########
## Projects
## --------
ADD_SUBDIRECTORY(src/lib build/lib)
ADD_SUBDIRECTORY(src/test build/test)
ADD_SUBDIRECTORY(src/demo build/demo)

ENABLE_TESTING(TRUE)
ADD_TEST(test run/berkelium-cpp-test)
