#include "resource_manager.hpp"
#include "mesh.hpp"

#include "wrappers/shader_module.hpp"
#include "wrappers/render_pipeline.hpp"
#include "wrappers/uniform_buffer.hpp"

namespace runtime::graphics_engine::webgpu {

ResourceManager::ResourceManager(Device& device, Surface& surface, Queue& queue)
    : _device(device)
    , _surface(surface)
    , _queue(queue)
{}

std::unique_ptr<IRenderPipeline> ResourceManager::createRenderPipeline(
    PipelineConfig config,
    char const* shaderModule,
    std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes,
    std::initializer_list<Layout const> layouts)
{
    ShaderModule module(_device, shaderModule);
    return std::make_unique<RenderPipeline>(config, module, _device, _surface, vertexAttributes, layouts);
}

std::shared_ptr<IUniformBuffer> ResourceManager::createUniformBuffer(std::span<std::byte const> bytes) {
    return std::make_shared<UniformBuffer>(_device, _queue, bytes);
}

std::unique_ptr<IMesh> ResourceManager::createMesh(std::span<BinaryVertices const> vertices) {
    return std::make_unique<Mesh>(_device, _queue, vertices);
}

std::unique_ptr<IMesh> ResourceManager::createIndexedMesh(BinaryVertices const& indices, std::span<std::span<std::byte const> const> vertices) {
    return std::make_unique<IndexedMesh>(_device, _queue, vertices, indices);
}

} // namespace runtime::graphics_engine::webgpu
