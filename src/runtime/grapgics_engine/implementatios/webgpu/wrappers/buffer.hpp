#pragma once

#include "device.hpp"
#include "queue.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

#include <cstddef>
#include <span>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Buffer, WGPUBuffer, buffer, wgpuBufferRelease);

class Buffer final : public BufferResourceHolder {
public:
    Buffer(Device const& device, Queue const& queue, std::span<std::byte const> bytes, WGPUBufferUsageFlags usage);
};

} // namespace runtime::graphics_engine::webgpu
