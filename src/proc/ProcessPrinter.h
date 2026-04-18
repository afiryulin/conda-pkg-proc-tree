#pragma once
#include <memory>

#include "ProcessInfo.h"

#define LAST_ITEM_PREFIX "└── "
#define BRANCHING_ITEM_PREFIX "├── "
#define NOT_LAST_ITEM_PREFIX "│   "
#define SPACING_PREFIX "    "

class ProcessPrinter
{
public:
    static void Print(const std::vector<std::shared_ptr<ProcessInfo>> roots, bool showThreads);

private:
    static void PrintRecursive(const std::shared_ptr<ProcessInfo> node, const std::string &prefix, bool isLast, bool showThreads);
};