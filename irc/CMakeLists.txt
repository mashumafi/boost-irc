add_library(irc_client irc_client.cpp irc_client.hpp)
target_link_libraries(irc_client ${Boost_LIBRARIES})
add_library(twitch_client twitch_client.cpp twitch_client.hpp)
target_link_libraries(twitch_client ${Boost_LIBRARIES})
