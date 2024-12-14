#include "surface.hpp"

#include <runtime/common/assert.hpp>

#include <glfw3webgpu.h>

namespace runtime::graphics_engine::webgpu {

namespace internal {

void surfaceDestructor(WGPUSurface surface) {
    wgpuSurfaceUnconfigure(surface);
    wgpuSurfaceRelease(surface);
}

} // internal

Surface::Surface(uint32_t width, uint32_t height, Instance const& instance, Adapter const& adapter, Device const& device, GLFWwindow& window)
    : SurfaceResourceHolder(glfwGetWGPUSurface(instance.instance(), &window))
{
    REQUIRE(_surface, "Could not create WebGPU surface");

    WGPUSurfaceConfiguration config = {};
    config.nextInChain = nullptr;
    config.width = width;
    config.height = height;

    _surfaceFormat = wgpuSurfaceGetPreferredFormat(_surface, adapter.adapter());
    config.format = _surfaceFormat;
    config.usage = WGPUTextureUsage_RenderAttachment;
    config.device = device.device();
    config.presentMode = WGPUPresentMode_Fifo;
    config.alphaMode = WGPUCompositeAlphaMode_Auto;
    // And we do not need any particular view format:
    config.viewFormatCount = 0;
    config.viewFormats = nullptr;
    wgpuSurfaceConfigure(_surface, &config);
}

} // runtime::graphics_engine::webgpu
