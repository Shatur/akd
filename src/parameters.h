#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <filesystem>
#include <vector>

class Parameters
{
public:
    Parameters(int argc, char *argv[]);

    bool printGroups() const;
    const std::filesystem::path &settingsPath() const;
    const std::vector<std::string> &layouts() const;
    const std::string &nextlayoutShortcut() const;

private:
    bool m_printGroups;
    std::filesystem::path m_settingsPath;
    std::vector<std::string> m_layouts;
    std::string m_nextlayoutShortcut;
};

#endif // PARAMETERS_H
