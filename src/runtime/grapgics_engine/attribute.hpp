#pragma once
#include <cstdint>

namespace runtime::graphics_engine {

struct Attribute {
    enum class Type : uint8_t {
        Float32,
        Float32x2,
        Float32x3,
        Float32x4,
        UInt32,
        UInt32x2,
        UInt32x3,
        UInt32x4,
    };

    Type type;
};

} // namespace runtime::graphics_engine
