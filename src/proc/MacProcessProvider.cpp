#include <sys/sysctl.h>
#include <memory>
#include <vector>

#include "MacProcessProvider.h"

std::vector<std::unique_ptr<ProcessInfo>>
MacProcessProvider::GetProcesses(const bool readThreads)
{
    std::vector<std::unique_ptr<ProcessInfo>> result;

    int mib[4] = {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};

    size_t size = 0;
    if (sysctl(mib, 4, nullptr, &size, nullptr, 0) < 0)
        return result;

    std::vector<char> buffer(size);

    if (sysctl(mib, 4, buffer.data(), &size, nullptr, 0) < 0)
        return result;

    int count = static_cast<int>(size / sizeof(kinfo_proc));
    auto *procList = reinterpret_cast<kinfo_proc *>(buffer.data());

    for (int i = 0; i < count; ++i)
    {
        auto proc = std::make_unique<ProcessInfo>();

        proc->pid = procList[i].kp_proc.p_pid;
        proc->parentPid = procList[i].kp_eproc.e_ppid;
        proc->name = procList[i].kp_proc.p_comm;

        if (readThreads)
        {
            LoadThreads(proc);
        }

        result.push_back(std::move(proc));
    }

    return result;
}

void MacProcessProvider::LoadThreads(std::unique_ptr<ProcessInfo> &proc)
{
    task_t task;
    kern_return_t kr = task_for_pid(mach_task_self(), pid, &task);
    if (kr != KERN_SUCCESS)
    {
        return threadsResult; // нет прав — просто пусто
    }

    thread_act_array_t threadList;
    mach_msg_type_number_t threadCount;

    kr = task_threads(task, &threadList, &threadCount);
    if (kr != KERN_SUCCESS)
    {
        return threadsResult;
    }

    for (mach_msg_type_number_t i = 0; i < threadCount; ++i)
    {
        thread_t thread = threadList[i];
        proc->threads.push_back(static_cast<int>(thread));
    }

    vm_deallocate(mach_task_self(),
                  reinterpret_cast<vm_address_t>(threadList),
                  threadCount * sizeof(thread_t));
}
