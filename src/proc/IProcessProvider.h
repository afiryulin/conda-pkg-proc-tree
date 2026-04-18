#pragma once

#include <vector>
#include <memory>

#include "ProcessInfo.h"

struct IProcessProvider
{
    virtual std::vector<std::unique_ptr<ProcessInfo>> GetProcesses() = 0;
    virtual ~IProcessProvider() = default;
};