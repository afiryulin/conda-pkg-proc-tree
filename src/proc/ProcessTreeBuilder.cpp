#include "ProcessTreeBuilder.h"
#include <iostream>

std::vector<std::shared_ptr<ProcessInfo>> ProcessTreeBuilder::BuildTree(std::vector<std::unique_ptr<ProcessInfo>> &processes)
{
    std::vector<std::shared_ptr<ProcessInfo>> roots;
    for (auto &proc : processes)
    {
        if (proc != nullptr && (proc->parentPid == 0 || proc->parentPid == 1))
        {
            std::shared_ptr<ProcessInfo> sharedProc = std::move(proc);
            roots.push_back(sharedProc);
        }
    }

    for (auto &root : roots)
    {
        AttachChildren(root, processes);
    }

    return roots;
}

void ProcessTreeBuilder::AttachChildren(
    std::shared_ptr<ProcessInfo> parent,
    std::vector<std::unique_ptr<ProcessInfo>> &processes)
{
    for (auto &proc : processes)
    {
        if (proc != nullptr && proc->parentPid == parent->pid)
        {
            std::shared_ptr<ProcessInfo> sharedProc = std::move(proc);
            parent->children.push_back(sharedProc);
            AttachChildren(sharedProc, processes);
        }
    }
}
