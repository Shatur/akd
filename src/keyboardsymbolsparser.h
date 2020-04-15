#ifndef KEYBOARDSYMBOLSPARSER_H
#define KEYBOARDSYMBOLSPARSER_H

#include "keyboardsymbols.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace KeyboardSymbolsParser
{
    namespace x3 = boost::spirit::x3;

    static const x3::rule<class SymbolsRule, KeyboardSymbols> symbolsRule = "symbols";
    static const x3::rule<class LayoutsRule, std::vector<std::string>> layoutRule = "layout";
    static const x3::rule<class OptionsRule, std::vector<std::string>> optionsRule = "options";

    // Server sends symbols with '_' instead of '+'
    const auto symbolsRule_def = "pc_" >> layoutRule >> -('_' >> optionsRule);
    const auto layoutRule_def = +x3::alpha >> *('_' >> +x3::alpha >> '_' >> x3::omit[x3::digit]);
    const auto optionsRule_def = x3::lexeme[+(x3::char_ - ')') >> x3::char_(')')] % '_';

    BOOST_SPIRIT_DEFINE(symbolsRule, layoutRule, optionsRule);
}

BOOST_FUSION_ADAPT_STRUCT(KeyboardSymbols,
    layout, options
)

#endif // KEYBOARDSYMBOLSPARSER_H
