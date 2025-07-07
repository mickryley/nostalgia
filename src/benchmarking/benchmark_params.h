#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

#include "log.h"

namespace nostalgia {

    struct BenchmarkParamSpec {
    std::string key;
    std::string typeName;            // "int", "string", etc 
    std::string description;         // Tooltip/helptext
    bool required = true;
    std::optional<std::string> defaultValue; // string-formatted for CLI
};

    class BenchmarkParams {
public:
    template <typename T>
    void set(const std::string& key, T value) {
        _params[key] = std::make_shared<Holder<T>>(std::move(value));
        //log::print("SET key='{}' value={} -- map size = {}", key, value, _params.size());

		//log::print("Setting {} to {}", key, value);
    }

    template <typename T>
    std::optional<T> get(const std::string& key) const {
        //log::print("GET key='{}' -> {}", key, _params.contains(key) ? "FOUND" : "MISSING");

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

    void mergeFromCLI(const std::unordered_map<std::string, std::string>& cliArgs);

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