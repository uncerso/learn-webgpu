#include "queue.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

Queue::Queue(Device const& device)
    : QueueResourceHolder(wgpuDeviceGetQueue(device.device()))
{
    REQUIRE(_queue, "Could not create WebGPU queue");
}

} // namespace runtime::graphics_engine::webgpu
