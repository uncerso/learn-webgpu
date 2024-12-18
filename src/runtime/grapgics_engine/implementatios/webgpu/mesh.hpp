#pragma once

#include "wrappers/device.hpp"
#include "wrappers/queue.hpp"
#include "wrappers/buffer.hpp"

#include <runtime/grapgics_engine/resources.hpp>
#include <runtime/grapgics_engine/binaty_vertices.hpp>

#include <webgpu/webgpu.h>

#include <cstddef>
#include <span>
#include <vector>

namespace runtime::graphics_engine::webgpu {

struct DrawableMesh : public IMesh {
    virtual void draw(WGPURenderPassEncoder const& encoder) = 0;
};

class Mesh final : public DrawableMesh {
public:
    Mesh(Device const& device, Queue const& queue, std::span<BinaryVertices const> vertices);

    void draw(WGPURenderPassEncoder const& encoder) override;

private:
    std::vector<Buffer> _vertexBuffers;
    uint32_t _vertexCount;
};

class IndexedMesh final : public DrawableMesh {
public:
    IndexedMesh(Device const& device, Queue const& queue, std::span<std::span<std::byte const> const> vertices, BinaryVertices const& indices);

    void draw(WGPURenderPassEncoder const& encoder) override;

private:
    std::vector<Buffer> _vertexBuffers;
    Buffer _indexBuffer;
    uint32_t _indexCount;
    WGPUIndexFormat _indexFormat;
};

} // namespace runtime::graphics_engine::webgpu
