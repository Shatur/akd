#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <boost/program_options/variables_map.hpp>

class Parameters
{
public:
    Parameters(int argc, char *argv[]);

    boost::program_options::variable_value &printGroups();
    boost::program_options::variable_value &skipRules();
    boost::program_options::variable_value &layouts();

    boost::program_options::variable_value &nextLayoutShortcut();

private:
    boost::program_options::variables_map m_parameters;
};

#endif // PARAMETERS_H
