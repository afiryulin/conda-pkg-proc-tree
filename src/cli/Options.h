#pragma once

struct Options
{
    bool showThreads{false};
    bool jsonOutput{false};
    bool showHelp{false};
};

struct OptionsParser
{
    static Options Parse(const int &argc, char **argv);
};