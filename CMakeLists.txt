cmake_minimum_required(VERSION 3.6)
project(Luves)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")

set(SOURCE_FILES
    examples/test_client.cpp
    examples/test_echo.cpp
        luves/test_hsha.cpp
    examples/test_logger.cpp
    examples/test_timer.cpp
    luves/buffer.cpp
    luves/buffer.h
    luves/channel.cpp
    luves/channel.h
    luves/epoll.cpp
    luves/epoll.h
    luves/eventhandle.cpp
    luves/eventhandle.h
    luves/hshaserver.cpp
    luves/hshaserver.h
    luves/kqueue.cpp
    luves/kqueue.h
    luves/logger.cpp
    luves/logger.h
    luves/luves.h
    luves/net.cpp
    luves/net.h
    luves/tcpconnection.cpp
    luves/tcpconnection.h
    luves/tcpserver.cpp
    luves/tcpserver.h
    luves/test_hsha.cpp
    luves/threadpool.cpp
    luves/threadpool.h
    luves/timer.cpp
    luves/timer.h)

add_executable(Luves ${SOURCE_FILES})