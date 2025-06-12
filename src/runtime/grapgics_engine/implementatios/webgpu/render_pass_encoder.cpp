#include "render_pass_encoder.hpp"
#include "wrappers/uniform_buffer.hpp"

#include "mesh.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

namespace {

WGPUColor toWGPU(glm::vec4 const& color) {
    return {
        .r = color.r,
        .g = color.g,
        .b = color.b,
        .a = color.a,
    };
}

} // namespace

RenderPassEncoder::RenderPassEncoder(
    CommandEncoder const& encoder,
    TextureView const& textureView,
    std::optional<TextureView> const& depthStencilTextureView,
    Device const& device,
    glm::vec4 const& clearColor
)
    : _device(device)
{
    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = textureView.view();
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = toWGPU(clearColor);

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.label = "RenderPass";
    renderPassDesc.nextInChain = nullptr;
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.occlusionQuerySet = nullptr;
    renderPassDesc.timestampWrites = nullptr;

    // We now add a depth/stencil attachment:
    WGPURenderPassDepthStencilAttachment depthStencilAttachment {
        .view = depthStencilTextureView ? depthStencilTextureView->view() : nullptr,
        // Operation settings comparable to the color attachment
        .depthLoadOp = WGPULoadOp_Clear,
        .depthStoreOp = WGPUStoreOp_Store,
        .depthClearValue = 1.0f,
        // we could turn off writing to the depth buffer globally here
        .depthReadOnly = false,

        // Stencil setup, mandatory but unused
        .stencilLoadOp = WGPULoadOp_Clear,
        .stencilStoreOp = WGPUStoreOp_Store,
        .stencilClearValue = 0,
        .stencilReadOnly = true,
    };

    renderPassDesc.depthStencilAttachment = depthStencilTextureView ? &depthStencilAttachment : nullptr;

    _encoder = wgpuCommandEncoderBeginRenderPass(encoder.encoder(), &renderPassDesc);
    REQUIRE(_encoder, "Could not create WebGPU render pass");
}

RenderPassEncoder::~RenderPassEncoder() {
    wgpuRenderPassEncoderEnd(_encoder);
    wgpuRenderPassEncoderRelease(_encoder);
}

void RenderPassEncoder::bindRenderPipeline(IRenderPipeline& pipeline) {
    auto* renderPipeline = static_cast<RenderPipeline*>(&pipeline);
    if (_activePipeline == renderPipeline)
        return;
    _activePipeline = renderPipeline;
    wgpuRenderPassEncoderSetPipeline(_encoder, _activePipeline->pipeline());
}

void RenderPassEncoder::draw(uint32_t vertexCount) {
    wgpuRenderPassEncoderDraw(_encoder, vertexCount, 1, 0, 0);
}

void RenderPassEncoder::draw(IMesh& mesh) {
    static_cast<DrawableMesh*>(&mesh)->draw(_encoder);
}

void RenderPassEncoder::bind(IUniformBuffer& buffer, uint32_t bindingPoint) {
    ASSERT(_activePipeline);
    WGPUBindGroup bg = static_cast<UniformBuffer*>(&buffer)->makeBindingGroup(_activePipeline->layout.bindingGroupLayoutAt(bindingPoint), _device, bindingPoint);
    wgpuRenderPassEncoderSetBindGroup(_encoder, bindingPoint, bg, 0, nullptr);
}

} // namespace runtime::graphics_engine::webgpu
