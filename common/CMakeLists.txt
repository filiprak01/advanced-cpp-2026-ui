set(COMMON_SOURCES
    state/SessionState.cpp
    state/ChannelState.cpp
    state/MessageState.cpp
)

add_library(app-common STATIC ${COMMON_SOURCES})

target_include_directories(app-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(app-common PUBLIC
    Qt6::Core
)
