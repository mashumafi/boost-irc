add_library(connection connection.cpp connection.hpp)

add_library(connection_manager connection_manager.cpp connection_manager.hpp)
target_link_libraries(connection_manager connection)
target_link_libraries(connection_manager request_parser)

add_library(mime_types mime_types.cpp mime_types.hpp)

add_library(reply reply.cpp reply.hpp)

add_library(request_handler request_handler.cpp request_handler.hpp)
target_link_libraries(request_handler reply)
target_link_libraries(request_handler mime_types)

add_library(request_parser request_parser.cpp request_parser.hpp)

add_library(server server.cpp server.hpp)
target_link_libraries(server connection_manager)
target_link_libraries(server request_handler)
