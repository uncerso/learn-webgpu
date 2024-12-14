#pragma once

#include "wrappers/command_encoder.hpp"
#include "wrappers/texture_view.hpp"

#include <runtime/grapgics_engine/render_pass_encoder.hpp>
#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

class RenderPassEncoder final : public IRenderPassEncoder {
public:
    RenderPassEncoder(CommandEncoder const& encoder, TextureView const& textureView);

    ~RenderPassEncoder();

    void bindRenderPipeline(IRenderPipeline& pipeline) override;
    void draw(uint32_t vertexCount) override;

private:
    WGPURenderPassEncoder _encoder;
};

} // runtime::graphics_engine::webgpu
