#pragma once

#include <cstddef>
#include <span>

namespace runtime::graphics_engine {

struct BinaryVertices {
    std::span<std::byte const> vertices;
    size_t stride;
};

} // namespace runtime::graphics_engine
