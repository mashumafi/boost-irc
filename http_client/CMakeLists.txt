add_library(http_client http_client.cpp http_client.hpp)

add_library(json_client json_client.cpp json_client.hpp)
target_link_libraries(json_client http_client)
