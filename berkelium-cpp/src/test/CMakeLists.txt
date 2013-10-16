CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-cpp-test)

###############
## Source Files
## ------------
FILE(GLOB_RECURSE SRC_TEST "*.cpp" "*.h")

##############
## Google Test
## -----------
SET(GTEST_ROOT "../../../gtest")
ADD_SUBDIRECTORY(${GTEST_ROOT} ${GTEST_ROOT})
INCLUDE_DIRECTORIES(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

ADD_DEFINITIONS(-DBERKELIUM_CPP_IMPL)
ADD_EXECUTABLE(berkelium-cpp-test ${SRC_TEST} ${INCLUDE_BK})
TARGET_LINK_LIBRARIES(berkelium-cpp-test berkelium-cpp gtest ${Boost_LIBRARIES})

# VS2012 doesn't support correctly the tuples yet
if(MSVC)
  add_definitions(/D _VARIADIC_MAX=10)
endif()