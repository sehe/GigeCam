cmake_minimum_required(VERSION 2.8.4)
project(OpenIPCam)
#set(Boost_DEBUG 1)
set( Boost_USE_STATIC_LIBS OFF )
set( Boost_USE_MULTITHREADED ON )

find_package(Boost 1.54.0 REQUIRED COMPONENTS system filesystem)

include_directories(${Boost_INCLUDE_DIRS})

link_directories(${Boost_LIBRARY_DIRS})
#MESSAGE( "boost lib dir:         " ${Boost_LIBRARY_DIRS} )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lboost_system -lboost_thread")

set(SOURCE_FILES main.cpp GigeCamera.cpp GigeCamera.h)
add_executable(OpenIPCam ${SOURCE_FILES})

target_link_libraries(OpenIPCam ${Boost_LIBRARIES})