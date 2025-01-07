#pragma once

#include "device.hpp"
#include "queue.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

#include <cstddef>
#include <span>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Buffer, WGPUBuffer, buffer, wgpuBufferRelease);

class Buffer : public BufferResourceHolder {
public:
    enum class Type { Vertex, Index, Uniform };

    Buffer(Device const& device, Queue const& queue, std::span<std::byte const> bytes, Type type);
    void rewrite(std::span<std::byte const> bytes, size_t startPos = 0);

private:
    Queue const& _queue;
};

} // namespace runtime::graphics_engine::webgpu
