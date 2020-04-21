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

bool Parameters::printGroups() const
{
    return m_parameters["general.print-groups"].as<bool>();
}

bool Parameters::skipRules() const
{
    return m_parameters["general.skip-rules"].as<bool>();
}

std::optional<std::vector<std::string>> Parameters::layouts()
{
    return findOptional<std::vector<std::string>>("general.layouts");
}

std::optional<std::string> Parameters::nextLayoutShortcut()
{
    return findOptional<std::string>("shortcuts.nextlayout");
}

template<typename T, typename Key>
std::optional<T> Parameters::findOptional(Key key)
{
    auto it = m_parameters.find(key);
    if (it != m_parameters.end())
        return boost::any_cast<T &&>(it->second.value());
    return std::nullopt;
}
