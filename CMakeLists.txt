set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.16)
project(SQLpp VERSION 0.1 DESCRIPTION "A simple C++ 17 ORM for SQLite3")

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE sqlite3)
target_include_directories(${PROJECT_NAME} INTERFACE lib)
target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

install(DIRECTORY lib/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})

add_executable(${PROJECT_NAME}_test main.cpp)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME})
