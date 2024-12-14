#include "../core/renderer.hpp"
#include "task_registry.hpp"

#include <memory>

namespace tasks {
namespace {

// We embbed the source of the shader module here
const char* shaderSource = R"(
@vertex
fn vs_main(@builtin(vertex_index) in_vertex_index: u32) -> @builtin(position) vec4f {
	var p = vec2f(0.0, 0.0);
	if (in_vertex_index == 0u) {
		p = vec2f(-0.5, -0.5);
	} else if (in_vertex_index == 1u) {
		p = vec2f(0.5, -0.5);
	} else {
		p = vec2f(0.0, 0.5);
	}
	return vec4f(p, 0.0, 1.0);
}

@fragment
fn fs_main() -> @location(0) vec4f {
	return vec4f(0.0, 0.4, 1.0, 1.0);
}
)";

struct FirstTriangleRenderer : public core::Renderer {
    static std::unique_ptr<core::Renderer> create(runtime::graphics_engine::IRenderer& renderer) {
        return std::make_unique<FirstTriangleRenderer>(renderer.resourceManager());
    }

    FirstTriangleRenderer(runtime::graphics_engine::IResourceManager& manager)
        : pipeline(manager.createRenderPipeline({}, shaderSource))
    {}

    void render(runtime::graphics_engine::IRenderer& renderer) override {
		auto& encoder = renderer.renderPassEncoder();
		encoder.bindRenderPipeline(*pipeline);
		encoder.draw(3);
    }

    std::unique_ptr<runtime::graphics_engine::IRenderPipeline> pipeline;
};

struct : TaskRegistry {
    std::string_view name() const noexcept override { return "First triangle"; }
    RendererCreator creator() const noexcept override { return FirstTriangleRenderer::create; }
} _;

} // namespace
} // tasks
