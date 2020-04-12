#include "keyboardsymbols.h"

#include <boost/algorithm/string/join.hpp>

std::string KeyboardSymbols::x11String() const
{
    std::string symbols = "pc+";

    for (size_t i = 0; i < layouts.size(); ++i) {
        symbols += layouts[i];
        if (i != 0)
            symbols += ':' + std::to_string(i + 1);
        symbols += '+';
    }

    symbols += boost::join(options, "+");
    return symbols;
}
