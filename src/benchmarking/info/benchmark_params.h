#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

#include "utils/log.h"

namespace nostalgia {

    struct BenchmarkParamSpec {
    std::string key;
    std::string type;            // "int", "string", etc 
    std::string description;         
    bool required = true;
    std::optional<std::string> default_value; // string-formatted for CLI
};

    class BenchmarkParams {
public:
    template <typename T>
    void set(const std::string& key, T value) {
        _params[key] = std::make_shared<Holder<T>>(std::move(value));
    }

    template <typename T>
    std::optional<T> get(const std::string& key) const {
        auto it = _params.find(key);
        if (it == _params.end()) return std::nullopt;
        auto holder = std::dynamic_pointer_cast<Holder<T>>(it->second);
        if (!holder) return std::nullopt;
        return holder->value;
    }

    bool contains(const std::string& key) const {
        return _params.contains(key);
    }

    const auto& debugDump() const { return _params; }

private:
    struct BaseHolder { virtual ~BaseHolder() = default; };
    template <typename T>
    struct Holder : BaseHolder {
        T value;
        Holder(T v) : value(std::move(v)) {}
    };
    std::unordered_map<std::string, std::shared_ptr<BaseHolder>> _params;
    };    
}