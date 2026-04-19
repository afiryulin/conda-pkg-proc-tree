cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME proc-tree)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES src/*.cpp)

find_package(fmt REQUIRED)

add_executable(${PROJECT_NAME}
    ${SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fmt::fmt
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)