#pragma once

#include <iostream>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

/**
 * @brief Command line helper
 *
 */
class CommandLine {
public:
    /**
     * @brief Run command line from argc and arg
     *
     * @param argc
     * @param argv
     * @return true Able to parse
     * @return false Unable to parse input input
     */
    bool run(int argc, char* argv[])
    {
        const boost::filesystem::path applicationName{argv[0]};
        printf(" ~* %s  *~\n", applicationName.filename().string().c_str());

        boost::program_options::variables_map variablesMap;

        try {
            store(boost::program_options::parse_command_line(argc, argv, _description), variablesMap);
            notify(variablesMap);
        } catch (const std::exception& exception) {
            std::cerr << "Error: " << exception.what() << std::endl;
            std::cerr << _description;
            return false;
        }

        if (variablesMap.count("help")) {
            std::cout << _description << std::endl;
            return false;
        }

        return true;
    }

    /**
     * @brief Return singleton
     *
     * @return CommandLine*
     */
    static CommandLine* self()
    {
        static CommandLine commandLine;
        return &commandLine;
    }

    // Connection string argument
    std::string connectionString;

private:
    CommandLine()
        : _description("Options")
    {
        _description.add_options()("help,h", "This help screen.")(
            "conn", boost::program_options::value<std::string>(&connectionString)->required(),
            "Link configuration with format "
            "(serial|udp):(device|IP):(baudrate|port).\nE.g: "
            "serial:/dev/ttyUSB0:115200 "
            "udp:192.168.2.2:12345");
    };

    boost::program_options::options_description _description;
};
