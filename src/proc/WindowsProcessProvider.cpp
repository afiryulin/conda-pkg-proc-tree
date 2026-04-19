#ifdef _WIN32

#include <windows.h>
#include <tlhelp32.h>
#include <unordered_map>
#include <iostream>

#include "WindowsProcessProvider.h"

std::vector<std::unique_ptr<ProcessInfo>> WindowsProcessProvider::GetProcesses(const bool readThreads)
{
    std::vector<std::unique_ptr<ProcessInfo>> result{};

    HANDLE procSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe;
    pe.dwSize = sizeof(PROCESSENTRY32);

    mThreadsCache.clear();
    mIsThreadsInit.store(false);

    if (Process32First(procSnap, &pe))
    {
        do
        {
            auto proc = std::make_unique<ProcessInfo>();
            proc->pid = pe.th32ProcessID;
            proc->parentPid = pe.th32ParentProcessID;
            proc->name = pe.szExeFile;

            if (readThreads)
            {
                LoadThreads(proc);
            }

            result.push_back(std::move(proc));
        } while (Process32Next(procSnap, &pe));
    }

    CloseHandle(procSnap);

    return result;
}

void WindowsProcessProvider::TakeThreadsSnapshot()
{
    HANDLE hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap != INVALID_HANDLE_VALUE)
    {
        THREADENTRY32 te;
        te.dwSize = sizeof(THREADENTRY32);
        if (Thread32First(hThreadSnap, &te))
        {
            do
            {
                mThreadsCache[te.th32OwnerProcessID].push_back(te.th32ThreadID);
            } while (Thread32Next(hThreadSnap, &te));
        }
        CloseHandle(hThreadSnap);
    }
}

void WindowsProcessProvider::LoadThreads(std::unique_ptr<ProcessInfo> &proc)
{
    if (!mIsThreadsInit.load())
    {
        mIsThreadsInit.store(true);
        TakeThreadsSnapshot();
        std::cout << "Cached " << mThreadsCache.size() << " threads" << std::endl;
    }

    auto it = mThreadsCache.find(proc->pid);
    if (it != mThreadsCache.end())
    {
        proc->threads = std::move(it->second);
    }
}

#endif