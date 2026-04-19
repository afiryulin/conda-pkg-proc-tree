#include <algorithm>
#include <filesystem>
#include <fstream>

#include "LinuxProcessProvider.h"

std::vector<std::unique_ptr<ProcessInfo>> LinuxProcessProvider::GetProcesses(const bool readThreads)
{
    std::vector<std::unique_ptr<ProcessInfo>> result{};

    for (const auto &dir : std::filesystem::directory_iterator("/proc"))
    {
        if (!dir.is_directory())
            continue;

        std::string pidStr = dir.path().filename().string();
        if (!std::all_of(pidStr.cbegin(), pidStr.cend(), ::isdigit))
            continue;

        std::ifstream statFile(dir.path() / "stat");
        if (!statFile)
            continue;

        auto proc = std::make_unique<ProcessInfo>();
        std::string comm;
        char state;
        statFile >> proc->pid >> comm >> state >> proc->parentPid;

        proc->name = comm;

        if (readThreads)
            LoadThreads(proc);

        result.push_back(std::move(proc));
    }

    return result;
}

void LinuxProcessProvider::LoadThreads(std::unique_ptr<ProcessInfo> &proc)
{
    std::string path = "/proc/" + std::to_string(proc->pid) + "/task";

    if (!std::filesystem::exists(path))
        return;

    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        std::string tidStr = entry.path().filename().string();
        if (std::all_of(tidStr.cbegin(), tidStr.cend(), ::isdigit))
            proc->threads.push_back(std::stoi(tidStr));
    }
}
