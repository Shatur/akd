#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <boost/program_options/variables_map.hpp>

class Parameters
{
public:
    Parameters(int argc, char *argv[]);

    bool printGroups() const;
    bool skipRules() const;

    std::optional<std::vector<std::string>> layouts();
    std::optional<std::string> nextLayoutShortcut();

private:
    template<typename T, typename Key>
    std::optional<T> findOptional(Key key);

    boost::program_options::variables_map m_parameters;
};

#endif // PARAMETERS_H
