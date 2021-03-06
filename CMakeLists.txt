# https://www.johnlamp.net/cmake-tutorial-6-realistically-getting-a-boost.html
# Boost_DIR=/home/ubuntu/workspace/boost_1_61_0/
# cmake . -G "Visual Studio 14 Win64"
# ./bootstrap.bat
# ./b2 -j8 --toolset=msvc-14.0 address-model=64 --build-type=complete stage --with-python

cmake_minimum_required(VERSION 2.8)

project(twitch)

find_package (Threads)

if(WIN32)
  set(BOOST_ROOT       C:/Boost/boost_1_61_0/)
  set(BOOST_INCLUDEDIR C:/Boost/boost_1_61_0/)
  set(BOOST_LIBRARYDIR C:/Boost/boost_1_61_0/stage/lib/)
  
  set(Boost_INCLUDE_DIRS C:/Boost/boost_1_61_0/)
  set(Boost_LIBRARY_DIRS C:/Boost/boost_1_61_0/stage/lib/)
  
  add_definitions(-DBOOST_ALL_NO_LIB)
  set(Boost_USE_STATIC_LIBS   ON)
else()
  set(Boost_USE_STATIC_LIBS  OFF)
endif()
set(Boost_USE_MULTITHREADED   ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost COMPONENTS system thread regex filesystem)

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

if(UNIX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=gnu++0x")
endif()

if(Boost_FOUND)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
    include_directories(${Boost_INCLUDE_DIRS})
    add_subdirectory(web_server)
    include_directories(web_server)
    add_subdirectory(web_socket_server)
    include_directories(web_socket_server)
    add_subdirectory(irc)
    include_directories(irc)
    add_subdirectory(http_client)
    include_directories(http_client)
    add_subdirectory(sqlite3)
    include_directories(sqlite3)
    add_subdirectory(src)
endif()
