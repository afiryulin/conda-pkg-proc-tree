#pragma once

#include "IProcessProvider.h"

class MacProcessProvider : public IProcessProvider
{
public:
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses(const bool readThreads = false) override;
};