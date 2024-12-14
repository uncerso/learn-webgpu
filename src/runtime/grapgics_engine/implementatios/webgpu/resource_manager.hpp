#pragma once

#include <runtime/grapgics_engine/resource_manager.hpp>
#include "wrappers/device.hpp"
#include "wrappers/surface.hpp"

namespace runtime::graphics_engine::webgpu {

class ResourceManager final : public IResourceManager {
public:
    ResourceManager(Device& device, Surface& surface);

    std::unique_ptr<IRenderPipeline> createRenderPipeline(PipelineConfig config, char const*  shaderModule) override;

protected:
    std::unique_ptr<IMesh> createMesh(std::span<std::byte const> vertices, size_t stride) override;

private:
    Device& _device;
    Surface& _surface;
};

} // runtime::graphics_engine::webgpu
