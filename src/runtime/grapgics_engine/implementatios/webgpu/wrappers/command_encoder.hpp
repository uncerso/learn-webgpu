#pragma once

#include "device.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(CommandEncoder, WGPUCommandEncoder, encoder, wgpuCommandEncoderRelease);

class CommandEncoder final : public CommandEncoderResourceHolder {
public:
    CommandEncoder(Device const& device);
};

} // runtime::graphics_engine::webgpu
