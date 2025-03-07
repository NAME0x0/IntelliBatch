# Tests CMakeLists.txt for ITD

# Find GoogleTest
find_package(GTest QUIET)
if(NOT GTEST_FOUND)
    message(STATUS "GoogleTest not found, downloading...")
    
    # Download and include GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.13.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Enable CTest
enable_testing()

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Utility function to add a test with common settings
function(add_itd_test TEST_NAME TEST_SOURCE)
    add_executable(${TEST_NAME} ${TEST_SOURCE})
    target_link_libraries(${TEST_NAME} PRIVATE 
        gtest 
        gtest_main
        ${wxWidgets_LIBRARIES}
        ${LUA_LIBRARIES}
    )
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Add unit tests
file(GLOB UNIT_TEST_SOURCES "unit/*_test.cpp")
foreach(TEST_SOURCE ${UNIT_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_itd_test(${TEST_NAME} ${TEST_SOURCE})
endforeach()

# Add integration tests
file(GLOB INTEGRATION_TEST_SOURCES "integration/*_test.cpp")
foreach(TEST_SOURCE ${INTEGRATION_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_itd_test(${TEST_NAME} ${TEST_SOURCE})
endforeach()

# Coverage target
if(ENABLE_COVERAGE)
    if(CMAKE_COMPILER_IS_GNUCXX)
        # Setup coverage flags
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage -lgcov")
        
        # Add coverage target
        add_custom_target(coverage
            COMMAND mkdir -p ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${CMAKE_MAKE_PROGRAM} test
            COMMAND lcov --capture --directory ${CMAKE_BINARY_DIR} --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
            COMMAND lcov --remove ${CMAKE_BINARY_DIR}/coverage/coverage.info '/usr/*' --output-file ${CMAKE_BINARY_DIR}/coverage/coverage.info
            COMMAND genhtml ${CMAKE_BINARY_DIR}/coverage/coverage.info --output-directory ${CMAKE_BINARY_DIR}/coverage
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMENT "Generating coverage report"
        )
    else()
        message(WARNING "Coverage is only supported with GCC")
    endif()
endif() 