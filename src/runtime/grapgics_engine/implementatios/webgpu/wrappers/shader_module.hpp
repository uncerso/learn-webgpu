#pragma once

#include "device.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(ShaderModule, WGPUShaderModule, module, wgpuShaderModuleRelease);

class ShaderModule final : public ShaderModuleResourceHolder {
public:
    ShaderModule(Device const& device, char const* shaderSource);
};

} // runtime::graphics_engine::webgpu
