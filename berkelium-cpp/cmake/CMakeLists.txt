CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-cpp)

####################
## Output Directorys
## -----------------
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "out")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "run")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "run")

###############
## Source Files
## ------------
INCLUDE_DIRECTORIES(../include)
FILE(GLOB_RECURSE SRC_INCLUDE "../include/*.hpp")
FILE(GLOB_RECURSE SRC_LIB "../src/impl/*.cpp")
FILE(GLOB_RECURSE SRC_DEMO1 "../src/demo/demo1/*.cpp")
FILE(GLOB_RECURSE SRC_TEST "../src/test/*.cpp" "../src/test/*.h")

###################################
## Visual Studio Source File Groups
## --------------------------------
SOURCE_GROUP("Header Files" FILES ${SRC_INCLUDE})

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
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ENABLE_CXX11}")
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

################
## berkelium-cpp
## -------------
ADD_DEFINITIONS(-DBERKELIUM_CPP_IMPL)
ADD_LIBRARY(berkelium-cpp STATIC ${SRC_LIB} ${SRC_INCLUDE})

##################
## berkelium demo1
## ---------------
ADD_EXECUTABLE(demo1 ${SRC_DEMO1} ${SRC_INCLUDE})
TARGET_LINK_LIBRARIES(demo1 berkelium-cpp ${Boost_LIBRARIES})

##############
## Google Test
## -----------
SET(GTEST_ROOT "../../gtest")
ADD_SUBDIRECTORY(${GTEST_ROOT} ${GTEST_ROOT})
ENABLE_TESTING(TRUE)
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
ADD_EXECUTABLE(berkelium-cpp-test ${SRC_TEST} ${SRC_INCLUDE})
TARGET_LINK_LIBRARIES(berkelium-cpp-test berkelium-cpp gtest gtest_main ${Boost_LIBRARIES})
ADD_TEST(berkelium-cpp-test run/berkelium-cpp-test)
