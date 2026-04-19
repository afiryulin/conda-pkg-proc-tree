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
    if (!node)
        return;

    std::cout << prefix << (isLast ? LAST_ITEM_PREFIX : BRANCHING_ITEM_PREFIX);
    std::cout << "[" << node->pid << "]  " << node->name << std::endl;

    std::string childPrefix = prefix + (isLast ? SPACING_PREFIX : NOT_LAST_ITEM_PREFIX);

    if (showThreads && !node->threads.empty())
    {

        bool hasChildren = !node->children.empty();
        std::string threadConnector = hasChildren ? BRANCHING_ITEM_PREFIX : LAST_ITEM_PREFIX;

        std::cout << childPrefix << "threads: ";
        for (size_t i = 0; i < node->threads.size(); i++)
            std::cout << node->threads[i] << (i == node->threads.size() - 1 ? "" : ", ");

        std::cout << std::endl;
    }

    for (size_t i = 0; i < node->children.size(); i++)
    {
        bool isLastChild = (i == node->children.size() - 1);
        PrintRecursive(node->children[i], childPrefix, isLastChild, showThreads);
    }
}
