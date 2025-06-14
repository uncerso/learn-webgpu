#pragma once

#include <runtime/grapgics_engine/resource_manager.hpp>
#include "wrappers/device.hpp"
#include "wrappers/surface.hpp"
#include "wrappers/queue.hpp"

namespace runtime::graphics_engine::webgpu {

class ResourceManager final : public IResourceManager {
public:
    ResourceManager(Device& device, Surface& surface, Queue& queue);

    std::unique_ptr<IRenderPipeline> createRenderPipeline(
        PipelineConfig config,
        char const*  shaderModule,
        std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes,
        std::initializer_list<Layout const> layouts) override;

protected:
    std::shared_ptr<IUniformBuffer> createUniformBuffer(std::span<std::byte const> bytes) override;
    std::unique_ptr<IMesh> createMesh(std::span<BinaryVertices const> vertices) override;
    std::unique_ptr<IMesh> createIndexedMesh(BinaryVertices const& indices, std::span<std::span<std::byte const> const> vertices) override;

private:
    Device& _device;
    Surface& _surface;
    Queue& _queue;
};

} // namespace runtime::graphics_engine::webgpu
