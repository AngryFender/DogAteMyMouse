#pragma once

#include "./interfaces/imatchengine.h"
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>

inline uint16_t char_into_uint16_t(char high, char low)
{
    return static_cast<uint16_t>(high) << 8 | static_cast<uint16_t>(low);
}

class MatchEngine final : public IMatchEngine 
{
public:
    ~MatchEngine() override = default;
    MatchEngine(std::unique_ptr<IKeyGen>&& keygen) :keygen_(std::move(keygen))
    {
    }

    std::optional<std::pair<float, float>> match_target(const char key) override
    {
        std::optional<std::pair<float, float>> result = std::nullopt;

        buffer_.push(key);
        if (buffer_.size() > BUFFER_SIZE)
            buffer_.pop();

        uint16_t key = char_into_uint16_t(buffer_.front(), buffer_.back());
        if (map_.contains(key))
            result = map_[key];
        
        return result;
    }

    std::vector<Key> get_targets(const std::vector<std::pair<float, float>>& points) override
    {
        const size_t size = points.size();
        map_.clear();
        map_.reserve(size);

        std::vector<Key> keys;
        keys.reserve(size);

        keys = keygen_->generate(points);

        if (keys.size() == size)
        {
            for (int i = 0; i < size; ++i)
            {
                uint16_t key = char_into_uint16_t(*keys[i].data(), *keys[i].data() + 1);
                map_[key] = points[i];
            }
        }
        else
        {
            keys.clear();
        }

        return keys;
    }

private:
    std::queue<char> buffer_;
    std::unordered_map<uint16_t, std::pair<float, float>> map_;
    std::unique_ptr<IKeyGen> keygen_;

};
