#include "parameters.h"

#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

Parameters::Parameters(int argc, char *argv[])
{
    po::options_description generic("Options");
    generic.add_options()
            ("help,h", "Prints this message")
            ("settings,s", po::value<fs::path>()->default_value(fs::path(getenv("HOME")) / ".config/akd/akd.conf"), "Path to settings file");

    po::options_description configuration("Configuration");
    configuration.add_options()
            ("general.print-groups,p", po::bool_switch(), "Print switched languages in stdout")
            ("general.skip-rules,r", po::bool_switch(), "Do not update keyboard rules, useful if you use only this program to work with keyboard")
            ("general.layouts,l", po::value<std::vector<std::string>>()->multitoken(), "Languages, separated by ','. Can be specified several times to define several layouts.")
            ("shortcuts.nextlayout,n", po::value<std::string>(), "Switch to next layout");


    po::options_description allOptions("Advanced keyboard daemon");
    allOptions.add(generic).add(configuration);

    store(parse_command_line(argc, argv, allOptions), m_parameters);

    if (const auto &settingsPath = m_parameters["settings"].as<fs::path>(); fs::exists(settingsPath))
        store(parse_config_file(settingsPath.c_str(), configuration), m_parameters);

    if (m_parameters.count("help")) {
        std::cout << allOptions;
        std::exit(1);
    }

    notify(m_parameters);
}

boost::program_options::variable_value &Parameters::printGroups()
{
    return m_parameters.at("general.print-groups");
}

boost::program_options::variable_value &Parameters::skipRules()
{
    return m_parameters.at("general.skip-rules");
}

boost::program_options::variable_value &Parameters::layouts()
{
    return m_parameters.at("general.layouts");
}

boost::program_options::variable_value &Parameters::nextLayoutShortcut()
{
    return m_parameters.at("shortcuts.nextlayout");
}
