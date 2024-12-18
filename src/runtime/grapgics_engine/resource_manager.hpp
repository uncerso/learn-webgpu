#pragma once

#include "attribute.hpp"
#include "resources.hpp"
#include "pipeline_config.hpp"
#include "binaty_vertices.hpp"

#include <memory>
#include <span>

namespace runtime::graphics_engine {

struct IResourceManager : NonMovable {
    virtual ~IResourceManager() = default;

    template <typename ... Vertex, size_t ... N>
    std::unique_ptr<IMesh> createMesh(std::span<Vertex, N> const ... vertices);

    virtual std::unique_ptr<IRenderPipeline> createRenderPipeline(
        PipelineConfig config,
        char const* shaderModule,
        std::initializer_list<std::initializer_list<Attribute const>> vertexAttributes = {}) = 0;

protected:
    virtual std::unique_ptr<IMesh> createMesh(std::span<BinaryVertices const> vertices) = 0;
};

template <typename ... Vertex, size_t ... N>
std::unique_ptr<IMesh> IResourceManager::createMesh(std::span<Vertex, N> const ... vertices) {
    static_assert(sizeof...(vertices) > 0, "Mesh must contains at least one vertex buffer");
    BinaryVertices const binVertices[sizeof...(vertices)] = {{
        .vertices = std::span{reinterpret_cast<std::byte const*>(vertices.data()), vertices.size() * sizeof(Vertex)},
        .stride = sizeof(Vertex),
    }...};
    return createMesh(binVertices);
}

} // namespace runtime::graphics_engine
