#pragma once

#include "wrappers/command_encoder.hpp"
#include "wrappers/texture_view.hpp"

#include <runtime/grapgics_engine/render_pass_encoder.hpp>
#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

class RenderPassEncoder final : public IRenderPassEncoder {
public:
    RenderPassEncoder(CommandEncoder const& encoder, TextureView const& textureView);

    ~RenderPassEncoder();

    void bindRenderPipeline(IRenderPipeline& pipeline) override;
    void draw(uint32_t vertexCount) override;
    void draw(IMesh& mesh) override;

private:
    WGPURenderPassEncoder _encoder;
};

} // namespace runtime::graphics_engine::webgpu
