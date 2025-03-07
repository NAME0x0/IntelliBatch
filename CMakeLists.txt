cmake_minimum_required(VERSION 3.15)
project(ITD VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add compile options
if(MSVC)
    add_compile_options(/W4 /MP)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

# Find wxWidgets
find_package(wxWidgets REQUIRED COMPONENTS core base stc aui adv)
include(${wxWidgets_USE_FILE})

# Find Lua
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Add subdirectories
add_subdirectory(src)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

# Resource files
file(GLOB_RECURSE RESOURCE_FILES "res/*.*")

# Main application target
add_executable(ITD 
    src/main.cpp
    ${RESOURCE_FILES}
)

# Link libraries
target_link_libraries(ITD PRIVATE 
    ${wxWidgets_LIBRARIES}
    ${LUA_LIBRARIES}
)

# Windows specific
if(WIN32)
    set_target_properties(ITD PROPERTIES WIN32_EXECUTABLE TRUE)
    target_compile_definitions(ITD PRIVATE -D_UNICODE)
endif()

# Copy resource files to build directory
add_custom_command(
    TARGET ITD POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/res
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/res
)

# Install
install(TARGETS ITD DESTINATION bin)
install(DIRECTORY res/ DESTINATION bin/res)

# Testing
option(BUILD_TESTS "Build the tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Documentation
option(BUILD_DOCS "Build the documentation" OFF)
if(BUILD_DOCS)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        add_subdirectory(docs)
    endif()
endif()

# Package information
set(CPACK_PACKAGE_NAME "ITD")
set(CPACK_PACKAGE_VENDOR "Your Organization")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Integrated Terminal Desktop")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_INSTALL_DIRECTORY "ITD")
include(CPack) 