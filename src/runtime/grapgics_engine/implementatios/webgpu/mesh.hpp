#pragma once

#include "wrappers/device.hpp"
#include "wrappers/queue.hpp"

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
    ~Mesh();

    void draw(WGPURenderPassEncoder const& encoder) override;

private:
    std::vector<WGPUBuffer> _vertexBuffers;
    uint32_t _vertexCount;
};

} // namespace runtime::graphics_engine::webgpu
