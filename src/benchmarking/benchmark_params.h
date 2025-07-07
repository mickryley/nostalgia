#pragma once

#include <unordered_map>
#include <string>
#include <memory>
#include <optional>

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
    }

    template <typename T>
    T get(const std::string& key) const {
        auto it = _params.find(key);
        if (it == _params.end()) throw std::runtime_error("Missing key: " + key);
        auto holder = std::dynamic_pointer_cast<Holder<T>>(it->second);
        if (!holder) throw std::runtime_error("Bad type for key: " + key);
        return holder->value;
    }

    bool contains(const std::string& key) const {
        return _params.contains(key);
    }

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