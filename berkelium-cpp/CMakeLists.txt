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
## MSVC
## -------------
IF(MSVC)
	# switch statement contains 'default' but no 'case' labels
	ADD_DEFINITIONS(/wd4065)
	# nonstandard extension used: enum 'Berkelium::Ipc::CommandId' used in qualified name
	ADD_DEFINITIONS(/wd4482)
ENDIF()

################
## System #DEFINES
## -------------
IF(CMAKE_HOST_WIN32)
	# win32 AND win64
	ADD_DEFINITIONS(/DOS_WINDOWS)
	IF(CMAKE_CL_64)
		ADD_DEFINITIONS(/DOS_64)
	ELSE()
		ADD_DEFINITIONS(/DOS_32)
	ENDIF()
ELSE()
	MESSAGE(FATAL_ERROR "host system not supported!")
ENDIF()

################
## Librarys
## -------------
SET(BERKELIUM_CPP_LIBS berkelium-cpp)

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
ADD_SUBDIRECTORY(src/fake build/fake)
ADD_SUBDIRECTORY(src/test build/test)
ADD_SUBDIRECTORY(src/demo build/demo)

ENABLE_TESTING(TRUE)
ADD_TEST(test-fake run/berkelium-cpp-test --berkelium-host=run/host-fake)
ADD_TEST(test-real run/berkelium-cpp-test)
