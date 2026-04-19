#pragma once
#include <filesystem>

#include "IProcessProvider.h"

class LinuxProcessProvider : public IProcessProvider
{
public:
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses(const bool readThreads = false) override;

private:
    void LoadThreads(std::unique_ptr<ProcessInfo> &proc, const std::filesystem::path &taskPath);
};