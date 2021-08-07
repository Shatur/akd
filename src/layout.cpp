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

#include "layout.h"

#include <boost/algorithm/string/join.hpp>
#include <boost/tokenizer.hpp>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

Layout::Layout(Display &display, std::string layout, const std::vector<std::string> &options)
    : m_layoutString(std::move(layout))
    , m_display(display)
{
    if (options.empty())
        return;

    m_symbols = "pc+";
    boost::tokenizer layoutTokenizer(m_layoutString, boost::char_separator(","));
    for (auto it = layoutTokenizer.begin(); it != layoutTokenizer.end(); ++it) {
        m_symbols += it.current_token();
        if (it != layoutTokenizer.begin())
            m_symbols += ':' + std::to_string(std::distance(layoutTokenizer.begin(), it) + 1);
        m_symbols += '+';
    }

    m_symbols += boost::join(options, "+");
}

void Layout::apply()
{
    // Replace layouts with specified and generate new symbols string
    XkbComponentNamesRec currentComponents{};
    currentComponents.symbols = m_symbols.data();

    // Send it back to X11
    const std::unique_ptr<XkbDescRec, XlibDeleter> newDesc(XkbGetKeyboardByName(&m_display, XkbUseCoreKbd, &currentComponents, XkbGBN_SymbolsMask, 0, true));
    if (!newDesc)
        throw std::logic_error("Unable to build keyboard description with the following symbols: " + m_symbols);

    if (s_currentVarDefs) {
        s_currentVarDefs->layout = m_layoutString.data();
        if (!XkbRF_SetNamesProp(&m_display, s_currentRulesPath.get(), s_currentVarDefs.get()))
            throw std::logic_error("Unable to set keyboard rules for " + m_symbols);
    }
}

std::string_view Layout::groupName(unsigned char group) const
{
    return {m_layoutString.data() + group * 2 + group, 2};
}

void Layout::saveKeyboardRules(Display &display)
{
    char *path;
    s_currentVarDefs.reset(new XkbRF_VarDefsRec);

    if (!XkbRF_GetNamesProp(&display, &path, s_currentVarDefs.get()))
        throw std::logic_error("Unable to get keyboard rules");

    s_currentRulesPath.reset(path);

    // Free layout to replace it with pointer to std::string later
    if (s_currentVarDefs->layout)
        XFree(s_currentVarDefs->layout);
}
