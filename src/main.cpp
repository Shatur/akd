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

#include "keyboarddaemon.h"

#include <iostream>
#include <filesystem>

#include <boost/program_options.hpp>

namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    fs::path settings;
    std::vector<std::string> layouts;
    std::string nextlayout;

    po::options_description generic("Options");
    generic.add_options()
            ("help,h", "Prints this message")
            ("settings,s", po::value(&settings)->default_value(fs::path(getenv("HOME")) / ".config/akd/akd.conf"), "Path to settings file");

    po::options_description configuration("Configuration");
    configuration.add_options()
            ("general.layouts,l", po::value(&layouts)->multitoken(), "Languages, separated by ','. Can be specified several times to define several layouts.")
            ("shortcuts.nextlayout,n", po::value(&nextlayout), "Switch to next layout");


    po::options_description allOptions("Advanced keyboard daemon");
    allOptions.add(generic).add(configuration);

    po::variables_map parameters;
    store(parse_command_line(argc, argv, allOptions), parameters);
    if (fs::exists(settings))
        store(parse_config_file(settings.c_str(), configuration), parameters);

    if (parameters.count("help")) {
        std::cout << allOptions;
        return 1;
    }

    try {
        notify(parameters);

        KeyboardDaemon daemon;
        daemon.setLayouts(layouts);
        daemon.addNextLayoutShortcut(nextlayout);
        daemon.processEvents();
    } catch (std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
