#ifdef _WIN32
#pragma once

#include <unordered_map>
#include <atomic>

#include "IProcessProvider.h"

class WindowsProcessProvider : public IProcessProvider
{
public:
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses(const bool readThreads = false);

private:
    void LoadThreads(std::unique_ptr<ProcessInfo> &proc);
    void TakeThreadsSnapshot();

private:
    std::atomic<bool> mIsThreadsInit{false};
    std::unordered_map<uint32_t, std::vector<uint32_t>> mThreadsCache{};
};

#endif // _WIN32