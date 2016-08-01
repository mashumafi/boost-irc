add_library(web_socket_server server_ws_impl.cpp server_ws_impl.hpp)
target_link_libraries(web_socket_server ${Boost_LIBRARIES})
target_link_libraries(web_socket_server ${OPENSSL_CRYPTO_LIBRARY})
