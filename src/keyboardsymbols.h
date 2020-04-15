#ifndef KEYBOARDSYMBOLS_H
#define KEYBOARDSYMBOLS_H

#include <vector>
#include <string>

struct KeyboardSymbols
{
    std::vector<std::string> layout;
    std::vector<std::string> options;

    std::string x11String() const;
};

#endif // KEYBOARDSYMBOLS_H
