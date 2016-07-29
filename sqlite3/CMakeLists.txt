add_library(sqlite3 sqlite3.c sqlite3.h)
if(UNIX)
  target_link_libraries(sqlite3 dl)
endif()