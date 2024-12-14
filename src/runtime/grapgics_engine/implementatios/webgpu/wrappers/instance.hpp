#pragma once

#include "resource_holder.hpp"

#include <runtime/common/exception.hpp>

#include <webgpu/webgpu.hpp>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Instance, WGPUInstance, instance, wgpuInstanceRelease);

class Instance final : public InstanceResourceHolder {
public:
    Instance() {
        WGPUInstanceDescriptor descriptor = {};
        descriptor.nextInChain = nullptr;
        _instance = wgpuCreateInstance(&descriptor);
        REQUIRE(_instance, "Could not initialize WebGPU");
    }
};

} // runtime::graphics_engine::webgpu
