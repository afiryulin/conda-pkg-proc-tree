#include "Options.h"

#include <string>

Options OptionsParser::Parse(const int &argc, char **argv)
{
    Options result{};

    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--threads")
            result.showThreads = true;

        if (arg == "--json")
            result.jsonOutput = true;

        if (arg == "--help")
            result.showHelp = true;
    }

    return result;
}