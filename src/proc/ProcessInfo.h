#pragma once

#include <memory>
#include <string>
#include <vector>
#include <cstdint>

struct ProcessInfo
{
    uint32_t pid{};
    uint32_t parentPid{};
    std::string name;

    std::vector<uint32_t> threads;
    std::vector<std::shared_ptr<ProcessInfo>> children;
};
