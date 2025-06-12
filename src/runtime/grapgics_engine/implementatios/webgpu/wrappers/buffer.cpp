#include "buffer.hpp"
#include <vector>

namespace runtime::graphics_engine::webgpu {

namespace {

size_t alignedSize4(size_t realSize) {
    return (realSize + 3) & ~size_t(3);
}

size_t alignedSize16(size_t realSize) {
    return (realSize + 15) & ~size_t(15);
}

WGPUBufferUsage toWGPU(Buffer::Type type) {
    switch (type) {
        case Buffer::Type::Vertex : return WGPUBufferUsage_Vertex;
        case Buffer::Type::Index  : return WGPUBufferUsage_Index;
        case Buffer::Type::Uniform: return WGPUBufferUsage_Uniform;
    }
}

size_t alignCapacity(size_t realSize, Buffer::Type type) {
    switch (type) {
        case Buffer::Type::Vertex : [[fallthrough]];
        case Buffer::Type::Index  : return alignedSize4(realSize);
        case Buffer::Type::Uniform: return alignedSize16(realSize);
    }
}

} // namespace

Buffer::Buffer(Device const& device, Queue const& queue, std::span<std::byte const> bytes, Type type)
    : _queue(queue)
{
    WGPUBufferDescriptor bufferDesc {
        .nextInChain = nullptr,
        .label = nullptr,
        .usage = static_cast<WGPUBufferUsageFlags>(WGPUBufferUsage_CopyDst | toWGPU(type)) ,
        .size = alignCapacity(bytes.size(), type),
        .mappedAtCreation = false,
    };
    _buffer = wgpuDeviceCreateBuffer(device.device(), &bufferDesc);

    rewrite(bytes);
}

void Buffer::rewrite(std::span<std::byte const> bytes, size_t startPos) {
    auto const size = alignedSize4(bytes.size());
    if (size == bytes.size()) {
        wgpuQueueWriteBuffer(_queue.queue(), _buffer, startPos, bytes.data(), size);
        return;
    }

    std::vector<std::byte> tmp(size);
    std::copy(bytes.begin(), bytes.end(), tmp.begin());
    wgpuQueueWriteBuffer(_queue.queue(), _buffer, startPos, tmp.data(), size);
}

} // namespace runtime::graphics_engine::webgpu
