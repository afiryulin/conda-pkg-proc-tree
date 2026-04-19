#include "ProcessTreeBuilder.h"

#include <unordered_map>

std::vector<std::shared_ptr<ProcessInfo>> ProcessTreeBuilder::BuildTree(std::vector<std::unique_ptr<ProcessInfo>> &processes)
{
    std::unordered_map<uint32_t, std::shared_ptr<ProcessInfo>> processesMap;
    std::vector<std::shared_ptr<ProcessInfo>> roots;

    for (auto &proc : processes)
    {
        if (proc != nullptr /* && (proc->parentPid == 0 || proc->parentPid == 1)*/)
        {
            uint32_t pid = proc->pid;
            processesMap[pid] = std::move(proc);
        }
    }

    for (auto const &[pid, proc] : processesMap)
    {
        auto it = processesMap.find(proc->parentPid);

        if (it != processesMap.end() && pid != proc->parentPid)
            it->second->children.push_back(proc);
        else
            roots.push_back(proc);
    }

    return roots;
}