cmake_minimum_required(VERSION 3.16)
project(advanced-cpp-2026-ui VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
option(BUILD_ADMIN_UI "Build the optional admin/server UI extension." OFF)
option(CLIENT_UI_USE_BACKEND_REPO "Link client-ui with backend commonLib/clientLib from the sibling repository." ON)
option(BUILD_UI_TESTS "Build the client UI unit tests." OFF)
set(CLIENT_UI_BACKEND_REPO_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../advanced-cpp-2026" CACHE PATH
    "Path to the backend repository used by client-ui.")

# Export compile commands for clang-tidy / IDE tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Network WebSockets)

add_subdirectory(common)

if(CLIENT_UI_USE_BACKEND_REPO)
    if(NOT EXISTS "${CLIENT_UI_BACKEND_REPO_DIR}/common/CMakeLists.txt"
       OR NOT EXISTS "${CLIENT_UI_BACKEND_REPO_DIR}/client/CMakeLists.txt")
        message(FATAL_ERROR
            "Backend repository was not found at '${CLIENT_UI_BACKEND_REPO_DIR}'. "
            "Expected sibling repo '../advanced-cpp-2026'.")
    endif()

    set(BUILD_SERVER_BACKEND OFF CACHE BOOL "Build the server backend executable." FORCE)
    set(BUILD_CLIENT_BACKEND OFF CACHE BOOL "Build the client backend smoke-test executable." FORCE)
    set(BUILD_BACKEND_TESTS OFF CACHE BOOL "Build the backend test suite." FORCE)

    add_subdirectory("${CLIENT_UI_BACKEND_REPO_DIR}/common" "${CMAKE_BINARY_DIR}/backend-common")
    add_subdirectory("${CLIENT_UI_BACKEND_REPO_DIR}/client" "${CMAKE_BINARY_DIR}/backend-client")
endif()

add_subdirectory(client-ui)
if(BUILD_ADMIN_UI AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/admin-ui/CMakeLists.txt")
    add_subdirectory(admin-ui)
elseif(BUILD_ADMIN_UI)
    message(WARNING "BUILD_ADMIN_UI is ON, but admin-ui is not present in this MVP checkout.")
endif()

if(BUILD_UI_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
