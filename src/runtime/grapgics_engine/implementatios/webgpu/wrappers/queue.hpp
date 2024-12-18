#pragma once

#include "device.hpp"
#include "command_buffer.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Queue, WGPUQueue, queue, wgpuQueueRelease);

class Queue final : public QueueResourceHolder {
public:
    Queue(Device const& device);

    void submit(CommandBuffer const& buffer) {
        wgpuQueueSubmit(_queue, 1, &buffer.buffer());
    }
};

} // namespace runtime::graphics_engine::webgpu
