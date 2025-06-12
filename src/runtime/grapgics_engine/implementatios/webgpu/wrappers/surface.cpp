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

    _surfaceFormat = wgpuSurfaceGetPreferredFormat(_surface, adapter.adapter());
    WGPUSurfaceConfiguration config {
        .nextInChain = nullptr,
        .device = device.device(),
        .format = _surfaceFormat,
        .usage = WGPUTextureUsage_RenderAttachment,
        // And we do not need any particular view format:
        .viewFormatCount = 0,
        .viewFormats = nullptr,
        .alphaMode = WGPUCompositeAlphaMode_Auto,
        .width = width,
        .height = height,
        .presentMode = WGPUPresentMode_Fifo,
    };
    wgpuSurfaceConfigure(_surface, &config);
}

} // namespace runtime::graphics_engine::webgpu
