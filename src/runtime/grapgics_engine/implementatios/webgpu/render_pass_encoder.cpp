#include "render_pass_encoder.hpp"
#include "wrappers/render_pipeline.hpp"

#include "mesh.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

RenderPassEncoder::RenderPassEncoder(CommandEncoder const& encoder, TextureView const& textureView) {
    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = textureView.view();
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = WGPUColor{ 0.9, 0.1, 0.2, 1.0 };

    WGPURenderPassDescriptor renderPassDesc = {};
    renderPassDesc.label = "RenderPass";
    renderPassDesc.nextInChain = nullptr;
    renderPassDesc.colorAttachmentCount = 1;
    renderPassDesc.colorAttachments = &renderPassColorAttachment;
    renderPassDesc.depthStencilAttachment = nullptr;
    renderPassDesc.timestampWrites = nullptr;

    _encoder = wgpuCommandEncoderBeginRenderPass(encoder.encoder(), &renderPassDesc);
    REQUIRE(_encoder, "Could not create WebGPU render pass");
}

RenderPassEncoder::~RenderPassEncoder() {
    wgpuRenderPassEncoderEnd(_encoder);
    wgpuRenderPassEncoderRelease(_encoder);
}

void RenderPassEncoder::bindRenderPipeline(IRenderPipeline& pipeline) {
    wgpuRenderPassEncoderSetPipeline(_encoder, static_cast<RenderPipeline*>(&pipeline)->pipeline());
}

void RenderPassEncoder::draw(uint32_t vertexCount) {
    wgpuRenderPassEncoderDraw(_encoder, vertexCount, 1, 0, 0);
}

void RenderPassEncoder::draw(IMesh& mesh) {
    static_cast<DrawableMesh*>(&mesh)->draw(_encoder);
}

} // namespace runtime::graphics_engine::webgpu
