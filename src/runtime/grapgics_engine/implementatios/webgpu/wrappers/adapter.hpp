#pragma once

#include "instance.hpp"
#include "resource_holder.hpp"

#include <webgpu/webgpu.h>

namespace runtime::graphics_engine::webgpu {

RESOURCE_HOLDER(Adapter, WGPUAdapter, adapter, wgpuAdapterRelease);

class Adapter final : public AdapterResourceHolder {
public:
    Adapter(Instance const& instance);
};

} // namespace runtime::graphics_engine::webgpu
