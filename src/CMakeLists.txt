add_executable(bot bot.cpp twitch_client.cpp twitch_client.hpp)
target_link_libraries(bot irc_client sqlite3 http_client http_server)
target_link_libraries(bot ${Boost_LIBRARIES})
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
