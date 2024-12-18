#pragma once

#include "device.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(CommandEncoder, WGPUCommandEncoder, encoder, wgpuCommandEncoderRelease);

class CommandEncoder final : public CommandEncoderResourceHolder {
public:
    CommandEncoder(Device const& device);
};

} // namespace runtime::graphics_engine::webgpu
