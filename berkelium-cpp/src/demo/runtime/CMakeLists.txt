CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(berkelium-demo-runtime)

ADD_EXECUTABLE(demo-runtime1-cpp runtime1.cpp)
TARGET_LINK_LIBRARIES(demo-runtime1-cpp ${BERKELIUM_CPP_LIBS})

ADD_EXECUTABLE(demo-runtime1-c runtime1.c)
TARGET_LINK_LIBRARIES(demo-runtime1-c ${BERKELIUM_CPP_LIBS})

ADD_EXECUTABLE(demo-runtime2-cpp runtime2.cpp)
TARGET_LINK_LIBRARIES(demo-runtime2-cpp ${BERKELIUM_CPP_LIBS})

ADD_EXECUTABLE(demo-runtime2-c runtime2.c)
TARGET_LINK_LIBRARIES(demo-runtime2-c ${BERKELIUM_CPP_LIBS})
