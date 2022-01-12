cmake_minimum_required(VERSION 3.12)
project(DEVICE)

set(GENSRC
    ${CMAKE_CURRENT_SOURCE_DIR}/ping-device-ping360.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ping-device-ping1d.cpp
)
# Generate ping-cpp devices
add_custom_command(
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../generate/generate-device.py --output-dir ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generates ping device class."
    OUTPUT ${GENSRC}
)
add_custom_target(generate-devices
    DEPENDS ${GENSRC}
)

add_library(
    DEVICE
STATIC
    ping-device-ping1d.cpp
    ping-device-ping360.cpp
    ping-device.cpp
)

include_directories(${HAL_SOURCE_DIR})

target_link_libraries(
    DEVICE
PRIVATE
    HAL
    PING_MESSAGES
)

target_include_directories(
    DEVICE
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_dependencies(DEVICE generate-devices)
