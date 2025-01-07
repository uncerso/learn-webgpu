#pragma once

#include "device.hpp"

#include <runtime/grapgics_engine/layout.hpp>
#include <webgpu/webgpu.h>
#include <vector>

namespace runtime::graphics_engine::webgpu {

struct PipelineLayout {
    PipelineLayout(Device const& device, std::initializer_list<Layout const> layouts);
    ~PipelineLayout();

    WGPUBindGroupLayout bindingGroupLayoutAt(uint32_t pos) const { return _bindGroupLayouts.at(pos); }
    WGPUPipelineLayout layout() const noexcept { return _layout; }

private:
    std::vector<WGPUBindGroupLayout> _bindGroupLayouts;
    WGPUPipelineLayout _layout = nullptr;
};


} // namespace runtime::graphics_engine::webgpu
