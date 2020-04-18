#include "parameters.h"

#include <boost/program_options.hpp>

#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

Parameters::Parameters(int argc, char *argv[])
{
    po::options_description generic("Options");
    generic.add_options()
            ("help,h", "Prints this message")
            ("settings,s", po::value(&m_settingsPath)->default_value(fs::path(getenv("HOME")) / ".config/akd/akd.conf"), "Path to settings file");

    po::options_description configuration("Configuration");
    configuration.add_options()
            ("general.print-groups,p", po::bool_switch(&m_printGroups), "Print switched languages in stdout")
            ("general.layouts,l", po::value(&m_layouts)->multitoken(), "Languages, separated by ','. Can be specified several times to define several layouts.")
            ("shortcuts.nextlayout,n", po::value(&m_nextlayoutShortcut), "Switch to next layout");


    po::options_description allOptions("Advanced keyboard daemon");
    allOptions.add(generic).add(configuration);

    po::variables_map parameters;
    store(parse_command_line(argc, argv, allOptions), parameters);
    if (fs::exists(m_settingsPath))
        store(parse_config_file(m_settingsPath.c_str(), configuration), parameters);

    if (parameters.count("help")) {
        std::cout << allOptions;
        std::exit(1);
    }

    notify(parameters);
}

bool Parameters::printGroups() const
{
    return m_printGroups;
}

const std::filesystem::__cxx11::path &Parameters::settingsPath() const
{
    return m_settingsPath;
}

const std::vector<std::string> &Parameters::layouts() const
{
    return m_layouts;
}

const std::string &Parameters::nextlayoutShortcut() const
{
    return m_nextlayoutShortcut;
}
