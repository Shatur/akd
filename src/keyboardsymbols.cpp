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

#include "keyboardsymbols.h"

#include "x11deleters.h"

#include <boost/spirit/home/x3.hpp>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>

namespace x3 = boost::spirit::x3;

static constexpr x3::rule<class SymbolsRule, KeyboardSymbols> symbolsRule = "symbols";
static constexpr x3::rule<class GroupsRule, std::vector<std::string>> groupsRule = "groups";
static constexpr x3::rule<class OptionsRule, std::vector<std::string>> optionsRule = "options";

// Server sends symbols with '_' instead of '+'
static constexpr auto symbolsRule_def = "pc_" >> groupsRule >> -('_' >> optionsRule);
static constexpr auto groupsRule_def = +x3::alpha >> *('_' >> +x3::alpha >> '_' >> x3::omit[x3::digit]);
static constexpr auto optionsRule_def = x3::lexeme[+(x3::char_ - ')') >> x3::char_(')')] % '_';

BOOST_SPIRIT_DEFINE(symbolsRule, groupsRule, optionsRule);

KeyboardSymbols KeyboardSymbols::currentSymbols(Display &display)
{
    const std::unique_ptr<XkbDescRec, XlibDeleter> currentDesc(XkbGetKeyboardByName(&display, XkbUseCoreKbd, nullptr, XkbGBN_ServerSymbolsMask, 0, false));
    if (!currentDesc)
        throw std::logic_error("Unable to get keyboard symbols");

    KeyboardSymbols symbols;
    const std::unique_ptr<char[], XlibDeleter> currentSymbols(XGetAtomName(&display, currentDesc->names->symbols));
    x3::parse(currentSymbols.get(), currentSymbols.get() + strlen(currentSymbols.get()), symbolsRule, symbols);

    return symbols;
}
