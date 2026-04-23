set(CLIENT_SOURCES
    main.cpp
    src/pages/ConnectionPage.cpp
    src/pages/ChannelListPage.cpp
    src/pages/ChannelPage.cpp
    src/components/LoginFormComponent.cpp
    src/components/RegistrationFormComponent.cpp
    src/components/AuthToggleComponent.cpp
    src/components/ConnectionStatusComponent.cpp
    src/components/ChannelItemComponent.cpp
    src/components/MessageListComponent.cpp
    src/components/MessageItemComponent.cpp
    src/components/MessageInputComponent.cpp
    src/components/UserListComponent.cpp
    src/components/SidebarComponent.cpp
    src/components/HeaderBarComponent.cpp
    src/components/NotificationComponent.cpp
    src/components/LoadingComponent.cpp
    src/components/ErrorComponent.cpp
    src/controllers/ConnectionController.cpp
    src/controllers/AuthController.cpp
    src/controllers/ChannelController.cpp
    src/controllers/MessageController.cpp
    src/services/ConnectionService.cpp
    src/services/AuthService.cpp
    src/routing/Router.cpp
    src/routing/RouteGuard.cpp
)

add_executable(client-ui ${CLIENT_SOURCES})

target_include_directories(client-ui PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(client-ui PRIVATE
    app-common
    commonLib
    clientLib
    Qt6::Core
    Qt6::Widgets
    Qt6::Network
    Qt6::WebSockets
)

target_compile_definitions(client-ui PRIVATE
    CLIENT_UI_LOCAL_CLIENT_CONFIG_PATH="${PROJECT_SOURCE_DIR}/config/client.json"
    CLIENT_UI_BACKEND_CLIENT_CONFIG_PATH="${CLIENT_UI_BACKEND_REPO_DIR}/config/client.json"
)
