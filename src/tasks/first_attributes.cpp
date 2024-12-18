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

// We embbed the source of the shader module here
const char* shaderSource = R"(
@vertex
fn vs_main(@location(0) vPos: vec2f) -> @builtin(position) vec4f {
    return vec4f(vPos, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
    return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";

std::array<glm::vec2, 6> vertexData{{
    {-0.5f, -0.5f},
    {+0.5f, -0.5f},
    {+0.0f, +0.5f},

    {-0.55f, -0.5f},
    {-0.05f, +0.5f},
    {-0.55f, +0.5f},
}};

constexpr std::initializer_list<std::initializer_list<runtime::graphics_engine::Attribute const>> attributes {
    {{runtime::graphics_engine::Attribute::Type::Float32x2}},
};

struct SingleAttributeRenderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<SingleAttributeRenderer>(renderer.resourceManager());
    }

    SingleAttributeRenderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource, attributes))
        , mesh(manager.createMesh(std::span{vertexData}))
    {}

    void render(runtime::graphics_engine::IRenderer& renderer) override {
        auto& encoder = renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.draw(*mesh);
    }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "002 Single attribute"; }
    RendererCreator creator() const noexcept override { return SingleAttributeRenderer::create; }
} _;

} // namespace task1

namespace task2 {

// We embbed the source of the shader module here
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

std::array<Vertex, 6> vertexData {{
    {{-0.5f, -0.5f}, {1.0, 0.0, 0.0}},
    {{+0.5f, -0.5f}, {0.0, 1.0, 0.0}},
    {{+0.0f, +0.5f}, {0.0, 0.0, 1.0}},

    {{-0.55f, -0.5f}, {1.0, 1.0, 0.0}},
    {{-0.05f, +0.5f}, {0.0, 1.0, 1.0}},
    {{-0.55f, +0.5f}, {1.0, 0.0, 1.0}},
}};

constexpr std::initializer_list<std::initializer_list<runtime::graphics_engine::Attribute const>> attributes {{
    {runtime::graphics_engine::Attribute::Type::Float32x2},
    {runtime::graphics_engine::Attribute::Type::Float32x3},
}};

struct MultipleAttributeRenderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<MultipleAttributeRenderer>(renderer.resourceManager());
    }

    MultipleAttributeRenderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource, attributes))
        , mesh(manager.createMesh(std::span{vertexData}))
    {}

    void render(runtime::graphics_engine::IRenderer& renderer) override {
        auto& encoder = renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.draw(*mesh);
    }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "003 Multiple attribute single buffer"; }
    RendererCreator creator() const noexcept override { return MultipleAttributeRenderer::create; }
} _;

} // namespace task2

namespace task3 {

std::array<glm::vec2, 6> posVertexData {{
    {-0.5f, -0.5f},
    {+0.5f, -0.5f},
    {+0.0f, +0.5f},

    {-0.55f, -0.5f},
    {-0.05f, +0.5f},
    {-0.55f, +0.5f},
}};

std::array<glm::vec3, 6> colorVertexData {{
    {1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {0.0, 0.0, 1.0},

    {1.0, 1.0, 0.0},
    {0.0, 1.0, 1.0},
    {1.0, 0.0, 1.0},
}};

using Attribute = runtime::graphics_engine::Attribute;

constexpr std::initializer_list<std::initializer_list<Attribute const>> attributes {
    {{Attribute::Type::Float32x2}},
    {{Attribute::Type::Float32x3}},
};

struct MultipleAttributeRenderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<MultipleAttributeRenderer>(renderer.resourceManager());
    }

    MultipleAttributeRenderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, task2::shaderSource, attributes))
        , mesh(manager.createMesh(std::span{posVertexData}, std::span{colorVertexData}))
    {}

    void render(runtime::graphics_engine::IRenderer& renderer) override {
        auto& encoder = renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.draw(*mesh);
    }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "004 Multiple attribute many buffers"; }
    RendererCreator creator() const noexcept override { return MultipleAttributeRenderer::create; }
} _;

} // namespace task3
} // namespace
} // namespace tasks
