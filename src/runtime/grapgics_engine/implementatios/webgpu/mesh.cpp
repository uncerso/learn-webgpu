#include "mesh.hpp"

namespace runtime::graphics_engine::webgpu {

Mesh::Mesh(Device const& device, Queue const& queue, std::span<BinaryVertices const> vertices) {
    _vertexBuffers.reserve(vertices.size());
    _vertexCount = static_cast<uint32_t>(vertices[0].vertices.size() / vertices[0].stride);

    for (auto const& binVertices : vertices) {
        WGPUBufferDescriptor bufferDesc{};
        bufferDesc.nextInChain = nullptr;
        bufferDesc.size = binVertices.vertices.size();
        bufferDesc.usage = WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex;
        bufferDesc.mappedAtCreation = false;
        auto buffer = wgpuDeviceCreateBuffer(device.device(), &bufferDesc);
        wgpuQueueWriteBuffer(queue.queue(), buffer, 0, binVertices.vertices.data(), binVertices.vertices.size());
        _vertexBuffers.push_back(std::move(buffer));
    }
}

Mesh::~Mesh() {
    for (auto const& buffer : _vertexBuffers)
        wgpuBufferRelease(buffer);
}

void Mesh::draw(WGPURenderPassEncoder const& encoder) {
    for (uint32_t i = 0; i < _vertexBuffers.size(); ++i)
        wgpuRenderPassEncoderSetVertexBuffer(encoder, i, _vertexBuffers[i], 0, wgpuBufferGetSize(_vertexBuffers[i]));
    // wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, _vertexBuffers[0], 0, wgpuBufferGetSize(_vertexBuffers[0]));
    // wgpuRenderPassEncoderSetVertexBuffer(encoder, 0, _vertexBuffer, 0, wgpuBufferGetSize(_vertexBuffer));
    wgpuRenderPassEncoderDraw(encoder, _vertexCount, 1, 0, 0);
}

} // namespace runtime::graphics_engine::webgpu
