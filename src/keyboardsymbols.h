/*
 *  Copyright © 2019-2021 Hennadii Chernyshchyk <genaloner@gmail.com>
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

#ifndef KEYBOARDSYMBOLSPARSER_H
#define KEYBOARDSYMBOLSPARSER_H

#include <boost/fusion/include/adapt_struct.hpp>

#include <string>
#include <vector>

using Display = class _XDisplay;

struct KeyboardSymbols {
    std::vector<std::string> groups;
    std::vector<std::string> options;

    [[nodiscard]] static KeyboardSymbols currentSymbols(Display &display);
};

BOOST_FUSION_ADAPT_STRUCT(KeyboardSymbols, groups, options)

#endif // KEYBOARDSYMBOLSPARSER_H
