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
#include "parameters.h"

#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>

namespace po = boost::program_options;
namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    try {
        Parameters parameters(argc, argv);

        KeyboardDaemon daemon(parameters);
        daemon.processEvents();
    } catch (std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
