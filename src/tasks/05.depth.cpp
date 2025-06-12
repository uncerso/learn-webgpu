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
    @location(0) pos: vec3f,
    @location(1) color: vec3f,
};

struct FragmentInput {
    @location(0) color: vec3f,
};

struct VertexOutput {
    @builtin(position) position: vec4f,
    @location(0) color: vec3f,
}

struct UniformData {
    color: vec4f,
    time: f32,
};

@group(0) @binding(0) var<uniform> uData: UniformData;

@vertex
fn vs_main(in: VertexInput) -> VertexOutput {
    var out: VertexOutput;
    let ratio = 640.0 / 480.0;

    let angle = uData.time;
    let alpha = cos(angle);
    let beta = sin(angle);
    var pos = vec3f(
        in.pos.x,
        alpha * in.pos.y + beta * in.pos.z,
        alpha * in.pos.z - beta * in.pos.y,
    );
    out.position = vec4f(pos.x, pos.y * ratio, pos.z * 0.5 + 0.5, 1.0);
    out.color = in.color;
    return out;
}

@fragment
fn fs_main(in: FragmentInput) -> @location(0) vec4f {
    return vec4f(pow(in.color * uData.color.rgb, vec3f(2.2)), uData.color.a);
}
)";

struct Vertex {
    glm::vec3 pos;
    glm::vec3 color;
};

std::array<Vertex, 5> vertexData {{
    // # The base
    {{-0.5, -0.5, -0.3}, {1.0, 1.0, 1.0}},
    {{+0.5, -0.5, -0.3}, {1.0, 1.0, 1.0}},
    {{+0.5, +0.5, -0.3}, {1.0, 1.0, 1.0}},
    {{-0.5, +0.5, -0.3}, {1.0, 1.0, 1.0}},

    // # And the tip of the pyramid
    {{+0.0, +0.0, +0.5}, {0.5, 0.5, 0.5}},
}};

std::array<uint16_t, 18> indexData {{
    // # Base
     0, 1, 2,
     0, 2, 3,
    // # Sides
     0, 1, 4,
     1, 2, 4,
     2, 3, 4,
     3, 0, 4,
}};

struct UniformData {
    glm::vec4 color;
    float time;
};

constexpr size_t UBO_BINDING_POINT = 0;

constexpr std::initializer_list<runtime::graphics_engine::Layout const> layouts {{
    .bindingPoint = UBO_BINDING_POINT,
    .access = runtime::graphics_engine::Layout::Access::VertexAndFragment,
}};

constexpr std::initializer_list<std::initializer_list<runtime::graphics_engine::Attribute const>> attributes {{
    {runtime::graphics_engine::Attribute::Type::Float32x3},
    {runtime::graphics_engine::Attribute::Type::Float32x3},
}};

struct Renderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<Renderer>(renderer.resourceManager());
    }

    Renderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource, attributes, layouts))
        , mesh(manager.createMesh(std::span{indexData}, std::span{vertexData}))
        , ubo(manager.createUniformBuffer(UniformData{}))
    {}

    void render(FrameContext const& context) override {
        rewrite(*ubo, UniformData{{0.0f, 1.0f, 0.4f, 1.0f}, context.renderTime});
        auto& encoder = context.renderer.renderPassEncoder();
        encoder.bindRenderPipeline(*pipeline);
        encoder.bind(*ubo, UBO_BINDING_POINT);
        encoder.draw(*mesh);
    }

    glm::vec4 bgColor() const noexcept override { return {0.05, 0.05, 0.05, 1.0}; }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
    std::unique_ptr<runtime::graphics_engine::IMesh> mesh;
    std::shared_ptr<runtime::graphics_engine::ITUniformBuffer<UniformData>> ubo;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "009 Depth buffer"; }
    RendererCreator creator() const noexcept override { return Renderer::create; }
} _;

} // namespace task1

} // namespace
} // namespace tasks
