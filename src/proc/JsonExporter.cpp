#include <nlohmann/json.hpp>
#include <iostream>

#include "JsonExporter.h"

using json = nlohmann::json;
void to_json(json &data, const ProcessInfo &pi)
{
    data = json{
        {"pid", pi.pid},
        {"parentPid", pi.parentPid},
        {"name", pi.name},
        {"threads", pi.threads},
        {"children", pi.children}};
}

void JsonExporter::Export(std::vector<std::shared_ptr<ProcessInfo>> roots, int depth)
{
    // for (const auto &root : roots)
    // {
    //     if (!root)
    //         continue;

    //     std::string indent(depth * 2, ' ');
    //     std::cout << indent << "[PID: " << root->pid << "] " << root->name << std::endl;

    //     Export(root->children, depth + 1);
    // }

    json j = roots;
    std::cout << j.dump(4) << std::endl;
}