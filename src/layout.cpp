#include "layout.h"

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/join.hpp>

Layout::Layout(std::string layout, const std::vector<std::string> &options)
    : layoutString(std::move(layout))
{
    if (options.empty())
        return;

    symbols = "pc+";
    boost::tokenizer layoutTokenizer(layoutString, boost::char_separator(","));
    for (auto it = layoutTokenizer.begin(); it != layoutTokenizer.end(); ++it) {
        symbols += it.current_token();
        if (it != layoutTokenizer.begin())
            symbols += ':' + std::to_string(std::distance(layoutTokenizer.begin(), it) + 1);
        symbols += '+';
    }

    symbols += boost::join(options, "+");
}

std::string_view Layout::groupName(unsigned char group) const
{
    return {layoutString.data() + group * 2 + group, 2};
}
