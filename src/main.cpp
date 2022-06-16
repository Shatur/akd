/*
 *  Copyright © 2019-2021 Hennadii Chernyshchyk <genaloner@gmail.com>
 *
 *  This file is part of Advanced Keyboard Daemon.
 *
 *  Advanced Keyboard Daemon is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  Advanced Keyboard Daemon is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Advanced Keyboard Daemon. If not, see <https://www.gnu.org/licenses/>.
 */

#include "keyboarddaemon.h"
#include "parameters.h"

#include <boost/program_options.hpp>

#include <filesystem>
#include <iostream>

int main(int argc, char *argv[])
{
    try {
        const Parameters parameters(argc, argv);
        if (parameters.isPrintInfoOnly())
            return 0;

        KeyboardDaemon daemon(parameters);
        if (daemon.needProcessEvents())
            daemon.processEvents();
    } catch (std::exception &error) {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
