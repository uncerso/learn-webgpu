#pragma once

#include "resources.hpp"
#include "pipeline_config.hpp"

#include <memory>
#include <span>

namespace runtime::graphics_engine {

struct IResourceManager : NonMovable {
    virtual ~IResourceManager() = default;

    template <typename Vertex>
    std::unique_ptr<IMesh> createMesh(std::span<Vertex const> vertices);

    virtual std::unique_ptr<IRenderPipeline> createRenderPipeline(PipelineConfig config, char const* shaderModule) = 0;

protected:
    virtual std::unique_ptr<IMesh> createMesh(std::span<std::byte const> vertices, size_t stride) = 0;
};

template <typename Vertex>
std::unique_ptr<IMesh> IResourceManager::createMesh(std::span<Vertex const> vertices) {
    return createMesh(std::span{reinterpret_cast<std::byte const*>(vertices.data()), vertices.size()}, sizeof(Vertex));
}

} // runtime::graphics_engine
