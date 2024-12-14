#include "queue.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

namespace {

void onQueueWorkDone(WGPUQueueWorkDoneStatus status, void* /* pUserData */) {
    std::cout << "Queued work finished with status: " << status << std::endl;
}

} // namespace

Queue::Queue(Device const& device)
    : QueueResourceHolder(wgpuDeviceGetQueue(device.device()))
{
    REQUIRE(_queue, "Could not create WebGPU queue");
    wgpuQueueOnSubmittedWorkDone(_queue, onQueueWorkDone, nullptr /* pUserData */);
}

} // runtime::graphics_engine::webgpu
