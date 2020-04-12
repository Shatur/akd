#ifndef LAYOUTSYMBOLSGRAMMAR_H
#define LAYOUTSYMBOLSGRAMMAR_H

#include "keyboardsymbols.h"

#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/adapt_struct.hpp>

namespace KeyboardSymbolsParser
{
    namespace x3 = boost::spirit::x3;

    x3::rule<class SymbolsRule, KeyboardSymbols> const symbolsRule = "symbols";
    x3::rule<class LayoutsRule, std::vector<std::string>> const layoutsRule = "layouts";
    x3::rule<class OptionsRule, std::vector<std::string>> const optionsRule = "options";

    // Server sends symbols with '_' instead of '+'
    const auto symbolsRule_def = "pc_" >> layoutsRule >> -('_' >> optionsRule);
    const auto layoutsRule_def = +x3::alpha >> *('_' >> +x3::alpha >> '_' >> x3::omit[x3::digit]);
    const auto optionsRule_def = x3::lexeme[+(x3::char_ - ')') >> x3::char_(')')] % '_';

    BOOST_SPIRIT_DEFINE(symbolsRule);
    BOOST_SPIRIT_DEFINE(layoutsRule);
    BOOST_SPIRIT_DEFINE(optionsRule);
}

BOOST_FUSION_ADAPT_STRUCT(KeyboardSymbols,
    layouts, options
)

#endif // LAYOUTSYMBOLSGRAMMAR_H
