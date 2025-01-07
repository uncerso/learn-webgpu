#include "uniform_buffer.hpp"

namespace runtime::graphics_engine::webgpu {

WGPUBindGroup UniformBuffer::makeBindingGroup(WGPUBindGroupLayout bindGroupLayout, Device const& device, uint32_t bindingGroup) {
    auto it = _pipelinesToBindingGroups.find(bindGroupLayout);
    if (it != _pipelinesToBindingGroups.end())
        return it->second.group();


    WGPUBindGroupEntry binding{};
    binding.nextInChain = nullptr;
    binding.binding = bindingGroup;
    binding.buffer = _buffer;
    binding.offset = 0;
    binding.size = wgpuBufferGetSize(_buffer);

    WGPUBindGroupDescriptor bindGroupDesc{};
    bindGroupDesc.nextInChain = nullptr;
    bindGroupDesc.layout = bindGroupLayout;
    bindGroupDesc.entryCount = 1;
    bindGroupDesc.entries = &binding;
    auto bindGroup = wgpuDeviceCreateBindGroup(device.device(), &bindGroupDesc);
    _pipelinesToBindingGroups.insert({bindGroupLayout, {bindGroup}});
    return bindGroup;
}

} // namespace runtime::graphics_engine::webgpu
