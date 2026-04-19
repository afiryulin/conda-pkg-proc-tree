#ifdef _WIN32

#include "IProcessProvider.h"
#include <windows.h>
#include <tlhelp32.h>

#include <iostream>

struct WindowsProcessProvider : IProcessProvider
{
    std::vector<std::unique_ptr<ProcessInfo>> GetProcesses() override
    {
        std::vector<std::unique_ptr<ProcessInfo>> result;

        HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(snap, &pe))
        {
            do
            {
                auto proc = std::make_unique<ProcessInfo>();
                proc->pid = pe.th32ProcessID;
                proc->parentPid = pe.th32ParentProcessID;
                proc->name = pe.szExeFile;
                result.push_back(std::move(proc));
            } while (Process32Next(snap, &pe));
        }

        CloseHandle(snap);
        return result;
    }
};

#endif