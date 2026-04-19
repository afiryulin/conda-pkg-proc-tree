#pragma once

#include <fmt/color.h>

#include "ProcessInfo.h"

struct PrintHelper final
{
    static fmt::color PickColor(const std::shared_ptr<ProcessInfo> &node)
    {
        if (node->parentPid == 0)
            return fmt::color::lime_green;

        if (node->threads.size() > 10)
            return fmt::color::medium_purple;

        if (node->children.empty())
            return fmt::color::gold;

        return fmt::color::sky_blue;
    }

    static const std::string PickIcon(const std::shared_ptr<ProcessInfo> &node)
    {
        if (node->parentPid == 0)
            return GREEN_CIRCLE;
        if (node->threads.size() > 10)
            return PURPURE_CIRCLE;
        if (node->children.empty())
            return GEAR;

        return BLUE_CIRCLE;
    }

    inline static const std::string GREEN_CIRCLE = "\U0001F6E2";
    inline static const std::string PURPURE_CIRCLE = "\U0001F7E3";
    inline static const std::string BLUE_CIRCLE = "\U0001F535";
    inline static const std::string GEAR = "\u2699\ufe0f";
    inline static const std::string TREE = "\U0001F333";
    inline static const std::string THREAD = "\U0001F3F5";
};