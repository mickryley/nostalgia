#pragma once

#include <array>
#include <string>
#include "utils/log.h"

namespace nostalgia {
    // ObjectIDs are shared across allocator variants for parameter passing
    enum class ObjectID {
        NONE = 0,
        Vector2D,
        Vector3D,
        COUNT
    };
    namespace{
        constexpr std::size_t object_count = static_cast<std::size_t>(ObjectID::COUNT);
    }
    constexpr std::array<const char*, object_count> object_id_names = {
        "None",
        "Vector2D",
        "Vector3D"
    };

    static_assert(object_id_names.size() == object_count,
     "ObjectID array size mismatch");

    constexpr std::string object_id_to_string(ObjectID id) {
        return object_id_names[static_cast<std::size_t>(id)];
    }

    struct ObjectType {
        const ObjectID id;
        const std::string label;
        const std::string description;
    };
}