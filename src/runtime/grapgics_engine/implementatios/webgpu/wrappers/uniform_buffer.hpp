#pragma once

#include "buffer.hpp"
#include "device.hpp"
#include "resource_holder.hpp"

#include <runtime/grapgics_engine/resources.hpp>
#include <webgpu/webgpu.h>

#include <cstddef>
#include <unordered_map>

namespace runtime::graphics_engine::webgpu {

class UniformBuffer final : public Buffer, public IUniformBuffer {
public:
    UniformBuffer(Device const& device, Queue const& queue, std::span<std::byte const> bytes)
        : Buffer(device, queue, bytes, Buffer::Type::Uniform)
    {}

    WGPUBindGroup makeBindingGroup(WGPUBindGroupLayout bindGroupLayout, Device const& device, uint32_t bindingGroup);

    void rewriteImpl(std::span<std::byte const> bytes, size_t startPos) override { Buffer::rewrite(bytes, startPos); }

private:
    RESOURCE_HOLDER(BindGroup, WGPUBindGroup, group, wgpuBindGroupRelease);
    struct BindGroup : BindGroupResourceHolder {};

    std::unordered_map<WGPUBindGroupLayout, BindGroup> _pipelinesToBindingGroups; // ABA
};

} // namespace runtime::graphics_engine::webgpu
