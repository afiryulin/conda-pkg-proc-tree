#pragma once

#include <memory>
#include <string>
#include <vector>

struct ProcessInfo
{
    int pid{};
    int parentPid{};
    std::string name;

    std::vector<int> threads;
    std::vector<std::shared_ptr<ProcessInfo>> children;
};
