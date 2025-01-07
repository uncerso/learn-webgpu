#pragma once

#include "resources.hpp"

#include <cstdint>

namespace runtime::graphics_engine {

struct IRenderPassEncoder : NonMovable {
    virtual ~IRenderPassEncoder() = default;

    virtual void bindRenderPipeline(IRenderPipeline& pipeline) = 0;
    virtual void bind(IUniformBuffer& buffer, uint32_t bindingPoint) = 0; // in fact we should retain resources

    virtual void draw(uint32_t vertexCount) = 0;
    virtual void draw(IMesh& mesh) = 0;
};

} // namespace runtime::graphics_engine
