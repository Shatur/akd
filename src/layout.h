#ifndef LAYOUT_H
#define LAYOUT_H

#include <string>
#include <vector>

struct Layout
{
    Layout(std::string layout, const std::vector<std::string> &options = {});

    std::string_view groupName(unsigned char group) const;

    std::string layoutString;
    std::string symbols;
};

#endif // LAYOUT_H
