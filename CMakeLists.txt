cmake_minimum_required(VERSION 3.4)
set (CMAKE_CXX_STANDARD 17)

project(GtaOnlineSwitchSinglePlayer)

aux_source_directory(./src SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
