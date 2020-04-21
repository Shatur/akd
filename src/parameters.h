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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <boost/program_options/variables_map.hpp>

class Parameters
{
public:
    Parameters(int argc, char *argv[]);

    bool printGroups() const;
    bool skipRules() const;

    std::optional<std::vector<std::string>> layouts();
    std::optional<std::string> nextLayoutShortcut();

private:
    template<typename T, typename Key>
    std::optional<T> findOptional(Key key);

    boost::program_options::variables_map m_parameters;
};

#endif // PARAMETERS_H
