#include <iostream>

#include "cli/Options.h"
#include "proc/ProcessTreeBuilder.h"
#include "proc/ProcessPrinter.h"

#ifdef _WIN32
#include <windows.h> // SetConsoleOutputCP(...)
#include "proc/WindowsProcessProvider.h"
#else
#include "proc/LinuxProcessProvider.h"
#endif

int main(int argc, char **argv)
{
#ifdef _WIN32
    WindowsProcessProvider provider;
    SetConsoleOutputCP(65001); // Set UTF-8 output encoding
#else
    LinuxProcessProvider provider;
#endif

    Options opts = OptionsParser::Parse(argc, argv);

    if (opts.showThreads)
        std::cout << "Thread showing turned on" << std::endl;

    if (opts.jsonOutput)
        std::cout << "JSON output turned on" << std::endl;

    auto processes = provider.GetProcesses(opts.showThreads);

    ProcessTreeBuilder builder;
    auto root = builder.BuildTree(processes);

    ProcessPrinter::Print(root, opts.showThreads);

    return 0;
}