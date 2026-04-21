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

void JsonExporter::Export(std::vector<std::shared_ptr<ProcessInfo>> roots)
{
    json j = roots;
    std::cout << j.dump(4) << std::endl;
}