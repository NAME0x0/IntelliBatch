# Source files
set(ITD_SOURCES
    main.cpp
    app.cpp
    mainframe.cpp
    terminal/terminalwx.cpp
    widgets/widgetmanager.cpp
    widgets/clockwidget.cpp
    widgets/sysmonwidget.cpp
    explorer/yaziexplorer.cpp
    ui/taskbar.cpp
    ui/tilingmanager.cpp
    lua/luascript.cpp
    search/indexer.cpp
    search/searchbar.cpp
    config/configmanager.cpp
)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Add sources to main application target
target_sources(ITD PRIVATE ${ITD_SOURCES})

# Create directories for organization
add_custom_command(
    OUTPUT create_directories
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/terminal
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/widgets
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/explorer
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/ui
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/lua
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/search
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_SOURCE_DIR}/config
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/terminal
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/widgets
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/explorer
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/ui
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/lua
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/search
    COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_SOURCE_DIR}/include/config
)

add_custom_target(
    ensure_directories
    DEPENDS create_directories
)

add_dependencies(ITD ensure_directories) 