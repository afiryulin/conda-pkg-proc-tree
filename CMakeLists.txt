cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME proc-tree)
project(${PROJECT_NAME} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(fmt CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

set(SOURCES
    src/main.cpp
    src/cli/Options.cpp
    src/proc/JsonExporter.cpp
    src/proc/ProcessPrinter.cpp
    src/proc/ProcessTreeBuilder.cpp
)

if(WIN32)
    list(APPEND SOURCES src/proc/WindowsProcessProvider.cpp)
elseif(APPLE)
    list(APPEND SOURCES src/proc/MacProcessProvider.cpp)
elseif(UNIX)
    list(APPEND SOURCES src/proc/LinuxProcessProvider.cpp)
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        fmt::fmt
        nlohmann_json::nlohmann_json
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)