#include "IProcessProvider.h"
#include <algorithm>
#include <filesystem>
#include <fstream>

class LinuxProcessProvider : public IProcessProvider
{
public:
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses() override
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
            LoadThreads(proc);
            result.push_back(std::move(proc));
        }

        return result;
    }

private:
    void LoadThreads(std::unique_ptr<ProcessInfo> &proc)
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
};
