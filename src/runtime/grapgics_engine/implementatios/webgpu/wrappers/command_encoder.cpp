#include "command_encoder.hpp"

#include <runtime/common/assert.hpp>

namespace runtime::graphics_engine::webgpu {

CommandEncoder::CommandEncoder(Device const& device) {
    WGPUCommandEncoderDescriptor encoderDesc = {};
    encoderDesc.nextInChain = nullptr;
    encoderDesc.label = "CommandEncoder";
    _encoder = wgpuDeviceCreateCommandEncoder(device.device(), &encoderDesc);
    REQUIRE(_encoder, "Could not create WebGPU encoder");
}

} // namespace runtime::graphics_engine::webgpu
