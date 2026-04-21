#pragma once

#include <vector>
#include <memory>
#include <nlohmann/json.hpp>

#include "ProcessInfo.h"

namespace nlohmann
{
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>>
    {
        static void to_json(json &j, const std::shared_ptr<T> &val) noexcept
        {
            if (val)
                j = *val;
            else
            {
                j = nullptr;
            }
        }
    };
}

struct JsonExporter final
{
    static void Export(std::vector<std::shared_ptr<ProcessInfo>> roots);
};