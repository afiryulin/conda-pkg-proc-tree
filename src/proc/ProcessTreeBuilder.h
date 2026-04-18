#pragma once

#include <memory>
#include <vector>

#include "ProcessInfo.h"

class ProcessTreeBuilder
{
public:
    std::vector<std::shared_ptr<ProcessInfo>> BuildTree(std::vector<std::unique_ptr<ProcessInfo>> &processes);

private:
    void AttachChildren(std::shared_ptr<ProcessInfo> parent,
                        std::vector<std::unique_ptr<ProcessInfo>> &processes);
};