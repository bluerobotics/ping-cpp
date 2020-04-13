#pragma once

#include <functional>
#include <string>

#include <fmt/color.h>
#include <fmt/core.h>

class PingDeviceTest {
public:
    enum class Status {
        // Something is wrong (invalid test)
        ERROR = -1,
        // It's a valid value but we can't make sure (generic test pass)
        OK = 0,
        // We are sure that the value is correct (value must be exact)
        SUCCESS,
    };

    static void test(std::string testName, bool isOk, Status goodStatus = Status::SUCCESS) {
        Status status = isOk ? goodStatus : Status::ERROR;
        fmt::text_style style;
        std::string result;
        switch(status) { 
            case Status::OK:
                style = fg(fmt::color::white);
                result = "OK";
                break;
            case Status::SUCCESS:
                style = fg(fmt::terminal_color::bright_green);
                result = "SUCCESS";
                break;
            default:
                style = fmt::emphasis::bold | fg(fmt::color::red);
                result = "ERROR";
        }
        fmt::print(style, "Test {}: {}\n", testName, result);

        if (status == Status::ERROR) {
            throw std::runtime_error(std::string("Test failed: ") + testName);
        }
    };

    static void test(std::string testName, std::function<bool(void)> function, Status goodStatus = Status::SUCCESS) {
        test(testName, function(), goodStatus);
    };
};
