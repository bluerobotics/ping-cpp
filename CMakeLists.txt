cmake_minimum_required(VERSION 3.12)
project(ping-cpp LANGUAGES CXX)

# Use standard C++ version and remove extensions to allow better compatibility
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_EXTENSIONS OFF)

# Enable compile checks for msvc, clang and gcc
if(MSVC)
  add_compile_options(/W4 /WX)
else()
  add_compile_options(-Wall -Wextra -pedantic -Werror)
  # Can be removed moving to C++17, or we could move this library out from header only
  add_compile_options(-Wno-unused-function)
endif()

# Used for the command line interface
find_package(Boost 1.67 COMPONENTS filesystem program_options REQUIRED)
include_directories(${Boost_INCLUDE_DIR})

# Include modules
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

option(PING_CPP_BUILD_TESTS "" ON)
message("Building tests: ${PING_CPP_BUILD_TESTS}")

if (PING_CPP_BUILD_TESTS)
    # Add fmt subdirectory
    add_subdirectory(lib/fmt/)

    # test-device
    add_executable(test-device)
    target_include_directories(test-device PRIVATE lib/fmt/include/)
    target_sources(
        test-device
    PRIVATE
        test/test-device.cpp
    )
    target_link_libraries(
        test-device
    PRIVATE
        DEVICE
        HAL
        PING_MESSAGES
        ${Boost_LIBRARIES}
        fmt::fmt
    )

    # test-device-ping1d
    add_executable(test-device-ping1d)
    target_include_directories(test-device-ping1d PRIVATE lib/fmt/include/)
    target_sources(
        test-device-ping1d
    PRIVATE
        test/test-device-ping1d.cpp
    )
    target_link_libraries(
        test-device-ping1d
    PRIVATE
        DEVICE
        HAL
        PING_MESSAGES
        ${Boost_LIBRARIES}
        fmt::fmt
    )

    # test-device-ping360
    add_executable(test-device-ping360)
    target_include_directories(test-device-ping360 PRIVATE lib/fmt/include/)
    target_sources(
        test-device-ping360
    PRIVATE
        test/test-device-ping360.cpp
    )
    target_link_libraries(
        test-device-ping360
    PRIVATE
        DEVICE
        HAL
        PING_MESSAGES
        ${Boost_LIBRARIES}
        fmt::fmt
    )

    # test-message
    add_executable(test-message)
    target_include_directories(test-message PRIVATE lib/fmt/include/)
    target_sources(
        test-message
    PRIVATE
        test/test-message.cpp
    )
    target_link_libraries(
        test-message
    PRIVATE
        PING_MESSAGES
    )
endif()
