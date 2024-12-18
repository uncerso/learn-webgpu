#pragma once

#include "command_encoder.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(CommandBuffer, WGPUCommandBuffer, buffer, wgpuCommandBufferRelease);

class CommandBuffer final : public CommandBufferResourceHolder {
public:
    CommandBuffer(CommandEncoder const& encoder);
};

} // namespace runtime::graphics_engine::webgpu
