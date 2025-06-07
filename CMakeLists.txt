AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/BreakingNews.cpp")
AC_ADD_SCRIPT("${CMAKE_CURRENT_LIST_DIR}/src/ServerStats.cpp")
AC_ADD_SCRIPT_LOADER("BreakingNews" "${CMAKE_CURRENT_LIST_DIR}/src/loader.cpp")

AC_ADD_CONFIG_FILE("${CMAKE_CURRENT_LIST_DIR}/conf/breakingnews.conf.dist") 