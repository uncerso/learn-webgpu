#pragma once

#include "resources.hpp"

#include <cstdint>

namespace runtime::graphics_engine {

struct IRenderPassEncoder : NonMovable {
    virtual ~IRenderPassEncoder() = default;

    virtual void bindRenderPipeline(IRenderPipeline& pipeline) = 0;
    virtual void draw(uint32_t vertexCount) = 0;
};

} // runtime::graphics_engine
