#pragma once
#include <cstdint>

namespace runtime::graphics_engine {

struct Layout {
    enum class Access {
        Vertex, Fragment, VertexAndFragment,
    };

    uint32_t bindingPoint;
    Access access;
};

} // namespace runtime::graphics_engine
