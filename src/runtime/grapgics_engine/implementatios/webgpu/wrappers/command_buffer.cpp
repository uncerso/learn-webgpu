#include "command_buffer.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

CommandBuffer::CommandBuffer(CommandEncoder const& encoder) {
    WGPUCommandBufferDescriptor cmdBufferDescriptor = {};
    cmdBufferDescriptor.nextInChain = nullptr;
    cmdBufferDescriptor.label = "CommandBuffer";
    _buffer = wgpuCommandEncoderFinish(encoder.encoder(), &cmdBufferDescriptor);
    REQUIRE(_buffer, "Could not create WebGPU command buffer");
}

} // namespace runtime::graphics_engine::webgpu
