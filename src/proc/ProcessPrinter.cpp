#include <iostream>

#include "ProcessPrinter.h"

void ProcessPrinter::Print(const std::vector<std::shared_ptr<ProcessInfo>> roots, bool showThreads)
{
    for (const auto &root : roots)
    {
        PrintRecursive(root, "", false, showThreads);
    }
}

void ProcessPrinter::PrintRecursive(const std::shared_ptr<ProcessInfo> node, const std::string &prefix, bool isLast, bool showThreads)
{
    if (nullptr == node)
    {
        std::cout << "Node nullptr\n";
        return;
    }

    if (showThreads && !node->threads.empty())
    {

        std::cout << prefix << SPACING_PREFIX << SPACING_PREFIX << "Threads: ";
        for (auto thrd : node->threads)
        {
            std::cout << thrd << " ";
        }
        std::cout << std::endl;
    }

    std::cout << prefix << (isLast ? LAST_ITEM_PREFIX : BRANCHING_ITEM_PREFIX);
    std::cout << node->pid << "      " << node->name << std::endl;

    std::string newPrefix = prefix + (isLast ? SPACING_PREFIX : NOT_LAST_ITEM_PREFIX);

    for (size_t i = 0; i < node->children.size(); i++)
    {
        bool lastChild = (i == node->children.size() - 1);
        PrintRecursive(node->children[i], newPrefix, lastChild, showThreads);
    }
}
