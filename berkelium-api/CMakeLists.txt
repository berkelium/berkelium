CMAKE_MINIMUM_REQUIRED(VERSION 2.8.3)

PROJECT(berkelium-api)

IF(NOT DEFINED BERKELIUM_BUILD)
	MESSAGE(FATAL_ERROR "this project should be build using berkelium build system!")
ENDIF()

INCLUDE(CMakeParseArguments)

FIND_PROGRAM(XSLTPROC xsltproc DOC "xsltproc transforms XML via XSLT")

SET(API ${CMAKE_CURRENT_SOURCE_DIR}/api CACHE PATH "berkelium api files")

SET(API_DEPS "${API}/api.xml")

#find_package(Java REQUIRED)

#${Java_JAVA_EXECUTABLE} -jar xslt.jar

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
	LIST(APPEND API_DEPS "${FILE}")
	XSLT("${FILE}" "${XSLT}" "${API}/api.xml" ARGS "${ARG_ARGS}")
ENDMACRO()

###########
## Build API Dependencies
## --------
MACRO(BUILD_API_DEPS TARGET LANG)
	GET_FILENAME_COMPONENT(FILE "${TARGET}" ABSOLUTE)
	LIST(APPEND API_DEPS "${FILE}")
	XSLT("${FILE}" "${API}/api-all.xslt" "${API}/api.xml" ARGS "list" "${LANG}")
ENDMACRO()

BUILD_API_DEPS(deps/c.txt c)
BUILD_API_DEPS(deps/cpp.txt cpp)
BUILD_API_DEPS(deps/java-impl.txt java-impl)
BUILD_API_DEPS(deps/jni.txt jni)

ADD_CUSTOM_TARGET(berkelium-api DEPENDS ${API_DEPS})
