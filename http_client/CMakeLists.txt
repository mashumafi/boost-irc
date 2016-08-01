add_library(http_client http_client.cpp http_client.hpp json_client.cpp json_client.hpp)
target_link_libraries(http_client ${Boost_LIBRARIES})
