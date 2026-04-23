find_package(GTest REQUIRED)

add_executable(ui-unit-tests
    controllers/channel_controller_test.cpp
    controllers/message_controller_test.cpp
    state/channel_state_test.cpp
    state/message_state_test.cpp
    state/session_state_test.cpp
    services/auth_service_test.cpp
    ../client-ui/src/controllers/ChannelController.cpp
    ../client-ui/src/controllers/MessageController.cpp
    ../client-ui/src/services/AuthService.cpp
    ../client-ui/src/services/ConnectionService.cpp
)

target_include_directories(ui-unit-tests PRIVATE
    ${PROJECT_SOURCE_DIR}/client-ui
    ${PROJECT_SOURCE_DIR}/client-ui/src
    ${PROJECT_SOURCE_DIR}/common
)

target_link_libraries(ui-unit-tests PRIVATE
    app-common
    commonLib
    clientLib
    Qt6::Core
    Qt6::Network
    Qt6::WebSockets
    GTest::gtest_main
)

target_compile_definitions(ui-unit-tests PRIVATE
    CLIENT_UI_LOCAL_CLIENT_CONFIG_PATH="${PROJECT_SOURCE_DIR}/config/client.json"
    CLIENT_UI_BACKEND_CLIENT_CONFIG_PATH="${CLIENT_UI_BACKEND_REPO_DIR}/config/client.json"
)

include(GoogleTest)
gtest_discover_tests(ui-unit-tests)
