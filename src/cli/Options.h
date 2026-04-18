#pragma once

struct Options
{
    bool showThreads{false};
    bool jsonOutput{false};
};

struct OptionsParser
{
    static Options Parse(const int &argc, char **argv);
};