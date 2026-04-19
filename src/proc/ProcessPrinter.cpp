#include <functional>
#include <fmt/core.h>
#include <fmt/color.h>

#include "ProcessPrinter.h"
#include "PrintHelper.h"

void ProcessPrinter::Print(const std::vector<std::shared_ptr<ProcessInfo>> roots, bool showThreads)
{
    std::function<int(const std::shared_ptr<ProcessInfo>)> countFn = [&](const std::shared_ptr<ProcessInfo> pi)
    {
        int total = 1;
        for (const auto &c : pi->children)
        {
            total += countFn(c);
        }

        return total;
    };

    int total = 0;
    for (const auto &root : roots)
    {
        total += countFn(root);
    }

    fmt::print(fmt::fg(fmt::color::cyan) | fmt::emphasis::bold, "\n{} PROC-TREE ({} processes)\n\n", PrintHelper::TREE, total);

    for (const auto &root : roots)
    {
        PrintRecursive(root, "", false, showThreads);
    }

    fmt::print("\n");
}

void ProcessPrinter::PrintRecursive(const std::shared_ptr<ProcessInfo> node, const std::string &prefix, bool isLast, bool showThreads)
{
    if (!node)
        return;

    std::string branch = isLast ? LAST_ITEM_PREFIX : BRANCHING_ITEM_PREFIX;
    std::string newPrefix = prefix + (isLast ? SPACING_PREFIX : NOT_LAST_ITEM_PREFIX);

    auto color = PrintHelper::PickColor(node);
    auto icon = PrintHelper::PickIcon(node);

    std::string pidStr = fmt::format("{:05}", node->pid);

    fmt::print("{}{}", prefix, branch);
    fmt::print(fmt::fg(color) | fmt::emphasis::bold,
               "{} [{}] {}\n",
               icon,
               pidStr,
               node->name);

    bool hasThreads = showThreads && !node->threads.empty();
    size_t totalBranches = node->children.size() + (hasThreads ? 1 : 0);

    size_t branchIndex = 0;
    if (hasThreads)
    {
        bool threadsIsLast = (++branchIndex == totalBranches);

        std::string threadsPrefix = isLast ? SPACING_PREFIX : NOT_LAST_ITEM_PREFIX;
        std::string tBranch = threadsIsLast ? LAST_ITEM_PREFIX : BRANCHING_ITEM_PREFIX;
        std::string tPrefix = newPrefix + (threadsIsLast ? SPACING_PREFIX : NOT_LAST_ITEM_PREFIX);

        fmt::print("{}{}", newPrefix, tBranch);
        fmt::print(fmt::fg(fmt::color::orange) | fmt::emphasis::bold,
                   "{} threads ({})\n",
                   PrintHelper::THREAD,
                   node->threads.size());

        for (size_t i = 0; i < node->threads.size(); ++i)
        {
            bool lastTid = (i == node->threads.size() - 1);
            std::string branchTid = lastTid ? LAST_ITEM_PREFIX : BRANCHING_ITEM_PREFIX;
            fmt::print("{}{}tid {}\n", tPrefix, branchTid, node->threads[i]);
        }
    }

    for (size_t i = 0; i < node->children.size(); i++)
    {
        bool isLastChild = (i == node->children.size() - 1);
        PrintRecursive(node->children[i], newPrefix, isLastChild, showThreads);
    }
}
