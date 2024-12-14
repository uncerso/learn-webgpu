#pragma once

#include "instance.hpp"
#include "adapter.hpp"
#include "device.hpp"
#include "resource_holder.hpp"

#include <runtime/common/exception.hpp>
#include <runtime/common/nonmovable.h>

#include <webgpu/webgpu.hpp>

struct GLFWwindow;

namespace runtime::graphics_engine::webgpu {

namespace internal {
void surfaceDestructor(WGPUSurface surface);
} // internal

RESOURCE_HOLDER(Surface, WGPUSurface, surface, internal::surfaceDestructor);

class Surface final : public SurfaceResourceHolder {
public:
    Surface(uint32_t width, uint32_t height, Instance const& instance, Adapter const& adapter, Device const& device, GLFWwindow& window);

    WGPUTextureFormat format() const noexcept { return _surfaceFormat; }

private:
    WGPUTextureFormat _surfaceFormat;
};

} // runtime::graphics_engine::webgpu
