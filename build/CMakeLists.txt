CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

PROJECT(berkelium)

####################
## Berkelium Build
## -----------------
SET(BERKELIUM_BUILD true)

####################
## Output Directorys
## -----------------
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/out")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/run")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/run")

###############
## Header Files
## ------------
INCLUDE_DIRECTORIES(../berkelium-cpp/include)

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
## MSVC
## -------------
IF(MSVC)
	# switch statement contains 'default' but no 'case' labels
	ADD_DEFINITIONS(/wd4065)
	# nonstandard extension used: enum 'Berkelium::Ipc::CommandId' used in qualified name
	ADD_DEFINITIONS(/wd4482)
	# use gtest as dll on windows
	SET(gtest_force_shared_crt on CACHE BOOL
		"Use shared (DLL) run-time lib even when Google Test is built as static lib.")
ENDIF()

################
## Tracing
## -------------
#ADD_DEFINITIONS(-DBERKELIUM_TRACE_C_RETURN)
#ADD_DEFINITIONS(-DBERKELIUM_TRACE_C_ENTER)
#ADD_DEFINITIONS(-DBERKELIUM_TRACE_C_LEAVE)
ADD_DEFINITIONS(-DBERKELIUM_TRACE_CPP_ENTER)
ADD_DEFINITIONS(-DBERKELIUM_TRACE_CPP_LEAVE)

#ADD_DEFINITIONS(-DBERKELIUM_TRACE_IO_SELECT)
#ADD_DEFINITIONS(-DBERKELIUM_TRACE_IO_DATA)
#ADD_DEFINITIONS(-DBERKELIUM_NO_HOST_REDIRECT)

################
## System #DEFINES
## -------------
IF(CMAKE_HOST_WIN32)
	# win32 AND win64
	ADD_DEFINITIONS(/DOS_WINDOWS)
ELSEIF(CMAKE_HOST_UNIX)
	ADD_DEFINITIONS(/DOS_UNIX)
ELSE()
	MESSAGE(FATAL_ERROR "host system not supported!")
ENDIF()

IF("${CMAKE_SIZEOF_VOID_P}" EQUAL "4")
	ADD_DEFINITIONS(/DOS_32)
	SET(BERKELIUM_BITS 32)
ELSEIF("${CMAKE_SIZEOF_VOID_P}" EQUAL "8")
	ADD_DEFINITIONS(/DOS_64)
	SET(BERKELIUM_BITS 64)
ELSE()
	MESSAGE(FATAL_ERROR "Only 32 and 64 Bit Systems are supported!")
ENDIF()

################
## Librarys
## -------------
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
    SET(BERKELIUM_CPP_LIBS berkelium-cpp Ws2_32.lib)
ELSE()
    SET(BERKELIUM_CPP_LIBS berkelium-cpp)
ENDIF()

#################
## System Defines
## --------------
IF("${CMAKE_SYSTEM_NAME}" STREQUAL "Linux")
	ADD_DEFINITIONS(-DLINUX)
	SET(BERKELIUM_SYSTEM "linux")
	SET(BERKELIUM_LIB_PREFIX "lib")
	SET(BERKELIUM_LIB_SUFFIX ".so")
ELSEIF("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
	ADD_DEFINITIONS(-DWINDOWS -DNOMINMAX)
	SET(BERKELIUM_SYSTEM "windows")
	SET(BERKELIUM_LIB_PREFIX "")
	SET(BERKELIUM_LIB_SUFFIX ".dll")
ELSE()
	MESSAGE(FATAL_ERROR "Only Linux Systems are supported!")
ENDIF()

SET(BERKELIUM_PLATFORM "${BERKELIUM_SYSTEM}${BERKELIUM_BITS}")
SET(BERKELIUM_CHROMIUM_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../chromium/src/out/Debug")
SET(BERKELIUM_DIST_DIR "${CMAKE_CURRENT_BINARY_DIR}/run")
SET(BERKELIUM_DIST_NAME "berkelium-${BERKELIUM_PLATFORM}.zip")
SET(BERKELIUM_DIST_ZIP "${BERKELIUM_DIST_DIR}/${BERKELIUM_DIST_NAME}")

###########
## Berkelium API
## --------
ADD_SUBDIRECTORY(../berkelium-api build/berkelium-api)

###########
## Berkelium C/C++ API
## --------
ADD_SUBDIRECTORY(../berkelium-cpp build/berkelium-cpp)

###########
## Berkelium Host
## --------
ADD_SUBDIRECTORY(../berkelium-host build/berkelium-host)

###########
## Berkelium Java API
## --------
FIND_PACKAGE(JNI)

IF(JNI_FOUND)
	# Eclipse: Find JNI Includes...
	INCLUDE_DIRECTORIES(${JNI_INCLUDE_DIRS})
	#MESSAGE(STATUS "JNI_INCLUDE_DIRS=${JNI_INCLUDE_DIRS}")
	#MESSAGE(STATUS "JNI_LIBRARIES=${JNI_LIBRARIES}")
	ADD_SUBDIRECTORY(../berkelium-java build/berkelium-java)
ELSE()
	MESSAGE(WARNING "No JDK found: skipping berkelium-java!")
ENDIF()

###########
## Berkelium Tests
## --------
ENABLE_TESTING(TRUE)
ADD_TEST(test-fake run/berkelium-cpp-test --berkelium-host=run/host-fake)
ADD_TEST(test-real run/berkelium-cpp-test)
