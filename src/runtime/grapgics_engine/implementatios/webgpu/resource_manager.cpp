#include "resource_manager.hpp"
#include "wrappers/shader_module.hpp"
#include "wrappers/render_pipeline.hpp"

namespace runtime::graphics_engine::webgpu {

ResourceManager::ResourceManager(Device& device, Surface& surface)
    : _device(device)
    , _surface(surface)
{}

std::unique_ptr<IRenderPipeline> ResourceManager::createRenderPipeline(PipelineConfig config, char const* shaderModule) {
    ShaderModule module(_device, shaderModule);
    return std::make_unique<RenderPipeline>(config, module, _device, _surface);
}

std::unique_ptr<IMesh> ResourceManager::createMesh(std::span<std::byte const> , size_t ) {
    return nullptr;
}

} // runtime::graphics_engine::webgpu
