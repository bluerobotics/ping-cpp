cmake_minimum_required(VERSION 3.12)
project(ping-cpp)

set(CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror")
include_directories(src/device src/hal src/message)

set(DEVICE_SRC
  src/device/ping-device.cpp
  src/hal/ping-port-linux.cpp
  src/hal/ping-time-linux.cpp
)

add_executable(test-device test/test-device.cpp ${DEVICE_SRC})
add_executable(test-device-ping1d test/test-device-ping1d.cpp src/device/ping-device-ping1d.cpp ${DEVICE_SRC})
add_executable(test-message test/test-message.cpp)
