# Check if python is installed with the necessary components
find_package(Python3 QUIET REQUIRED COMPONENTS Interpreter)

execute_process(
    COMMAND ${Python3_EXECUTABLE} -m pip show jinja2
    OUTPUT_QUIET
    RESULT_VARIABLE EXIT_CODE
)

if(NOT ${EXIT_CODE} EQUAL 0)
    message(
        WARNING
        "The Python3 package \"jinja2\" is not installed. We are going to install it."
    )

    execute_process(
        COMMAND ${Python3_EXECUTABLE} -m pip install jinja2 --user
        RESULT_VARIABLE EXIT_CODE
    )

    if(NOT ${EXIT_CODE} EQUAL 0)
        message(
            FATAL_ERROR
            "Failed to install the Python3 package \"jinja2\". Make sure that pip3 is installed and working."
        )
    endif()

endif()
