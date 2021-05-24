# ping-cpp

[![Travis Test](https://travis-ci.com/bluerobotics/ping-cpp.svg?branch=master)](https://travis-ci.com/github/bluerobotics/ping-cpp)
[![Linux hardware test](https://github.com/bluerobotics/ping-cpp/actions/workflows/hardware-test.yml/badge.svg)](https://github.com/bluerobotics/ping-cpp/actions/workflows/hardware-test.yml)

C++ API implementation of [ping-protocol](https://docs.bluerobotics.com/ping-protocol/).

This repository has the ping_message base class, and templates + scripts to automatically generate device-specific message subclasses.

## Usage

#### Generated files

If you would like to use the C++ API in your project, checkout the [deployment branch](https://github.com/bluerobotics/ping-cpp/tree/deployment).

#### CMake project

The **master branch** can be used directly in CMake based projects for desktop or SBC applications.
##### Example

```sh
# Clone the repository
git clone https://github.com/bluerobotics/ping-cpp
cd ping-cpp
# Update the submodules
git submodule update --init --recursive
# Configure and build the project in debug mode
cmake -B build -DCMAKE_BUILD_TYPE=Debug && cmake --build build --parallel --config Debug
# Connect a Ping device and use the correct USB as argument
./build/test-device --conn serial:/dev/ttyUSB0:115200
# If you have a Ping1D connected, you can run the test program for the device
./build/test-device-ping1d --conn serial:/dev/ttyUSB0:115200
```
