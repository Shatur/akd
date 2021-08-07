/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Advanced Keyboard Daemon is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "parameters.h"

#include "cmake.h"

#include <boost/format.hpp>
#include <boost/program_options.hpp>

#include <algorithm>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

Parameters::Parameters(int argc, char *argv[])
{
    po::options_description commands("Commands");
    commands.add_options()
            ("help,h", "Print usage information and exit.")
            ("version,v", "Print version number and exit.")
            ("print-current-group,c", po::bool_switch(), "Print current group and exit.")
            ("print-current-group-index,d", po::bool_switch(), "Print current group index and exit.")
            ("next-group,x", po::bool_switch(), "Switch to the next group and exit.")
            ("set-group,i", po::value<unsigned>()->value_name("index"), "Switch group to the specified index.");

    po::options_description settings("Settings");
    settings.add_options()
            ("settings,s", po::value<fs::path>()->value_name("path")->default_value(defaultConfigPath()), "Path to settings file.");

    po::options_description daemonConfiguration("Daemon configuration");
    daemonConfiguration.add_options()
            ("general.different-groups,g", po::bool_switch(), "Use different groups for each window.")
            ("general.different-layout,a", po::bool_switch(), "Use different layouts for each window.")
            ("general.print-groups,p", po::bool_switch(), "Print switched languages in stdout.")
            ("general.layouts,l", po::value<std::vector<std::string>>()->multitoken(), "Languages separated by ','. Can be specified several times to define several layouts.")
            ("general.default-group,e", po::value<unsigned>(), "The index of the group to switch when changing the layout.")
            ("general.skip-rules", po::bool_switch(), "Do not update keyboard rules. Improves performance, but other applications won't be aware of the layout changes. Use with caution.")
            ("shortcuts.nextlayout,n", po::value<std::string>()->value_name("shortcut"), "Shortcut to switch to next layout.");

    po::options_description allOptions;
    allOptions.add(commands).add(settings).add(daemonConfiguration);

    store(parse_command_line(argc, argv, allOptions), m_parameters);

    const size_t specifiedCommandsCount = specifiedOptionsCount(commands);
    if (specifiedCommandsCount > 1)
        throw std::logic_error("You can't specify more than one command at a time");
    if (specifiedCommandsCount != 0 && (specifiedOptionsCount(settings) != 0 || specifiedOptionsCount(daemonConfiguration) != 0))
        throw std::logic_error("You can't execute commands and specify daemon configuration at the same time");

    if (const auto &settingsPath = m_parameters["settings"].as<fs::path>(); fs::exists(settingsPath))
        store(parse_config_file(settingsPath.c_str(), daemonConfiguration), m_parameters);

    if (m_parameters.count("help")) {
        std::cout << boost::format("Usage: %s [options]") % argv[0] << allOptions;
        m_printInfoOnly = true;
        return;
    }

    if (m_parameters.count("version")) {
        std::cout << boost::format("%s %d.%d.%d\n") % PROJECT_LABEL % PROJECT_VERSION_MAJOR % PROJECT_VERSION_MINOR % PROJECT_VERSION_PATCH;
        m_printInfoOnly = true;
        return;
    }

    notify(m_parameters);
    m_printInfoOnly = false;
}

bool Parameters::isPrintCurrentGroup() const
{
    return m_parameters["print-current-group"].as<bool>();
}

bool Parameters::isPrintCurrentGroupIndex() const
{
    return m_parameters["print-current-group-index"].as<bool>();
}

bool Parameters::isSwitchToNextGroup() const
{
    return m_parameters["next-group"].as<bool>();
}

std::optional<unsigned char> Parameters::groupToSet() const
{
    return findOptional<unsigned char>("set-group");
}

bool Parameters::isPrintInfoOnly() const
{
    return m_printInfoOnly;
}

bool Parameters::isUseDifferentGroups() const
{
    return m_parameters["general.different-groups"].as<bool>();
}

bool Parameters::useDifferentLayouts() const
{
    return m_parameters["general.different-layout"].as<bool>();
}

bool Parameters::isPrintGroups() const
{
    return m_parameters["general.print-groups"].as<bool>();
}

bool Parameters::isSkipRules() const
{
    return m_parameters["general.skip-rules"].as<bool>();
}

std::optional<unsigned char> Parameters::defaultGroup() const
{
    return findOptional<unsigned char>("general.default-group");
}

std::optional<std::vector<std::string>> Parameters::layouts() const
{
    return findOptional<std::vector<std::string>>("general.layouts");
}

std::optional<std::string> Parameters::nextLayoutShortcut() const
{
    return findOptional<std::string>("shortcuts.nextlayout");
}

size_t Parameters::specifiedOptionsCount(const boost::program_options::options_description &optionsGroup) const
{
    const std::ptrdiff_t count = std::count_if(optionsGroup.options().begin(), optionsGroup.options().end(), [this](const boost::shared_ptr<boost::program_options::option_description> &option) {
        auto it = m_parameters.find(option->long_name());
        if (it == m_parameters.end())
            return false;
        return !it->second.defaulted();
    });
    return static_cast<size_t>(count);
}

template<typename T, typename Key>
std::optional<T> Parameters::findOptional(Key key) const
{
    auto it = m_parameters.find(key);
    if (it != m_parameters.end()) {
        // Boost represents unsigned char as a character, so store it as unsigned int and cast on read
        if constexpr (std::is_same_v<T, unsigned char>)
            return boost::numeric_cast<unsigned char>(boost::any_cast<unsigned>(it->second.value()));
        return boost::any_cast<T>(it->second.value());
    }
    return std::nullopt;
}

fs::path Parameters::defaultConfigPath()
{
    const char *home = getenv("HOME");
    if (home == nullptr)
        return {};

    return fs::path(home) / ".config/akd/akd.conf";
}
