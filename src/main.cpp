#include <fmt/core.h>

#include "cli/Options.h"
#include "proc/JsonExporter.h"
#include "proc/ProcessTreeBuilder.h"
#include "proc/ProcessPrinter.h"

#ifdef _WIN32
#include <windows.h> // SetConsoleOutputCP(...)
#include "proc/WindowsProcessProvider.h"
#elif defined(__linux__)
#include "proc/LinuxProcessProvider.h"
#elif defined(__APPLE__)
#include "proc/MacProcessProvider.h"
#endif //_WIN32

int main(int argc, char **argv)
{

    std::unique_ptr<IProcessProvider> provider;

#ifdef _WIN32
    provider = std::make_unique<WindowsProcessProvider>();
    SetConsoleOutputCP(65001); // Set UTF-8 output encoding
#elif defined(__linux__)
    provider = std::make_unique<LinuxProcessProvider>();
#elif defined(__APPLE__)
    provider = std::make_unique<MacProcessProvider>();
#endif // _WIN32

    Options opts = OptionsParser::Parse(argc, argv);

    if (opts.showHelp)
    {
        fmt::print("This application show process tree.\nOptions:\n\t--help\t\tshow this text\n\t--threads\tbuild threads \n\t--json\t\toutput in JSON format\n");
        return 0;
    }

    auto processes = provider->GetProcesses(opts.showThreads);

    ProcessTreeBuilder builder;
    auto roots = builder.BuildTree(processes);
    if (opts.jsonOutput)
    {
        JsonExporter::Export(roots);
    }
    else
        ProcessPrinter::Print(roots, opts.showThreads);

    return 0;
}