set(SHARED_SOURCES
    services/WebSocketService.cpp
    services/MessageService.cpp
    services/ChannelService.cpp
    services/UserService.cpp
    state/AppState.cpp
    state/SessionState.cpp
    state/ChannelState.cpp
    state/MessageState.cpp
    events/UIEventDispatcher.cpp
    events/EventMapper.cpp
    events/EventHandler.cpp
)

add_library(app-shared STATIC ${SHARED_SOURCES})

# Expose this directory as an include root so consumers write:
#   #include "services/WebSocketService.h"
target_include_directories(app-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app-shared PUBLIC
    Qt6::Core
    Qt6::Network
    Qt6::WebSockets
)
