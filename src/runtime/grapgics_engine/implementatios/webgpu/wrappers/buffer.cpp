#include "buffer.hpp"

namespace runtime::graphics_engine::webgpu {

Buffer::Buffer(Device const& device, Queue const& queue, std::span<std::byte const> bytes, WGPUBufferUsageFlags usage) {
    WGPUBufferDescriptor bufferDesc{};
    bufferDesc.nextInChain = nullptr;
    bufferDesc.size = (bytes.size() + 3) & ~size_t(3);
    bufferDesc.usage = usage;
    bufferDesc.mappedAtCreation = false;
    _buffer = wgpuDeviceCreateBuffer(device.device(), &bufferDesc);
    wgpuQueueWriteBuffer(queue.queue(), _buffer, 0, bytes.data(), bufferDesc.size);
}

} // namespace runtime::graphics_engine::webgpu
