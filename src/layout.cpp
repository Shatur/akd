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

#include "layout.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

Layout::Layout(std::string layout, const std::vector<std::string> &options)
    : layoutString(std::move(layout))
{
    if (options.empty())
        return;

    symbols = "pc+";
    boost::tokenizer layoutTokenizer(layoutString, boost::char_separator(","));
    for (auto it = layoutTokenizer.begin(); it != layoutTokenizer.end(); ++it) {
        symbols += it.current_token();
        if (it != layoutTokenizer.begin())
            symbols += ':' + std::to_string(std::distance(layoutTokenizer.begin(), it) + 1);
        symbols += '+';
    }

    symbols += boost::join(options, "+");
}

std::string_view Layout::groupName(unsigned char group) const
{
    return {layoutString.data() + group * 2 + group, 2};
}
