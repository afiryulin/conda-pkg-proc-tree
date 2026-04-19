#pragma once

#include <memory>
#include <vector>

#include "ProcessInfo.h"

class ProcessTreeBuilder
{
public:
    std::vector<std::shared_ptr<ProcessInfo>> BuildTree(std::vector<std::unique_ptr<ProcessInfo>> &processes);
};