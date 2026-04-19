#include <filesystem>
#include <fstream>

#include <iostream>
#include <system_error>

#include "LinuxProcessProvider.h"

std::vector<std::unique_ptr<ProcessInfo>> LinuxProcessProvider::GetProcesses(const bool readThreads)
{
    std::vector<std::unique_ptr<ProcessInfo>> result{};
    result.reserve(256);

    std::error_code errCode;
    auto procIt = std::filesystem::directory_iterator("/proc", errCode);

    if (errCode)
        return result;

    for (const auto &procDir : procIt)
    {
        const auto &path = procDir.path();
        std::string pidStr = path.filename().string();

        if (!std::isdigit(pidStr[0]))
            continue;

        std::ifstream statFile(path / "stat");
        if (!statFile)
            continue;

        std::string line;

        if (!std::getline(statFile, line))
            continue;

        size_t openBracket = line.find('(');
        size_t closeBracket = line.rfind(')');

        if (openBracket == std::string::npos || closeBracket == std::string::npos)
            continue;

        auto proc = std::make_unique<ProcessInfo>();
        proc->pid = std::stoi(line.substr(0, openBracket));
        proc->name = line.substr(openBracket + 1, closeBracket - openBracket - 1);

        std::string restInfo = line.substr(closeBracket + 2);
        std::stringstream ss(restInfo);
        char state;
        ss >> state >> proc->parentPid;

        if (readThreads)
        {
            LoadThreads(proc, path / "task");
        }
        result.push_back(std::move(proc));
    }

    return result;
}

void LinuxProcessProvider::LoadThreads(std::unique_ptr<ProcessInfo> &proc, const std::filesystem::path &taskPath)
{
    std::error_code errCode;

    auto threadsIt = std::filesystem::directory_iterator(taskPath, errCode);

    if (errCode)
        return;

    for (const auto &entry : threadsIt)
    {
        std::string tidStr = entry.path().filename().string();
        if (std::isdigit(tidStr[0]))
            proc->threads.push_back(std::stoi(tidStr));
    }
}