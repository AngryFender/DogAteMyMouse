#pragma once

#include "./interfaces/imatchengine.h"
#include <queue>
#include <unordered_map>
#include <memory>
#include <utility>
#include <iostream>

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

    std::optional<std::pair<float, float>> match_target_keys(const char keypress) override
    {
        std::optional<std::pair<float, float>> result = std::nullopt;

        buffer_.push(keypress);
        
        if (buffer_.size() < BUFFER_SIZE)
            return result;

        uint16_t key = char_into_uint16_t(buffer_.front(), buffer_.back());
        if (map_.contains(key))
        {
            result = map_[key];
            while (!buffer_.empty()) 
            {
                buffer_.pop();
            }
        }

        while (!buffer_.empty()) {
            buffer_.pop();
        }
        return result;
    }

    std::vector<Key> get_target_keys(const std::vector<std::pair<float, float>>& coordinates, const ScreenInfo& info) override
    {
        const size_t size = coordinates.size();
        map_.clear();
        map_.reserve(size);

        std::vector<Key> keys;
        keys.reserve(size);

        keys = keygen_->generate(coordinates, info);

        if (keys.size() == size)
        {
            for (int i = 0; i < size; ++i)
            {
                const Key& data = keys[i];
                uint16_t key = char_into_uint16_t(data[0], data[1]);
                map_[key] = coordinates[i];
            }
        }
        else
        {
            keys.clear();
        }

        return keys;
    }

    void clear()
    {
        while (!buffer_.empty()) {
            buffer_.pop();
        }
    }

private:
    std::queue<char> buffer_;
    std::unordered_map<uint16_t, std::pair<float, float>> map_;
    std::unique_ptr<IKeyGen> keygen_;

};
