#ifdef _WIN32

#include "IProcessProvider.h"
#include <windows.h>
#include <tlhelp32.h>

struct WindowsProcessProvider : IProcessProvider
{
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses() override
    {
        std::vector<std::unique_ptr<ProcessInfo>> result{};

        return result;
    }
}

#endif