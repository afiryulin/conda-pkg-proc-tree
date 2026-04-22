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
        auto proc = std::make_shared<ProcessInfo>();

        proc->pid = procList[i].kp_proc.p_pid;
        proc->parentPid = procList[i].kp_eproc.e_ppid;
        proc->name = procList[i].kp_proc.p_comm;

        result.push_back(proc);
    }

    return result;
}