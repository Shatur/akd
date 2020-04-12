#ifndef LAYOUTSYMBOLS_H
#define LAYOUTSYMBOLS_H

#include <vector>
#include <string>

struct KeyboardSymbols
{
    std::vector<std::string> layouts;
    std::vector<std::string> options;

    std::string x11String() const;
};

#endif // LAYOUTSYMBOLS_H
