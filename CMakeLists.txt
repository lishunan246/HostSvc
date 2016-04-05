cmake_minimum_required(VERSION 3.5)
project(HostSvc)

if (WIN32)
    set(ASIO_INCLUDE "D:/asio-1.10.6/include" CACHE PATH "ASIO库的include目录")
    include_directories(${ASIO_INCLUDE})
    set(PROTOBUF_INCLUDE "D:/protobuf-2.6.1/src" CACHE PATH "protobuf库的include目录")
    include_directories(${PROTOBUF_INCLUDE})

endif (WIN32)

find_package(Threads REQUIRED)

if (WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0602")
else(WIN32)

    message("LINUX")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif(WIN32)
set(SOURCE_FILES main.cpp HostSvcCommon.h)
add_executable(HostSvc ${SOURCE_FILES})
add_executable(Client client.cpp HostSvcCommon.h)
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})
