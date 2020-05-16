/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Crow Translate is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "parameters.h"
#include "cmake.h"

#include <boost/program_options.hpp>
#include <boost/format.hpp>

#include <filesystem>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

Parameters::Parameters(int argc, char *argv[])
{
    po::options_description commands("Commands");
    commands.add_options()
            ("help,h", "Print usage information and exit.")
            ("version,v", "Print version number and exit.")
            ("settings,s", po::value<fs::path>()->value_name("path")->default_value(fs::path(getenv("HOME")) / ".config/akd/akd.conf"), "Path to settings file.");

    po::options_description configuration("Configuration");
    configuration.add_options()
            ("general.different-groups,g", po::bool_switch(), "Use different groups for each window.")
            ("general.different-layout,a", po::bool_switch(), "Use different layouts for each window.")
            ("general.print-groups,p", po::bool_switch(), "Print switched languages in stdout.")
            ("general.skip-rules,r", po::bool_switch(), "Do not update keyboard rules, useful if you use only this program to work with keyboard.")
            ("general.layouts,l", po::value<std::vector<std::string>>()->multitoken(), "Languages separated by ','. Can be specified several times to define several layouts.")
            ("shortcuts.nextlayout,n", po::value<std::string>(), "Switch to next layout.");

    po::options_description allOptions;
    allOptions.add(commands).add(configuration);

    store(parse_command_line(argc, argv, allOptions), m_parameters);

    if (const auto &settingsPath = m_parameters["settings"].as<fs::path>(); fs::exists(settingsPath))
        store(parse_config_file(settingsPath.c_str(), configuration), m_parameters);

    if (m_parameters.count("help")) {
        std::cout << boost::format("Usage: %s [options]") % argv[0] << allOptions;
        std::exit(0);
    }

    if (m_parameters.count("version")) {
        std::cout << boost::format("%s %d.%d.%d\n") % akd_DESCRIPTION % akd_VERSION_MAJOR % akd_VERSION_MINOR % akd_VERSION_PATCH;
        std::exit(0);
    }

    notify(m_parameters);
}

bool Parameters::useDifferentGroups() const
{
    return m_parameters["general.different-groups"].as<bool>();
}

bool Parameters::useDifferentLayouts() const
{
    return m_parameters["general.different-layout"].as<bool>();
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
