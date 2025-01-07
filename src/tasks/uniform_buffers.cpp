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

@group(0) @binding(0) var<uniform> uTime: f32;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    let ratio = 640.0 / 480.0;

    var offset = vec2f(-0.6875, -0.463);
    offset += 0.3 * vec2f(cos(uTime), sin(uTime));

    out.position = vec4f(in.pos.x + offset.x, (in.pos.y + offset.y) * ratio, 0.0, 1.0);
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

std::array<Vertex, 15> vertexData {{
    {{0.5  , 0.0  }, {0.0, 0.353, 0.612}},
    {{1.0  , 0.866}, {0.0, 0.353, 0.612}},
    {{0.0  , 0.866}, {0.0, 0.353, 0.612}},
    {{0.75 , 0.433}, {0.0, 0.4  , 0.7}},
    {{1.25 , 0.433}, {0.0, 0.4  , 0.7}},
    {{1.0  , 0.866}, {0.0, 0.4  , 0.7}},
    {{1.0  , 0.0  }, {0.0, 0.463, 0.8}},
    {{1.25 , 0.433}, {0.0, 0.463, 0.8}},
    {{0.75 , 0.433}, {0.0, 0.463, 0.8}},
    {{1.25 , 0.433}, {0.0, 0.525, 0.91}},
    {{1.375, 0.65 }, {0.0, 0.525, 0.91}},
    {{1.125, 0.65 }, {0.0, 0.525, 0.91}},
    {{1.125, 0.65 }, {0.0, 0.576, 1.0}},
    {{1.375, 0.65 }, {0.0, 0.576, 1.0}},
    {{1.25 , 0.866}, {0.0, 0.576, 1.0}},
}};

std::array<uint16_t, 15> indexData {{
     0,  1,  2,
     3,  4,  5,
     6,  7,  8,
     9, 10, 11,
    12, 13, 14,
}};

constexpr std::initializer_list<std::initializer_list<runtime::graphics_engine::Attribute const>> attributes {{
    {runtime::graphics_engine::Attribute::Type::Float32x2},
    {runtime::graphics_engine::Attribute::Type::Float32x3},
}};

constexpr size_t UBO_BINDING_POINT = 0;

constexpr std::initializer_list<runtime::graphics_engine::Layout const> layouts {{
    .bindingPoint = UBO_BINDING_POINT,
    .access = runtime::graphics_engine::Layout::Access::Vertex,
}};

struct Renderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<Renderer>(renderer.resourceManager());
    }

    Renderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource, attributes, layouts))
        , mesh(manager.createMesh(std::span{indexData}, std::span{vertexData}))
        , ubo(manager.createUniformBuffer(0.0f))
    {}

    void render(FrameContext const& context) override {
        rewrite(*ubo, context.renderTime);
        auto& encoder = context.renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.bind(*ubo, UBO_BINDING_POINT);
        encoder.draw(*mesh);
    }

    glm::vec4 bgColor() const noexcept override { return {0.05, 0.05, 0.05, 1.0}; }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
    std::shared_ptr<runtime::graphics_engine::ITUniformBuffer<float>> ubo;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "007 Uniform buffer"; }
    RendererCreator creator() const noexcept override { return Renderer::create; }
} _;

} // namespace task1

} // namespace
} // namespace tasks
