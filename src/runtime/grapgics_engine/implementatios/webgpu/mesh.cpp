#include "mesh.hpp"

namespace runtime::graphics_engine::webgpu {

Mesh::Mesh(Device const& device, Queue const& queue, std::span<BinaryVertices const> vertices)
    : _vertexCount{static_cast<uint32_t>(vertices[0].vertices.size() / vertices[0].stride)}
{
    _vertexBuffers.reserve(vertices.size());

    for (auto const& binVertices : vertices)
        _vertexBuffers.push_back(Buffer{device, queue, binVertices.vertices, WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex});
}

void Mesh::draw(WGPURenderPassEncoder const& encoder) {
    for (uint32_t i = 0; i < _vertexBuffers.size(); ++i) {
        auto const& buffer = _vertexBuffers[i].buffer();
        wgpuRenderPassEncoderSetVertexBuffer(encoder, i, buffer, 0, wgpuBufferGetSize(buffer));
    }
    wgpuRenderPassEncoderDraw(encoder, _vertexCount, 1, 0, 0);
}

IndexedMesh::IndexedMesh(Device const& device, Queue const& queue, std::span<std::span<std::byte const> const> vertices, BinaryVertices const& indices)
    : _indexBuffer{device, queue, indices.vertices, WGPUBufferUsage_CopyDst | WGPUBufferUsage_Index}
    , _indexCount{static_cast<uint32_t>(indices.vertices.size() / indices.stride)}
    , _indexFormat(indices.stride == 2 ? WGPUIndexFormat_Uint16 : WGPUIndexFormat_Uint32)
{
    _vertexBuffers.reserve(vertices.size());

    for (auto const& binVertices : vertices)
        _vertexBuffers.emplace_back(device, queue, binVertices, WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex);
}

void IndexedMesh::draw(WGPURenderPassEncoder const& encoder) {
    wgpuRenderPassEncoderSetIndexBuffer(encoder, _indexBuffer.buffer(), _indexFormat, 0, wgpuBufferGetSize(_indexBuffer.buffer()));
    for (uint32_t i = 0; i < _vertexBuffers.size(); ++i) {
        auto const& buffer = _vertexBuffers[i].buffer();
        wgpuRenderPassEncoderSetVertexBuffer(encoder, i, buffer, 0, wgpuBufferGetSize(buffer));
    }
    wgpuRenderPassEncoderDrawIndexed(encoder, _indexCount, 1, 0, 0, 0);
}

} // namespace runtime::graphics_engine::webgpu
