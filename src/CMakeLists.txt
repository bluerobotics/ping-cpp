# Include each sub folder
set(INCLUDE_DIRS
    device
    hal
    message
)

include(${PROJECT_SOURCE_DIR}/cmake/CheckPython.cmake)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${INCLUDE_DIRS})

foreach(directory ${INCLUDE_DIRS})
    add_subdirectory(${directory})
endforeach()
