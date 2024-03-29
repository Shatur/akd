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

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

#include <filesystem>
#include <optional>

class Parameters
{
public:
    Parameters(int argc, char *argv[]);

    [[nodiscard]] bool isPrintInfoOnly() const;

    [[nodiscard]] bool isPrintCurrentGroup() const;
    [[nodiscard]] bool isPrintCurrentGroupIndex() const;
    [[nodiscard]] bool isSwitchToNextGroup() const;
    [[nodiscard]] std::optional<unsigned char> groupToSet() const;

    [[nodiscard]] bool isUseDifferentGroups() const;
    [[nodiscard]] bool useDifferentLayouts() const;
    [[nodiscard]] bool isPrintGroups() const;
    [[nodiscard]] bool isSkipRules() const;
    [[nodiscard]] std::optional<unsigned char> defaultGroup() const;

    [[nodiscard]] std::optional<std::vector<std::string>> layouts() const;
    [[nodiscard]] std::optional<std::string> nextLayoutShortcut() const;

private:
    [[nodiscard]] size_t specifiedOptionsCount(const boost::program_options::options_description &optionsGroup) const;

    template<typename T, typename Key>
    [[nodiscard]] std::optional<T> findOptional(Key key) const;

    [[nodiscard]] static std::filesystem::path defaultConfigPath();

    boost::program_options::variables_map m_parameters;
    bool m_printInfoOnly;
};

#endif // PARAMETERS_H
