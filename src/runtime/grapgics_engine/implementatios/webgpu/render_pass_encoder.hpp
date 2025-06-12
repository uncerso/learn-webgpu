#pragma once

#include "wrappers/command_encoder.hpp"
#include "wrappers/texture_view.hpp"
#include "wrappers/render_pipeline.hpp"

#include <runtime/grapgics_engine/render_pass_encoder.hpp>
#include <webgpu/webgpu.h>
#include <glm/glm.hpp>

namespace runtime::graphics_engine::webgpu {

class RenderPassEncoder final : public IRenderPassEncoder {
public:
    RenderPassEncoder(
        CommandEncoder const& encoder,
        TextureView const& textureView,
        std::optional<TextureView> const& depthStencilTextureView,
        Device const& device,
        glm::vec4 const& clearColor
    );

    ~RenderPassEncoder();

    void bindRenderPipeline(IRenderPipeline& pipeline) override;
    void bind(IUniformBuffer& buffer, uint32_t bindingPoint) override;

    void draw(uint32_t vertexCount) override;
    void draw(IMesh& mesh) override;

private:
    WGPURenderPassEncoder _encoder;
    Device const& _device;
    RenderPipeline* _activePipeline = nullptr;
};

} // namespace runtime::graphics_engine::webgpu
