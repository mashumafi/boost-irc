add_library(web_server server_http_impl.cpp server_http_impl.hpp)
target_link_libraries(web_server ${Boost_LIBRARIES})
target_link_libraries(web_server ${OPENSSL_CRYPTO_LIBRARY})
