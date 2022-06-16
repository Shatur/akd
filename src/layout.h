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

#ifndef LAYOUT_H
#define LAYOUT_H

#include "x11deleters.h"

#include <memory>
#include <string>
#include <vector>

class Layout
{
public:
    explicit Layout(Display &display, std::string layout, const std::vector<std::string> &options = {});

    void apply();

    [[nodiscard]] std::string_view groupName(unsigned char group) const;

    static void saveKeyboardRules(Display &display);

private:
    std::string m_layoutString;
    std::string m_symbols;
    Display &m_display;

    static inline std::unique_ptr<XkbRF_VarDefsRec, VarDefsWithoutLayoutDeleter> s_currentVarDefs;
    static inline std::unique_ptr<char[], XlibDeleter> s_currentRulesPath;
};

#endif // LAYOUT_H
