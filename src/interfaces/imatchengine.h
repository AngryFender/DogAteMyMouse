#pragma once
#include <optional>
#include <vector>
#include <string>

class IMatchEngine
{
public:
    ~IMatchEngine() = default;
    virtual std::optional<std::pair<float, float>> match_target(const char key) = 0;
    virtual std::vector<std::string> get_targets(const std::vector<std::pair<float, float>>& points) = 0;
};