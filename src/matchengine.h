#pragma once

#include "./interfaces/imatchengine.h"
#include <queue>
#include <unordered_map>

constexpr size_t BUFFER_SIZE = 2;

class MatchEngine final : public IMatchEngine 
{
public:
    ~MatchEngine() = default;
    MatchEngine() = default;

    std::optional<std::pair<float, float>> match_target(const char key) override
    {
        std::optional<std::pair<float, float>> result = std::nullopt;

        buffer_.push(key);
        if (buffer_.size() > BUFFER_SIZE)
            buffer_.pop();

        uint16_t key = static_cast<uint16_t>(buffer_.front()) << 8 | static_cast<uint16_t>(buffer_.back());
        if (map_.contains(key))
            result = map_[key];
        
        return result;
    }

    std::vector<std::string> get_targets(const std::vector<std::pair<float, float>>& points) override
    {
        std::vector<std::string> result;

        return result;
    }

private:
    std::queue<char> buffer_;
    std::vector<std::string>& targets_;
    std::unordered_map<uint16_t, std::pair<float, float>> map_;

};
