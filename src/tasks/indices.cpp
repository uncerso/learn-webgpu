#include "../core/renderer.hpp"
#include "task_registry.hpp"

#include <runtime/grapgics_engine/attribute.hpp>

#include <glm/glm.hpp>

#include <memory>
#include <array>
#include <initializer_list>

namespace tasks {
namespace {

namespace task1 {

const char* shaderSource = R"(
struct VertexInput {
    @location(0) pos: vec2f,
    @location(1) color: vec3f,
};

struct FragmentInput {
    @location(0) color: vec3f,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
}

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    out.position = vec4f(in.pos, 0.0, 1.0);
    out.color = in.color;
    return out;
}

@fragment
fn fs_main(in: FragmentInput) -> @location(0) vec4f {
    return vec4f(in.color, 1.0);
}
)";

struct Vertex {
    glm::vec2 pos;
    glm::vec3 color;
};

std::array<Vertex, 4> vertexData {{
    {{-0.5f, -0.5f}, {1.0, 0.0, 0.0}},
    {{+0.5f, -0.5f}, {0.0, 1.0, 0.0}},
    {{+0.5f, +0.5f}, {0.0, 0.0, 1.0}},
    {{-0.5f, +0.5f}, {1.0, 1.0, 0.0}},
}};

std::array<uint16_t, 6> indexData {{0, 1, 2, 0, 2, 3}};

constexpr std::initializer_list<std::initializer_list<runtime::graphics_engine::Attribute const>> attributes {{
    {runtime::graphics_engine::Attribute::Type::Float32x2},
    {runtime::graphics_engine::Attribute::Type::Float32x3},
}};

struct Renderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<Renderer>(renderer.resourceManager());
    }

    Renderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource, attributes))
        , mesh(manager.createMesh(std::span{indexData}, std::span{vertexData}))
    {}

    void render(FrameContext const& context) override {
        auto& encoder = context.renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.draw(*mesh);
    }

    glm::vec4 bgColor() const noexcept override { return {0.05, 0.05, 0.05, 1.0}; }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "005 Single vertex buffer + indices"; }
    RendererCreator creator() const noexcept override { return Renderer::create; }
} _;

} // namespace task1

namespace task2 {

std::array<glm::vec2, 4> posVertexData {{
    {-0.5f, -0.5f},
    {+0.5f, -0.5f},
    {+0.5f, +0.5f},
    {-0.5f, +0.5f},
}};

std::array<glm::vec3, 4> colorVertexData {{
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},
    {1.0, 1.0, 0.0},
}};

using Attribute = runtime::graphics_engine::Attribute;

constexpr std::initializer_list<std::initializer_list<Attribute const>> attributes {
    {{Attribute::Type::Float32x2}},
    {{Attribute::Type::Float32x3}},
};

struct Renderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<Renderer>(renderer.resourceManager());
    }

    Renderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, task1::shaderSource, attributes))
        , mesh(manager.createMesh(std::span{task1::indexData}, std::span{posVertexData}, std::span{colorVertexData}))
    {}

    void render(FrameContext const& context) override {
        auto& encoder = context.renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.draw(*mesh);
    }

    glm::vec4 bgColor() const noexcept override { return {0.05, 0.05, 0.05, 1.0}; }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "006 Many vertex buffers + indices"; }
    RendererCreator creator() const noexcept override { return Renderer::create; }
} _;

} // namespace task2
} // namespace
} // namespace tasks
