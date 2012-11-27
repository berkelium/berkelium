CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-cpp-pngrenderer)

FILE(GLOB_RECURSE SRC "*.cpp")

ADD_EXECUTABLE(pngrenderer ${SRC})
TARGET_LINK_LIBRARIES(pngrenderer berkelium-cpp ${Boost_LIBRARIES})
