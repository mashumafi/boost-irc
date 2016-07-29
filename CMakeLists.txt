# https://www.johnlamp.net/cmake-tutorial-6-realistically-getting-a-boost.html
# Boost_DIR=/home/ubuntu/workspace/boost_1_61_0/

cmake_minimum_required(VERSION 2.8)

find_package (Threads)

if(WIN32)
  set(BOOST_ROOT       C:/Boost/boost_1_61_0/)
  set(BOOST_INCLUDEDIR C:/Boost/boost_1_61_0/)
  set(BOOST_LIBRARYDIR C:/Boost/boost_1_61_0/stage/lib/)
  
  set(Boost_INCLUDE_DIRS C:/Boost/boost_1_61_0/)
  set(Boost_LIBRARY_DIRS C:/Boost/boost_1_61_0/stage/lib/)
  
  add_definitions(-DBOOST_ALL_NO_LIB)
  set(Boost_USE_STATIC_LIBS     ON)
else()
  set(Boost_USE_STATIC_LIBS    OFF)
endif()
set(Boost_USE_MULTITHREADED   ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread regex filesystem)

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    include_directories(irc)
    include_directories(http_client)
    include_directories(http_server)
    include_directories(sqlite3)
    add_subdirectory(irc)
    add_subdirectory(http_client)
    add_subdirectory(http_server)
    add_subdirectory(sqlite3)
    add_executable(bot bot.cpp)
    target_link_libraries(bot irc_client twitch_client sqlite3 http_client json_client server)
endif()
