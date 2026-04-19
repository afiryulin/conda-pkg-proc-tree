#ifdef _WIN32
#pragma once

#include "IProcessProvider.h"

class WindowsProcessProvider : public IProcessProvider
{
public:
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses(const bool readThreads = false);
};

#endif // _WIN32