#include "wrappers/instance.hpp"
#include "wrappers/adapter.hpp"
#include "wrappers/device.hpp"
#include "wrappers/surface.hpp"
#include "wrappers/queue.hpp"
#include "wrappers/texture_view.hpp"
#include "render_pass_encoder.hpp"

#include "resource_manager.hpp"

#include <runtime/grapgics_engine/renderer.hpp>

#include <webgpu/wgpu.h>

namespace runtime::graphics_engine::webgpu {

class Renderer final : public IRenderer {
public:
    Renderer(webgpu::Device&& device, webgpu::Surface&& surface, webgpu::Queue&& queue);

    IResourceManager& resourceManager() override { return *_resourceManager; }
    IRenderPassEncoder& renderPassEncoder() override;

    bool tryStartFrame(glm::vec4 const& clearColor) override;
    void finishFrame() override;

private:
    webgpu::Device _device;
    webgpu::Surface _surface;
    webgpu::Queue _queue;

    std::unique_ptr<ResourceManager> _resourceManager;

    std::optional<TextureView> _textureView;
    std::optional<CommandEncoder> _encoder;
    std::optional<RenderPassEncoder> _renderPassEncoder;
};

Renderer::Renderer(webgpu::Device&& device, webgpu::Surface&& surface, webgpu::Queue&& queue)
    : _device(std::move(device))
    , _surface(std::move(surface))
    , _queue(std::move(queue))
    , _resourceManager(std::make_unique<ResourceManager>(_device, _surface, _queue))
{}

IRenderPassEncoder& Renderer::renderPassEncoder() {
    REQUIRE(_renderPassEncoder, "A frame is not started");
    return *_renderPassEncoder;
}

bool Renderer::tryStartFrame(glm::vec4 const& clearColor) {
    REQUIRE(!_encoder, "A frame was already started");
    _textureView = TextureView::tryCreate(_surface);
    if (!_textureView)
        return false;

    _encoder.emplace(_device);
    _renderPassEncoder.emplace(*_encoder, *_textureView, _device, clearColor);
    return true;
}

void Renderer::finishFrame() {
    REQUIRE(_encoder, "A frame is not started");
    _renderPassEncoder.reset();
    _queue.submit({*_encoder});
    _encoder.reset();
    _textureView.reset();

    wgpuSurfacePresent(_surface.surface());
    wgpuDevicePoll(_device.device(), false, nullptr);
}

} // namespace runtime::graphics_engine::webgpu

namespace runtime::graphics_engine {

std::unique_ptr<IRenderer> createRenderer(uint32_t width, uint32_t height, GLFWwindow& window) {
    webgpu::Instance instance;
    webgpu::Adapter adapter(instance);
    webgpu::Device device(adapter);
    webgpu::Surface surface(width, height, instance, adapter, device, window);
    webgpu::Queue queue(device);

    return std::make_unique<webgpu::Renderer>(std::move(device), std::move(surface), std::move(queue));
}

} // namespace runtime::graphics_engine
