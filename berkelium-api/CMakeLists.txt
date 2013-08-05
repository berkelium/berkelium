CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

PROJECT(berkelium-api)

IF(NOT DEFINED BERKELIUM_BUILD)
	MESSAGE(FATAL_ERROR "this project should be build using berkelium build system!")
ENDIF()

INCLUDE(CMakeParseArguments)

FIND_PROGRAM(XSLTPROC xsltproc DOC "xsltproc transforms XML via XSLT")

SET(API ${CMAKE_CURRENT_SOURCE_DIR}/api CACHE PATH "berkelium api files")

SET_DIRECTORY_PROPERTIES(PROPERTIES CLEAN_NO_CUSTOM 1)

MACRO(XSLT TARGET XSLT XML)
	CMAKE_PARSE_ARGUMENTS(ARG "" "" "ARGS" ${ARGN})

	SET(ARGS "")
	LIST(LENGTH ARG_ARGS to)
	SET(i 0)

	WHILE(${i} LESS ${to})
		LIST(GET ARG_ARGS ${i} key)
		MATH(EXPR i "${i} + 1")

		LIST(GET ARG_ARGS ${i} value)
		MATH(EXPR i "${i} + 1")

		LIST(APPEND ARGS "--stringparam")
		LIST(APPEND ARGS "${key}")
		LIST(APPEND ARGS "${value}")
	ENDWHILE()

	LIST(APPEND ARGS "-o")
	LIST(APPEND ARGS "${TARGET}")
	LIST(APPEND ARGS "${XSLT}")
	LIST(APPEND ARGS "${XML}")

	ADD_CUSTOM_COMMAND(
		OUTPUT "${TARGET}"
		COMMAND "${XSLTPROC}" ${ARGS}
		DEPENDS "${XSLT}" "${XML}"
		COMMENT "xslt ${TARGET}"
	)
ENDMACRO()

MACRO(API TARGET XSLT)
	GET_FILENAME_COMPONENT(FILE "${TARGET}" ABSOLUTE)
	CMAKE_PARSE_ARGUMENTS(ARG "" "" "ARGS" ${ARGN})
	XSLT("${FILE}" "${XSLT}" "${API}/api.xml" ARGS "${ARG_ARGS}")
ENDMACRO()

###########
## Build API Dependencies
## --------
SET(API_DEPS "")

MACRO(BUILD_API_DEPS VAR TARGET LANG)
	# build absolute path
	GET_FILENAME_COMPONENT(FILE "${TARGET}" ABSOLUTE)
	# append to API_DEPS
	LIST(APPEND API_DEPS "${FILE}")
	XSLT("${FILE}" "${API}/api-all.xslt" "${API}/api.xml" ARGS "list" "${LANG}")

	# read deps into VAR
	# you need to recall cmake if you added new files to the api!!
	FILE(STRINGS "${FILE}" "${VAR}")
	SET("${VAR}" "${${VAR}}" CACHE STRING "cached source deps" FORCE)
ENDMACRO()

BUILD_API_DEPS(BERKELIUM_SRC_CPP deps/cpp.txt cpp)
BUILD_API_DEPS(BERKELIUM_SRC_JAVA_IMPL deps/java-impl.txt java-impl)

###########
## Build JNI Implementation
## --------
MACRO(BUILD_API_IMPL TARGET XSLT)
	# build absolute path
	GET_FILENAME_COMPONENT(FILE "${TARGET}" ABSOLUTE)
	# append to API_DEPS
	LIST(APPEND API_DEPS "${FILE}")
	XSLT("${FILE}" "${API}/${XSLT}" "${API}/api.xml")
ENDMACRO()

BUILD_API_IMPL("../berkelium-cpp/include/berkelium.h" "api-c.xslt")
BUILD_API_IMPL("../berkelium-cpp/include/Berkelium/API/Berkelium.hpp" "api-cpp-global.xslt")
BUILD_API_IMPL("../berkelium-cpp/src/lib/Impl/BerkeliumC.cpp" "api-c-impl.xslt")
BUILD_API_IMPL("../berkelium-java/src/main/cpp/berkelium-jni.cpp" "api-jni.xslt")

###########
## Note: rerun cmake
## --------
ADD_CUSTOM_COMMAND(
	OUTPUT rerun-cmake
	COMMAND ${CMAKE_COMMAND} -E echo_append
	COMMENT "You need to rerun cmake if you added new files!"
)

# this is not part of the default build
# call this command by hand if you changed the api
ADD_CUSTOM_TARGET(berkelium-api DEPENDS ${API_DEPS} rerun-cmake)
