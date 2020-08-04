/*
 *  Copyright © 2019-2020 Hennadii Chernyshchyk <genaloner@gmail.com>
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
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a get of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef KEYBOARDSYMBOLSPARSER_H
#define KEYBOARDSYMBOLSPARSER_H

#include <boost/fusion/include/adapt_struct.hpp>
#include <boost/spirit/home/x3.hpp>

struct KeyboardSymbols
{
    std::vector<std::string> groups;
    std::vector<std::string> options;
};

BOOST_FUSION_ADAPT_STRUCT(KeyboardSymbols,
    groups, options
)

namespace KeyboardSymbolsParser
{
    namespace x3 = boost::spirit::x3;

    static const x3::rule<class SymbolsRule, KeyboardSymbols> symbolsRule = "symbols";
    static const x3::rule<class GroupsRule, std::vector<std::string>> groupsRule = "groups";
    static const x3::rule<class OptionsRule, std::vector<std::string>> optionsRule = "options";

    // Server sends symbols with '_' instead of '+'
    const auto symbolsRule_def = "pc_" >> groupsRule >> -('_' >> optionsRule);
    const auto groupsRule_def = +x3::alpha >> *('_' >> +x3::alpha >> '_' >> x3::omit[x3::digit]);
    const auto optionsRule_def = x3::lexeme[+(x3::char_ - ')') >> x3::char_(')')] % '_';

    BOOST_SPIRIT_DEFINE(symbolsRule, groupsRule, optionsRule);
}

#endif // KEYBOARDSYMBOLSPARSER_H
