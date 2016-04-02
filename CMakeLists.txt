cmake_minimum_required(VERSION 3.5)
project(HostSvc)

if (WIN32)
    set(ASIO_INCLUDE "D:/asio-1.10.6/include" CACHE PATH "ASIO库的include目录")
    include_directories(${ASIO_INCLUDE})

endif ()

find_package(Threads REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp HostSvcCommon.h)
add_executable(HostSvc ${SOURCE_FILES})
add_executable(Client client.cpp HostSvcCommon.h)
target_link_libraries(Client ${CMAKE_THREAD_LIBS_INIT})