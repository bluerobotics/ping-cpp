cmake_minimum_required(VERSION 3.12)
project(ping-cpp)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror")
include_directories(src/device src/hal src/message)

set(DEVICE_SRC
  src/device/ping-device.cpp
  src/hal/ping-port-linux.cpp
  src/hal/ping-time-linux.cpp
  # add ping-message header as a dependency here to trigger the generate step
  src/message/ping-message-common.h
)

add_executable(test-device test/test-device.cpp ${DEVICE_SRC})
add_executable(test-device-ping1d test/test-device-ping1d.cpp src/device/ping-device-ping1d.cpp ${DEVICE_SRC})
add_executable(test-message test/test-message.cpp)

set(PINGCPP_SRC ${PROJECT_SOURCE_DIR}/src)

add_custom_command(OUTPUT ${PINGCPP_SRC}/device/ping-device-ping1d.cpp
  COMMAND ${PINGCPP_SRC}/generate/generate-device.py --output-dir ${PINGCPP_SRC}/device
  COMMENT "generating ping-cpp device"
)

add_custom_command(OUTPUT ${PINGCPP_SRC}/message/ping-message-common.h
  COMMAND ${PINGCPP_SRC}/generate/generate-message.py --output-dir ${PINGCPP_SRC}/message
  COMMENT "generating ping-cpp message"
)
