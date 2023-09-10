################################################### PROJECT ###################################################
set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.16)
project(SQLpp VERSION 0.1 DESCRIPTION "A simple C++ 17 ORM for SQLite3")

################################################### LIBRARY ###################################################
add_library(${PROJECT_NAME}
        lib/core/types.h lib/core/macros.h lib/core/db.h
        lib/table/table.h lib/table/column.h lib/table/foreignkey.h
        lib/queries/operations/select.h lib/queries/operations/delete.h
        lib/queries/operations/update.h lib/queries/operations/insert.h
        lib/queries/expression.h lib/queries/result.h lib/queries/base.h)

set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER "")

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(${PROJECT_NAME}.pc.in ${PROJECT_NAME}.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

################################################### TESTING ###################################################
add_executable(${PROJECT_NAME}_test main.cpp)
target_include_directories(${PROJECT_NAME}_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_link_libraries(${PROJECT_NAME}_test sqlite3)