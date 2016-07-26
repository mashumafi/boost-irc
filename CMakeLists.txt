# https://www.johnlamp.net/cmake-tutorial-6-realistically-getting-a-boost.html
# Boost_DIR=/home/ubuntu/workspace/boost_1_61_0/

cmake_minimum_required(VERSION 2.8)
find_package (Threads)
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS system thread regex)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

if(Boost_FOUND)
    include_directories(SYSTEM ${Boost_INCLUDE_DIRS})
    add_executable(bot.out bot.cpp irc.cpp twitch.cpp sqlite3.c)
    target_link_libraries(bot.out ${Boost_LIBRARIES} dl)
endif()
